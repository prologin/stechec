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
  MsgIllegal pkt(from);
  pkt.was_token = token;
  sendPacket(pkt);
}

inline int SRules::getCurrentTeamId() const
{
  if (getState() == GS_COACH1 || getState() == GS_COACH2)
    return getState();
  if (getState() == GS_INITHALF)
    return coach_begin_;
  return -1;
}

inline int SRules::getCurrentOpponentTeamId() const
{
  int id = getCurrentTeamId();
  if (id == -1)
    return id;
  return id == GS_COACH1 ? GS_COACH2 : GS_COACH1;
}

inline SField* SRules::getField()
{
  return field_;
}

inline SBall* SRules::getBall()
{
  return ball_;
}
