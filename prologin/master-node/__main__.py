# -*- encoding: utf-8 -*-
# This file is part of Stechec.
#
# Copyright (c) 2011 Pierre Bourdon <pierre.bourdon@prologin.org>
# Copyright (c) 2011 Association Prologin <info@prologin.org>
#
# Stechec is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Stechec is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Stechec.  If not, see <http://www.gnu.org/licenses/>.

import gevent
import gevent.event
import gevent.monkey
import gevent.socket

# We are not in the worker node and are not supposed to use subprocesses, so a
# monkey.patch_all() should be safe. However, if this was to change, beware
# that monkey_os() interacts badly with libevent2.
gevent.monkey.patch_all()

from SimpleXMLRPCServer import SimpleXMLRPCServer
from worker import Worker

import copy
import logging
import logging.handlers
import optparse
import paths
import psycopg2
import xmlrpclib
import utils
import yaml

utils.init_psycopg_gevent()

class MasterNode(object):
    def __init__(self, config):
        self.config = config
        self.workers = {}

        self.spawn_tasks()

    def spawn_tasks(self):
        # Setup locks/events/queues
        self.dispatch_needed = gevent.event.Event()

        self.janitor = gevent.spawn(self.janitor_task)
        self.dbwatcher = gevent.spawn(self.dbwatcher_task)

    def update_worker(self, worker):
        hostname, port, slots, max_slots = worker
        key = hostname, port
        if key not in self.workers:
            logging.warn("registered new worker: %s:%d" % (hostname, port))
            self.workers[key] = Worker(hostname, port, slots, max_slots)
        else:
            logging.debug("updating worker: %s:%d %d/%d" % (
                              hostname, port, slots, max_slots
                         ))
            self.workers[key].update(slots, max_slots)

    def heartbeat(self, worker):
        hostname, port, slots, max_slots = worker
        usage = (1.0 - float(slots) / max_slots) * 100
        logging.info('received heartbeat from %s:%d, usage is %.2f' % (
                         hostname, port, usage
                    ))
        self.update_worker(worker)

    def janitor_task(self):
        while True:
            all_workers = copy.copy(self.workers)
            for worker in all_workers.itervalues():
                if not worker.is_alive(self.config['worker']['timeout_secs']):
                    logging.warn("timeout detected for worker %s:%d" % (
                                     worker.hostname, worker.port
                                ))
                    del self.workers[(worker.hostname, worker.port)]

            gevent.sleep(1)

    def connect_to_db(self):
        self.db = psycopg2.connect(
            host=self.config['sql']['host'],
            port=self.config['sql']['port'],
            user=self.config['sql']['user'],
            password=self.config['sql']['password'],
            database=self.config['sql']['database'],
            async=True,
        )

    def check_requested_compilations(self):
        logging.error("todo!")
        return False

    def check_requested_matches(self):
        logging.error("todo!")
        return False

    def dbwatcher_task(self):
        self.connect_to_db()
        while True:
            compils = self.check_requested_compilations()
            matches = self.check_requested_matches()
            if compils or matches:
                self.dispatch_needed.set()
            gevent.sleep(1)

class MasterNodeProxy(object):
    def __init__(self, master):
        self.master = master

    def heartbeat(self, secret, worker):
        self.master.heartbeat(worker)
        return True

if __name__ == '__main__':
    parser = optparse.OptionParser()
    parser.add_option('-c', '--config', dest='config_file',
                      default=paths.config_file,
                      help='The configuration file.')
    parser.add_option('-l', '--local-logging', action='store_true',
                      dest='local_logging', default=False,
                      help='Activate logging to stdout.')
    parser.add_option('-v', '--verbose', action='store_true',
                      dest='verbose', default=False,
                      help='Verbose mode.')
    options, args = parser.parse_args()

    loggers = []
    loggers.append(logging.handlers.SysLogHandler('/dev/log'))
    if options.local_logging:
        loggers.append(logging.StreamHandler())
    for logger in loggers:
        logger.setFormatter(logging.Formatter(
            'master-node: [%(levelname)s] %(message)s'
        ))
        logging.getLogger('').addHandler(logger)
    logging.getLogger('').setLevel(
        logging.DEBUG if options.verbose else logging.INFO
    )

    config = yaml.load(open(options.config_file))
    s = SimpleXMLRPCServer(('0.0.0.0', config['master']['port']),
                           logRequests=False)
    s.register_introspection_functions()

    master = MasterNode(config)
    s.register_instance(MasterNodeProxy(master))

    try:
        s.serve_forever()
    except KeyboardInterrupt:
        pass
