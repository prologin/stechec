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

#ifndef CTEAMMSG_HH_
# define CTEAMMSG_HH_

# include "CTeam.hh"

/*!
** @brief Handle messages for CTeam objects.
** @ingroup tbt_client_api
**
** There is a single instance of this class, that dispatch
** message to the good CTeam object.
*/
class CTeamMsg
{
public:
  CTeamMsg(CRules* r);
  ~CTeamMsg();

  void setTeam(int team_id, CTeam *t);

private:
  CTeam* getTeam(int token, int team_id);

  void msgTeamInfo(const MsgTeamInfo* m);
  void msgPlayerCreate(const MsgPlayerCreate* m);
  void msgReroll(const MsgReroll* m);
  void msgTouchdooown(const MsgTouchdooown* m);

  CRules* r_;
  CTeam* t_[2];
};

#endif /* !CTEAMMSG_HH_ */
