/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2005, 2006 Prologin
*/

#ifndef STECHECSERVERRESOLVER_HH_
# define STECHECSERVERRESOLVER_HH_

# include "StechecServer.hh"

/*!
** Hook on the server-side to authorize/deny actions wanted by the
**  client.
**
** This class is called *only on server* side.
** It receives, after each turn, a list of actions made by clients.
**
** You must:
**  - update your game data.
**  - send informations to clients so that they can update their
**    data (Differ)
**
** You can:
**  - access to the GameData class: g_
**  - send messages to client: void SendToAll(const StechecPkt&)
*/
class StechecServerResolver : protected StechecServer
{
public:

  StechecServerResolver(GameData* game, Server* server);
  virtual ~StechecServerResolver();
  
  virtual void ApplyResolver(CommandListRef cmdList[]) = 0;

private:

  //! Thus only SRules can call ApplyResolverPriv.
  friend class SRules;

  //! @brief Sort cmdList by token, eventually call ApplyResolver.
  void ApplyResolverPriv(CommandList& cmdList);
};

#endif // !STECHECSERVERRESOLVER_HH_
