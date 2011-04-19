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

# Taille des tra�n�es de moto
TAILLE_TRAINEE = 120

# Longueur maximale de l'allongement
MAX_ALLONGEMENT = 5

# Nombre de points d'action � rajouter avec bonus
AJOUT_PA = 5

# �num�ration repr�sentant une erreur renvoy�e par une des fonctions d'action
(
    OK, # <- aucune erreur n'est survenue
    ID_INVALIDE, # <- identifiant invalide
    POSITION_INVALIDE, # <- la position sp�cifi�e est invalide
    PLUS_DE_PA, # <- vous n'avez pas assez de points d'action
    BONUS_INVALIDE, # <- vous n'avez pas ce bonus
    PAS_A_TOI, # <- l'unit� n'est pas a vous
) = range(6)


# �num�ration repr�sentant les diff�rents types de case
(
    VIDE, # <- rien n'est pr�sent sur la case
    OBSTACLE, # <- cette case est inaccessible
    POINT_CROISEMENT, # <- point de croisement de tra�n�es
    SOURCE, # <- source ou consommateur d'energie
) = range(4)


# �num�ration repr�sentant les diff�rents types de bonii
(
    PAS_BONUS, # <- ceci n'est pas un bonus :-)
    BONUS_CROISEMENT, # <- bonus permettant de croiser deux tra�n�es de moto sur une case
    PLUS_LONG, # <- bonus permettant d'agrandir une tra�n�e de moto
    PLUS_PA, # <- bonus permettant d'avoir plus de points d'action
    BONUS_REGENERATION, # <- bonus permettant de regenerer une source d'energie
) = range(5)


from collections import namedtuple

# Repr�sente une position sur le terrain du jeu
position = namedtuple("position",
    'x ' # <- coordonn�e en X
    'y ' # <- coordonn�e en Y
)


# Caracteristiques d'une source d'�nergie
source_energie = namedtuple("source_energie",
    'id ' # <- identifiant de la source d'�nergie
    'pos ' # <- position de la source d'�nergie
    'capacite ' # <- coefficient repr�sentant les points d'�nergie que la source va vous apporter
    'capacite_max ' # <- coefficient repr�sentant la capacit� de la source lorsqu'elle est charg�e au maximum
)


# Repr�sente une tra�n�e de moto sur le terrain
trainee_moto = namedtuple("trainee_moto",
    'id ' # <- identifiant de la tra�nee
    'emplacement ' # <- position de chaque composant de la tra�n�e de moto
    'team ' # <- identifiant de l'�quipe qui poss�de cette tra�n�e de moto
    'longueur ' # <- taille maximale de la tra�n�e
)
