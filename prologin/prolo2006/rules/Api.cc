/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2006 Prologin
*/

#include <stdlib.h>

#include "Api.hh"
#include "Astar.hh"
#include "GameData.hh"

// global used in interface.cc
Api* api;

Api::Api(GameData* gameData, Client* c) :
  StechecApi(gameData, c)
{
  api = this;
}

int Api::equipes()
{
  return g_->getNbPlayer();
}

int Api::taille_equipe()
{
  return MAX_GOODMEN;
}

int Api::mon_equipe()
{
  return g_->getUid();
}

int Api::taille_ville_x()
{
  return g_->map_size.col;
}

int Api::taille_ville_y()
{
  return g_->map_size.row;
}

// vic: retourner UNKNOWN si pas possible.
// balt: FIXED
int Api::distance(int x_d, int y_d, int x_a, int y_a)
{
  TEST_POS(x_d, y_d);
  TEST_POS(x_a, y_a);
  if (g_->terrain_type[y_d][x_d] == WALL ||
      g_->terrain_type[y_a][x_a] == WALL ||
      g_->terrain_type[y_d][x_d] == BET_PLACE ||
      (g_->terrain_type[y_a][x_a] == BET_PLACE &&
       (g_->getUid() >= 0 && c_->player_fog[g_->getUid()][y_a][x_a] == 0)))
    return UNKNOWN;
  std::pair<char, short> res;
  Position start(y_d, x_d);
  Position end(y_a, x_a);

  res = astar(&start, &end);
  if (res.second == 0 && x_d != x_a)
      return UNKNOWN;
  return res.second;
}

// vic: retourner les cst INC/DEC_X/Y, ou UNKNOWN si pas possible.
// balt: FIXED
int Api::meilleure_direction(int x_d, int y_d, int x_a, int y_a)
{
  TEST_POS(x_d, y_d);
  TEST_POS(x_a, y_a);
  if (g_->terrain_type[y_d][x_d] == WALL ||
      g_->terrain_type[y_a][x_a] == WALL ||
      g_->terrain_type[y_d][x_d] == BET_PLACE ||
      (g_->terrain_type[y_a][x_a] == BET_PLACE
       && (g_->getUid() >= 0 && c_->player_fog[g_->getUid()][y_a][x_a] == 0)))
    return UNKNOWN;

  std::pair<char, short> res;
  Position start(y_d, x_d);
  Position end(y_a, x_a);

  res = astar(&start, &end);
  if (res.second == 0 && x_d != x_a)
      return UNKNOWN;
  return res.first;
}

// vic: voir si
int Api::regarde(int x, int y)
{
  TEST_POS(x, y);
  if (g_->terrain_type[y][x] == STANDARD)
    {
      for (int id = 0; id < g_->getNbPlayer(); ++id)
	for (int i = 0; i < MAX_GOODMEN; ++i)
	  if (g_->getUid() < 0 || g_->players[id].team[i].row == y &&
	      g_->players[id].team[i].col == x &&
	      c_->player_fog[g_->getUid()][y][x]
	      && g_->players[id].team[i].getState() != STATE_BACK_IN_THE_FUTURE)
	    return GOODMAN;
      return STANDARD;
    }
  else if (g_->terrain_type[y][x] == BET_PLACE &&
	   (g_->getUid() >=  0 && c_->player_fog[g_->getUid()][y][x] == 0))
    return WALL;
   else
    return g_->terrain_type[y][x];
}

// vic: fonction ajoutee. TODO
// balt: FIXED
// ghalaax: FIXED verif x, y
int Api::visible(int x, int y)
{
  TEST_POS(x, y);
  if (g_->getUid() >= 0 &&
      c_->player_fog[g_->getUid()][y][x] == 0)
    return 0;
  return 1;
}

// vic: fonction ajoutee. TODO


int Api::visible_depuis(int from_x, int from_y, int to_x, int to_y)
{
  TEST_POS(from_x, from_y);
  TEST_POS(to_x, to_y);
  return g_->visibility[from_y][from_x][to_y][to_x];
}

//id : player_id
int Api::score(int id)
{
  if (id >= g_->getNbPlayer() || id < 0)
    return BAD_ARGUMENT;
  return g_->players[id].getScore();
}

int Api::current_date()
{
  return g_->player_turn;
}

