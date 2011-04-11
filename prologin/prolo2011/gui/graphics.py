# -*- coding: utf-8 -*-

import sys
import time

class Graphics:
    def __init__(self, pipe, new_turn, end_turn):
        self.game_state = None
        self.pipe = pipe
        self.new_turn = new_turn
        self.end_turn = end_turn

    def run(self):
        while True:
            self.new_turn.wait()
            print 'Gui: running'
            print 'Gui: Waiting 2 seconds...',
            sys.stdout.flush()
            time.sleep(2)
            self.game_state = self.pipe.get()
            if self.game_state == None:
                break
            self.new_turn.clear()
            print 'Gui: OK'
            self.end_turn.set()

    def init(self):
        pass

    def quit(self):
        pass
