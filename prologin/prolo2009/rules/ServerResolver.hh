/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
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
  // Copy all the orders from cmdList to g_->pass_threw_orders_. Only the orders
  // that affect an (x,y) position in the map (from FIRST_CLIENT_MSG to LAST_XY_ORDERS)
  // are copied. Besides, only *one* order per cell will be copied (the last one,
  // the only useful for the other clients).
  // g_->nb_pass_threw_orders_ is set to be the number of copied (pruned) orders.
  // During phase 2, auction orders are copied (and 0-auctions are added
  // for the players who haven't made any).
  void CopyPassThrewOrder(CommandListRef cmdList[]);
  // Gather all money and score updates from the playing clients. g_->argent_ and 
  // g_->score_ are updated.
  void ResolveMoneyAndScores(CommandListRef cmdList[]);

};

#endif // !SERVERRESOLVER_HH_