int Api::almanach_visible()
{
  int	i;
  int	id;

  if (g_->getUid() >= 0 &&
      c_->player_fog[g_->getUid()][g_->almanach.row][g_->almanach.col] == 0)
    return 0;
    for (i = 0; i < g_->getNbPlayer(); ++i)
    for (id = 0; id < MAX_GOODMEN; ++id)
      if (g_->players[i].team[id].hasAlmanach())
	{
	  if (g_->players[i].team[id].getState() == STATE_BACK_IN_THE_FUTURE)
	    return 0;
	  return 1;
	}
  return 1;
}

int Api::position_almanach_x()
{
  int	i;
  int	id;

  IF_FOG(g_->almanach.col, g_->almanach.row);
  for (i = 0; i < g_->getNbPlayer(); ++i)
    for (id = 0; id < MAX_GOODMEN; ++id)
      if (g_->players[i].team[id].hasAlmanach())
	{
	  if (g_->players[i].team[id].getState() == STATE_BACK_IN_THE_FUTURE)
	    return UNKNOWN;
	  return g_->almanach.col;
	}
  return g_->almanach.col;
}

int Api::position_almanach_y()
{
  int	i;
  int	id;

  IF_FOG(g_->almanach.col, g_->almanach.row);
  for (i = 0; i < g_->getNbPlayer(); ++i)
    for (id = 0; id < MAX_GOODMEN; ++id)
      if (g_->players[i].team[id].hasAlmanach())
	{
	  if (g_->players[i].team[id].getState() == STATE_BACK_IN_THE_FUTURE)
	    return UNKNOWN;
	  return g_->almanach.row;
	}
  return g_->almanach.row;
}

// balt: FIXME : verif du droit de regard
// ghalaax: FIXED...
int Api::qui_possede_almanach()
{
  for (int id = 0; id < g_->getNbPlayer(); ++id)
    for (int i = 0; i < MAX_GOODMEN; ++i)
      if (g_->players[id].team[i].hasAlmanach())
	{
	  if (g_->getUid() < 0 ||
	      c_->player_fog[id]
	      [g_->players[id].team[i].row]
	      [g_->players[id].team[i].col] &&
	      g_->players[id].team[i].getState() != STATE_BACK_IN_THE_FUTURE)
	    return id;
	  else
	    return UNKNOWN;
	}
  return NO_UNIT;
}

// balt: FIXED
// balt: FIXME : verif du droit de regard
int Api::joueur_possede_almanach()
{
  for (int id = 0; id < g_->getNbPlayer(); ++id)
    for (int i = 0; i < MAX_GOODMEN; ++i)
      if (g_->players[id].team[i].hasAlmanach())
	{
	  if (g_->getUid() < 0 ||
	      c_->player_fog[id]
	      [g_->players[id].team[i].row]
	      [g_->players[id].team[i].col] &&
	      g_->players[id].team[i].getState() != STATE_BACK_IN_THE_FUTURE)
	    return GOOD_FROM_IDS(id, i);
	  else
	    return UNKNOWN;
	}
  return UNKNOWN;
}

int Api::delorean_visible ()
{
  if (g_->getUid() >= 0 &&
      c_->player_fog[g_->getUid()][g_->delorean.row][g_->delorean.col] == 0)
    return 0;
  if (g_->delorean.warpTime() > g_->player_turn)
    return 0;
  return 1;
}

int Api::position_delorean_x()
{
  IF_FOG(g_->delorean.col, g_->delorean.row);
  if (g_->delorean.warpTime() > g_->player_turn)
    return UNKNOWN;
  return g_->delorean.col;
}

int Api::position_delorean_y()
{
  IF_FOG(g_->delorean.col, g_->delorean.row);
  if (g_->delorean.warpTime() > g_->player_turn)
    return UNKNOWN;
  return g_->delorean.row;
}

int Api::autonomie_delorean()
{
  // Spectators have always the right to see that.
  if (g_->getUid() < 0)
    return g_->delorean.getAutonomie();
  for (int i = 0; i < MAX_GOODMEN; ++i)
    if (g_->p->team[i].hasDelorean())
      return g_->delorean.getAutonomie();
  return UNKNOWN;
}

