/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2006 Prologin
*/

#include "GameData.hh"
#include "ServerEntry.hh"


ServerEntry::ServerEntry(GameData* game, Server* server, xml::XMLConfig& cfg) :
   StechecServerEntry(game, server, cfg)
{
   g_->max_date = -1;
}

ServerEntry::~ServerEntry()
{
  g_->FreeData();
}


// don't know what to do with this method for now.. let met think of this
int        ServerEntry::ParseOptions()
{
  cfg_.switchSection("game");
  g_->max_date = cfg_.getData<int>("max_turn");
  mapFile = cfg_.getData<std::string>("map");
  cfg_.switchSection("server");

  // Check for crasy people.
  if (g_->max_date > MAX_DATE)
    {
      ERR("Bad parameter max_turn (" << g_->max_date << "). Should be less than "
          << MAX_DATE);
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
      ERR("Can't open map " << map_file);
      return 1;
   }
   fgets(comment, 127, f);
   comment[strlen(comment) - 1] = 0;

   /* Reading map nb_players and size*/
  if (fscanf(f, "%d %d %d", &max_map_player, &g_->map_size.row, &g_->map_size.col) != 3)
    MAP_ERROR(f, ": general information: parse error.");
  if (g_->map_size.row > MAX_MAP_SIZE || g_->map_size.col > MAX_MAP_SIZE)
    MAP_ERROR(f, ": maximum map size is " << MAX_MAP_SIZE << ".");
  if (max_map_player < g_->getNbPlayer())
     MAP_ERROR(f,  ": has only `" << max_map_player
	       << "' start position... and there are `"
	       << g_->getNbPlayer() << "' players.");

  /* Reading delorean's start positions */
  if (fscanf(f, "%d %d", &g_->delorean.row, &g_->delorean.col) != 2)
     MAP_ERROR(f,": start position: parse error.");

  g_->delorean.row--;
  g_->delorean.col--;
  s_->updateDeloreanPosition(g_->delorean,
			 g_->delorean.row,
			 g_->delorean.col);

  /* Reading almanach's start positions */
  if (fscanf(f, "%d %d", &g_->almanach.row, &g_->almanach.col) != 2)
     MAP_ERROR(f,": start position: parse error.");
  g_->almanach.row--;
  g_->almanach.col--;
  s_->updateAlmanachPosition(g_->almanach,
			 g_->almanach.row,
			 g_->almanach.col);

  /* Reading player's start positions */
  for (int i = 0; i < max_map_player; i++)
  {
     if (fscanf(f, "%d %d", &g_->players[i].start.row, &g_->players[i].start.col) != 2)
	MAP_ERROR(f,": start position: parse error.");
     g_->players[i].start.row--;
     g_->players[i].start.col--;
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
	     MAP_ERROR(f,  ": unexpected eof while reading map.");
	 }
       while (c <= 13);
       if (c == '.' )
         {
           g_->terrain_type[row][col] = STANDARD;
           g_->terrain_graphic[row][col] = GRASSY_AREA;
         }
       else if (c == 'r' || c == 'R')
         {
           g_->terrain_type[row][col] = STANDARD;
           g_->terrain_graphic[row][col] = STREET;
         }
       else if (c == 'B' || c == 'b' || c == 'X' || c == 'x')
         {
           g_->terrain_type[row][col] = WALL;
           g_->terrain_graphic[row][col] = BUILDING;
         }
       else if (c == 'P' || c == 'p')
	 {
	   g_->terrain_type[row][col] = BET_PLACE;
           g_->terrain_graphic[row][col] = BET_PLACE;
	   s_->addCasino(g_, row, col);
	   LOG3("Adding a casino");
	 }
       else
	 MAP_ERROR(f, ": unknown char in map ("
		   << row << ", " << col << "): " << c);
     }
  return 0;
}

// This function is just for debug mode
// It should be deleted later

