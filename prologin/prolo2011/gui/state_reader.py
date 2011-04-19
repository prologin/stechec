# -*- coding: utf-8 -*-

import threading
import Queue as queue

from api import *
import game

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
        if not self.pipe.empty():
            self.turn += 1
            game_state = self.pipe.get()
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

    def put_state(self):
        '''
        Put the next game state.
        Must be called in the Stechec thread.
        '''

        if self.waiting_end:
            return
        game_state = self.build_game()
        self.pipe.put(game_state)
        self.end_turn.clear()
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

    def build_game(self):
        '''
        Build a Game object from the Stechec’s API and return it.
        Shouldn’t be called out of this class (Stechec thread).
        '''

        g = game.GameState()
        g.turn_no = tour_actuel()
        g.actions = actions_effectuees()

        g.ground = game.Grid(lambda x: VIDE)
        g.bonusgrid = game.Grid(regarder_type_bonus)
        g.objgrid = game.Grid(lambda x: [])
        g.motos = [
                game.Moto(m.team, [(pos.x, pos.y) for pos in m.emplacement],
                          m.id, g.objgrid)
                for m in trainees_moto()
                ]
        g.sources = [game.Source((s.pos.x, s.pos.y),
                                 s.capacite, s.capacite_max,
                                 s.id,
                                 g.objgrid)
                        for s in sources_energie()]

        for y in xrange(TAILLE_TERRAIN):
            for x in xrange(TAILLE_TERRAIN):
                c = regarder_type_case(position(x=x, y=y))
                if c in (OBSTACLE, POINT_CROISEMENT):
                    g.ground[(x, y)] = c

        g.teams = [game.Team(i, 0, []) for i in xrange(nombre_equipes())]
        for (no, s) in enumerate(scores()):
            g.teams[no].bonus = regarder_bonus(no)
            g.teams[no].score = s

        return g


class DumpReader(Reader):
    def __init__(self):
        raise NotImplementedError()
