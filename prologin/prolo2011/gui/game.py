# -*- coding: utf-8 -*-

from api import *

class GameState:
    def __init__(self):
        self.groud = [[VIDE] * TAILLE_TERRAIN for i in xrange(TAILLE_TERRAIN)]
        self.objgrid = [[None] * TAILLE_TERRAIN for i in
                xrange(TAILLE_TERRAIN)]
