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

class CompilationTask(object):
    def __init__(self, config, user, champ_id):
        self.contest = config['master']['contest']
        self.secret = config['master']['shared_secret']
        self.user = user
        self.champ_id = champ_id

    @property
    def slots_taken(self):
        return 1

    def execute(self, worker):
        worker.rpc.compile_champion(
            self.secret, self.contest, self.user, self.champ_id
        )

    def __repr__(self):
        return "<Compilation: %s/%d>" % (self.user, self.champ_id)
