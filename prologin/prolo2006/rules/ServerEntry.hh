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

#ifndef SERVERENTRY_HH_
# define SERVERENTRY_HH_

# include "Contest.hh"
# include "Server.hh"

class ServerEntry : public StechecServerEntry
{
public:

  ServerEntry(GameData* game, Server* server, const ConfSection* cfg);
  virtual ~ServerEntry();

  // Different entry pointg
  virtual int	beforeGame();
  virtual int	initGame();
  virtual int	beforeNewTurn();
  virtual int	afterNewTurn();
  virtual int	afterGame();

  // Indique la fin du jeu
  virtual bool	isMatchFinished();

  // Recupere le score
  virtual int	getScore(int uid);
  void		calculScore();
private:
  int		ParseOptions();
  int		LoadMap(const std::string& map_file);
  void		displayMap();
  void		pathCalculation();
  void		goodManEndTurn(GoodMan&);
  void	        deloreanEndTurn();
  void		almanachEndTurn();
  void		betEndTurn(Bet*);

  std::string	mapFile;

  int		score_[MAX_PLAYER];
};

#endif // !SERVERENTRY_HH_
