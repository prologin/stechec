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
    read_size = 0
    while True:
        try:
            chunk = p.stdout.read(4096)
            read_size += len(chunk)
            if not chunk:
                break

            if read_size < 2**18:
                chunks.append(chunk)
        except IOError, ex:
            if ex[0] != errno.EAGAIN:
                raise
            sys.exc_clear()
        gevent.socket.wait_read(p.stdout.fileno())
    p.stdout.close()

    if read_size >= 2**18:
        chunks.append("\n\nLog truncated to stay below 256K\n")

    # Wait for process completion
    while p.poll() is None:
        gevent.sleep(0.05)

    # Return data
    return (p.returncode, ''.join(chunks))

def champion_path(config, contest, user, champ_id):
    return os.path.join(config['paths']['data_root'], contest, 'champions',
                        user, str(champ_id))

def match_path(config, contest, match_id):
    match_id_high = "%03d" % (match_id / 1000)
    match_id_low = "%03d" % (match_id % 1000)
    return os.path.join(config['paths']['data_root'], contest, "matches",
                        match_id_high, match_id_low)

def compile_champion(config, contest, user, champ_id):
    """
    Compiles the champion at $dir_path/champion.tgz to $dir_path/champion.so.

    Returns a tuple (ok, output), with ok = True/False and output being the
    output of the compilation script.
    """
    dir_path = champion_path(config, contest, user, champ_id)
    cmd = [paths.compile_script, config['paths']['data_root'], dir_path]
    retcode, stdout = communicate(cmd)
    return retcode == 0

def spawn_server(cmd, path, match_id, callback):
    retcode, stdout = communicate(cmd)

    log_path = os.path.join(path, "server.log")
    open(log_path, "w").write(stdout)
    callback(retcode, stdout, match_id)

def run_server(config, server_done, port, contest, match_id, opts):
    """
    Runs the Stechec server and wait for client connections.
    """
    path = match_path(config, contest, match_id)
    try:
        os.makedirs(path)
    except OSError:
        pass

    config_path = os.path.join(path, "config_server.ini")
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
    gevent.spawn(spawn_server, cmd, path, match_id, server_done)
    gevent.sleep(0.25) # let it start

def spawn_client(cmd, path, match_id, champ_id, callback):
    retcode, stdout = communicate(cmd)
    log_path = os.path.join(path, "log-champ-%d.log" % champ_id)
    open(log_path, "w").write(stdout)
    callback(retcode, stdout, match_id, champ_id)

def run_client(config, ip, port, contest, match_id, user, champ_id, cb):
    dir_path = champion_path(config, contest, user, champ_id)
    mp = match_path(config, contest, match_id)
    cmd = [paths.stechec_client,
               "-i", str(champ_id),
               "-r", contest,
               "-a", dir_path,
               "-l", "champion",
               "-d", ip,
               "-p", str(port),
               "-m", "250000",
               "-t", "50",
               "-f", "45000",
    ]
    gevent.spawn(spawn_client, cmd, mp, match_id, champ_id, cb)
