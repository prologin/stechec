/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2006 Prologin
*/

#include "Server.hh"

#include "GameData.hh"
#include "ServerEntry.hh"

void	Server::changeState(GoodMan& gdm, int state)
{
  gdm.setState(state);
  SendToAll(GOODMAN_CHANGE_STATE, gdm.get_player(), 2, gdm.get_id(), state /*FIXME A voir */);
}

void	Server::updateDelorean(GoodMan& gdm, bool state)
{
  if (state)
    gdm.getDelorean();
  else
    gdm.lostDelorean();
  SendToAll(GOODMAN_DELOREAN, -1, 3, gdm.get_id(), state ? 1 : 0, gdm.get_player());
}

void	Server::updateAlmanach(GoodMan& gdm, bool state)
{
   if (state)
      gdm.getAlmanach();
   else
      gdm.lostAlmanach();
   SendToAll(GOODMAN_ALMANACH, -1, 3, gdm.get_id(), state ? 1 : 0, gdm.get_player());
}

void	Server::updateGdmPosition(GoodMan& gdm, int row, int col)
{
  LOG3("Moving goodman from %1 - %2 to %3 - %4",
       gdm.row, gdm.col, row, col);
   gdm.row = row;
   gdm.col = col;
   SendToAll(MOVE_GOODMAN, gdm.get_player(), 3, gdm.get_id(), row, col);
}

void	Server::updateMoney(GoodMan& gdm, unsigned money)
{
  LOG3("Goodman %1 won %2", gdm.get_id(), money);
  gdm.setMoney(money);
  SendToAll(GOODMAN_MONEY_CHANGE, gdm.get_player(), 2, gdm.get_id(), money);
}


void	Server::updateDeloreanPosition(Delorean& delorean, int row, int col)
{
   delorean.row = row;
   delorean.col = col;
   SendToAll(DELOREAN_MOVE, -1, 2, row, col);
}

void	Server::updateDeloreanAuto(Delorean& delorean, unsigned int auton)
{
  delorean.setAutonomie(auton);
  SendToAll(DELOREAN_AUTO, -1, 1, auton);
}

void	Server::updateReloadTime(Delorean& delorean, int time)
{
  delorean.setReloading(time);
  SendToAll(DELOREAN_RELOADING, -1, 1, time);
}

void	Server::updateAlmanachPosition(Almanach& almanach, int row, int col)
{
   almanach.row = row;
   almanach.col = col;
   SendToAll(ALMANACH_MOVE, -1, 2, row, col);
}

void	Server::updateBetTimeLeft(Bet* bet, unsigned time)
{
  bet->setTimeLeft(time);
  LOG3("Bet time left for bet: %1 is now %2", bet->getId(), time);
  //  SendToAll(BET_TIME, -1, 2, bet->getId(), time);
}

void	Server::updateDate(unsigned date)
{
   g_->player_turn = date;
   SendToAll(CHANGE_DATE, -1, 1, date);
}

void	Server::updateRam(Player& player, int player_id, int bet_id)
{
   player.memorize(bet_id);
   SendToAll(MEMORIZE, player_id, 1, bet_id);
}

void	Server::updateRom(Player& player, int player_id, int bet_id, int result, bool win)
{
   player.recordGame(bet_id, win, result);
   SendToAll(UPDATE_ROM, player_id, 3, bet_id, win ? 1 : 0, result);
}

void		Server::addCasino(GameData *g, unsigned int row, unsigned int col)
{
   g->addCasino(row, col);
   SendToAll(NEW_CASINO, -1, 2, row, col);
}

void		Server::addBet(Almanach& almanach,
			       unsigned date,
			       unsigned kitty,
			       unsigned result,
			       unsigned casino_id)
{
   almanach.addBet(date, kitty, result, casino_id);
   SendToAll(NEW_BET, -1, 4, date, kitty, result, casino_id);
}
