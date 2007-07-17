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

#include <time.h>
#include "GameData.hh"
#include "ServerEntry.hh"
#include "xml/xml_config.hh"

ServerEntry::ServerEntry(GameData* game, Server* server, xml::XMLConfig& cfg) :
  StechecServerEntry(game, server, cfg)
{
  g_->max_date = -1;
  srand(time(0));
}

ServerEntry::~ServerEntry()
{
  g_->FreeData();
}


// don't know what to do with this method for now.. let met think of this
int        ServerEntry::ParseOptions()
{
  g_->max_date = cfg_.getData<int>("game", "max_turn");
  mapFile = cfg_.getData<std::string>("game", "map");

  // Check for crasy people.
  if (g_->max_date > MAX_TURN)
    {
      ERR("Bad parameter max_turn (%1). Should be less than %2.",
          g_->max_date, MAX_TURN);
      return 1;
    }
  return 0;
}


int        ServerEntry::LoadMap(const std::string& map_file)
{
  int		row, col;
  int		max_map_player;
  char		comment[128] = {0}; /* contain the map's comment*/
  char		c; /* used for map scanning */
  FILE*	f;

  if ((f = OpenMap(map_file.c_str())) == NULL)
    {
      ERR("Can't open map %1", map_file);
      return 1;
    }
  fgets(comment, 127, f);
  comment[strlen(comment) - 1] = 0;
  /* Reading map nb_players and size*/

  if (fscanf(f, "%d %d %d %d", &max_map_player, &g_->map_size.row, &g_->map_size.col, &g_->max_new_seeds) != 4)
    {
      fclose(f);
      ERR("%1: general information: parse error.", map_file);
      return 1;
    }
  LOG3("info1 %1 %2 %3", max_map_player, g_->map_size.row, g_->map_size.col);
  if (g_->map_size.row > MAX_MAP_SIZE || g_->map_size.col > MAX_MAP_SIZE)
    {
      fclose(f);
      ERR("%1: maximum map size is %2.", map_file, MAX_MAP_SIZE);
      return 1;
    }
  if (max_map_player < g_->getNbPlayer())
    {
      fclose(f);
      ERR("%1: has only `%2' start position... and there are `%3' players.",
	  map_file, max_map_player, g_->getNbPlayer());
      return 1;
    }

  /* FIXME: wait for Fabecc explanation (Reading player's start positions) */
  for (int i = 0; i < max_map_player; i++)
    {
      if (fscanf(f, "%d %d", &g_->players[i].row, &g_->players[i].col) != 2)
	{
	  fclose(f);
	  ERR("%1: start position: parse error.", map_file);
	  return 1;
	}
      LOG3("player %1 %2 %3", i, g_->players[i].row, g_->players[i].col);
    }

  /* Virus start position */
  int nb_virus = 0;
  if (fscanf(f, "%d", &nb_virus) != 1)
    {
      fclose(f);
      ERR("%1: general information: parse error.", map_file);
      return 1;
    }
  LOG3("nb_vir %1", nb_virus);
  for (int i = 0; i < nb_virus; ++i)
    {
      int tmp_x, tmp_y, tmp_mal;
      if (fscanf(f, "%d %d %d", &tmp_y, &tmp_x, &tmp_mal) != 3)
	{
	  fclose(f);
	  ERR("%1: general information: parse error.", map_file);
	  return 1;
	}
      LOG3("virus %1 %2 %3 : %4", tmp_y, tmp_x, tmp_mal, g_->_virus.size ());
      g_->_virus.push_back(new Virus(tmp_y, tmp_x, tmp_mal, g_));
    }

  /* malloc, cAllocing the map... */
  g_->InitMap();

  /* filling the map */
  for (row = 0; row < g_->map_size.row; row++)
    for (col = 0; col < g_->map_size.col; col++)
      {
	/* it's a pretty loosy parsing, but who cares... */
	do
	  {
	    if (fscanf(f, "%c", &c) != 1)
	      {
		fclose(f);
		ERR("%1: unexpected eof while reading map.", map_file);
		return 1;
	      }
	  }
	while (c <= 13);
	// List of possibility for a case

	/*
	  Possibility of box:
	  - v|V : Vessel
	  - c|C : Flesh with a Cell
	  - f|F : Flesh without Cell
	*/
	if (c == 'v' || c == 'V')
	  {
	    g_->terrain_type[row][col] = VESSEL;
	  }
	else if (c == 'c' || c == 'C')
	  {
	    g_->terrain_type[row][col] = FLESH;
	    g_->chair += 2;
	    g_->_cells.push_back(new Cellule(row, col, CELL_MITOSIS_MATERIAL * 2, g_));
	  }
	else if (c == 'f' || c == 'F')
	  {
	    g_->terrain_type[row][col] = FLESH;
	    g_->chair++;
	  }
	else
	  {
	    fclose(f);
	    ERR("%1: unknown char in map (%2, %3): %4",
		map_file, row, col, (int)c);
	    return 1;
	  }
      }
  g_->chair /= 2;
  return 0;
}



