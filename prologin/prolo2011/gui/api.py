# -*- coding: iso-8859-1 -*-
try:
    from _api import *
except ImportError:
    pass

# Taille du terrain
TAILLE_TERRAIN = 30

# Nombre de tours par partie
FIN_PARTIE = 150

# Nombre de points d'action par tour
MAX_PA = 3

# Taille des traînées de moto
INTENSITE_TRAINEE = 120

# Longueur maximale de l'allongement
MAX_ALLONGEMENT = 5

# Nombre de points d'action à rajouter avec bonus
AJOUT_PA = 5

# Énumération représentant une erreur renvoyée par une des fonctions d'action
(
    OK, # <- aucune erreur n'est survenue
    ID_INVALIDE, # <- identifiant invalide
    POSITION_INVALIDE, # <- la position spécifiée est invalide
    PLUS_DE_PA, # <- vous n'avez pas assez de points d'action
    BONUS_INVALIDE, # <- vous n'avez pas ce bonus
    PAS_A_TOI, # <- l'unité n'est pas a vous
    INTENSITE_INVALIDE, # <- cette intensité est invalide
) = range(7)


# Énumération représentant les différents types de case
(
    VIDE, # <- rien n'est présent sur la case
    OBSTACLE, # <- cette case est inaccessible
    POINT_CROISEMENT, # <- point de croisement de traînées
    UNITE, # <- unité d'énergie
) = range(4)


# Énumération représentant les différents types de bonii
(
    PAS_BONUS, # <- ceci n'est pas un bonus :-)
    BONUS_CROISEMENT, # <- bonus permettant de croiser deux traînées de moto sur une case
    PLUS_LONG, # <- bonus permettant d'agrandir une traînée de moto
    PLUS_PA, # <- bonus permettant d'avoir plus de points d'action
    BONUS_REGENERATION, # <- bonus permettant de regenerer une unité d'énergie
) = range(5)


from collections import namedtuple

# Représente une position sur le terrain du jeu
position = namedtuple("position",
    'x ' # <- coordonnée en X
    'y ' # <- coordonnée en Y
)


# Caracteristiques d'une unité d'énergie
unite_energie = namedtuple("unite_energie",
    'id ' # <- identifiant de l'unité d'énergie
    'pos ' # <- position de l'unité d'énergie
    'valeur ' # <- coefficient représentant les points d'énergie que l'unité va vous apporter
    'valeur_max ' # <- coefficient représentant la capacité de l'unité lorsqu'elle est chargée au maximum
)


# Représente une traînée de moto sur le terrain
trainee_moto = namedtuple("trainee_moto",
    'id ' # <- identifiant de la traînee
    'emplacement ' # <- position de chaque composant de la traînée de moto
    'team ' # <- identifiant de l'équipe qui possède cette traînée de moto
    'intensite ' # <- taille maximale de la traînée
)


