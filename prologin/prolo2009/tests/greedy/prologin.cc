///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_cxx.rb
//

#include <iostream>
#include <deque>
#include <list>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include "prologin.hh"

using namespace std;

#define MAX_HULK 5

int display(int x, int y)
{
  const int M = TAILLE_CARTE / 2;
  char output[100] = {0};
  output[ROUTE] = '|';
  output[MONUMENT] = 'M';
  output[RESERVATION] = 'R';
  output[VIDE] = ' ';

  cout << "#" << numero_tour() << endl;
  for (int i = 35; i < 65; ++i) {
    for (int j = 35; j < 65; ++j) {
      if (i == y && j == x)
      {
	cout << "#";
	continue;
      }

      if (i == M && j == M)
      {
	cout << "*";
	continue;
      }

      int k = type_case(j, i);
      if (k != MAISON)
	cout << output[k];
      else
	cout << appartenance(j, i);
    }
    cout << endl;
  }
}

static const int d4[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
static const int d8[8][2] = {{0,1},{0,-1},{1,0},{-1,0},
			     {1,1},{1,-1},{-1,-1},{-1,1}};
static const int d16[16][2] = {{-2,-2},{2,1},{-2,0},{-2,1},
			       {-1,-2},{0,2},{-1,2},{0,-2},
			       {1,-2},{-2,2},{-2,-1},{2,0},
			       {1,2},{2,-2},{2,-1},{2,2}};
static const int d24[24][2] = {{-3,-3},{-3,-2},{-3,-1},{-3,0},{-3,1},{3,1},
			       {-3,2},{-3,3},{-2,-3},{-2,3},{-1,-3},{3,2},
			       {-1,3},{0,-3},{0,3},{1,-3},{1,3},{2,-3},
			       {2,3},{3,-3},{3,-2},{3,-1},{3,0},{3,3}};

struct point
{
  int x;
  int y;
};

struct position
{
  int x;
  int y;
  std::list<point> l;
};

static bool dans_les_bornes(int y, int x)
{
  return y >= 0 && x >= 0 && x < TAILLE_CARTE && y < TAILLE_CARTE;
}

static bool vend_une_maison()
{
  int x = -1;
  int y = -1;
  int v = -1;

  for (int i = 0; i < TAILLE_CARTE; ++i)
    for (int j = 0; j < TAILLE_CARTE; ++j)
    {
      if (type_case(j, i) != MAISON || appartenance(j, i) != 0)
	continue;

      int v2 = -1;
      if (valeur_case(j, i) > 1)
      {
        for (unsigned k = 0; k < 8; ++k)
          if (dans_les_bornes(i + d8[k][0], j + d8[k][1])
	      && type_case(i + d8[k][0], j + d8[k][1]) != VIDE)
            ++v2;
      }
      if (v2 > v)
      {
        v = v2;
        x = j;
        y = i;
      }
    }

  return vendre_maison(x, y) == SUCCES;
}

static void anti_hulk()
{
  for (int i = 0; i < TAILLE_CARTE; ++i)
    for (int j = 0; j < TAILLE_CARTE; ++j)
    {
      if (type_case(j, i) != MAISON || appartenance(j, i) != 0)
	continue;

      if (valeur_case(j, i) > MAX_HULK)
	vendre_maison(j, i);
    }
}

static int place_une_maison(int nb_free_roads, int bonus)
{
  int x = -1;
  int y = -1;
  int v = -12;
  std::list<point> l;
  std::list<point> empty;

  int max_roads = ((finances(0) - cout_achat_maison()) << 1) + nb_free_roads;

  for (int i = 0; i < TAILLE_CARTE; ++i)
    for (int j = 0; j < TAILLE_CARTE; ++j)
    {
      int nb = std::abs((int) l.size() - nb_free_roads + 1) >> 1;
      if (v - nb >= valeur_case(j, i))
	continue;

      if (type_case(j, i) == MAISON
      	  && appartenance(j, i) != 0 && appartenance(j, i) != MAIRIE)
        if (v - nb < valeur_case(j, i) / 2 - cout_achat_maison()
      	    && finances(0) >= valeur_case(j, i) - 1)
        {
          x = j;
          y = i;
	  v = valeur_case(j, i) / 2 - cout_achat_maison();
      	  l = empty;
      	  continue;
        }

      if (construction_possible(j, i) == SUCCES)
      {
	x = j;
	y = i;
	v = valeur_case(j, i);
	l = empty;
	continue;
      }

      if (construction_possible(j, i) == NON_CONNEXE)
      {
	int n = ((valeur_case(j, i) - v) << 1) + l.size();
	assert(n >= 2);
	n = std::min(n + bonus, max_roads);

	bool visited[TAILLE_CARTE][TAILLE_CARTE];

	std::deque<position> deque;
	for (int k = 0; k < 8; k++)
	  deque.push_back(
	    (position) {j + d8[k][0], i + d8[k][1], std::list<point>()});

        while (!deque.empty())
	{
	  const position p = deque.front();
	  deque.pop_front();
	  if (!dans_les_bornes(p.x, p.y))
	    continue;

	  int type = type_case(p.x, p.y);
	  if (type != VIDE && type != ROUTE)
	    continue;

	  if (type == ROUTE || route_possible(p.x, p.y) == SUCCES)
	  {
	    x = j;
	    y = i;
	    v = valeur_case(j, i);
	    l = p.l;
	    l.push_front((point) {p.x, p.y});
	    break;
	  }

	  assert(type == VIDE);
	  if (visited[p.x][p.y])
	    continue;
	  visited[p.x][p.y] = true;

	  if (p.l.size() < n)
	    for (int k = 0; k < 4; k++)
	    {
	      position next = { p.x + d4[k][0], p.y + d4[k][1], p.l };
	      next.l.push_front((point) {p.x, p.y});
	      deque.push_back(next);
	    }
	}
      }
    }

  for (std::list<point>::iterator i = l.begin(); i != l.end(); ++i)
    construire_route(i->x, i->y);

  if (type_case(x, y) == MAISON)
  {
    detruire_maison(x, y);
    std::cout << construire_maison(x, y) << std::endl;
    vendre_maison(x, y);
  }
  else
  {
    std::cout << construire_maison(x, y) << std::endl;
    if (valeur_case(x, y) > MAX_HULK)
      vendre_maison(x, y);
  }

  display(x, y);
  return l.size();
}

static void place_le_monument()
{
  int m = monument_en_cours();
  int p = prestige_monument(m);
  int x = -1;
  int y = -1;
  int v = -100;
  std::list<point> l;
  std::list<point> empty;

  int max_roads = finances(0) << 1;

  std::vector<const int (*)[2]> ds(3);
  ds[0] = d8;
  ds[1] = d16;
  ds[2] = d24;

  for (int i = 0; i < TAILLE_CARTE; ++i)
    for (int j = 0; j < TAILLE_CARTE; ++j)
    {
      if (type_case(j, i) != VIDE)
	continue;

      int v2 = 0;
      for (int n = 0; n < portee_monument(m); ++n)
	for (int k = 0; k < 8; ++k)
	{
	  int i2 = i + ds[n][k][0];
	  int j2 = j + ds[n][k][1];
	  if (!dans_les_bornes(j2, i2))
	    continue;

	  if (type_case(j2, i2) == MAISON && appartenance(j2, i2) != MAIRIE)
	    v2 += p * ((appartenance(j2, i2) == 0) ? 1 : -1);
	}

      int nb = std::abs((int) l.size() + 1) >> 1;;
      if (v - nb >= v2)
	continue;

      if (construction_possible(j, i) == SUCCES)
      {
	v = v2;
	x = j;
	y = i;
	l = empty;
	continue;
      }

      if (construction_possible(j, i) == NON_CONNEXE)
      {
	int n = ((v2 - v) << 1) + l.size();
	assert(n >= 1);
	n = std::min(n, max_roads);

	bool visited[TAILLE_CARTE][TAILLE_CARTE];

	std::deque<position> deque;
	for (int k = 0; k < 8; k++)
	  deque.push_back(
	    (position) {j + d8[k][0], i + d8[k][1], std::list<point>()});

        while (!deque.empty())
	{
	  const position p = deque.front();
	  deque.pop_front();
	  if (!dans_les_bornes(p.x, p.y))
	    continue;

	  int type = type_case(p.x, p.y);
	  if (type != VIDE && type != ROUTE)
	    continue;

	  if (type == ROUTE || route_possible(p.x, p.y) == SUCCES)
	  {
	    x = j;
	    y = i;
	    l = p.l;
	    l.push_front((point) {p.x, p.y});
	    break;
	  }

	  assert(type == VIDE);
	  if (visited[p.x][p.y])
	    continue;
	  visited[p.x][p.y] = true;

	  if (p.l.size() < n)
	    for (int k = 0; k < 4; k++)
	    {
	      position next = { p.x + d4[k][0], p.y + d4[k][1], p.l };
	      next.l.push_front((point) {p.x, p.y});
	      deque.push_back(next);
	    }
	}
      }
    }

  for (std::list<point>::iterator i = l.begin(); i != l.end(); ++i)
    construire_route(i->x, i->y);

  construire_monument(x, y);
}

///
// Fonction appelee au debut de la partie
//
void init_game()
{
  srand(time(NULL));
}

///
// Fonction appelee a la fin de la partie
//
void end_game()
{
  std::cout << "Score 1= " << score(0) + finances(0) << std::endl
	    << "Score 2= " << score(1) + finances(1) << std::endl
	    << "Score 3= " << score(2) + finances(2) << std::endl;
}

///
// Fonction appelee a chaque phase de jeu
//
void jouer()
{
  if (!mon_tour())
    return;

  display(0, 0);

  bool b = true;
  while (finances(0) < 5 && b)
    b = vend_une_maison();

  int i = place_une_maison(1, 2);

  b = true;
  while (finances(0) < 5 && b)
    b = vend_une_maison();

  place_une_maison(i % 2 == 0 ? 2 : 1, 0);
}

///
// Fonction appelee a chaque phase d'enchere
//
void enchere()
{
  int i = std::abs(prestige_monument(monument_en_cours()));
  encherir(std::min(i - 1, finances(0) - 1));
}

///
// Fonction appelee a chaque phase de placement de monument
//
void placement()
{
  if (!mon_tour())
    return;
  place_le_monument();
  anti_hulk();
}
