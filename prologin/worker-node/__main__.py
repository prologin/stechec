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
import xmlrpclib

class WorkerNode(object):
    def __init__(self, config):
        self.master = self.create_master_connection(config)
        self.interval = config['master']['interval']
        self.infos = ('localhost', 8068)

        self.heartbeat_greenlet = gevent.spawn(self.send_heartbeat)

    def create_master_connection(self, config):
        host, port = (config['master']['host'], config['master']['port'])
        url = "http://%s:%d/" % (host, port)
        return xmlrpclib.ServerProxy(url)

    def send_heartbeat(self):
        while True:
            try:
                self.master.heartbeat('secret', self.infos)
            except gevent.socket.error:
                print 'master down, retrying heartbeat in %ds' % self.interval
            gevent.sleep(self.interval)

if __name__ == '__main__':
    s = SimpleXMLRPCServer(('localhost', 8068))
    s.register_introspection_functions()

    worker = WorkerNode({ 'master': {'host': 'localhost',
                                     'port': 8067,
                                     'interval': 2}})
    s.register_instance(worker)

    s.serve_forever()