int Api::qui_possede_delorean()
{
  for (int id = 0; id < g_->getNbPlayer(); ++id)
    for (int i = 0; i < MAX_GOODMEN; ++i)
      if (g_->players[id].team[i].hasDelorean())
	{
	  if (g_->getUid() < 0 || c_->player_fog[id]
	      [g_->players[id].team[i].row]
	      [g_->players[id].team[i].col] &&
	      g_->players[id].team[i].getState() != STATE_BACK_IN_THE_FUTURE)
	    return id;
	  else
	    return UNKNOWN;
	}
  return NO_UNIT;
}

// vic: FIXME: voir yaml.
//ghalaax: FIXED
int Api::joueur_possede_delorean()
{
  for (int id = 0; id < g_->getNbPlayer(); ++id)
    for (int i = 0; i < MAX_GOODMEN; ++i)
      if (g_->players[id].team[i].hasDelorean())
	{
	  if (g_->getUid() < 0 || c_->player_fog[id]
	      [g_->players[id].team[i].row]
	      [g_->players[id].team[i].col] &&
	      g_->players[id].team[i].getState() != STATE_BACK_IN_THE_FUTURE)
	    return GOOD_FROM_IDS(id, i);
	  else
	    return UNKNOWN;
	}
  return NO_UNIT;
}


//goodman id
int Api::joueur_visible (int id)
{
  if (!ID_EXISTS(id))
    return BAD_ARGUMENT;
  if (g_->getUid() < 0 || c_->player_fog[g_->getUid()]
      [g_->players[GET_TEAM(id)].team[GET_GOOD(id)].row]
      [g_->players[GET_TEAM(id)].team[GET_GOOD(id)].col])
    return 1;
  return 0;
}

//goodman id
int Api::equipe(int id)
{
  if (!ID_EXISTS(id))
    return BAD_ARGUMENT;
  return GET_TEAM(id);
}

//goodman id
int Api::position_joueur_x(int id)
{
  if (!ID_EXISTS(id))
    return BAD_ARGUMENT;
  if (g_->getUid() < 0)
    return g_->players[GET_TEAM(id)].team[GET_GOOD(id)].col;
  if (g_->players[GET_TEAM(id)].team[GET_GOOD(id)].getState() == STATE_BACK_IN_THE_FUTURE)
    return UNKNOWN;
  IF_FOG(g_->players[GET_TEAM(id)].team[GET_GOOD(id)].col,
	 g_->players[GET_TEAM(id)].team[GET_GOOD(id)].row);
  return g_->players[GET_TEAM(id)].team[GET_GOOD(id)].col;
}

//goodman id
int Api::position_joueur_y(int id)
{
  if (!ID_EXISTS(id))
    return BAD_ARGUMENT;
  if (g_->getUid() < 0)
    return g_->players[GET_TEAM(id)].team[GET_GOOD(id)].row;
  if (g_->players[GET_TEAM(id)].team[GET_GOOD(id)].getState() == STATE_BACK_IN_THE_FUTURE)
    return UNKNOWN;
  IF_FOG(g_->players[GET_TEAM(id)].team[GET_GOOD(id)].col,
	 g_->players[GET_TEAM(id)].team[GET_GOOD(id)].row);
  return g_->players[GET_TEAM(id)].team[GET_GOOD(id)].row;
}

//goodman id
int Api::etat_joueur(int id)
{
  if (!ID_EXISTS(id))
    return BAD_ARGUMENT;
  IF_FOG(g_->players[GET_TEAM(id)].team[GET_GOOD(id)].col,
	 g_->players[GET_TEAM(id)].team[GET_GOOD(id)].row);
  return g_->players[GET_TEAM(id)].team[GET_GOOD(id)].getState();
}

// vic: FIXME: test fog avant (retourner UNKNOWN),
// et retourner INVALID_COMMAND si pas BET_PLACE
// balt: FIXED
int Api::casino(int x, int y)
{
  TEST_POS(x, y);
  IF_FOG(x, y);
  if (g_->terrain_type[y][x] == BET_PLACE)
    {
      for (unsigned i = 0; i < g_->casinos.size(); ++i)
	if (g_->casinos[i]->row == y && g_->casinos[i]->col == x)
	  {
	    return g_->casinos[i]->get_id();
	  }
    }
  else
    return INVALID_COMMAND;
  return UNKNOWN;
}

