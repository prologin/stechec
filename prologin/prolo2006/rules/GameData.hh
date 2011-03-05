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

#ifndef GAMEDATA_HH_
# define GAMEDATA_HH_

# include <list>
# include <map>
# include "Contest.hh"
# include "Constant.hh"

/*****************************************************************************/
/*!
** The superclass for all objects (unities & buildings)
*/

class                Object : public Position
{
public:
  Object();
  unsigned		get_id() const { return id_; }
  unsigned		get_player() const { return player_id_; }
  /*Maybe adding a method named void action()
    which will do standard action on the objects*/
  void			set_id(int id) { id_ = id; }
  void			set_player(int player_id) { player_id_ = player_id; }
private:
  unsigned		id_;		/* Object's identifier */
  unsigned		player_id_;	/* Player's identifier */
};


/*****************************************************************************/
/*!
** The class for the Good Men
*/

class		GoodMan : public Object
{
public:
  GoodMan ();
  int			getState() const		{return state; }
  unsigned int		getMoney() const		{return money; }
  unsigned int		getPlayedNb() const		{return played_nb; }
  void			setPlayedNb(unsigned char nb)	{played_nb = nb; }
  bool			hasAlmanach() const		{return almanach; }
  bool			hasDelorean() const		{return delorean; }
  unsigned		warpTime() const		{return warp_time; }
  void			setAction(bool act)		{action = act;}
  //  void			setPlayedNb(unsigned char nb)	{played_nb = nb;}
  void			setState(int st)		{state = st; }
  void			addMoney(int val)		{money += val; }
  void			setMoney(int val)		{money = val; }
  void			getDelorean()			{delorean = true; }
  void			getAlmanach()			{almanach = true; }
  void			lostAlmanach()			{almanach = false; }
  void			lostDelorean()			{delorean = false; }
  void			setWarpTime(unsigned nb)	{warp_time = nb; }
  void			turnAction(unsigned player_turn);
  GoodMan&		clone(GoodMan& res) const;
  void			restore(GoodMan*);
  unsigned int		getKoTime()			{return ko_time; }
  void			setKoTime(unsigned int time)	{ko_time = time; }
  bool			action;		/* True if the GoodMan is in action */
  int			played_nb;
private:
  int			state;		/* GoodMan's state */
  unsigned		money;		/* GoodMan's money */
  bool			almanach;	/* True if the GoodMan has the almanach */
  bool			delorean;	/* True if the GoodMan has the Delorean */
  unsigned		warp_time;	/* Number of remaining turns to
					   effectly return in the time */
  unsigned int		ko_time;
};

/*****************************************************************************/
/*!
** The class for the DeLorean
*/

class		Delorean : public Object
{
public:
  Delorean();
  unsigned char	reloadingTimeLeft() const	{ return reload_time; }
  void		setReloading(int time)		{ reload_time = time; }
  unsigned char	warpTimeLeft() const		{ return time_to_warp; }
  void		setWarpTimeLeft(int time)	{ time_to_warp = time; }
  void		turnAction();
  bool		hasWarped()			{return has_warped; }
  void		setWarped(bool warp)		{has_warped = warp; }
  void		cancelTimeTrip()		{
						    reload_time = DELOREAN_RELOAD;
						    time_to_warp = 0;
						    has_warped = false;
						}
  unsigned int	getAutonomie()			{return autonomie_;}
  void		setAutonomie(unsigned int autonomie){autonomie_ = autonomie;}

  unsigned		warpTime() const		{return warp_time; }
  void			setWarpTime(unsigned nb)	{warp_time = nb; }
private:
  unsigned char	reload_time;
  unsigned char	time_to_warp;
  unsigned	warp_time;	/* date where to warp*/
  bool		has_warped;
  unsigned int	autonomie_;
};

/*****************************************************************************/
/*!
** The class for the Bet
*/