void	ServerEntry::displayMap()
{
   int		i;
   int		j;

   std::cout << "Map size : row : " << g_->map_size.row
	     << " col : " << g_->map_size.col << std::endl;
   std::cout << "Delorean Position : row : " << g_->delorean.row
	     << " col : " << g_->delorean.col << std::endl;
   std::cout << "Almanach Position : row : " << g_->almanach.row
	     << " col : " << g_->almanach.col << std::endl;
   for (int i = 0; i < g_->getNbPlayer(); i++)
   {
      std::cout << "Player " << i
		<< " : row : " << g_->players[i].start.row
		<< " col : " << g_->players[i].start.col
		<< std::endl;
   }
   for (i = 0; i < g_->map_size.row; i++)
     {
       for (j = 0; j < g_->map_size.col; j++)
	 {
	   if (g_->terrain_type[i][j] == STANDARD)
	     std::cout << ".";
	   if (g_->terrain_type[i][j] == WALL)
	     std::cout << "X";
	   if (g_->terrain_type[i][j] == BET_PLACE)
	     std::cout << "P";
	 }
       std::cout << std::endl;
     }
}


// void		ServerEntry::pathCalculation()
// {
//   Position	*tmp_start;
//   Position	*tmp_end;

//   for (int i_start = 0; i_start < g_->map_size.row; ++i_start)
//     for (int j_start = 0; j_start < g_->map_size.col; ++j_start)
//       if (g_->terrain_type[i_start][j_start] != WALL)
// 	{
// 	  tmp_start = new Position(i_start, j_start);
// 	  for (int i_end = 0; i_end < g_->map_size.row; ++i_end)
// 	    for (int j_end = 0; j_end < g_->map_size.col; ++j_end)
// 	      {
// 		tmp_end = new Position(i_end, j_end);
// 		g_->paths[std::pair(*tmp_start, *tmp_end)] =
// 		  astar(tmp_start, tmp_end);
// 	      }
// 	}
//}

int		ServerEntry::beforeGame()
{
  unsigned	iter;
  unsigned	bet_iter;

  //  srand(time((time_t*)&iter));
  srand(42);
  LOG3("Random seed is " << time((time_t*)&iter));
  //  g_->players = new Player[MAX_TEAM];
  if (ParseOptions())
    return 1;

  if (LoadMap(mapFile))
    return 1;
  //  displayMap();
  //  pathCalculation();

  g_->p = 0;
  SendToAll(INIT_DATA, -1, 3, g_->max_date, g_->map_size.row, g_->map_size.col);

  //Sending map to clients
  for (int x = 0; x < g_->map_size.row; x++)
    for (int y = 0; y < g_->map_size.col; y++)
      SendToAll(MAP_CONTENT, -1, 3, x, y, g_->terrain_graphic[x][y]);

  // Positionning players
  for (int i = 0; i < g_->getNbPlayer(); ++i)
    {
//       unsigned	step;
//       int	a, b;
//       bool	map_out = true;
//       unsigned	nb_placed = 0;
      unsigned	x;// = g_->players[i].start.row;
      unsigned	y;//= g_->players[i].start.col;

//       LOG3("Going to place around x: " << x << ", y: " << y);
//       for (step = 0; nb_placed != MAX_GOODMEN; ++step, map_out = true)
// 	{
// 	  LOG3("Step : " << step);
// 	  for (a = (((x - step) > 0) ? (x - step) : 0); printf("a : %d, x + step: %d, step : %d\n",
// 							       a, x + step, step) &&
// 		 a <= x + step; ++a)
// 	    for (b = (((y - step) > 0) ? (y - step) : 0); printf("b : %d, y + step: %d, step : %d\n",
// 								 b, y + step, step) &&
// 		   b <= y + step; ++b)
// 	      {
// 		LOG3("Trying to place player : " << nb_placed << " in x :" << a << " , y :" << b);
// 		if (a >= 0 && a < g_->map_size.row
// 		    && b >= 0 && b < g_->map_size.col)
// 		  {
// 		    map_out = false;
// 		    if (g_->terrain_type[a][b] != WALL || g_->terrain_type[a][b] != BET_PLACE)
// 		      {
// 			g_->players[i].team[nb_placed].row = a;
// 			g_->players[i].team[nb_placed].col = b;
// 			LOG2("Positioning player " << i << " id " << nb_placed
// 			     << ": " << a << ", " << b);
// 			++nb_placed;
// 		      }
// 		    if (nb_placed == MAX_GOODMEN)
// 		      break;
// 		  }
// 		if (nb_placed == MAX_GOODMEN)
// 		  break;
// 	      }
// 	  if (map_out == true)
// 	    {
// 	      ERR("Invalid Map: Too Small");
// 	      return 1;
// 	    }
// 	}
      for (int j = 0; j < MAX_GOODMEN; ++j)
	{
	  g_->players[i].team[j].row = g_->players[i].start.row;
	  g_->players[i].team[j].col = g_->players[i].start.col;
	  GoodMan&	gdm = g_->players[i].team[j];
	  gdm.set_player(i);
	  gdm.set_id(j);
	  x = g_->players[i].team[j].row;
	  y = g_->players[i].team[j].col;
	  SendToAll(NEW_GOODMAN, -1, 4, i, j, x, y);
	}
    }
  //bets init
  for (iter = 0; iter < g_->casinos.size(); ++iter)
    {
      for (bet_iter = 0; bet_iter < (MAX_DATE / 10); ++bet_iter)
	{
	  if (rand() % 2 == 0)
	    s_->addBet(g_->almanach,
		       bet_iter * 10,
		       (rand() % 80) + 20,
		       rand() % MAX_BET,
		       iter);
	}
    }
  return 0;
}

