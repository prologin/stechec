/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2006, 2007 Prologin
*/

#include "GameData.hh"

GameData::GameData () :
   player_turn (0),
   delorean_found(false)
{}

Player::Player()
{

}

// we cant put this into the destructor, as we aren't sure
// that data will be malloc'ed
void	GameData::FreeData ()
{
}


void	GameData::InitMap ()
{
}


Object::Object()
{
  id_ = -42;
  player_id_ = -42;
}


/***************************************************************************/
/*!
** Implementation of GoodMan methods
*/

GoodMan::GoodMan ()
{
   state = STATE_NORMAL;
   action = false;
   almanach = false;
   delorean = false;
   money = 0;
   warp_time = 0;
   ko_time = 0;
}

GoodMan&	GoodMan::clone(GoodMan& res) const
{
  res.state = state;
  res.money = money;
  res.almanach = almanach;
  res.delorean = false;
  res.warp_time = 0;
  res.row = row;
  res.col = col;
  res.ko_time = ko_time;
  return res;
}

void		GoodMan::restore(GoodMan* gdm)
{
   if (state != STATE_TIME_WARP && state != STATE_BACK_IN_THE_FUTURE)
      state = gdm->state;
   else
      state = STATE_NORMAL;
   money = gdm->money;
   almanach = gdm->almanach;
   delorean = false;
   warp_time = 0;
}

void		GoodMan::turnAction(unsigned player_turn)
{
   if (warp_time == player_turn && state == STATE_BACK_IN_THE_FUTURE)
   {
      warp_time = 0;
      state = STATE_NORMAL;
   }
   if (ko_time != 0)
      ko_time--;
   else if (ko_time == 0 && state == STATE_KO)
      state = STATE_NORMAL;
}


/***************************************************************************/
/*!
** Implementation of Almanach methods
*/

Almanach::Almanach()
{
}

int			Almanach::getGameOfCasino(unsigned casino_id)
{
   std::vector<Bet*>::iterator	i = almanach.begin();

   for (; i != almanach.end(); ++i)
      if ((*i)->getTimeLeft() > 0
	  && (*i)->getCasinoId() == casino_id)
	 return (*i)->getId();
   return UNKNOWN;
}

int			Almanach::getCasinoOfGame(unsigned bet_id)
{
   std::vector<Bet*>::iterator	i = almanach.begin();

   for (; i != almanach.end(); ++i)
     if ((*i)->getId() == bet_id)
       return (*i)->getCasinoId();
   return UNKNOWN;
}

int			Almanach::getGameNumber()
{
   return almanach.size();
}

int			Almanach::getGameKitty(unsigned bet_id)
{
   std::vector<Bet*>::iterator	i = almanach.begin();

   for (; i != almanach.end(); ++i)
      if ((*i)->getId() == bet_id)
	 return (*i)->getKitty();
   return UNKNOWN;
}

int			Almanach::getGameDate(unsigned bet_id)
{
   std::vector<Bet*>::iterator	i = almanach.begin();

   for (; i != almanach.end(); ++i)
      if ((*i)->getId() == bet_id)
	 return (*i)->getDate();
   return UNKNOWN;
}

/***************************************************************************/
/*!
** Implementation of Player methods
*/

unsigned int	Player::getScore() const
{
  unsigned int	res;
  int		i;

  for (res = 0, i = 0; i < MAX_GOODMEN; ++i)
    res += team[i].getMoney();
  return res;
}

bool			Player::knowGame(int bet_id)
{
   memory::iterator		i = rom.begin();
   std::list<int>::iterator	j = ram.begin();

   for (; i != rom.end(); ++i)
      if ((*i).first == bet_id)
	 return true;
   for (; j != ram.end(); ++j)
      if ((*j) == bet_id)
	 return true;
   return false;
}

bool		Player::hasWon(unsigned int bet_id)
{
   memory::iterator		i = rom.begin();
   std::list<int>::iterator	j = ram.begin();

   for (; i != rom.end(); ++i)
      if ((*i).first == bet_id && (*i).second->first)
	 return true;
   for (; j != ram.end(); ++j)
      if ((*j) == bet_id)
	 return true;
   return false;
}

bool		Player::knowAndLost(unsigned int bet_id, unsigned char res)
{
   memory::iterator		i = rom.begin();

   for (; i != rom.end(); ++i)
      if ((*i).first == bet_id && (*i).second->second == res && (*i).second->first == false)
	 return true;
   return false;
}

void		Player::memorize(int bet_id)
{
   if (ram.size() == RAM_SIZE)
      ram.pop_back();
   ram.push_front(bet_id);
}

void		Player::recordGame(std::pair<int, std::pair<bool, int>* > *res)
{
  memory::iterator	i = rom.begin();

  for (; i != rom.end(); ++i)
    {
      if ((*i).first == res->first && (*i).second->first)
	{
	   delete res->second;
	   delete res;
           return;
	}
    }
  rom.push_front(*res);
}

void		Player::recordGame(unsigned int bet_id, bool win, unsigned result)
{
  std::pair<int, std::pair<bool, int>* >*	record = new std::pair<int, std::pair<bool, int>* >;
  std::pair<bool, int>*				res = new std::pair<bool, int>;

  record->first = bet_id;

  res->first = win;
  res->second = result;

  record->second = res;
  recordGame(record);
}

/***************************************************************************/
/*!
** Implementation of Bet methods
*/

Bet::Bet(int casino, unsigned kit, unsigned date_bet, unsigned res, unsigned newid)
{
   open = false;
   nb_players = 0;
   casino_id = casino;
   date = date_bet;
   kitty = kit;
   id_ = newid;
   good_nb = res;
   game_time_left = 0;
}

bool		Bet::addToBet(GoodMan* gdm)
{
   unsigned	i;

   for (i = 0; i < nb_players; ++i)
      if (gdm == players[i])
	return false;
   players[nb_players] = gdm;
   //	 gdm->setState(STATE_BETTING);
   nb_players++;
   return true;
}


/***************************************************************************/
/*!
** Implementation of Delorean methods
*/

Delorean::Delorean()
{
   reload_time = 0;
   time_to_warp = 0;
   has_warped = false;
   warp_time = 0;
   autonomie_ = DELOREAN_AUTONOMIE;
}

/*!
** Implementation of Casinos methods
*/

Casino::Casino()
{
}




/*!
** @brief Module description
*/
extern "C" const struct RuleDescription rules_description = {
  "prolo2006",
  "Prologin 2006 final contest rules",
  1,
  0,
};
