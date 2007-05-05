/*
** TowBowlTactics, an adaptation of the tabletop game Blood Bowl.
** 
** Copyright (C) 2006 The TBT Team.
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

inline void SRules::sendIllegal(int token, int from) const
{
  sendIllegal(token, from, ERR_UNASSIGNED);
}

inline void SRules::sendIllegal(int token, int from, enum eError error) const
{
  MsgIllegal pkt(from);
  pkt.was_token = token;
  pkt.error = error;
  sendPacket(pkt);
}

inline int SRules::getCurrentTeamId() const
{
  if (getState() == GS_COACH1)
    return 0;
  if (getState() == GS_COACH2)
    return 1;
  if (getState() == GS_INITKICKOFF
      || getState() == GS_KICKOFF
      || getState() == GS_TOUCHBACK)
    return coach_receiver_;
  return -1;
}

inline int SRules::getCurrentOpponentTeamId() const
{
  int id = getCurrentTeamId();
  if (id == -1)
    return -1;
  return (id + 1) % 2;
}

inline int SRules::getOpponentTeamId(int team_id) const
{
  return (team_id == 0) ? 1 : 0;
}

inline int SRules::getOpponentTeamId(STeam* team) const
{
  return getOpponentTeamId(team->getTeamId());
}

inline STeam* SRules::getCurrentTeam()
{
  return getTeam(getCurrentTeamId());
}

inline STeam* SRules::getCurrentOpponentTeam()
{
  return getTeam(getCurrentOpponentTeamId());
}

inline STeam* SRules::getOpponentTeam(int team_id)
{
  return getTeam(getOpponentTeamId(team_id));
}

inline STeam* SRules::getOpponentTeam(STeam* team)
{
  return getTeam(getOpponentTeamId(team));
}

inline SField* SRules::getField()
{
  return field_;
}

inline SBall* SRules::getBall()
{
  return ball_;
}

inline STeam* SRules::getTeam(int team_id)
{
  if (team_id == 0 || team_id == 1)
    return team_[team_id];
  return NULL;
}

inline Dice* SRules::getDice()
{
  return dice_;
}

inline SActionHandler* SRules::getActionHandler()
{
  return action_handler_;
}
