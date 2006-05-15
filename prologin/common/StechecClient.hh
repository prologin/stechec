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

#ifndef STECHECCLIENT_HH_
# define STECHECCLIENT_HH_

/*!
**
*/
class StechecClient
{
public:

  StechecClient(GameData* game);  
  StechecClient(GameData* game, Client* client);
  virtual ~StechecClient();

protected:
  GameData*     g_;
  Client*       c_;
};



inline StechecClient::StechecClient(GameData* game)
  : g_(game)
{
  c_ = reinterpret_cast<Client*>(this);
}
  
inline StechecClient::StechecClient(GameData* game, Client* client)
  : g_(game),
    c_(client)
{
}

inline StechecClient::~StechecClient()
{
}

#endif // !STECHECCLIENT_HH_
