# This file has been generated, if you wish to
# modify it in a permanent way, please refer
# to the script file : gen/generator_python.rb

import api

directions4 = [[0,1],[0,-1],[1,0],[-1,0]]

INFINI = 30000
HORS_TERRAIN = -12
PAS_DE_MAISON = -2
PAS_DE_MONUMENT = -3
FINANCES_DEPASSEES = -4
BLOCAGE = -5
JOUEUR_INCORRECT = -7
NON_CONNEXE = -8
CASE_OCCUPEE = -9
ACTION_INTERDITE = -10
TROP_LOIN = 50
SUCCES = 0
MAIRIE = 3
VIDE = 0
MAISON = 1
RESERVATION = 2
MONUMENT = 3
ROUTE = 4
MAX_MONUMENTS = 14
TAILLE_CARTE = 100
MAX_DISTANCE = 12

def vendre_pour(besoin_argent) :
	if besoin_argent <= 0 :
		return
	maisons = []
	for i in range(TAILLE_CARTE):
		for j in range(TAILLE_CARTE):
			v = api.valeur_case(j,i)
			maisons.append((v,i,j))
	maisons.sort()
	while besoin_argent > 0 and len(maisons) > 0 :
		m = maisons.pop()
		besoin_argent -= m[0] + 1
		api.vendre_maison(m[2], m[1])


def plus_proche_route(c):
	best_place = ()
	mincost = 1000
	how_far = 6
	for i in range(c[1]-how_far, c[1]+how_far+1):
		for j in range(c[2]-how_far, c[2]+how_far+1):
			if api.type_case(j,i) != ROUTE:
				continue
			d = api.distance(j,i,c[2],c[1])
			if d > 0 and d < mincost:
				mincost = d
				best_place = (i,j)
	return mincost


def Build(c, what_to_build):
	if api.type_case(c[2],c[1]) != VIDE:
		return False
	best_place = (-1,-1)
	mincost = 1000
	for i in range(TAILLE_CARTE):
		for j in range(TAILLE_CARTE):
			if api.type_case(j,i) != ROUTE:
				continue
			d = api.distance(j,i,c[2],c[1])
			if d > 0 and d < mincost:
				mincost = d;
				best_place = (i,j)
				
	assert best_place[0] > 0
	
	while max(abs(c[1]-best_place[0]), abs(c[2]-best_place[1])) > 1:
		if api.finances(0) == 0:
			vendre_pour(api.cout_achat_route())
		best = api.distance(best_place[1],best_place[0],c[2],c[1])
		bestdir = 0
		for dir in directions4:
			d = api.distance(best_place[1] + dir[1], best_place[0] + dir[0], c[2], c[1])
			if d < best:
				best = d;
				bestdir = dir		
		if best >= TROP_LOIN:
			return False
		best_place = (best_place[0] + bestdir[0], best_place[1] + bestdir[1])
		api.construire_route(best_place[1], best_place[0])
	
	if what_to_build == "maison":
		vendre_pour(api.cout_achat_route() - api.finances(0))
		api.construire_maison(c[2],c[1])
	else:
		assert what_to_build == "monument"
		api.construire_maison(c[2],c[1])
	return True


def jouer():
	if not api.mon_tour():
		return
	pos = []
	for i in range(TAILLE_CARTE):
		for j in range(TAILLE_CARTE):			
			if api.type_case(j,i) == VIDE or api.type_case(j,i) == RESERVATION \
				    and api.appartenance(j,i) == 0 and api.construction_possible(j,i) != BLOCAGE:
				v = api.valeur_case(j,i) - plus_proche_route((0,i,j));
				pos.append((v, i, j));
	pos.sort()
	for k in range(2):
		c = pos.pop()
		Build(c, "maison")
	if api.numero_tour() == 15:
		print "******** Python Final score = ", api.score(0) + api.finances(0), " **********"


def enchere():
	api.encherir(1 + api.prestige_monument(api.monument_en_cours()) / 3)


def placement():
	if not api.mon_tour():
		return
	bestval = 0
	bestpos = (-1,-1)
	for i in range(TAILLE_CARTE):
		for j in range(TAILLE_CARTE):
			if api.type_case(j,i) == VIDE and api.construction_possible(j,i) != BLOCAGE:
				val = -plus_proche_route((0,i,j))/2
				r = api.portee_monument(api.monument_en_cours())
				prestige = api.prestige_monument(api.monument_en_cours())
				for y in range(i-r,i+r+1):	
					for x in range(j-r,j+r+1):
						if not api.type_case(x,y) == MAISON:
							continue
						if api.appartenance(x,y) == 0:
							val += prestige
						else:
							val -= prestige
				if val > bestval:
					bestval = val
					bestpos = (i,j)
	if bestpos[0] < 0:
		return
	Build((0,) + bestpos, "monument")

# Fonction appelee au debut de la partie
def init_game():
	pass # Place ton code ici

# Fonction appelee a la fin de la partie
def end_game():
	pass # Place ton code ici