int         ServerEntry::initGame()
{
  return 0;
}

int         ServerEntry::beforeNewTurn()
{
  return 0;
}

void		ServerEntry::goodManEndTurn(GoodMan& gdm)
{
  if (gdm.warpTime() == g_->player_turn && gdm.getState() == STATE_BACK_IN_THE_FUTURE)
    {
      gdm.setWarpTime(0);
      s_->changeState(gdm, STATE_NORMAL);
    }
  if (gdm.getKoTime() != 0)
    gdm.setKoTime(gdm.getKoTime() - 1);
  else if (gdm.getKoTime() == 0 && gdm.getState() == STATE_KO)
    s_->changeState(gdm, STATE_NORMAL);
}

void		ServerEntry::deloreanEndTurn()
{
  int		j, k;

   if (g_->delorean.reloadingTimeLeft() != 0)
     s_->updateReloadTime(g_->delorean, g_->delorean.reloadingTimeLeft() - 1);
   else if (g_->delorean.warpTimeLeft() != 0)
     {
       g_->delorean.setWarpTimeLeft(g_->delorean.warpTimeLeft() - 1);
       LOG3("Still " << g_->delorean.warpTimeLeft() << " turns to wait....." )
     }
   else if (g_->delorean.warpTimeLeft() == 0 && g_->delorean.hasWarped())
   {
     s_->updateReloadTime(g_->delorean, DELOREAN_RELOAD);
     g_->delorean.setWarped(false);
   }
   for (j = 0; j < g_->getNbPlayer(); ++j)
     for (k = 0; k < MAX_GOODMEN; ++k)
       if (g_->players[j].team[k].hasDelorean())
	 s_->updateDeloreanPosition(g_->delorean,
				    g_->players[j].team[k].row, g_->players[j].team[k].col);
}

