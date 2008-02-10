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

#ifndef SERVERRESOLVER_HH_
# define SERVERRESOLVER_HH_

# include "GameData.hh"
# include "Server.hh"

class ServerResolver : public StechecServerResolver
{
public:
  ServerResolver(GameData* game, Server* server);

  virtual void	ApplyResolver(CommandListRef cmdList[]);

private:
  void ResolveOrder(StechecPkt *pkt, int type);

};

#endif // !SERVERRESOLVER_HH_
