/*
** TowBowlTactics, an adaptation of the tabletop game Blood Bowl.
** 
** Copyright (C) 2007 The TBT Team.
** 
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
** 
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
** 
** The TBT Team consists of people listed in the `AUTHORS' file.
*/

inline STeam* STeamMsg::getTeam(int token, int team_id)
{
  if (team_id < 0 || team_id > 1 || t_[team_id] == NULL)
    {
      LOG3("Token `%1' is addressed to non-existent team `%2'.)",
          r_->stringifyToken(token), team_id);
      r_->sendIllegal(token, team_id);
      return NULL;
    }
  return t_[team_id];
}
