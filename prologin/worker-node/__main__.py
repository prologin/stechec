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
import gevent.monkey
import gevent.socket

# We do not use .patch_all() because .patch_os() must NOT be called.
#
# It overrides os.fork() which is used in subprocess.Popen and causes a
# strange bug in libevent when the hub is reinitialized (infinite loop on ^C).
gevent.monkey.patch_select()
gevent.monkey.patch_socket()
gevent.monkey.patch_ssl()
gevent.monkey.patch_time()
gevent.monkey.patch_thread()

from SimpleXMLRPCServer import SimpleXMLRPCServer

import os.path
import operations
import paths
import socket
import xmlrpclib
import yaml

class WorkerNode(object):
    def __init__(self, config):
        self.config = config
        self.interval = config['master']['heartbeat_secs']
        self.hostname = socket.gethostname()
        self.port = config['worker']['port']
        self.slots = self.max_slots = config['worker']['available_slots']
        self.secret = config['master']['shared_secret']
        self.min_srv_port = config['worker']['port_range_start']
        self.max_srv_port = config['worker']['port_range_end']
        self.srv_port = self.min_srv_port

        self.heartbeat_greenlet = gevent.spawn(self.send_heartbeat)

    def get_worker_infos(self):
        return (self.hostname, self.port, self.slots, self.max_slots)

    @property
    def master(self):
        config = self.config
        host, port = (config['master']['host'], config['master']['port'])
        url = "http://%s:%d/" % (host, port)
        return xmlrpclib.ServerProxy(url)

    @property
    def available_server_port(self):
        """
        Be optimistic and hope that:
        - nobody will use the ports in the port range but us
        - there will never be more servers than ports in the range
        """
        port = self.srv_port
        self.srv_port += 1
        if self.srv_port > self.max_srv_port:
            self.srv_port = self.min_srv_port
        return port

    def send_heartbeat(self):
        while True:
            try:
                self.master.heartbeat(self.secret, self.get_worker_infos())
            except gevent.socket.error:
                print 'master down, retrying heartbeat in %ds' % self.interval
            gevent.sleep(self.interval)

    def start_work(self, work, slots, *args, **kwargs):
        if self.slots < slots:
            return False, self.secret, self.get_worker_infos()

        self.slots -= slots

        def real_work():
            job_done = True
            try:
                job_done, func, args_li = work(*args, **kwargs)
            finally:
                if job_done:
                    self.slots += slots
            func(self.secret, self.get_worker_infos(), *args_li)

        gevent.spawn(real_work)
        return True, self.secret, self.get_worker_infos()

    def compile_champion(self, contest, user, champ_id):
        dir_path = os.path.join(self.config['paths']['data_root'],
                                contest, "champions", user, str(champ_id))
        ret = operations.compile_champion(self.config, dir_path,
                                          user, champ_id)
        return True, self.master.compilation_result, (champ_id, ret)

    def run_server(self, contest, match_id, opts=''):
        port = self.available_server_port
        match_id_high = "%03d" % (match_id / 1000)
        match_id_low = "%03d" % (match_id % 1000)
        match_path = os.path.join(self.config['paths']['data_root'],
                                  contest, "matches", match_id_high,
                                  match_id_low)
        operations.run_server(self, port, contest, opts, match_path)
        return False, self.master.match_ready, (match_id, port)

class WorkerNodeProxy(object):
    """
    Proxies XMLRPC requests to the WorkerNode.
    """

    def __init__(self, node):
        self.node = node

    def compile_champion(self, secret, *args, **kwargs):
        return self.node.start_work(
            self.node.compile_champion, 1, *args, **kwargs
        )

    def run_server(self, secret, *args, **kwargs):
        return self.node.start_work(
            self.node.run_server, 1, *args, **kwargs
        )

def read_config(filename):
    return yaml.load(open(filename))

if __name__ == '__main__':
    config = read_config(paths.config_file)
    s = SimpleXMLRPCServer(('localhost', config['worker']['port']),
                           logRequests=False)
    s.register_introspection_functions()

    worker = WorkerNode(config)
    s.register_instance(WorkerNodeProxy(worker))

    s.serve_forever()
