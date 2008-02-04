/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2008 Prologin
*/

#include <vector>
#include "ServerResolver.hh"

ServerResolver::ServerResolver(GameData* game, Server* server)
  : StechecServerResolver(game, server)
{
}

static void     ResolveMoves(CommandListRef &cmdList)
{
  CommandListRef::iterator iter;

  for (iter = cmdList.begin(); iter != cmdList.end(); ++iter)
  {
    int robot_id = (*iter)->arg[0];
    LOG2("Robot %1 is moving", robot_id);
  }
}

void ServerResolver::ApplyResolver(CommandListRef cmdList[])
{
  // cmdList is an array representing each types of packets.
  // cmdList[PCK_TYPE] evaluates to a list, driven by an iterator.
  ResolveMoves(cmdList[MOVE]);
}