// vic: FIXME: verifier les valeurs de retour par rapport au yaml.
// balt: FIXED: UNKNOWN --> BAD_ARGUMENT si id faux
int Api::casino_ouvert(int id)
{
  if (id < 0)
    return BAD_ARGUMENT;
  if ((unsigned)id >= g_->casinos.size())
    return BAD_ARGUMENT;
  IF_FOG(g_->casinos[id]->col, g_->casinos[id]->row);
  if (g_->almanach.getGameOfCasino(id) != UNKNOWN)
    return 1;
  return 0;
}

//balt: FIXME: verifier si casino est ouvert
//ghalaax: Already FIXED in GameData.hh
int Api::jeu_du_casino(int id)
{
  if (id < 0)
    return BAD_ARGUMENT;
  if ((unsigned)id >= g_->casinos.size())
    return BAD_ARGUMENT;
  IF_FOG(g_->casinos[id]->col, g_->casinos[id]->row);
  return g_->almanach.getGameOfCasino(id);
}

int Api::jeux()
{
  for (int i = 0; i < MAX_GOODMEN; ++i)
    if (g_->getUid() < 0 || g_->p->team[i].hasAlmanach())
      return g_->almanach.getGameNumber();
  return UNKNOWN;
}

int Api::cagnotte(int id)
{
  if (id < 0 ||
      (unsigned)id >= g_->almanach.getAlmanach().size())
    return BAD_ARGUMENT;
 for (int i = 0; i < MAX_GOODMEN; ++i)
    if (g_->getUid() < 0 ||
	g_->p->team[i].hasAlmanach() || g_->p->knowGame(id))
      return g_->almanach.getGameKitty(id);
 return UNKNOWN;
}

int Api::casino_du_jeu(int id)
{
  if (id < 0 ||
      (unsigned)id >= g_->almanach.getAlmanach().size())
    return BAD_ARGUMENT;
  for (int i = 0; i < MAX_GOODMEN; ++i)
    if (g_->getUid() < 0 || g_->p->team[i].hasAlmanach() || g_->p->knowGame(id))
      return g_->almanach.getCasinoOfGame(id);
  return UNKNOWN;
}

int Api::date_jeu(int id)
{
  if (id < 0 ||
      (unsigned)id >= g_->almanach.getAlmanach().size())
    return BAD_ARGUMENT;
  for (int i = 0; i < MAX_GOODMEN; ++i)
    if (g_->getUid() < 0 || g_->p->team[i].hasAlmanach()
	|| g_->p->knowGame(id))
      return g_->almanach.getGameDate(id);
  return UNKNOWN;
}

int Api::memorise_jeu(int id)
{
  if (id < 0 ||
      (unsigned)id >= g_->almanach.getAlmanach().size())
    return BAD_ARGUMENT;

  for (int i = 0; i < MAX_GOODMEN; ++i)
    if (g_->p->team[i].hasAlmanach())
      {
	TEST_ID_ACTION(GET_GOOD(i));
	if (g_->p->team[i].getState() != STATE_NORMAL)
	  return INVALID_COMMAND;
	g_->p->team[i].action = true;
	StechecPkt com(MEMORIZE, -1);
	com.Push(1, id);
	SendToServer(com);
	return SUCCESS;
      }
  return UNKNOWN;
}

int Api::convecteur_temporel(int id, int date)
{
  if (date < 0 || !ID_EXISTS(id))
    return BAD_ARGUMENT;
  TEST_ID_ACTION(GET_GOOD(id));
  GoodMan&	gdm = g_->p->team[GET_GOOD(id)];

  if (g_->delorean.reloadingTimeLeft () != 0)
    return INVALID_COMMAND;
  if (gdm.hasDelorean() == false)
    return INVALID_COMMAND;
  if (gdm.getState() != STATE_NORMAL)
    return INVALID_COMMAND;
  StechecPkt com(DELOREAN, -1);
  com.Push(2, date, GET_GOOD(id));
  SendToServer(com);
  gdm.action = true;
  return SUCCESS;
}

