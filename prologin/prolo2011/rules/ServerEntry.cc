/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2011 Prologin
*/

#include "Constant.hh"
#include "GameData.hh"
#include "ServerEntry.hh"
#include "misc/Conf.hh"

#include <string>
#include <fstream>

ServerEntry::ServerEntry(GameData* game, Server* server,
                         const ConfSection* cfg)
  : StechecServerEntry(game, server, cfg)
{
}

ServerEntry::~ServerEntry(void)
{
}

int ServerEntry::loadMap()
{
  std::string map_file;
  std::ifstream f;
  std::string line;
  
  map_file = cfg_->getValue<std::string>("map");
  LOG2("Map file is: \"%1\"", map_file);  
  f.open(map_file.c_str());
  if (!f){
    ERR("Unable to load file \"%1\"", map_file);
    return 1;
  }
  getline(f, line);
  int nsources = atoi(line.c_str());
  int * sources = new int[nsources];

  for (int i = 0; i < nsources; ++i){
    getline(f, line);
    sources[i] = atoi(line.c_str());
  }
  int indice_source = 0;
  g_->sources.reserve(nsources);
  SendToAll(NB_SOURCES, -1, nsources);
  for (int y = 0; y < TAILLE_TERRAIN; ++y){
    getline(f, line);
    if (!f){
      ERR("Not enough lines in map file");
      return 1;
    }
    if (line.size() != TAILLE_TERRAIN){
      ERR("Line %1 doesn't have the good size", y + 1);
      return 1;
    }
    for (int x = 0; x < TAILLE_TERRAIN; ++x){
      Case &c = g_->get_case(x, y);
      c.source_id = -1;
      c.nb_trainees_moto = 0;
      if (line[x] == ' '){
	c.bonus = PAS_BONUS;
	c.type = VIDE;
      }else if (line[x] == 'X'){
	c.bonus = PAS_BONUS;
	c.type = OBSTACLE;
      }else if (line[x] == '!'){
	c.bonus = PAS_BONUS;
	c.type = SOURCE;
	c.source_id = indice_source;
	SourceEnergie s;
	s.pos.x = x;
	s.pos.y = y;
	s.potentiel_max = sources[indice_source];
	s.potentiel_cur = sources[indice_source];
	SendToAll(SOURCE_CONTENT, -1, 7, indice_source, x, y, sources[indice_source]);
	indice_source++;
	g_->sources.push_back(s);
      }else if (line[x] == 'C'){
	c.type = BONUS;
	c.bonus = BONUS_CROISEMENT;
      }else if (line[x] == 'L'){
	c.type = BONUS;
	c.bonus = PLUS_LONG;
      }else if (line[x] == 'P'){
	c.type = BONUS;
	c.bonus = PLUS_PA;
      }else if (line[x] == 'R'){
	c.type = BONUS;
	c.bonus = BONUS_REGENERATION;
      }else{
	ERR("Ligne \"%1\" invalide...", line);
	return 1;
      }
    }
  }
  delete[] sources;
  return 0;
}

int		ServerEntry::beforeGame(void)
{
  g_->Init();
  if (loadMap())
    return 1;
  for (int y = 0; y < TAILLE_TERRAIN; ++y) {
    for (int x = 0; x < TAILLE_TERRAIN; ++x) {
      Case &c = g_->get_case(x, y);
      SendToAll(CONTENU_CASE, -1, 5, x, y, c.bonus, c.type, c.source_id);
    }
  }
  return 0;
}

int         ServerEntry::initGame(void)
{
  return 0;
}

int         ServerEntry::beforeNewTurn(void)
{
  g_->team_switched();
  return 0;
}

#define ARG2(a, i) a[(i)], a[(i) + 1]
#define ARG4(a, i) ARG2(a, i), ARG2(a, i + 2)
#define ARG8(a, i) ARG4(a, i), ARG4(a, i + 4)
#define ARG10(a, i) ARG8(a, i), ARG2(a, i + 8)
int         ServerEntry::afterNewTurn(void)
{ // forward the actions
  for (std::vector<std::vector<int> >::iterator it = g_->packets.begin();
       it != g_->packets.end(); ++it)
  {
    SendToAll((*it)[0], -1, 10, ARG10((*it), 1)); // maximum 10 parametres par action...
  }
  return 0;
}


int         ServerEntry::afterGame(void)
{
  return 0;
}

bool        ServerEntry::isMatchFinished(void)
{
  return g_->isMatchFinished();
}

int ServerEntry::getScore(int uid)
{
  // TODO
  return 0;
}