void		ServerEntry::betEndTurn(Bet* bet)
{
   unsigned	i;
   unsigned	j;
   time_t	played_nb;
   unsigned	nb_winners = 0;
   GoodMan*	winners[MAX_TEAM * MAX_GOODMEN];


   if (g_->player_turn < bet->getDate() || g_->player_turn > (bet->getDate() + BET_LENGTH))
     {
       bet->setTimeLeft(0);
       bet->setOpen(false);
     }
   else
     {
       bet->setOpen(true);
       bet->setTimeLeft(BET_LENGTH + (bet->getDate() - g_->player_turn));
       LOG3("Bet time left for bet : " << bet->getId()
	    << " is now " << BET_LENGTH + (bet->getDate() - g_->player_turn));
       //       s_->updateBetTimeLeft(bet, );
       if (g_->player_turn == bet->getDate())
	 {
	   LOG3("Opening bet :" << bet->getId()
		<< " from casino : " << bet->getCasinoId());
	 }
       if (bet->getTimeLeft() != 0)
	 {
	   for (i = 0; i < bet->getNbPlayer(); ++i)
	     {
	       LOG3("Man " << bet->getPlayers(i) << " i've the number : " << (int)bet->getPlayers(i)->played_nb);
	       LOG3("Man " << bet->getPlayers(i) << " my action is : " << (int)bet->getPlayers(i)->action);
	     }
	   //Adding good men to bets (must be here because of time trip)
	   for (i = 0; i < g_->getNbPlayer(); ++i)
	     for (j = 0; j < MAX_GOODMEN; ++j)
	       if (g_->casinos[bet->getCasinoId()]->row == g_->players[i].team[j].row &&
		   g_->casinos[bet->getCasinoId()]->col == g_->players[i].team[j].col)
		 {
		   if (bet->addToBet(&g_->players[i].team[j]))
		     {
		       LOG3("Adding goodman " << &g_->players[i].team[j] << " of player " << i << " to bet");
		       s_->changeState(g_->players[i].team[j], STATE_BETTING);
		       //setting goodmen's played_nb
		       if (g_->players[i].hasAlmanach() || g_->players[i].hasWon(bet->getId()))
			 {
			   g_->players[i].team[j].played_nb = bet->getGoodNb();
			   LOG3("Man" << j << " I know the game, i've the good number : " << (int)g_->players[i].team[j].played_nb);
			   LOG3("The REAL GOOD NB IS " << (int)bet->getGoodNb());
			 }
		       else
			 {
			   do
			     {
			       played_nb = rand() % MAX_BET;
			     }while (g_->players[i].knowAndLost(bet->getId(), played_nb));
			   bet->getPlayers(bet->getNbPlayer() - 1)->played_nb = played_nb;
			 }
		     }
		   else
		     LOG3("Already have goodman " << j << " of player " << i << " in bet");
		 }
	 }
       //bet resolving
       else if (bet->getTimeLeft() == 0 && bet->isOpen())
	 {
	   bet->setOpen(false);
	   for (i = 0; i < bet->getNbPlayer(); ++i)
	     {
	       LOG3(" Good nb : " << bet->getGoodNb() <<
		    " played Nb : " << (int)bet->getPlayers(i)->played_nb);
	       if (bet->win(bet->getPlayers(i)->played_nb))
		 {
		   LOG3(bet->getPlayers(i)->get_id() << " won !!");
		   winners[nb_winners++] = bet->getPlayers(i);
		 }
	       else
		 LOG3(bet->getPlayers(i)->get_id() << " loose !!");
	       s_->updateRom(g_->players[bet->getPlayers(i)->get_player()],
			 bet->getPlayers(i)->get_player(),
			 bet->getId(),
			 bet->getPlayers(i)->played_nb,
			 bet->win(bet->getPlayers(i)->played_nb));
	     }
	   for (i = 0; i < nb_winners; ++i)
	       s_->updateMoney(*winners[i], winners[i]->getMoney() + bet->getKitty() / nb_winners);
	   for (i = 0; i < 4; ++i)
	     {
	       int r = g_->casinos[bet->getCasinoId()]->row + (i == 1 ? 1 : (i == 3 ? -1 : 0));
	       int c = g_->casinos[bet->getCasinoId()]->col + (i == 0 ? -1 : (i == 2 ? 1 : 0));
	       if (r >= 0 && r < g_->map_size.row && c >= 0
		   && c < g_->map_size.col &&
		   g_->terrain_type[r][c] == STANDARD)
		 {
		   LOG3("Bet ends, goodmen placed on " <<
			r << " - " << c);
		   for (i = 0; i < bet->getNbPlayer(); ++i)
		     {
		       s_->updateGdmPosition(*bet->getPlayers(i),
					     r, c);
		       s_->changeState(*bet->getPlayers(i), STATE_NORMAL);
		     }
		   break;
		 }
	     }
	   bet->nb_players = 0;
	 }
     }
}

void         ServerEntry::almanachEndTurn()
{
   std::vector<Bet*>::iterator	i = g_->almanach.getAlmanach().begin();
   int				j;
   int				k;

   for (; i != g_->almanach.getAlmanach().end(); ++i)
     betEndTurn(*i);
   for (j = 0; j < g_->getNbPlayer(); ++j)
     for (k = 0; k < MAX_GOODMEN; ++k)
       if (g_->players[j].team[k].hasAlmanach())
	 s_->updateAlmanachPosition(g_->almanach,
				    g_->players[j].team[k].row, g_->players[j].team[k].col);
}

