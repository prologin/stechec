# -*- coding: iso-8859-1 -*-
# This file has been generated, if you wish to
# modify it in a permanent way, please refer
# to the script file : gen/generator_python.rb

import sys
import threading

from api import *

import game
import state_reader
import graphics

try:
    import psyco
    psyco.full()
except:
    pass

state_reader = state_reader.StechecReader()
graphics = graphics.Graphics(state_reader)
gfx_thread = threading.Thread(target=graphics.run)

def init_game():
    graphics.init()
    gfx_thread.start()

def jouer():
    state_reader.put_state()

def end_game():
    state_reader.do_end()