// These functions are just for debugging map loader

bool	ServerEntry::displayCell(int i, int j)
{
  for (std::vector<Cellule*>::iterator it =
	 g_->_cells.begin ();
       it != g_->_cells.end (); ++it)
    if ((*it)->col == j && (*it)->row == i)
      {
	std::cout << "C";
	return true;
      }
  return false;
}

void	ServerEntry::displayMap()
{
  int		i;
  int		j;


  std::cout << "Map size : row : " << g_->map_size.row
	    << " col : " << g_->map_size.col << std::endl;
  for (i = 0; i < g_->map_size.row; i++)
    {
      for (j = 0; j < g_->map_size.col; j++)
	{
	  int type = -1;
 	  std::vector<Virus*>::iterator it;
 	  for (it = g_->_virus.begin(); it != g_->_virus.end(); ++it)
 	    if ((*it)->col == j && (*it)->row == i)
 	      {
 		type = (*it)->Maladie();
 		break;
 	      }
	  if (type != -1)
	    std::cout << type;
	  else if (g_->bacterias[i][j])
	    std::cout << "B";
	  else if (g_->terrain_type[i][j] == VESSEL)
	    std::cout << "V";
	  else if (!displayCell(i, j) && g_->terrain_type[i][j] == FLESH)
	    std::cout << "F";
	}
      std::cout << std::endl;
    }
}

void	ServerEntry::CheckMap ()
{
  int nb_possible = 0;
  for (int i = 0; i < g_->map_size.row; ++i)
    for (int j = 0; j < g_->map_size.col; ++j)
      if (g_->terrain_type[i][j] == VESSEL)
	nb_possible++;
  if (nb_possible < g_->max_new_seeds)
    g_->max_new_seeds = nb_possible;
}

int		ServerEntry::beforeGame()
{
  if (ParseOptions())
    return 1;
  if (LoadMap(mapFile))
    return 1;
  //  displayMap();
  //  pathCalculation();

  LOG1("No players connected %1", g_->getNbPlayer ());
  LOG1("No team connected %1", g_->getNbTeam ());

  int seed = time(0);
  CheckMap ();
  SendToAll(INIT_DATA, -1, 5, g_->max_date, g_->map_size.row, g_->map_size.col, g_->max_new_seeds, seed);
  g_->srand (seed);

  //Sending map to clients
  for (int x = 0; x < g_->map_size.row; x++)
    for (int y = 0; y < g_->map_size.col; y++)
      SendToAll(MAP_CONTENT, -1, 3, x, y, g_->terrain_type[x][y]);

  g_->init ();

  // Positionning Virus
  for (unsigned i = 0; i < g_->_virus.size(); ++i)
    SendToAll(NEW_VIRUS, -1, 3, g_->_virus[i]->row, g_->_virus[i]->col,
	      g_->_virus[i]->Maladie());

  for (unsigned i = 0; i < g_->_cells.size (); ++i)
    SendToAll(NEW_CELL, -1, 3, g_->_cells[i]->row, g_->_cells[i]->col);

  // Add Leucocytes
  for (int i = 0; i < g_->getNbPlayer(); ++i)
    {
      LOG3("Send leucocyte id %1, equipe %2", i, g_->getTeamId(i));
      s_->addLeucocyte(g_->getTeamId(i), i, g_->players[i].col, g_->players[i].row);
    }
  return 0;
}

int         ServerEntry::initGame()
{
  return 0;
}

int         ServerEntry::beforeNewTurn()
{
  LOG4("Random is now : %1", g_->rand());
  return 0;
}

int         ServerEntry::afterNewTurn()
{
//   displayMap();
  g_->PlayTurn ();
  g_->calculScore ();
  s_->sendScore ();
//   g_->player_turn++;
  return 0;
}


int         ServerEntry::afterGame()
{
  g_->end ();
  return 0;
}

bool        ServerEntry::isMatchFinished()
{
  if (g_->getCurrentTurn () >= g_->max_date)
    {
      LOG1("Match finished");
      calculScores ();
    }
  else
    LOG4("Max date : %1 and turn : %2", g_->max_date, g_->player_turn);
  return g_->getCurrentTurn () >= g_->max_date;
}

static int	*tab_score = NULL;


void		ServerEntry::calculScores()
{
  g_->calculScore ();
}

int        ServerEntry::getScore(int uid)
{
  //fonction utilisee par l'interface web (et apres somme par cette interface par team)
  calculScores ();
  return g_->players[uid].score_;
}
