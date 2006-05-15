/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2004, 2005, 2006 Prologin
*/

#ifndef CLIENT_HH_
# define CLIENT_HH_

# include "Contest.hh"

/*!
** @ingroup stechec_ant
** @{
*/

/*!
** @class _ant__Client
** @brief Ant Client.
*/
class Client: public StechecClient
{
public:
  Client(GameData *g) : StechecClient(g) {}
  virtual ~Client() {}
};

#endif /* !CLIENT_HH_ */