int         ServerEntry::afterNewTurn()
{
  unsigned	i;
  unsigned	j;
  int	i1;
  bool		add_turn = true;
  int	j1;
  bool	found = false;

  //Time trip algorithm
  if (g_->delorean.hasWarped() && g_->delorean.warpTimeLeft() == 0)
    {
      LOG3("The delorean is warping this turn");
      for (i = 0; i < g_->getNbPlayer() && !found; ++i)
	for (j = 0; j < MAX_GOODMEN && !found; ++j)
	  if (g_->players[i].team[j].hasDelorean())
	    found = true;
      --j;
      --i;
      if (g_->delorean.warpTime() > g_->player_turn)
	{
	  LOG3("Going in the future.........");
	  g_->players[i].team[j].setWarpTime(g_->delorean.warpTime());
	  s_->changeState(g_->players[i].team[j], STATE_BACK_IN_THE_FUTURE);
	}
      else if (g_->delorean.warpTime() < g_->player_turn)
	{
	  LOG3("Going in the past.........");
	  add_turn = false;
	  s_->updateDate(g_->delorean.warpTime());
	  g_->delorean.setWarpTime(0);
	  for (i1 = 0; i1 < g_->getNbPlayer(); ++i1)
	    for (j1 = 0; j1 < MAX_GOODMEN; ++j1)
	      {
		s_->updateMoney(g_->players[i1].team[j1],
				g_->save_past[g_->player_turn][i1][j1].getMoney());
		if (i == i1 && j == j1)
		  {
		    s_->updateDelorean(g_->players[i].team[j], true);
		    s_->changeState(g_->players[i].team[j], STATE_NORMAL);
		    if (g_->players[i].team[j].hasAlmanach()
			|| g_->save_past[g_->player_turn][i][j].hasAlmanach())
		      s_->updateAlmanach(g_->players[i].team[j], true);
		  }
		else
		  {
		    if (g_->save_past[g_->player_turn][i1][j1].getState() != STATE_TIME_WARP
			&& g_->save_past[g_->player_turn][i1][j1].getState() != STATE_BACK_IN_THE_FUTURE)
		      s_->changeState(g_->players[i1].team[j1], g_->save_past[g_->player_turn][i1][j1].getState());
		    else
		      s_->changeState(g_->players[i1].team[j1], STATE_NORMAL);
		    s_->updateAlmanach(g_->players[i1].team[j1], !(g_->players[i].team[j].hasAlmanach())
				       && g_->save_past[g_->player_turn][i1][j1].hasAlmanach());
		    s_->updateDelorean(g_->players[i1].team[j1], false);
		    s_->updateGdmPosition(g_->players[i1].team[j1], g_->save_past[g_->player_turn][i1][j1].row,
					  g_->save_past[g_->player_turn][i1][j1].col);
		  }
	      }
	}
    }
  almanachEndTurn();
  deloreanEndTurn();
  if (add_turn)
    s_->updateDate(g_->player_turn + 1);
  //goodmen endturn
  for (i = 0; i < g_->getNbPlayer(); ++i)
    for (j = 0; j < MAX_GOODMEN; ++j)
      goodManEndTurn(g_->players[i].team[j]);
  //saving goodmen
   for (i = 0; i < g_->getNbPlayer(); ++i)
     for (j = 0; j < MAX_GOODMEN; ++j)
       g_->save_past[g_->player_turn][i][j] =
	 g_->players[i].team[j].clone(g_->save_past[g_->player_turn - (add_turn ? 1 : 0)][i][j]);
  return 0;
}


int         ServerEntry::afterGame()
{
  return 0;
}

bool        ServerEntry::isMatchFinished()
{
  return g_->player_turn >= g_->max_date;
}

static int	*tab_score = NULL;

void		ServerEntry::calculScore()
{
  for (int id = 0; id < g_->getNbPlayer(); ++id)
    {
      int score = g_->players[id].getScore();
      for (int i = 0; i < 5; ++i)
	if (tab_score[i] == -1)
	  {
	    tab_score[i] = score;
	    break;
	  }
	else if (score == tab_score[i])
	  break;
	else if (score > tab_score[i])
	  {
	    int tmp2 = tab_score[i];
	    for (int j = i, tmp = 0; j < 5; tmp = tab_score[j + 1],
		   tab_score[j + 1] = tmp2, ++j, tmp2 = tmp)
	      ;
	    tab_score[i] = score;
	    break;
	  }
    }
}

int        ServerEntry::getScore(int uid)
{
  if (tab_score == NULL)
    {
      tab_score = new int[6]();
      for (int i = 0; i < 6; ++i)
	tab_score[i] = -1;
      calculScore();
    }
  // TODO gestion des ex aequo
  for (int i = 0; i < 5; ++i)
    if (g_->players[uid].getScore() == tab_score[i])
      return (5 - i) * 200;
  return 0;
}
