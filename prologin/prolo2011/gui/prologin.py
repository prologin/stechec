# -*- coding: iso-8859-1 -*-
# This file has been generated, if you wish to
# modify it in a permanent way, please refer
# to the script file : gen/generator_python.rb

from api import *

import threading
import Queue as queue

import game
import graphics

try:
    import psyco
    psyco.full()
except:
    pass

pipe = queue.Queue()
new_turn = threading.Event()
end_turn = threading.Event()
graphics = graphics.Graphics(pipe, new_turn, end_turn)
gfx_thread = threading.Thread(target=graphics.run)

def init_game():
    graphics.init()
    gfx_thread.start()

def jouer():
    print 'Main: jouer()'
    game_state = game.GameState()
    pipe.put(game_state)
    end_turn.clear()
    new_turn.set()
    print 'Main: waiting for the GUI'
    end_turn.wait()
    print 'Main: OK, jouer done'

def end_game():
    graphics.quit()
