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

# We are not in the worker node and are not supposed to use subprocesses, so a
# monkey.patch_all() should be safe. However, if this was to change, beware
# that monkey_os() interacts badly with libevent2.
gevent.monkey.patch_all()

from SimpleXMLRPCServer import SimpleXMLRPCServer

import logging
import logging.handlers
import optparse
import paths
import xmlrpclib
import yaml

class MasterNode(object):
    def __init__(self, config):
        self.config = config

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
    s.register_instance(master)

    try:
        s.serve_forever()
    except KeyboardInterrupt:
        pass
