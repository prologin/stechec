#!/usr/bin/env python
#-*- encoding: utf-8 -*-
"""
master.py
Implements the master remote control protocol

Copyright (c) 2008 Pierre "delroth" Bourdon <root@delroth.is-a-geek.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
"""

from concours.settings import MASTER_SERVER
import socket

def request(message):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect(MASTER_SERVER)
    sock.send(message)
    response = ''
    buffer = sock.recv(1024)
    while buffer != '':
        response += buffer
        buffer = sock.recv(1024)
    sock.close()
    return response

def match_list():
    return [tuple(f.split()) for f in request('match list').split('\n') if f]

def match_summary():
    return request('match summary').split()

def machine_list():
    from datetime import datetime
    ret = []
    for f in request('machine list').split('\n'):
        if not f: continue
        ip, port, statut, ping, slots, usage = f.split()
        ping = datetime.fromtimestamp(float(ping))
        ret.append((ip, port, statut, ping, slots, usage))
    
    return ret

def machine_summary():
    return request('machine summary').split()
