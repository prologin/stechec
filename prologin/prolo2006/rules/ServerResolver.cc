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

#include "ServerResolver.hh"

void	ServerResolver::ApplyResolver(CommandListRef cmdList[])
{
  CommandListRef::iterator it;

  moveGoodMen(cmdList[MOVE_GOODMAN]);

  for (int i = 0; i < LAST_MSG; ++i)
    if (i != MOVE_GOODMAN)
      for (it = cmdList[i].begin(); it != cmdList[i].end(); ++it)
	switch (i)
	  {
	  case GOODMAN_CHANGE_STATE:
	    GoodManChangeState(*it);
	    break;
	  case MEMORIZE:
	    Memorize(*it);
	    break;
	  case DELOREAN:
	    Delorean(*it);
	    break;
	  }
}

void		ServerResolver::battleGoodmen()
{
  GoodMan	*on_case[MAX_TEAM * MAX_GOODMEN];
  unsigned	nb_same;
  unsigned	nb_left;
  unsigned	row;
  unsigned	col;
  int		tmp_i;
  int		tmp_id;
  unsigned	randm;
  unsigned	team;
  bool		same;
  int		i;
  int		id;

  for (tmp_i = 0; tmp_i < g_->getNbPlayer(); ++tmp_i)
    for (tmp_id = 0; tmp_id < MAX_GOODMEN; ++tmp_id)
      {
	on_case[0] = &g_->players[tmp_i].team[tmp_id];
	nb_same = 1;
	row = g_->players[tmp_i].team[tmp_id].row;
	col = g_->players[tmp_i].team[tmp_id].col;
	if (g_->terrain_type[row][col] == BET_PLACE)
	  continue;

	for (i = 0; i < g_->getNbPlayer(); ++i)
	  for (id = 0; id < MAX_GOODMEN; ++id)
	    {
	      // on ne prends que les gens qui sont
	      //en etat de se battre et sur la case voulue
	      if ((i == tmp_i && id == tmp_id) ||
		  (g_->players[i].team[id].getState() != STATE_NORMAL
		   && g_->players[i].team[id].getState() != STATE_TIME_WARP))
		continue;
	      if (g_->players[i].team[id].row == row &&
		  g_->players[i].team[id].col == col)
		on_case[nb_same++] =
		  &g_->players[i].team[id];
	    }
	nb_left = nb_same;
	team = on_case[0]->get_player();
	//on verifie si on a pas qu'une equipe sur la case
	for (i = 1, same = true; i < nb_same && same; ++i)
	  if (team != on_case[i]->get_player())
	    same = false;
	//si que meme equipe, on se bat pas
	//(ou que le joueur est seul sur la case)
	if (same)
	  continue;
	same = true;
	//si deux joueurs se trouvent sur la meme case, et qu'ils ont tous les deux un objet, rien ne se passe
	if (nb_same == 2 && (on_case[0]->hasDelorean() || on_case[0]->hasAlmanach())
	    &&  (on_case[1]->hasDelorean() || on_case[1]->hasAlmanach()))
	  continue;
	//ceux qui on les objet sont ko et les perdent
	for (i = 0; i < nb_same; ++i)
	  if (on_case[i]->hasDelorean())
	    {
	      s_->updateDelorean(*on_case[i], false);
	      s_->changeState(*on_case[i], STATE_KO);
	      on_case[i]->setKoTime(KO_TIME);
	      on_case[i] = 0;
	      nb_left--;
	      same = false;
	    }
	  else if (on_case[i]->hasAlmanach())
	    {
	      s_->updateAlmanach(*on_case[i], false);
	      s_->changeState(*on_case[i], STATE_KO);
	      on_case[i]->setKoTime(KO_TIME);
	      on_case[i] = 0;
	      nb_left--;
	      same = false;
	    }
	if (same && ((g_->almanach.row == row && g_->almanach.col == col)
		     || (g_->delorean.row == row && g_->delorean.col == col)))
	  same = false;
	for (i = 0; i < nb_same; ++i)
	  if (on_case[i])
	    team = on_case[i]->get_player();

	for (same = true; i < nb_same && same; ++i)
	  if (on_case[i] && team != on_case[i]->get_player())
	    same = false;
	//on combat
	while (!same)
	  {
	    randm = rand() % nb_same;
	    if (on_case[randm])
	      {
		s_->changeState(*on_case[randm], STATE_KO);
		on_case[randm]->setKoTime(KO_TIME);
		on_case[randm] = 0;
		nb_left--;
	      }
	    for (i = 0; i < nb_same; ++i)
	      if (on_case[i])
		team = on_case[i]->get_player();
	    for (same = true; i < nb_same && same; ++i)
	      if (on_case[i] && team != on_case[i]->get_player())
		same = false;
	  }
      }
}

