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

gevent.monkey.patch_all()

from SimpleXMLRPCServer import SimpleXMLRPCServer

import os.path
import operations
import paths
import socket
import xmlrpclib
import yaml

class WorkerNode(object):
    def __init__(self, config):
        self.master = self.create_master_connection(config)
        self.interval = config['master']['heartbeat_secs']
        self.hostname = socket.gethostname()
        self.port = config['worker']['port']
        self.slots = self.max_slots = config['worker']['available_slots']
        self.secret = config['master']['shared_secret']

        self.heartbeat_greenlet = gevent.spawn(self.send_heartbeat)

    def get_worker_infos(self):
        return (self.hostname, self.port, self.slots, self.max_slots)

    def create_master_connection(self, config):
        host, port = (config['master']['host'], config['master']['port'])
        url = "http://%s:%d/" % (host, port)
        return xmlrpclib.ServerProxy(url)

    def send_heartbeat(self):
        while True:
            try:
                self.master.heartbeat(self.secret, self.get_worker_infos())
            except gevent.socket.error:
                print 'master down, retrying heartbeat in %ds' % self.interval
            gevent.sleep(self.interval)

    def compile_champion(self, contest, user, champ_id):
        dir_path = os.path.join(config['paths']['data_root'],
                                contest, "champions", user, str(champ_id))
        return operations.compile_champion(config, dir_path, user, champ_id)

class WorkerNodeProxy(object):
    """
    Proxies XMLRPC requests to the WorkerNode.
    """

    def __init__(self, node):
        self.node = node

    def compile_champion(self, secret, *args, **kwargs):
        return self.node.compile_champion(*args, **kwargs)

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