int		Api::deplace_joueur(int id, int x, int y)
{
  LOG3(" Asking for moving goodman %1", id);
  if (!ID_EXISTS(id))
    return BAD_ARGUMENT;
  TEST_POS(x, y);
  TEST_ID_ACTION(GET_GOOD(id));

  GoodMan&	gdm = g_->p->team[GET_GOOD(id)];
  unsigned	x2 = gdm.col;
  unsigned	y2 = gdm.row;
  unsigned	i;
  unsigned	xr, yr;
  int		dir;

  Position start(y2, x2);
  Position end(y, x);

  TEST_POS(x, y);
  if (gdm.getState() != STATE_NORMAL)
    return INVALID_COMMAND;
  LOG3("try to move GoodMan row: %1 column: %2, from row: %3 col: %4",
       y, x, y2, x2);
//   if ((x == x2 && abs(y - y2) > 1) || (y == y2 && abs(x - x2) > 1)
//       || (x != x2 && y != y2))
//     return BAD_ARGUMENT;

  if (g_->terrain_type[y][x] == WALL)
    return INVALID_COMMAND;
  dir = astar(&start, &end).first;
  if (dir < 0)
    return INVALID_COMMAND;
  xr = x2 + (dir == INC_X ? 1 : (dir == DEC_X ? -1 : 0));
  yr = y2 + (dir == INC_Y ? 1 : (dir == DEC_Y ? -1 : 0));
  LOG3("Dir found : %1 gives x : %2, y : %3", (int)dir, xr, yr);
  assert(g_->terrain_type[yr][xr] != WALL);

  for (i = 0; i < g_->casinos.size (); ++i)
    if (yr == g_->casinos[i]->row && xr == g_->casinos[i]->col)
      {
	if (casino_ouvert(i) == 0)
	  {
	    LOG3("Casino not opened");
	    return INVALID_COMMAND;
	  }
	break;
      }

  LOG3("Api: move GoodMan row: %1, column: %2", yr, xr);
  StechecPkt com(MOVE_GOODMAN, -1);
  com.Push(3, GET_GOOD(id), yr, xr);
  SendToServer(com);

  gdm.action = true;
  return SUCCESS;
}

Astar*	isInList(std::list<Astar>& l, int row, int col)
{
  std::list<Astar>::iterator	i;

  for (i = l.begin(); i != l.end(); ++i)
    if ((*i).cur.row == row && (*i).cur.col == col)
      return &(*i);
  return 0;
}


std::pair<char, short>	Api::astar(Position *start, Position *end)
{
  std::list<Astar>	open;
  std::list<Astar>	closed;
  Astar			shortest;
  Astar*		tmp;
  Astar			*suc;
  Astar			end_n = Astar(end->row, end->col);
  std::list<Astar>::iterator	i;
  int			it = 0;

  open.push_back(Astar(start->row, start->col));
  while (!open.empty())
    {
      if (it >= (g_->map_size.row * g_->map_size.col))
	return std::pair<char, short>(-1, 0);
      ++it;

      end_n.findShort(open, shortest);
      if (shortest.cur.row == end->row && shortest.cur.col == end->col)
	{
	  tmp = &shortest;
	  return tmp->astar_res();
	}
      closed.push_back(shortest);
      tmp = &closed.back();
      for (char i = 0; i < 4; ++i)
	{
	  unsigned	row = tmp->cur.row + (i == 1 ? 1 : (i == 3 ? -1 : 0));
	  unsigned	col = tmp->cur.col + (i == 0 ? -1 : (i == 2 ? 1 : 0));
	  if (row >= g_->map_size.row || col >= g_->map_size.col)
	    continue;
	  // Pour eviter que les chemins passent par des BET_PLACES
	  // A part pour si c'est la destination
	  if (g_->terrain_type[row][col] != WALL &&
	      ((row == end->row && col == end->col) ||
	       g_->terrain_type[row][col] != BET_PLACE))
	    {
	      if ((suc = isInList(open, row, col)) != 0)
		{
		  if (suc->g < tmp->g + 1)
		    {
		      suc->parent = tmp;
		      suc->g = tmp->g + 1;
		    }
		}
	      else
		open.push_back(Astar(row, col, tmp, tmp->g + 1, end_n.distance(row, col)));
	    }
	}
    }
  if (open.empty())
    return std::pair<char, short>(-1, 0);
  return tmp->astar_res();
}


int Api::terrain_graphique(int x, int y)
{
  TEST_POS(x, y);
  //fixme fog ? not sure.. ghalaax: I feel the same...
  return g_->terrain_graphic[y][x];
}

// See comments in prologin/common/StechecApi.hh
void Api::teamSwitched()
{
  // Shortcut to get the selected team for viewers.
  if (g_->getUid() >= 0)
    g_->p = &g_->players[g_->getUid()];
  else
    g_->p = NULL;
}
