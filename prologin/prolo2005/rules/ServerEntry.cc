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

#include <cmath>

#include "GameData.hh"
#include "ServerEntry.hh"


ServerEntry::ServerEntry(GameData* game, Server* server, xml::XMLConfig& cfg) :
  StechecServerEntry(game, server, cfg)
{
  g_->maxTurn = 20;
}

ServerEntry::~ServerEntry()
{
  g_->FreeData();
}

int        ServerEntry::ParseOptions()
{
  cfg_.switchSection("game");
  g_->maxTurn = cfg_.getData<int>("max_turn");
  mapFile = cfg_.getData<std::string>("map");
  cfg_.switchSection("server");
  return 0;
}

int        ServerEntry::LoadMap(const std::string& map_file)
{
  int i;
  unsigned line, col;
  int max_map_player;
  char comment[128] = {0};
  FILE *f;

  if ((f = OpenMap(map_file.c_str())) == NULL)
    {
      // FIXME: fucking strerror_r
      //char err[256];
      //int res = strerror_r(errno, err, 256);
      ERR("Can't open map %1", map_file); //<< ": " << err);
      return 1;
    }

  fgets(comment, 127, f);
  comment[strlen(comment) - 1] = 0;

  if (fscanf(f, "%d %d %d", &max_map_player, &g_->map_size_x, &g_->map_size_y) != 3)
    {
      fclose(f);
      ERR("%1: general information: parse error.", map_file);
      return 1;
    }

  if (max_map_player < g_->getNbPlayer())
    {
      fclose(f);
      ERR("%1: has only `%2' start position... and there are `%3' players.",
	  map_file, max_map_player, g_->getNbPlayer());
      return 1;
    }

  /* On prend la position de depart des joueurs */
  for (i = 0; i < max_map_player; i++)
    {
      if (fscanf(f, "%d %d", &g_->players[i].start_x, &g_->players[i].start_y) != 2)
        {
          fclose(f);
          ERR("%1: start position: parse error.", map_file);
          return 1;
        }
      g_->players[i].start_x--;
      g_->players[i].start_y--;
    }

  /* malloc, calloc de la map... */
  g_->InitMap();

  /** remplissage de la map */
  for (line = 0; line < g_->map_size_x; line++)
    {
      for (col = 0; col < g_->map_size_y; col++)
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
              g_->terrain_type[line][col] = TERRAIN;
              break;
            case 'w':
              g_->terrain_type[line][col] = WATER;
              break;
            case 'p':
              g_->terrain_type[line][col] = PLASMA;
              break;
            case 'o':
              g_->terrain_type[line][col] = ORE;
              break;
            default:
              fclose(f);
              ERR("%1: unknown char in map (%2, %3): %4",
                  map_file, line, col, c);
              return 1;
            }
        }
    }

  LOG2("Map %1 loaded (%2), for `%3' players.", map_file, comment, max_map_player);

  fclose(f);
  return 0;
}

/*
** Fonction qui fait un bruit continu
*/
static int Noise (const unsigned int i,
                  const unsigned int j)
{
  const float x(i / 10.f);
  const float y(j / 10.f);
  const float sum = ((sinf(x) + sinf(y)) / 4 +
                     (sinf(2 * x) + sinf(2 * y)) / 8 +
                     (sinf(4 * x) + sinf(4 * y)) / 16) + 0.6f;
  return (int)(sum * 82);
}

