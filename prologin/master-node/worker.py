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

import time

class Worker(object):
    def __init__(self, hostname, port, slots, max_slots):
        self.hostname = hostname
        self.port = port
        self.slots = slots
        self.max_slots = max_slots
        self.keep_alive()

    def update(self, slots, max_slots):
        self.slots = slots
        self.max_slots = max_slots
        self.keep_alive()

    def keep_alive(self):
        self.last_heartbeat = time.time()

    def is_alive(self, timeout):
        return (time.time() - self.last_heartbeat) < timeout
