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

from subprocess import Popen, PIPE, STDOUT

import errno
import fcntl
import gevent
import gevent.socket
import paths
import os
import os.path
import sys

def communicate(cmdline, data=''):
    """
    Asynchronously communicate with an external process, sending data on its
    stdin and receiving data from its stdout and stderr streams.

    Returns (retcode, stdout).
    """
    p = Popen(cmdline, stdin=PIPE, stdout=PIPE, stderr=STDOUT)
    fcntl.fcntl(p.stdin, fcntl.F_SETFL, os.O_NONBLOCK)
    fcntl.fcntl(p.stdout, fcntl.F_SETFL, os.O_NONBLOCK)

    # Write stdin data
    total_bytes = len(data)
    written_bytes = 0
    while written_bytes < total_bytes:
        try:
            written_bytes += os.write(p.stdin.fileno(), data[written_bytes:])
        except IOError, ex:
            if ex[0] != errno.EAGAIN:
                raise
            sys.exc_clear()
        gevent.socket.wait_write(p.stdin.fileno())
    p.stdin.close()

    # Read stdout data
    chunks = []
    while True:
        try:
            chunk = p.stdout.read(4096)
            if not chunk:
                break
            chunks.append(chunk)
        except IOError, ex:
            if ex[0] != errno.EAGAIN:
                raise
            sys.exc_clear()
        gevent.socket.wait_read(p.stdout.fileno())
    p.stdout.close()

    # Wait for process completion
    while p.poll() is None:
        gevent.sleep(0.05)

    # Return data
    return (p.returncode, ''.join(chunks))

def compile_champion(config, dir_path, user, champ_id):
    """
    Compiles the champion at $dir_path/champion.tgz to $dir_path/champion.so.

    Returns a tuple (ok, output), with ok = True/False and output being the
    output of the compilation script.
    """
    cmd = [paths.compile_script, config['paths']['data_root'], dir_path]
    retcode, stdout = communicate(cmd)
    return retcode == 0

def spawn_server(worker, cmd):
    retcode, stdout = communicate(cmd)
    worker.slots += 1

def run_server(worker, port, contest, opts, match_path):
    """
    Runs the Stechec server and wait for client connections.
    """
    try:
        os.makedirs(match_path)
    except OSError:
        pass

    config_path = os.path.join(match_path, "config_server.ini")
    config = '''
[server]
listen_port=%(port)d
verbose=2

[%(contest)s]
verbose=0
%(opts)s
''' % locals()
    open(config_path, 'w').write(config)
    cmd = [paths.stechec_server, "-c", config_path]
    gevent.spawn(spawn_server, worker, cmd)
    gevent.sleep(0.25) # let it start
