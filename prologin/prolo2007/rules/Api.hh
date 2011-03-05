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

#ifndef API_HH_
# define API_HH_

# include <Contest.hh>
# include "GameData.hh"
# include "Client.hh"

# define TEST_POS(x, y) \
  if (x >= g_->map_size.col || y >= g_->map_size.row || y < 0 || x < 0) \
    return BAD_ARGUMENT;

# define CHECK_PLAYER(id) \
  if ((id) < 0 || (id) >= g_->getNbPlayer()) \
    return BAD_ARGUMENT;

# define CHECK_TEAM(id)						\
  if ((id) < 0 || !g_->getAllIdFromTeamId(id, NULL, NULL))	\
    return BAD_ARGUMENT;

# define FOG(x, y)						\
  (g_->getUid() > -1 && g_->getUid() < UID_VIEWER_BASE && c_->player_fog[g_->getUid ()][y][x] == 0)

# define CHECK_FOG(x, y)				\
  if (FOG(x, y))				\
    return UNKNOWN;

#define ALREADY_ACT(id) \
  (g_->players[(id)].alreadyPlayed())

#define SET_ACT() \
  g_->players[g_->getUid()].setPlayed();

#define CHECK_DEAD() \
  if (g_->players[g_->getUid()].getState () == STATE_DEAD)\
    return INVALID_COMMAND;

#define TRY_ACT()								\
  do {										\
    if (g_->players[g_->getUid()].getState () != STATE_NORMAL ||		\
        ALREADY_ACT(g_->getUid()))						\
        {									\
	  LOG2("Forbidden action (bad id or it has already acted this turn)");	\
	  return INVALID_COMMAND;						\
	}									\
  } while (0)


/*!
** Method of this call are called by the candidat, throught 'interface.cc'
**
** You can:
**  - access to the GameData class: g_
**  - send message to the server: void SendToServer(struct s_com&)
  */
class Api: public StechecApi
{

public:

  Api(GameData* gameData, Client* c);

  virtual ~Api() { }

protected:
  virtual void teamSwitched();

public:
  int equipes();
  int nombre_leucocyte ();
  int taille_equipe(int team_number);
  int mon_equipe();
  int mon_identifiant();
  int taille_corps_x();
  int taille_corps_y();
  int regarde(int x, int y);
  int visible(int x, int y);
  int visible_depuis(int from_x, int from_y, int to_x, int to_y);
  int score(int id);
  int battement_courant();
  int leucocyte_visible(int id);
  int bacteries_presentes (int x, int y);
  int identifiant_leucocyte(int x, int y);
  int equipe(int id);
  int position_leucocyte_x(int id);
  int position_leucocyte_y(int id);
  int etat_cellule(int x, int y);
  int etat_leucocyte(int id);
  int connait_type(int x, int y);
  int virus_type(int x, int y);
  int definit_competence(int rapidite, int anticorps, int messages, int vision);
  int deplace_leucocyte(int x, int y);
  int phagocyte(int x, int y);
  int emet_anticorps();
  int transmettre_message(int message_arg1, int message_arg2, int message_arg3, int message_arg4);
  int recevoir_prochain_message();
  int emetteur_message(int message_id);
  int corps_message(int message_id, int argument_id);

  // For GUI only
  int terrainAt(int x, int y);
  int getLeucocyte(int team, int id, int *x, int *y);
  int getVirus(int id, int *x, int *y, int *type);
  int getBacteria(int x, int y);
  int getCell(int id, int *x, int *y, int *state);
  int getFood(int x, int y);
  int getAntibody(int player_id, int x, int y);
};


#endif /* !API_HH_ */