void	ServerResolver::moveGoodMen(CommandListRef& cmdList)
{
  const	StechecPkt* elt;
  Player*	p;
  int		id;
  bool	get_delo = false;
  bool	get_alma = false;

  for (CommandListRef::iterator it = cmdList.begin();
       it != cmdList.end(); ++it)
    {
      elt = *it;
      p = &g_->players[elt->client_id];
      id = elt->arg[0];
      int	x = elt->arg[2];
      int	y = elt->arg[1];
      int	casino_id;
      GoodMan&	gdm = p->team[id];

      if (g_->terrain_type[y][x] == BET_PLACE)
	{
	  for (unsigned i = 0; i < g_->casinos.size(); ++i)
	    if (g_->casinos[i]->row == y && g_->casinos[i]->col == x)
	      {
		casino_id = i;
		break;
	      }
	  if (g_->almanach.getGameOfCasino(casino_id) == UNKNOWN)
	    {
	      LOG3("No game opened in this casino");
	      return;
	    }
	  s_->updateDelorean(p->team[id], false);
 	}
      gdm.row = y;
      gdm.col = x;
      LOG3("Resolver: move GoodMan row: %1, column: %2", y, x);
      SendToAll(*elt);
    }
  //les persos se battent
  battleGoodmen();

  for (int i = 0; i < g_->getNbPlayer(); ++i)
    for (id = 0; id < MAX_GOODMEN; ++id)
      {
	p = &g_->players[i];
	if (p->team[id].hasDelorean())
	  get_delo = true;
	if (p->team[id].hasAlmanach())
	  get_alma = true;
      }
  for (int i = 0; i < g_->getNbPlayer(); ++i)
    for (id = 0; id < MAX_GOODMEN; ++id)
      {
	p = &g_->players[i];
	if (p->team[id].getState() != STATE_NORMAL)
	  continue;
	if (p->team[id].row == g_->delorean.row
	    && p->team[id].col == g_->delorean.col && !get_delo)
	  {
	    if (g_->delorean.hasWarped())
		s_->changeState(p->team[id], STATE_TIME_WARP);
	    s_->updateDelorean(p->team[id], true);
	    get_delo = true;
	  }
	if (p->team[id].row == g_->almanach.row
	    && p->team[id].col == g_->almanach.col && !get_alma)
	  {
	    s_->updateAlmanach(p->team[id], true);
	    get_alma = true;
	  }
      }
}

void	ServerResolver::GoodManChangeState(const StechecPkt* elt)
{
  Player*	p = &g_->players[elt->client_id];
  int		id = elt->arg[0];
  GoodMan&	gmd = p->team[id];

  gmd.setState(elt->arg[1]);
  SendToAll(*elt);
}

void		ServerResolver::Memorize(const StechecPkt* elt)
{
  Player*	p = &g_->players[elt->client_id];
  int		id = elt->arg[0];

  p->memorize(id);
  LOG2("Memorized bet number %1", id);
  SendToAll(*elt);
}

//ghalaax: FIXING
void		ServerResolver::Delorean(const StechecPkt* elt)
{
  unsigned	date = elt->arg[0];
  int		id = elt->arg[1]; //id du goodman concerne
  GoodMan&	gdm = g_->players[elt->client_id].team[id];

  if (g_->delorean.hasWarped()
      || g_->delorean.getAutonomie() == 0)
    return;
  g_->delorean.setWarped(true);
  if ((unsigned)abs(date - g_->player_turn) >= g_->delorean.getAutonomie())
    date = g_->player_turn
      + (g_->delorean.getAutonomie() *
	 (date > g_->player_turn ? 1 : -1));
  g_->delorean.setWarpTime(date);
  LOG3("Preparing to warp to %1", date);
  s_->updateDeloreanAuto(g_->delorean,
			 g_->delorean.getAutonomie()
			 - abs(date - g_->player_turn));
  s_->changeState(gdm, STATE_TIME_WARP);
  g_->delorean.setWarpTimeLeft(DELOREAN_WARP_TIME);
}
