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

inline Api::Api(CRules* rules)
  : BaseApi<CRules>(rules)
{
  selected_team_ = NULL;
}

inline Api::~Api()
{
}

/*
** Actions
*/

inline void Api::doEndTurn()
{
  rules_->sendPacket(MsgEndTurn());
}

inline void Api::doMoveTurnMarker()
{
  rules_->sendPacket(ActMoveTurnMarker());
}

inline void Api::doAskIllegalProcedure()
{
  rules_->sendPacket(ActIllegalProc());
}

inline bool Api::doPlaceBall(const Point& pos)
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);

  Position bpos(pos);
  if (!rules_->field_->intoField(bpos))
    {
      LOG2("Kickoff rejected. Ball not on the field: " << pos);
      return false;
    }
  MsgBallPos pkt;
  pkt.row = bpos.row;
  pkt.col = bpos.col;
  rules_->sendPacket(pkt);

  // FIXME: that is all we have to do in inithalf right now.
  //  say we are ok.
  MsgInitHalf pkt_endinit;
  rules_->sendPacket(pkt_endinit);  
  return true;
}

inline bool Api::doMovePlayer(int p, const Point& to)
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
  return rules_->our_team_->movePlayer(p, to);
}

inline void Api::sendChatMessage(const std::string& msg)
{
  assert(rules_->getState() != GS_WAIT);
  MsgChat pkt;
  stringToPacket(pkt.msg, msg, sizeof(pkt.msg));
  rules_->sendPacket(pkt);
}

/*
** Accessors
*/

// Const helper.
inline void Api::switchTeam(int to_team)
{
  assert(rules_->getState() != GS_WAIT);
  if (to_team == US || (to_team <= 1 && to_team == rules_->getTeamId()))
    selected_team_ = rules_->our_team_;
  if (to_team == THEM || (to_team <= 1 && to_team != rules_->getTeamId()))
    selected_team_ = rules_->other_team_;
}

inline const std::string& Api::getTeamName() const
{
  return selected_team_->getTeamName();
}

inline const std::string& Api::getCoachName() const
{
  return selected_team_->getCoachName();
}

inline const CTeam* Api::getTeam() const
{
  return selected_team_;
}

inline const CPlayer* Api::getPlayer(int player_id) const
{
  return selected_team_->getPlayerConst(player_id);
}

inline const CPlayer* Api::getPlayer(const Point& pos) const
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
  if (!rules_->field_->intoField(Position(pos)))
    return NULL;
  return rules_->field_->getPlayerConst(Position(pos));
}

inline const Weather* Api::getWeather() const
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
  return rules_->weather_;
}

inline Point Api::getBallPosition() const
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
  return Point(rules_->ball_->getPosition());
}

inline const CField* Api::getField() const
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
  return rules_->field_;
}
