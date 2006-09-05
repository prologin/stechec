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


#define CHECK_TEAM                              \
{                                               \
  if (selected_team_ == NULL)                   \
    return BAD_TEAM;                            \
}

#define CHECK_PLAYER                            \
{                                               \
  if (selected_player_ == NULL)                 \
    return BAD_PLAYER;                          \
}

#define CHECK_POS(X, Y)                         \
{                                               \
  if (X < 0 || X >= COLS || Y < 0 || Y >= ROWS) \
    return BAD_ARGUMENT;                        \
}



inline Api::Api(CRules* rules)
  : BaseApi<CRules>(rules),
    selected_team_(NULL),
    selected_player_(NULL)
{
}

inline Api::~Api()
{
}

/*
** Actions
*/

inline void Api::doEndTurn()
{
	int team_id = rules_->getTeamId();
	if ((rules_->getState() == GS_COACH1&&team_id == 0)
			||(rules_->getState() == GS_COACH2&&team_id == 1))
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
  MsgInitKickoff pkt_endinit;
  rules_->sendPacket(pkt_endinit);  
  return true;
}

inline bool Api::doMovePlayer(int p, const Point& to)
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
  return rules_->our_team_->movePlayer(p, to);
}

inline bool Api::doStandUpPlayer(int p)
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
  return rules_->our_team_->standUpPlayer(p);
}

inline bool Api::doBlockPlayer(int p, const Point& to)
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
  return rules_->our_team_->blockPlayer(p, to);
}

inline void Api::sendChatMessage(const std::string& msg)
{
  if (rules_->getState() == GS_WAIT)
    return;
  MsgChat pkt;
  stringToPacket(pkt.msg, msg, sizeof(pkt.msg));
  rules_->sendPacket(pkt);
}

/*
** Accessors
*/

inline void Api::select_team(int team_id)
{
  assert(rules_->getState() != GS_WAIT);
  selected_team_ = NULL;
  selected_player_ = NULL;

  if (team_id == US || (team_id <= 1 && team_id == rules_->getTeamId()))
    selected_team_ = rules_->our_team_;
  if (team_id == THEM || (team_id <= 1 && team_id != rules_->getTeamId()))
    selected_team_ = rules_->other_team_;
}

inline const std::string& Api::getCoachName() const
{
  return selected_team_->getCoachName();
}

inline const std::string& Api::getTeamName() const
{
  return selected_team_->getTeamName();
}

inline const CTeam* Api::getTeam() const
{
  return selected_team_;
}

inline int Api::getTeamId() const
{
  return rules_->getTeamId();
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

inline const char* Api::getStateString() const
{
  switch (rules_->getState())
    {
    case GS_INITGAME:
      return "GS_INITGAME";
    case GS_INITKICKOFF:
      return "GS_INITKICKOFF";
    case GS_COACH1:
      return "GS_COACH1";
    case GS_COACH2:
      return "GS_COACH2";
    case GS_PAUSE:
      return "GS_PAUSE";
    }
  return BaseApi<CRules>::getStateString();
}

inline int Api::select_player(int player_id)
{
  CHECK_TEAM;
  selected_player_ = selected_team_->getPlayer(player_id);
  return selected_player_ == NULL ? BAD_PLAYER : SUCCESS;
}

inline int Api::move_lenght(int dst_x, int dst_y)
{
  CHECK_PLAYER;
  CHECK_POS(dst_x, dst_y);

  player_path_ = &rules_->field_->getPath(selected_player_->getPosition(),
                                          Position(dst_y, dst_x),
                                          selected_player_);
  return player_path_->size();
}

inline int Api::move_difficulty(int step)
{
  CHECK_PLAYER;
  if (player_path_ == NULL || step < 0 || step >= (int)player_path_->size())
    return BAD_ARGUMENT;

  Position step_pos((*player_path_)[step]);
  int nbt = rules_->field_->getNbTackleZone(selected_team_->getTeamId(), step_pos);
  if (nbt == 0)
    return 0;
  return (7 - std::min(selected_player_->getAg(), 6)) + nbt - 1;
}

inline int Api::move_path_x(int step)
{
  CHECK_PLAYER;
  if (player_path_ == NULL || step < 0 || step >= (int)player_path_->size())
    return BAD_ARGUMENT;

  return (*player_path_)[step].getX();
}

inline int Api::move_path_y(int step)
{
  CHECK_PLAYER;
  if (player_path_ == NULL || step < 0 || step >= (int)player_path_->size())
    return BAD_ARGUMENT;

  return (*player_path_)[step].getY();
}

inline int Api::move_possible(int dst_x, int dst_y)
{
  CHECK_PLAYER;
  CHECK_POS(dst_x, dst_y);
  
  return rules_->field_->getPath(selected_player_->getPosition(),
                                 Position(dst_y, dst_x),
                                 selected_player_).empty() ? 0 : 1;
}
