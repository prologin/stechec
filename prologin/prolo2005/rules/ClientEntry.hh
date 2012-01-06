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

#ifndef CLIENTENTRYPOINT_HH_
# define CLIENTENTRYPOINT_HH_

# include "Contest.hh"
# include "ClientDiffer.hh"
# include "Client.hh"

/*!
** This class provides hooks for, either for real player or for visio.
**
** You can:
**  - overload any virual method you wants
**  - retrieve message from server: void fetchCommand(const StechecPkt*)
**  - access to the GameData class: _g
**  - access to the Differ class: _differ
** 
** Note:
**  - prolo_after_game() (where you usually free memory) is called only
**    if prolo_before_game() was called.
*/
class ClientEntry: public StechecClientEntry
{
public: 

  ClientEntry(GameData* game, ClientDiffer* diff, Client* client);
    
  virtual int                beforeGame();
  virtual int                initGame();
  virtual int                beforeNewTurn();
  virtual int                afterNewTurn();
  virtual int                afterGame();
};

#endif /* !CLIENTENTRYPOINT_HH_ */