class	Bet
{
public:
  Bet();
  Bet(int, unsigned, unsigned, unsigned, unsigned);
  unsigned	getId() const				{ return id_; }
  void		setId(int id)				{ id_ = id; }
  unsigned	getCasinoId() const			{ return casino_id; }
  void		setCasinoid(int id)			{ casino_id = id; }
  unsigned	getDate() const				{ return date; }
  void		setDate(int new_date)			{ date = new_date; }
  unsigned	getKitty() const			{ return kitty; }
  void		setKitty(int new_kitty)			{ kitty = new_kitty; }
  unsigned	getGoodNb() const			{ return good_nb; }
  void		setGoodNb(int nb)			{ good_nb = nb; }
  unsigned	getTimeLeft() const			{ return game_time_left; }
  void		setTimeLeft(int time)			{ game_time_left = time; }
  bool		win(unsigned char played_nb) const	{ return good_nb == played_nb; }
  void		turnAction(unsigned player_turn);
  bool		addToBet(GoodMan*);
  void		setOpen(bool op)			{open = op; }
  bool		isOpen()				{return open; }
  int		getNbPlayer()				{return nb_players; }
  GoodMan*	getPlayers(int i)			{return players[i]; }
  unsigned	nb_players;
private:
  int		id_;
  unsigned	casino_id;
  unsigned	date;
  unsigned	kitty;
  unsigned char	good_nb;
  unsigned	game_time_left;
  GoodMan*	players[MAX_TEAM * MAX_GOODMEN];
  bool		open;
};

class		Almanach : public Object
{
public:
  Almanach();
  int			getGameOfCasino(unsigned casino_id);
  int			getCasinoOfGame(unsigned bet_id);
  int			getGameNumber();
  int			getGameDate(unsigned int id);
  int			getGameKitty(unsigned bet_id);
  void			turnAction(unsigned player_turn);
  std::vector<Bet*>&	getAlmanach()			{return almanach; }
  void			addBet(unsigned date, unsigned kitty, unsigned result, unsigned casino)
			{
			  almanach.push_back(new Bet(casino, kitty, date, result, almanach.size()));
			}
private:
  std::vector<Bet*>	almanach;
};


/*****************************************************************************/
/*!
** The class for the Players
*/

class			Player
{
  /* Accessibility typedef */
  typedef std::list<std::pair<int, std::pair<bool, int>* > >	memory;

public:
  Player ();
  GoodMan		team[MAX_GOODMEN];
  unsigned int		getScore() const;
  bool			knowGame(int bet_id);
  void			memorize(int bet_id);
  void			recordGame(std::pair<int, std::pair<bool, int>* >*);
  void			recordGame(unsigned int bet_id, bool win, unsigned result);
  bool			hasWon(unsigned int bet_id);
  bool			knowAndLost(unsigned int bet_id, unsigned char result);
  bool			hasAlmanach()
  {
    for (int i = 0; i < MAX_GOODMEN; ++i)
      if (team[i].hasAlmanach() && team[i].getState() != STATE_BACK_IN_THE_FUTURE)
	return true;
    return false;
  }
  struct Position	start;
private:
  memory		rom;
  std::list<int>	ram;

};

/*****************************************************************************/
/*!
** The class for the Casino
*/

class			Casino : public Object
{
public:
  Casino ();
};


/*!
** This class is meant to contain all data, accessible from
** everywhere.
**
*/

class GameData: public StechecGameData
{
public:
  /* Accessibility typedef */

  GameData();

  struct Position	map_size;
  int			max_date;
  unsigned		player_turn; ///< Actual Date.

  int			time_trip; // Id of the GoodMan that wrapped Otherwise -1
  uint8_t		terrain_type[MAX_MAP_SIZE][MAX_MAP_SIZE];
  uint8_t		terrain_graphic[MAX_MAP_SIZE][MAX_MAP_SIZE];

  //Data structure that will contains the pathfinding calculation
  //The std::pair<Position*,Position*> reference respectively to
  //the start position and the end position as asked by the player
  //The second pstd::pair contains the first move to do to go to
  //the end position from the start one, the second field of the
  //pair is the min length to go from start to end

  char				visibility[MAX_MAP_SIZE][MAX_MAP_SIZE][MAX_MAP_SIZE][MAX_MAP_SIZE];
  void			turnAction();
  void			addCasino(unsigned row, unsigned col)
			{
			  Casino*	res;
			  res = new Casino;
			  res->row = row;
			  res->col = col;
			  res->set_id(casinos.size());
			  casinos.push_back(res);
			}

  Delorean		delorean;
  Almanach		almanach;
  Player		players[MAX_TEAM];
  std::vector<Casino*>	casinos;
  Player		*p;

  GoodMan	save_past[MAX_DATE][MAX_TEAM][MAX_GOODMEN];
 bool		delorean_found;

  void InitMap();
  void FreeData();
};

#endif // !GAMEDATA_HH_
