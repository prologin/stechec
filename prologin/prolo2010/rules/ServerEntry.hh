/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2010 Prologin
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

  // Recupere le score, appele pour chaque joueur a la fin
  // de la partie
  virtual int	getScore(int uid);
};

#endif // !SERVERENTRY_HH_
