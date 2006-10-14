/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2004, 2005, 2006 Prologin
*/

#include "GameData.hh"
#include "ServerResolver.hh"
#include "ServerEntry.hh"
#include "xml/xml_config.hh"

ServerEntry::ServerEntry(GameData* game, Server* server, xml::XMLConfig& cfg) :
  StechecServerEntry(game, server, cfg)
{
  maxTurn = 20;
}

ServerEntry::~ServerEntry()
{
  delete[] g_->player;
  g_->FreeData();
}

int        ServerEntry::ParseOptions()
{
  maxTurn = cfg_.getData<int>("game", "max_turn");
  mapFile = cfg_.getData<std::string>("game", "map");
  return 0;
}

void        ServerEntry::InitPlayer()
{
  /* initialisation de la structure de chaque joueur. */
  for (int i = 0; i < g_->getNbPlayer(); i++)
    {
      player_t *p = &g_->player[i];
      p->id = i;
      p->nb_unit = START_UNIT_NUM;
      for (int j = 0; j < MAX_UNIT; j++)
        {
          /* Chaque fourmi commence a la position de sa fourmiliere */
          p->all_unit[j].x = p->anthill_x;
          p->all_unit[j].y = p->anthill_y;
        }
      g_->map[p->anthill_x][p->anthill_y] = ANTHILL;
    }
} 

int        ServerEntry::LoadMap(const char *map_file)
{
  int i;
  int line, col;
  FILE *f;

  if ((f = OpenMap(map_file)) == NULL)
    {
      char err[256];
      strerror_r(errno, err, 256);
      ERR("Can't open map `%1': %2", map_file, err);
      return 1;
    }
  
  if (fscanf(f, "%d %d %d", &max_map_player, &g_->map_size_y, &g_->map_size_x) != 3)
    {
      fclose(f);
      ERR("%1: general information: parse error.", map_file);
      return 1;
    }

  if (max_map_player < g_->getNbPlayer())
    {
      fclose(f);
      ERR("this map has only `%1' anthill... and there are `%2' players.",
	  max_map_player, g_->getNbPlayer());
      return 1;
    }

  /* On prend la position des fourmilieres */
  g_->player = new player_t[max_map_player];
  for (i = 0; i < max_map_player; i++)
    if (fscanf(f, "%d %d", &g_->player[i].anthill_y, &g_->player[i].anthill_x) != 2)
      {
        fclose(f);
        ERR("%1: anthill position: parse error.", map_file);
        return 1;
      }

  /* malloc, calloc... */
  g_->InitMap();
  g_->InitMapMisc();
  
  /** remplissage de la map */
  for (line = 0; line < g_->map_size_y; line++)
    {
      for (col = 0; col < g_->map_size_x; col++)
        {
          char c;
          /* it's a pretty loosy parsing, but who cares... */
          do
            if (fscanf(f, "%c", &c) != 1)
              {
                fclose(f);
                ERR("%1: unexpected eof while reading map.", map_file);
                return 1;
              }
          while (c <= 13);

          switch (c)
            {
            case '.':
              g_->map[col][line] = NOTHING;
              break;
            case 'x':
              g_->map[col][line] = OBSTACLE;
              break;
            case 'b':
              g_->map[col][line] = FOOD;
              break;
            default:
              fclose(f);
              ERR("%1: unknown char in map (%2, %3): %4",
		  map_file, line, col, c);
              return 1;
            }
        }
    }
  fclose(f);
  return 0;
}


/*!
** fonction qui permet d'envoyer la map, ou/et de faire
** des initialiation sur les joueurs avant le debut de la partie
** voir struct s_com et tools/module_reseau/ pour les envois
** voir server.h pour la structure game
*/
int        ServerEntry::beforeGame()
{
  if (ParseOptions())
    return 1;

  if (LoadMap(mapFile.c_str()))
    return 1;

  InitPlayer();

  // envoie des infos generales
  StechecPkt com2(INIT_MAP_SIZE, -1);
  com2.Push(2, g_->map_size_x, g_->map_size_y);
  SendToAll(com2);

  /* envoie la position des fourmilieres */
  for (int i = 0; i < g_->getNbPlayer(); i++)
    {
      StechecPkt com(POSITION_ANTHILL, -1);
      com.Push(3, i, g_->player[i].anthill_x, g_->player[i].anthill_y);
      SendToAll(com);
    }
  
  /* envoie la carte aux clients */
  for (int x = 0; x < g_->map_size_x; x++)
    for (int y = 0; y < g_->map_size_y; y++)
      {
        StechecPkt com(MAP_CONTENT, -1);
        com.Push(3, x, y, g_->map[x][y]);
        SendToAll(com);
      }

  /* envoie des nouvelles fourmis ! */
  for (int i = 0; i < g_->getNbPlayer(); i++)
    for (int j = 0; j < START_UNIT_NUM; j++)
      {
        StechecPkt com(ANT_NEW_UNIT, i);
        SendToAll(com);
      }

  return 0;
}

/*!
** fonction situee apres l'initialisation du joueur,
** mais avant la boucle de jeu
** Sert a faire des eventuels envois
*/
int         ServerEntry::initGame()
{
  return 0;
}


/*!
** permet d'effectuer diverses operations avant
** chaque tour de jeu
*/
int         ServerEntry::beforeNewTurn()
{
  return 0;
}

/*!
** permet d'effectuer diverses operations apres
** chaque tour de jeu
*/
int         ServerEntry::afterNewTurn()
{
  return 0;
}

/*!
** permet d'effectuer diverses operations a la fin
** du jeu
*/
int         ServerEntry::afterGame()
{
  return 0;
}

/*!
** defini si il reste des tours de jeu ou pas.
** retourne true si c'est la fin du jeu, false sinon
*/
bool        ServerEntry::isMatchFinished()
{
  return g_->getCurrentTurn() >= maxTurn;
}

int        ServerEntry::getScore(int uid)
{
  player_t *p = &g_->player[uid];

  int score = (p->nb_unit - START_UNIT_NUM) * 40 + p->amount_of_food * 2;
  for (int i = 0; i < p->nb_unit; i++)
    score += p->all_unit[i].food;
  return score;
}
