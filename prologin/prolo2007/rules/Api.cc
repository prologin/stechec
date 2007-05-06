/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2007 Prologin
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
  //  LOG3("NB team: %1, NB player: %2", g_->getNbTeam(), g_->getNbPlayer());
  return g_->getNbTeam();
}

int Api::nombre_leucocyte ()
{
  return g_->getNbPlayer();
}

int Api::taille_equipe(int team_id)
{
  int count = 0;
  int ids[MAX_PLAYER / MAX_TEAM];
  int nb_id;

  CHECK_TEAM(team_id);

  // Fetch all ids belonging to 'team_number'
  g_->getAllIdFromTeamId(team_id, ids, &nb_id);

  // Remove dead ones.
  for (int i = 0; i < nb_id; i++)
    if (g_->players[ids[i]].getState() != STATE_DEAD)
      count++;

  return count;
}

int Api::mon_equipe()
{
  return g_->getTeamId();
}


int Api::mon_identifiant()
{
  return g_->getUid();
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
  CHECK_FOG(x, y);
  if (g_->bacterias[y][x])
    return 1;
  return 0;
}

int Api::regarde(int x, int y)
{
  unsigned i;
  std::vector<Cellule*>::iterator it;

  TEST_POS(x, y);
  CHECK_DEAD();
  if (FOG(x, y))
    return g_->terrain_type[y][x];

  for (i = 0; i < (unsigned)g_->getNbPlayer(); ++i)
    if (g_->players[i].getX () == x && g_->players[i].getY() == y)
      return WHITE_CELL;
  for (i = 0; i < g_->_virus.size(); ++i)
    if (g_->_virus[i]->col == x && g_->_virus[i]->row == y)
      return VIRUS;
  for (it = g_->_cells.begin(); it != g_->_cells.end (); ++it)
    if ((*it)->col == x && (*it)->row == y)
      return CELL;

  return g_->terrain_type[y][x];
}

int Api::visible(int x, int y)
{
  CHECK_DEAD();
  TEST_POS(x, y);
  if (FOG(x, y))
    return 0;
  return 1;
}

int Api::score(int team_id)
{
  int ids[MAX_PLAYER / MAX_TEAM];
  int nb_id;
  int res = 0;

  CHECK_TEAM(team_id);
  g_->getAllIdFromTeamId(team_id, ids, &nb_id);
  for (int i = 0; i < nb_id; i++)
    res += g_->players[ids[i]].getScore();
  return res;
}

int Api::battement_courant()
{
  return g_->getCurrentTurn ();
}

int Api::leucocyte_visible(int id)
{
  CHECK_PLAYER(id);

  if (FOG(g_->players[id].getX(), g_->players[id].getY()))
    return 0;
  return 1;
}

int Api::identifiant_leucocyte(int x, int y)
{
  TEST_POS(x, y);
  CHECK_FOG(x, y);

  for (int i = 0; i < g_->getNbPlayer(); ++i)
    if (g_->players[i].col == x && g_->players[i].row == y)
      return i;

  return NO_UNIT;
}

int Api::equipe(int id)
{
  CHECK_PLAYER(id);

  return g_->getTeamId(id);
}

int Api::position_leucocyte_x(int id)
{
  CHECK_DEAD();
  CHECK_PLAYER(id);
  CHECK_FOG(g_->players[id].col, g_->players[id].row);

  return g_->players[id].col;
}

int Api::position_leucocyte_y(int id)
{
  CHECK_DEAD();
  //  LOG3("Looking for x: %1, y: %2", g_->players[id].col, g_->players[id].row);
  CHECK_PLAYER(id);
  CHECK_FOG(g_->players[id].col, g_->players[id].row);

  return g_->players[id].row;
}

int Api::etat_cellule(int x, int y)
{
  CHECK_DEAD();
  bool found = false;
  std::vector<Cellule*>::iterator it;
  TEST_POS(x, y);
  CHECK_FOG(x, y);
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
  CHECK_DEAD();
  CHECK_PLAYER(id);
  CHECK_FOG(g_->players[id].col, g_->players[id].row);

  return g_->players[id].getState();
}

int Api::connait_type(int x, int y)
{
  int type = -1;

  TEST_POS(x,y);
  CHECK_FOG(x,y);

  std::vector<Virus*>::iterator it;
  for (it = g_->_virus.begin(); it != g_->_virus.end(); ++it)
    if ((*it)->col == x && (*it)->row == y)
      {
	type = (*it)->Maladie();
	break;
      }
  if (type < 0)
    return NO_UNIT;
  return g_->knows_type(type, g_->getUid ());
}

int Api::virus_type(int x, int y)
{
  int type = -1;

  TEST_POS(x,y);
  CHECK_FOG(x,y);

  std::vector<Virus*>::iterator it;
  for (it = g_->_virus.begin(); it != g_->_virus.end(); ++it)
    if ((*it)->col == x && (*it)->row == y)
      {
	type = (*it)->Maladie();
	break;
      }
  if (type < 0)
    return NO_UNIT;
  return type;
}

