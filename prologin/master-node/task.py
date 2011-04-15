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

import gevent.queue

class CompilationTask(object):
    def __init__(self, config, user, champ_id):
        self.contest = config['master']['contest']
        self.secret = config['master']['shared_secret']
        self.user = user
        self.champ_id = champ_id

    @property
    def slots_taken(self):
        return 1

    def execute(self, master, worker):
        worker.rpc.compile_champion(
            self.secret, self.contest, self.user, self.champ_id
        )

    def __repr__(self):
        return "<Compilation: %s/%d>" % (self.user, self.champ_id)

class PlayerTask(object):
    def __init__(self, config, mid, hostname, port, cid, mpid, user):
        self.contest = config['master']['contest']
        self.secret = config['master']['shared_secret']
        self.mid = mid
        self.hostname = hostname
        self.port = port
        self.cid = cid
        self.mpid = mpid
        self.user = user

    @property
    def slots_taken(self):
        return 2 # It's usually fairly intensive, take 2 slots

    def execute(self, master, worker):
        worker.rpc.run_client(
            self.secret, self.contest, self.mid, self.hostname, self.port,
            self.user, self.cid, self.mpid
        )

class MatchTask(object):
    def __init__(self, config, mid, players, opts):
        self.config = config
        self.contest = config['master']['contest']
        self.secret = config['master']['shared_secret']
        self.mid = mid
        self.players = players
        self.opts = opts
        self.server_port = gevent.queue.Queue()

    @property
    def slots_taken(self):
        return 1 # Only the server is launched by this task

    def execute(self, master, worker):
        worker.rpc.run_server(
            self.secret, self.contest, self.mid, self.opts
        )
        port = self.server_port.get()
        for (cid, mpid, user) in self.players:
            t = PlayerTask(self.config, self.mid, worker.hostname, port, cid,
                           mpid, user)
            master.worker_tasks.append(t)
        master.to_dispatch.set()
