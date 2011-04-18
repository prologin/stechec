# -*- coding: utf-8 -*-

import threading
import Queue as queue

class Reader:
    '''
    This class just exhibit the reader interface needed by the gui.
    A Reader object is used to get the game state with the source abstraction.
    '''

    def __init__(self):
        pass

    def get_next_state(self):
        pass

    def go_next(self):
        pass

    def is_ended(self):
        pass

    def get_turn(self):
        pass

class StechecReader(Reader):
    '''
    Stechec reader get the game from the Stechec client.

    It must be used with two separated threads: one for the Stechec client, and
    one for the Gui. The Reader interface must be called only from the GUI
    thread, and the other methods only from the Stechec thread.
    '''

    def __init__(self):
        Reader.__init__(self)
        self.new_turn = threading.Event()
        self.end_turn = threading.Event()
        self.pipe = queue.Queue()
        self.end_game = threading.Event()
        self.waiting_turn = False
        self.realeased = threading.Event()
        self.turn = 0
        self.waiting_end = False

    def get_next_state(self):
        '''
        Return if available the next game state.
        Must only be called in the GUI thread.
        '''

        if self.waiting_turn:
            return None
        game_state = None
        if self.new_turn.is_set():
            self.turn += 1
            game_state = self.pipe.get()
            self.new_turn.clear()
            self.waiting_turn = True
        return game_state

    def go_next(self):
        '''
        Prepare the Stechec client to go to the next turn.
        Must be called in the GUI thread.
        '''

        if self.waiting_turn:
            self.waiting_turn = False
            self.end_turn.set()

    def put_state(self, game_state):
        '''
        Put the next game state.
        Must be called in the Stechec thread.
        '''

        if self.waiting_end:
            return
        self.pipe.put(game_state)
        self.end_turn.clear()
        self.new_turn.set()
        self.end_turn.wait()

    def is_ended(self):
        '''
        Return if the game is ended.
        Must be called in the GUI thread.
        '''

        return self.end_game.is_set()

    def wait_end(self):
        '''
        Wait the end of the game.
        Must be called in the GUI thread.
        '''

        self.end_turn.clear()
        self.end_turn.set()
        self.waiting_end = True

    def do_end(self):
        '''
        Make the game end.
        Must be called in the Stechec thread.
        '''

        self.end_game.set()

    def get_turn(self):
        '''
        Return the turn number.
        Should be called in the GUI thread.
        '''

        return self.turn

class DumpReader(Reader):
    def __init__(self):
        raise NotImplementedError()