/*!
** fonction qui permet d'envoyer la map, ou/et de faire
** des initialiation sur les joueurs avant le debut de la partie
** voir const StechecPkt et tools/module_reseau/ pour les envois
** voir server.h pour la structure game
*/
int        ServerEntry::beforeGame()
{
  g_->players = new Player[MAX_PLAYER];
  if (ParseOptions())
    return 1;

  if (LoadMap(mapFile))
    return 1;

  const unsigned shift_x(rand() % 100);
  const unsigned shift_y(rand() % 100);
  for (unsigned x = 0; x < g_->map_size_x; x++)
    for (unsigned y = 0; y < g_->map_size_y; y++)
      g_->terrain_value[x][y] = Noise (x + shift_x, y + shift_y);

  g_->p = 0;

  SendToAll(INIT_DATA, -1, 3, g_->maxTurn, g_->map_size_x, g_->map_size_y);

  /* envoie la carte aux clients */
  for (unsigned x = 0; x < g_->map_size_x; x++)
    for (unsigned y = 0; y < g_->map_size_y; y++)
      SendToAll(MAP_CONTENT, -1, 4, x, y, g_->terrain_type[x][y], g_->terrain_value[x][y]);

  for (int i = 0; i < g_->getNbPlayer(); i++)
    {
      unsigned a, b, k = 0;
      unsigned x = g_->players[i].start_x;
      unsigned y = g_->players[i].start_y;
      // care the overflow. 0 minus 1 is... big.
      unsigned da = x - 1 > 100000 ? 0 : x - 1;
      unsigned db = y - 1 > 100000 ? 0 : y - 1;
      
      for (a = da; a <= x + 1; a++)
        for (b = db; b <= y + 1; b++)
          {
            if (a < g_->map_size_x && b < g_->map_size_y
                && g_->terrain_type[a][b] != WATER)
              {
                g_->players[i].coleopteres[k].set_x(a);
                g_->players[i].coleopteres[k].set_y(b);
                g_->terrain_coleoptere[a][b] = &g_->players[i].coleopteres[k];
                LOG2("Positioning player `%1' id `%2': %3, %4", i, k, a, b);
                if (++k >= 5)
                  goto next2;
              }
          }

      if (k != 5)
        {
          da = x - 2 > 100000 ? 0 : x - 2;
          db = y - 2 > 100000 ? 0 : y - 2;
          for (a = da; a <= x + 2; a++)
            for (b = db; b <= y + 2; b++)
              {
                if (a < g_->map_size_x && b < g_->map_size_y
                    && g_->terrain_type[a][b] != WATER
                    && g_->terrain_coleoptere[a][b] == NULL)
                  {
                    g_->players[i].coleopteres[k].set_x(a);
                    g_->players[i].coleopteres[k].set_y(b);
                    g_->terrain_coleoptere[a][b] = &g_->players[i].coleopteres[k];
		    LOG2("Positioning player `%1' id `%2': %3, %4", i, k, a, b);
                    if (++k >= 5)
                      goto next2;
                  }
              }
        }
    next2:

      if (k != 5)
        {
          ERR("Error: can't place all units (%1 / 5) for player `%2' position x: %3, y: %4",
              k, i, x, y);
          return 1;
        }
          
      for (int j = 0; j < 5; j++)
        {
          Coleoptere& c = g_->players[i].coleopteres[j];
          c.set_player(i);
          c.set_id(j);
          c.Reinit();
          int x = g_->players[i].coleopteres[j].get_x();
          int y = g_->players[i].coleopteres[j].get_y();

          SendToAll(NEW_UNIT, i, 3, j, x, y);
        }
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

void        ServerEntry::coleoptere_end_turn(Coleoptere* col)
{
  if (col->state != STATE_DOCKING && col->state != STATE_REPAIRING && col->state != STATE_BUILDING)
    s_->UpdatePlasma(col, col->stock_plasma - 1);

  if (col->state == STATE_DOCKING && col->dock_build != 0 && col->dock_build->type == CENTRAL)
    {
      LOG3("Receiving plasma from central");
      int stock = col->stock_plasma + TRANSFERT_PLASMA_SPEED;
      if (stock >= MAX_STOCK_COL_PLASMA)
        {
          stock = MAX_STOCK_COL_PLASMA;
          s_->ChangeState(col, STATE_NORMAL);
        }
      s_->UpdatePlasma(col, stock);
    }

  if (col->state == STATE_DOCKING && col->dock_build != 0 && col->dock_build->type == MINE)
    {
      LOG3("Receiving ore from a mine");
      int stock = col->stock_ore + TRANSFERT_ORE_SPEED;
      if (stock >= MAX_STOCK_COL_ORE)
        {
          stock = MAX_STOCK_COL_ORE;
          s_->ChangeState(col, STATE_NORMAL);
        }
      s_->UpdateOre(col, stock);
    }

  if (col->stock_plasma <= 0)
    {
      s_->ChangeState(col, STATE_DEAD);
      return;
    }

  // need to transfer some ore...
  if (col->transfert_ore > 0)
    {
      int trans = col->transfert_ore > TRANSFERT_ORE_SPEED ? TRANSFERT_ORE_SPEED : col->transfert_ore;
      col->transfert_ore -= trans;
      s_->UpdateOre(col, col->stock_ore - trans);

      if (col->dock_col != NULL)
        {
          // ...a un autre coleo
          s_->UpdateOre(col->dock_col, col->dock_col->stock_ore + trans);
        }
      else
        {
          // ...a une usine
          Factory* f = static_cast<Factory*>(col->dock_build);
          s_->UpdateOre(f, f->stock_ore + trans);
        }

      if (col->transfert_ore == 0)
        {
          // end of transfert, put in normal state
          s_->ChangeState(col, STATE_NORMAL);
          if (col->dock_col != NULL)
            s_->ChangeState(col->dock_col, STATE_NORMAL);

          col->dock_col = NULL;
          col->dock_build = NULL;
        }
    }

  // building...
  if (col->build_time > 0)
    {
      col->build_time--;
      if (col->build_time == 0)
        {
          Building* f = col->dock_build;

          // end of building, put in normal state
          g_->terrain_building[col->get_x()][col->get_y()] = col->dock_build;
          col->dock_build->state = b_normal;
          col->dock_build = NULL;

          s_->ChangeState(col, STATE_NORMAL);
          SendToAll(BUILDING_FINISHED, col->get_player(), 2, col->get_x(), col->get_y());

          // calcul du score, decremente la valeur reelle du terrain
          for (unsigned x = f->get_x() - 5; x <= f->get_x() + 5; x++)
            for (unsigned y = f->get_y() - 5; y <= f->get_y() + 5; y++)
              if (x < g_->map_size_x && y < g_->map_size_y &&
                  abs(x - f->get_x()) + abs(y - f->get_y()) <= 5)
                if (g_->terrain_value_mod[x][y] > 0)
                  {
                    g_->terrain_value_mod[x][y] -= 5;
                    SendToAll(UPDATE_LAND_VALUE, -1, 3, x, y, g_->terrain_value_mod[x][y]);
                  }
        }
    }

  // approvisionnement...
  Building *b = g_->terrain_building[col->get_x()][col->get_y()];
  if (b != NULL)
    {
      if (col->state == STATE_DOCKING && b->type == MINE)
        {
          int stock = col->stock_ore + 100;
          if (stock > MAX_STOCK_COL_ORE)
            {
              stock = MAX_STOCK_COL_ORE;
              s_->ChangeState(col, STATE_NORMAL);
            }
          s_->UpdateOre(col, stock);
        }
      if (col->state == STATE_DOCKING && b->type == CENTRAL)
        {
          int stock = col->stock_plasma + 10;
          if (stock > MAX_STOCK_COL_PLASMA)
            {
              stock = MAX_STOCK_COL_PLASMA;
              s_->ChangeState(col, STATE_NORMAL);
            }
          s_->UpdatePlasma(col, stock);
        }
    }
}

void        ServerEntry::factory_end_turn(Factory* f)
{
  Player* p = &g_->players[f->get_player()];

  // if the building isn't finished, or is dead, do nothing;
  if (f->state != b_normal)
    return;

  if (f->buildlist <= 0)
    return;

  f->buildlist--;
  LOG3("Buidling coleoptera. (%1 turn left).", f->buildlist);
  if (f->buildlist % MAKE_COLEOPTERA_TIME == 0)
    {
      // add at end.
      int i = -1;
      for (int k = 0; k < MAX_COLEOPTERE; k++)
        if (p->coleopteres[i].state != STATE_DEAD)
          i = k;
      if (i == -1)
        {
          WARN("too many unit !");
          return;
        }
      if (++i == MAX_COLEOPTERE)
        i = 0; // FIXME

      if (p->coleopteres[i].state == STATE_DEAD)
        {
          LOG3("Use id %1 for new coleoptere.", i);
          p->coleopteres[i].state = STATE_NORMAL;
          p->coleopteres[i].set_player(f->get_player());
          p->coleopteres[i].set_id(i);
          p->coleopteres[i].Reinit();
          p->coleopteres[i].set_x(f->get_x());
          p->coleopteres[i].set_y(f->get_y());
          g_->terrain_coleoptere[f->get_x()][f->get_y()] = &p->coleopteres[i];

          SendToAll(NEW_UNIT, f->get_player(), 2, i, f->get_x(), f->get_y());
        }

    }

  if (f->col_repair != NULL)
    {
      Coleoptere* col = f->col_repair;

      s_->UpdateOre(f, f->stock_ore - 50);
      s_->UpdatePlasma(f, f->stock_plasma - 10);

      col->set_damage(col->get_damage() - 10);
      SendToAll(REPAIR_COLEOPTERA, col->get_player(), 2, col->get_id(), col->get_damage());

      if (col->get_damage() == 0)
        {
          // sortie d'usine
          int x = f->get_x();
          int y = f->get_y();

          if (g_->terrain_coleoptere[x][y] != NULL)
            {
              // crash a la sortie d'usine
              s_->ChangeState(col, STATE_DEAD);
              s_->ChangeState(g_->terrain_coleoptere[x][y], STATE_DEAD);
            }
          else
            {
              g_->terrain_coleoptere[x][y] = col;
              s_->ChangeState(col, STATE_NORMAL);
            }
        }
    }
}

/*!
** permet d'effectuer diverses operations apres
** chaque tour de jeu
*/
int         ServerEntry::afterNewTurn()
{
  int                i;
  unsigned        j;

  for (i = 0; i < g_->getNbPlayer(); i++)
    {
      for (j = 0; j < MAX_COLEOPTERE; j++)
        if (g_->players[i].coleopteres[j].state != STATE_DEAD)
          coleoptere_end_turn(&g_->players[i].coleopteres[j]);
      for (j = 0; j < g_->players[i].factory_count; j++)
        factory_end_turn(&g_->players[i].factories[j]);
    }
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
  return g_->getCurrentTurn() >= g_->maxTurn;
}

int        ServerEntry::getScore(int uid)
{
  int score = 0;
  Player& p = g_->players[uid];

  for (unsigned i = 0; i < p.hotel_count; i++)
    {
      Hotel& h = p.hotels[i];
      LOG2("Score: hotel %1", i);
      if (h.state == b_normal)
        {
          int s = (int)(g_->terrain_value[h.get_x()][h.get_y()] *
                        ((double)g_->terrain_value_mod[h.get_x()][h.get_y()] / 100));
          LOG3("Score: add %1 points", s);
          score += s;
        }
    }
  return score;
}
