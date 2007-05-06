/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2005, 2006 Prologin
*/

#include <stdlib.h>

#include "Api.hh"

// global used in interface.cc
Api* api;

Api::Api(GameData* gameData, Client* c) :
  StechecApi(gameData, c)
{
  api = this;
}

int Api::equipes()
{
  LOG1("Nb players connected : %1\n", g_->getNbPlayer ());
  return NB_TEAM(g_->getNbPlayer ());
}

int Api::taille_equipe(int team_number)
{
  int count = 0;
  if (!TEAM_EXISTS(team_number))
    return BAD_ARGUMENT;
  for (int i = team_number * MAX_WHITE_CELL;
       i < (team_number + 1) * MAX_WHITE_CELL; ++i)
    if (g_->players[i].getState () != STATE_DEAD)
      count++;
  return count;
}

int Api::mon_equipe()
{
  LOG1("GetUid : %1 wrapped uid : %2", g_->getUid (),
       g_->GetWrapUid ());
  return g_->GetWrapUid () / MAX_WHITE_CELL;
}

int Api::mon_identifiant()
{
  return g_->GetWrapUid ();
}

int Api::taille_corps_x()
{
  return g_->map_size.col;
}

int Api::taille_corps_y()
{
  return g_->map_size.row;
}

int Api::bacteries_presentes (int x, int y)
{
  TEST_POS (x, y);
  IF_FOG(x, y);
  if (g_->bacterias[y][x])
    return 1;
  return 0;
}

int Api::regarde(int x, int y)
{
  TEST_POS(x, y);
  if (FOG(x, y))
    {
      return g_->terrain_type[y][x];
    }
  for (int i = 0; i < g_->getNbPlayer (); ++i)
    {
      if (g_->players[i].getX () == x && g_->players[i].getY () == y)
	return WHITE_CELL;
    }
  for (int i = 0; i < g_->_virus.size (); ++i)
    if (g_->_virus[i]->getX () == x && g_->_virus[i]->getY () == y)
      return VIRUS;
  for (std::vector<Cellule*>::iterator it = g_->_cells.begin ();
       it != g_->_cells.end (); ++it)
    if ((*it)->col == x && (*it)->row == y)
      return CELL;
  return g_->terrain_type[y][x];
}

int Api::visible(int x, int y)
{
  TEST_POS(x, y);
  if (FOG(x, y))
    return 0;
  return 1;
}

int Api::score(int id)
{
  int res = 0;

  if (id >= NB_TEAM(g_->getNbPlayer()) || id < 0)
    return BAD_ARGUMENT;
  for (int i = id * MAX_WHITE_CELL; i < (id + 1) * MAX_WHITE_CELL; ++i)
    res += g_->players[id].getScore();
  return res;
}

int Api::current_beat()
{
  return g_->player_turn;
}

int Api::leucocyte_visible(int id)
{
  if (id < 0 || id >= g_->getNbPlayer ())
    return BAD_ARGUMENT;
  if (FOG(g_->players[id].getX(), g_->players[id].getY()))
    return 0;
  return 1;
}

int Api::identifiant_leucocyte(int x, int y)
{
  TEST_POS(x, y);
  IF_FOG(x, y);
  for (int i = 0; i < g_->getNbPlayer (); ++i)
    if (g_->players[i].col == x && g_->players[i].row == y)
      return i;
  return NO_UNIT;
}

int Api::equipe(int id)
{
  ID_EXISTS(id);
  return TEAM_FROM_ID(id);
}

int Api::position_leucocyte_x(int id)
{
  ID_EXISTS(id);
  IF_FOG(g_->players[id].col, g_->players[id].row);
  return g_->players[id].col;
}

int Api::position_leucocyte_y(int id)
{
  ID_EXISTS(id);
  LOG1("Looking for x : %1, y : %2", g_->players[id].col, g_->players[id].row);
  IF_FOG(g_->players[id].col, g_->players[id].row);
  return g_->players[id].row;
}

int Api::etat_cellule(int x, int y)
{
  bool found = false;
  std::vector<Cellule*>::iterator it;
  TEST_POS(x, y);
  IF_FOG(x, y);
  for (it = g_->_cells.begin();
       it != g_->_cells.end (); ++it)
    if ((*it)->row == y && (*it)->col == x)
      {
	found = true;
	break;
      }
  if (!found)
    return NO_UNIT;
  return (*it)->Sante ();
}

int Api::etat_leucocyte(int id)
{
  ID_EXISTS(id);
  IF_FOG(g_->players[id].col, g_->players[id].row);
  return g_->players[id].getState ();
}

int Api::connait_type(int x, int y)
{
  int type = -1;
  TEST_POS(x,y);
  IF_FOG(x,y);
  for (std::vector<Virus*>::iterator it = g_->_virus.begin ();
       it != g_->_virus.end (); ++it)
    if ((*it)->col == x && (*it)->row == y)
      {
	type = (*it)->Maladie ();
	break;
      }
  if (type < 0)
    return NO_UNIT;
  return g_->knows_type(type);
}

