# -*- coding: iso-8859-1 -*-
# This file has been generated, if you wish to
# modify it in a permanent way, please refer
# to the script file : gen/generator_python.rb

from api import *

try:
    import psyco
    psyco.full()
except:
    pass
# Fonction appellée au début de la partie.
def init_game():
    pass # Place ton code ici

# Fonction appellée pour la phase de retrait de KO.
def retirer_ko():
    unites = unites()
    is_ennemi = lambda u: return u.ennemi
    is_ko = lambda u: return (u.ko >= 0)
    ennemis = filter(is_ennemi, unites)
    kos = filter(is_ko, ennemis)
    return kos.pop()

# Fonction appellée pour la phase de jeu.
def jouer():
    unites = unites()
    afficher_unite(unites.pop())
    pass # Place ton code ici

# Fonction appellée à la fin de la partie.
def end_game():
    pass # Place ton code ici

