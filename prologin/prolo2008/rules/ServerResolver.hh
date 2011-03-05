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
  void ResolveOrderWithDirection(StechecPkt *pkt, int type);
  void ResolveSimpleOrder(StechecPkt *pkt, int type);

  int _directions[5][2]; // translations' vectors (column,line)

  // some utility, low_level functions :
  bool CheckPosition(int x, int y);
  int GetRobotIdInPos(int x, int y); //returns -1 if no robot
  bool CanDoSimpleMove(int x0, int y0, int x1, int y1, bool pushed);
  void UpdateRobotPos(int id, int new_x, int new_y, int dir, int pushed_by, int pushing);
  bool ApplyChainMove(int dir, int id, int x, int y, 
		      int next_x, int next_y, bool first, int pushed_by);
  bool ApplyDrop(int id, int dir, int x, int y, int target_x, int target_y);
  bool ApplyPickUpBall(int id, int x, int y);
  bool ApplyHook(int id, int x, int y, int target_x, int target_y);
  bool ApplyLaunch(int id, int dir, int x, int y);
  bool ApplyTurbo(int id);

  void LogAction(int action, int real_id, int arg=0);
  int vector_to_direction(int dx, int dy);
};

#endif // !SERVERRESOLVER_HH_