int Api::defini_competence(int rapidite, int anticorps, int messages, int vision)
{
  if (c_->GetCompetence ())
    return INVALID_COMMAND;
  if (rapidite + anticorps + messages + vision > MAX_SKILL)
    return BAD_ARGUMENT;

  StechecPkt com(COMPETENCE, -1);
  com.Push(5, g_->GetWrapUid (), rapidite, anticorps, messages, vision);
  SendToServer(com);
  c_->ResetFogOfWar();
  g_->players[g_->GetWrapUid ()].competences[PHAGO_SPEED] = rapidite;
  g_->players[g_->GetWrapUid ()].competences[ANTIBODY_NB] = anticorps;
  g_->players[g_->GetWrapUid ()].competences[MESSAGES_NB] = messages;
  g_->players[g_->GetWrapUid ()].competences[VISION] = vision;
  Position dep = Position(0, 0);
  c_->UpdateFogOfWar(g_->GetWrapUid (),
		     dep, g_->players[g_->GetWrapUid ()], false);

  c_->SetCompetence (true);
  return SUCCESS;
}

int Api::deplace_leucocyte(int x, int y)
{
  TRY_ACT();

  Leucocyte&	l = g_->players[g_->GetWrapUid ()];
  int		x2 = l.col;
  int		y2 = l.row;
  unsigned	xr, yr;
  int		dir;

  TEST_POS(x, y);
  LOG3("try to move Leucocyte row: %1 column: %2, from row: %3 col: %4",
       y, x, y2, x2);

  // Verif si c'est dans les 4 cases autour
  // 4 cases
  if (abs(x - x2) < abs(y - y2))
    {
      if (y < y2)
	y2--;
      else
	y2++;
    }
  else
    {
      if (x < x2)
	x2--;
      else
	x2++;
    }
  StechecPkt com(MOVE_LEUCO, -1);
  com.Push(3, g_->GetWrapUid (), y2, x2);
  SendToServer(com);

  SET_ACT();
  return SUCCESS;
}

int Api::phagocyte(int x, int y)
{
  TRY_ACT();
  // FIXME
  SET_ACT();
  return 0;
}

int Api::emet_anticorps()
{
  TRY_ACT();
  StechecPkt com(DROP_ANTIBODY, -1);
  com.Push(1, g_->GetWrapUid ());
  SendToServer(com);
  SET_ACT();
  return 0;
}

int Api::transmettre_message(int message_arg1, int message_arg2, int message_arg3, int message_arg4)
{
  if (g_->players[g_->GetWrapUid ()].NbSentMessages () >=
      g_->players[g_->GetWrapUid ()].MaxMessagesSendable ())
    return INVALID_COMMAND;
  StechecPkt com(TRANSMISSION, -1);
  com.Push(4, message_arg1,
	   message_arg2,
	   message_arg3,
	   message_arg4);
  SendToServer(com);
  g_->players[g_->GetWrapUid ()].SendMessage ();
  return SUCCESS;
}

int Api::recevoir_prochain_message()
{
  return g_->GetNextMessage(g_->GetWrapUid ());
}

int Api::corps_message(int message_id, int argument_id)
{
  if (!g_->MessageExists (message_id) || argument_id < 0
      || argument_id > 3)
    return BAD_ARGUMENT;
  return g_->GetMessage(message_id, argument_id);
}

// See comments in prologin/common/StechecApi.hh
void Api::teamSwitched()
{
  // Shortcut to get the selected team for viewers.
  if (g_->GetWrapUid() >= 0)
    g_->p = &g_->players[g_->GetWrapUid()];
  else
    g_->p = NULL;
}


// FOR THE GUI ONLY
// this is hardcore code :-)

int Api::terrainAt(int x, int y)
{
  TEST_POS(x, y);
  for (std::vector<Cellule*>::iterator it = g_->_cells.begin ();
       it != g_->_cells.end (); ++it)
    if ((*it)->col == x && (*it)->row == y)
      return FLESH;
  return g_->terrain_type[y][x];
}

int Api::getLeucocyte(int team, int id, int *x, int *y)
{
  for (int i = 0; i < g_->getNbPlayer (); ++i)
    {
      if (g_->players[i].get_player () == team &&
	  g_->players[i].get_id () == id)
	{
	  *x = g_->players[i].getX ();
	  *y = g_->players[i].getY ();
	  return 0;
	}
    }
  return -1;
}

int Api::getVirus(int id, int *x, int *y, int *type)
{
  for (int i = 0; i < g_->_virus.size (); ++i)
    {
      if (g_->_virus[i]->get_id () == id)
	{
	  *x = g_->_virus[i]->getX ();
	  *y = g_->_virus[i]->getY ();
	  *type = g_->_virus[i]->Maladie ();
	  return 0;
	}
    }
  return -1;
}

int Api::getBacteria(int id, int *x, int *y)
{
  return -1; // FIXME
}

int Api::getCell(int id, int *x, int *y, int *state)
{
  for (std::vector<Cellule*>::iterator it = g_->_cells.begin ();
       it != g_->_cells.end (); ++it)
    {
      if ((*it)->get_id () == id)
	{
	  *x = (*it)->col;
	  *y = (*it)->row;
	  *state = (*it)->Sante ();
	  return 0;
	}
    }
  return -1;
}

int Api::getFood(int x, int y)
{
  TEST_POS(x, y);
  assert(g_->nutriments[x][y] != NULL);
  return g_->nutriments[x][y]->material ();
}

int Api::getAntibody(int x, int y)
{
  TEST_POS (x, y);
  int nb = 0;
  for (int i = 0; i < g_->getNbPlayer (); ++i)
    nb += g_->players[i].antibodies[y][x];
  return nb;
}