int Api::definit_competence(int rapidite, int anticorps, int messages, int vision)
{
  if (c_->GetCompetence())
    return INVALID_COMMAND;
  if (rapidite < 0 || anticorps < 0 || messages < 0 || vision < 0)
    return BAD_ARGUMENT;
  if (rapidite + anticorps + messages + vision > MAX_SKILL)
    return BAD_ARGUMENT;

  StechecPkt com(COMPETENCE, -1);
  com.Push(4, rapidite, anticorps, messages, vision);
  SendToServer(com);

  c_->ResetFogOfWar();
  LOG1("UID : %1", g_->getUid());
  g_->players[g_->getUid()].competences[PHAGO_SPEED] = rapidite;
  g_->players[g_->getUid()].competences[ANTIBODY_NB] = anticorps;
  g_->players[g_->getUid()].competences[MESSAGES_NB] = messages;
  g_->players[g_->getUid()].competences[VISION] = vision;
  Position dep = Position(0, 0);
  c_->UpdateFogOfWar(g_->getUid(), dep, g_->players[g_->getUid()], false);
  c_->SetCompetence(true);

  return SUCCESS;
}

int Api::deplace_leucocyte(int x, int y)
{
  TRY_ACT();

  Leucocyte&	l = g_->players[g_->getUid()];
  int		x2 = l.col;
  int		y2 = l.row;
  //   unsigned	xr, yr;
  //   int		dir;

  TEST_POS(x, y);
  LOG3("try to move Leucocyte row: %1 column: %2, from row: %3 col: %4",
       y, x, y2, x2);
  if (x == x2 && y == y2)
    {
      LOG3("Deplacement useless...");
      return SUCCESS;
    }

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
  com.Push(2, y2, x2);
  SendToServer(com);
  SET_ACT();
  return SUCCESS;
}

int Api::phagocyte(int x, int y)
{
  TRY_ACT();
  TEST_POS(x, y);
  CHECK_FOG(x, y);
  bool found = false;
  int x1 = g_->players[g_->getUid ()].col;
  int y1 = g_->players[g_->getUid ()].row;
  int i;
  std::vector<Cellule*>::iterator it;
  if (abs(x - x1) + abs(y - y1) > 1)
    return BAD_ARGUMENT;
  found = g_->PhagocyteVirus (g_->getUid (), y, x, false) ||
    g_->PhagocyteCell (g_->getUid (), y, x, false) ||
    g_->PhagocyteLeucocyte(g_->getUid (), y, x, false);
  if (!found)
    return NO_UNIT;
  StechecPkt com(PHAGOCYTE, -1);
  com.Push(2, y, x);
  SendToServer(com);
  SET_ACT();
  return SUCCESS;
}

int Api::emet_anticorps()
{
  TRY_ACT();
  StechecPkt com(DROP_ANTIBODY, -1);
  SendToServer(com);
  SET_ACT();
  return 0;
}

int Api::transmettre_message(int message_arg1, int message_arg2,
			     int message_arg3, int message_arg4)
{
  CHECK_DEAD();
  int id = g_->getUid();

  if (g_->players[id].NbSentMessages() >= g_->players[id].MaxMessagesSendable())
    return INVALID_COMMAND;

  StechecPkt com(TRANSMISSION, -1);
  com.Push(4, message_arg1 % 65536, // because caml is such a pityful language...
	   message_arg2 % 65536,    // I don't understand why using such a poor
	   message_arg3 % 65536,    // and unsignificant language
	   message_arg4 % 65536);   // Please, code in JAVA :)
  SendToServer(com);
  g_->players[id].SendMessage();
  return SUCCESS;
}

int Api::recevoir_prochain_message()
{
  return g_->GetNextMessage(g_->getUid());
}

int Api::corps_message(int message_id, int argument_id)
{
  if (!g_->MessageExists(message_id) || argument_id < 0
      || argument_id > 3)
    return BAD_ARGUMENT;
  return g_->GetMessage(message_id, argument_id);
}



// FOR THE GUI ONLY
// this is hardcore code :-)

// See comments in prologin/common/StechecApi.hh
void Api::teamSwitched()
{
  // Shortcut to get the selected team for viewers.
  if (g_->getUid() >= 0)
    g_->p = &g_->players[g_->getUid()];
  else
    g_->p = NULL;
}

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
	  *x = g_->_virus[i]->getX();
	  *y = g_->_virus[i]->getY();
	  *type = g_->_virus[i]->Maladie ();
	  return 0;
	}
    }
  return -1;
}

int Api::getBacteria(int x, int y)
{
  TEST_POS (x, y);
  if (g_->bacterias[y][x])
    return g_->bacterias[y][x]->getPopulation ();
  return -1;
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
	  *state = (*it)->Saine ();
	  return 0;
	}
    }
  return -1;
}

int Api::getFood(int x, int y)
{
  TEST_POS(x, y);
  assert(g_->nutriments[y][x] != NULL);
  return g_->nutriments[y][x]->material ();
}

int Api::getAntibody(int player_id, int x, int y)
{
  TEST_POS (x, y);
  int nb = 0;
  if (player_id == -1)
    {
      for (int i = 0; i < g_->getNbPlayer (); ++i)
	if (g_->players[i].getState () != STATE_DEAD)
	  nb += g_->players[i].antibodies[y][x];
    }
  else
    if (g_->players[player_id].getState () != STATE_DEAD)
      nb = g_->players[player_id].antibodies[y][x];
  return nb;
}
