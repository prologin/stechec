#! /usr/bin/env python2
# -*- coding: utf-8 -*-

import sys
import gzip

from api import *

import graphics
import state_reader

#dump_file = open(sys.argv[1], 'r')
dump_file = gzip.open(sys.argv[1], 'r')
state_reader = state_reader.DumpReader(dump_file)
graphics = graphics.Graphics(state_reader)
graphics.init()
graphics.run()
