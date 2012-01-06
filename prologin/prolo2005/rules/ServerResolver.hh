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

#ifndef RESOLVER_HH_
# define RESOLVER_HH_

# include "GameData.hh"
# include "Server.hh"

/*!
** This class is called *only on server* side.
** It receives, after each turn, a list of actions made by clients.
**
** You must:
**  - update your game data.
**  - send informations to clients so that they can update their
**    data (Differ)
**
** You can:
**  - access to the GameData class: _g
**  - send messages to client: void SendToAll(const StechecPkt *data)
*/
class ServerResolver: public StechecServerResolver
{
public:

  ServerResolver(GameData* game, Server* server):
    StechecServerResolver(game, server)
  {}

  virtual void ApplyResolver(CommandListRef cmdList[]);

private:
  void MoveColeoptera(CommandListRef& cmdList);
  void Dock_to_coleoptera(const StechecPkt* elt);
  void Dock_to_building(const StechecPkt* elt);
  void Transfert(const StechecPkt* elt);
  void ColeopteraChangeState(const StechecPkt* elt);
  void ThrowPlasma(const StechecPkt* elt);
  void MakeBuilding(const StechecPkt* elt);
  void MakeColeoptera(const StechecPkt* elt);
  void RepairColeoptera(StechecPkt* elt);
  void RepairColeopteraByFactory(const StechecPkt* elt);
  
  void Explosion(int x, int y);
  void repair_coleoptera_by_coleoptera(const StechecPkt* elt);

  bool in_map(unsigned x, unsigned y)
  {
    return x < g_->map_size_x && y < g_->map_size_y;
  }
};

#endif // !RESOLVER_HH_
