/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2005, 2006 Prologin
*/

#ifndef STECHECCLIENTDIFFER_HH_
# define STECHECCLIENTDIFFER_HH_

#include "StechecGameData.hh"

/*!
** Receive game modification from the server, and apply them.
**
** This class is called *only on client* (player or visio) side.
** It receives any changes made by the server after each turn.
**
** You must:
**  - handle all or parts of messages that are given to ApplyDiff
**
** You can:
**  - access to the GameData class: g_
*/
class StechecClientDiffer : protected StechecClient
{
public:

  StechecClientDiffer(GameData* game, Client* client);
  virtual ~StechecClientDiffer();

  virtual void ApplyDiff(const StechecPkt *com) = 0;
};


inline StechecClientDiffer::StechecClientDiffer(GameData* game, Client* client)
  : StechecClient(game, client)
{
}

inline StechecClientDiffer::~StechecClientDiffer()
{
}


#endif // !STECHECCLIENTDIFFER_HH_
