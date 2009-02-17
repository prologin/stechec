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

#ifndef CLIENTENTRY_HH_
# define CLIENTENTRY_HH_

# include "Contest.hh"
# include "ClientDiffer.hh"
# include "Client.hh"

class ClientEntry : public StechecClientEntry
{
public:

  ClientEntry(GameData* game, ClientDiffer* diff, Client* client);

  virtual int	beforeGame();
  virtual int	initGame();
  virtual int	beforeNewTurn();
  virtual int	afterNewTurn();
  virtual int	afterGame();
};

#endif // !CLIENTENTRY_HH_
