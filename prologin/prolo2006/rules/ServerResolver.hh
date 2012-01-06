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

#ifndef SERVERRESOLVER_HH_
# define SERVERRESOLVER_HH_

# include "GameData.hh"
# include "Server.hh"

class ServerResolver : public StechecServerResolver
{
public:
  ServerResolver(GameData* game, Server* server):
    StechecServerResolver(game, server)
  {}

  virtual void ApplyResolver(CommandListRef cmdList[]);

private:
  void	moveGoodMen(CommandListRef& cmdList);
  void	GoodManChangeState(const StechecPkt* elt);
  void	battleGoodmen();
  void	Memorize(const StechecPkt* elt);
  void	Delorean(const StechecPkt* elt);
};

#endif // !SERVERRESOLVER_HH_
