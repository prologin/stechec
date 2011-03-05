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

#ifndef SERVER_HH_
# define SERVER_HH_

# include "Contest.hh"
# include "GameData.hh"

class Server : public StechecServer
{
public:
  Server(GameData *g) : StechecServer(g) {}
  virtual ~Server() {}

  void	changeState(GoodMan& gdm, int state);//
  void	updateDelorean(GoodMan& gdm, bool state);//
  void	updateAlmanach(GoodMan& gdm, bool state);//
  void	updateGdmPosition(GoodMan& gdm, int row, int col);//
  void	updateDeloreanPosition(Delorean& delorean, int row, int col);//
  void	updateReloadTime(Delorean& delorean, int time);//
  void	updateAlmanachPosition(Almanach& almanach, int row, int col);//
  void	updateDate(unsigned date);//
  void	updateMoney(GoodMan& gdm, unsigned money);//
  void	updateRam(Player& player, int player_id, int bet_id);//
  void	updateRom(Player& player, int player_id, int bet_id, int result, bool win);//
  void	updateBetTimeLeft(Bet* bet, unsigned time);
  void	addCasino(GameData* g, unsigned int row, unsigned int col);
  void	addBet(Almanach& almanach,
	       unsigned date,
	       unsigned kitty,
	       unsigned result,
	       unsigned casino_id);
  void  updateDeloreanAuto(Delorean& delorean, unsigned int auton);
};

#endif // !SERVER_HH_

