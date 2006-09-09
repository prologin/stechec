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

inline bool Api::doReroll()
{
	assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
	if (!selected_team_->canUseReroll())
    {
      LOG2("Can't use reroll (no one left or allready use one this turn).");
      return false;
    }
	if (rules_->getState() != GS_REROLL&&rules_->getState() != GS_BLOCK)
		{
      LOG2("Can't use reroll (no dice to reroll).");
      return false;
    }
		
	MsgReroll msg(rules_->our_team_->getTeamId());
	msg.reroll = true;
	rules_->sendPacket(msg);	return true;
}

inline bool Api::doAccept()
{
	assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
	if (rules_->getState() != GS_REROLL&&rules_->getState() != GS_BLOCK)
		{
      LOG2("You do not have the choice...");
      return false;
    }
	MsgReroll msg(rules_->our_team_->getTeamId());
	msg.reroll = false;
	rules_->sendPacket(msg);
	return true;
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
  if ((selected_team_->getTeamId() == 0 && bpos.row < 13)
			||(selected_team_->getTeamId() == 1 && bpos.row > 12))
    {
      LOG2("Kickoff rejected. Ball in your part of the field: " << pos);
      return false;
    }
  MsgBallPos pkt;
  pkt.row = bpos.row;
  pkt.col = bpos.col;
  rules_->sendPacket(pkt);
	
  return true;
}

inline bool Api::doGiveBall(int p)
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
	
	if (rules_->getState() != GS_TOUCHBACK)
		{
			LOG2("There is no touchback.");
			return false;
		}
	
	if (getPlayer(p) == NULL) 
		{
			LOG2("Player `" << p << "' does not exist.");
			return false;
		}
	
	if (getPlayer(p)->getStatus() != STA_STANDING) 
		{
			LOG2("Player `" << p << "' can't carry the ball.");
			return false;
		}
	MsgGiveBall pkt;
	pkt.player_id = p;
  rules_->sendPacket(pkt);
	return true;
}

inline bool Api::doMovePlayer(int p, const Point& to)
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
  return rules_->our_team_->movePlayer(p, to, MOVE);
}

inline bool Api::doBlitzMovePlayer(int p, const Point& to)
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
  return rules_->our_team_->movePlayer(p, to, BLITZ);
}

inline bool Api::doPassMovePlayer(int p, const Point& to)
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
  return rules_->our_team_->movePlayer(p, to, PASS);
}

inline bool Api::doMoveStandUpPlayer(int p)
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
  return rules_->our_team_->standUpPlayer(p, MOVE);
}

inline bool Api::doBlockStandUpPlayer(int p)
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
  return rules_->our_team_->standUpPlayer(p, BLOCK);
}

inline bool Api::doBlitzStandUpPlayer(int p)
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
  return rules_->our_team_->standUpPlayer(p, BLITZ);
}

inline bool Api::doPassStandUpPlayer(int p)
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
  return rules_->our_team_->standUpPlayer(p, PASS);
}

inline bool Api::doBlockPlayer(int p, const Point& to)
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
  return rules_->our_team_->blockPlayer(p, to, BLOCK);
}


inline bool Api::doBlitzBlockPlayer(int p, const Point& to)
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
  return rules_->our_team_->blockPlayer(p, to, BLITZ);
}

inline bool Api::doPassPlayer(int p, const Point& to)
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
  return rules_->our_team_->passPlayer(p, to);
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
** Accessors, for the game
*/

inline void Api::selectTeam(int team_id)
{
  assert(rules_->getState() != GS_WAIT);
  selected_team_ = NULL;
  selected_player_ = NULL;

  if (team_id == US || (team_id <= 1 && team_id == rules_->getTeamId()))
    selected_team_ = rules_->our_team_;
  if (team_id == THEM || (team_id <= 1 && team_id != rules_->getTeamId()))
    selected_team_ = rules_->other_team_;
}


inline int Api::myTeamId() const
{
  return rules_->getTeamId();
}

inline int Api::ballX() const
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
  return rules_->ball_->getPosition().getX();
}

inline int Api::ballY() const
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
  return rules_->ball_->getPosition().getY();
}


inline int Api::selectPlayer(int player_id)
{
  CHECK_TEAM;
  selected_player_ = selected_team_->getPlayer(player_id);
  return selected_player_ == NULL ? BAD_PLAYER : SUCCESS;
}

inline int Api::moveLength(int dst_x, int dst_y)
{
  CHECK_PLAYER;
  CHECK_POS(dst_x, dst_y);

  player_path_ = &rules_->field_->getPath(selected_player_->getPosition(),
                                          Position(dst_y, dst_x),
                                          selected_player_);
  return player_path_->size();
}

inline int Api::moveDifficulty(int step)
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

inline int Api::movePathX(int step)
{
  CHECK_PLAYER;
  if (player_path_ == NULL || step < 0 || step >= (int)player_path_->size())
    return BAD_ARGUMENT;

  return (*player_path_)[step].getX();
}

inline int Api::movePathY(int step)
{
  CHECK_PLAYER;
  if (player_path_ == NULL || step < 0 || step >= (int)player_path_->size())
    return BAD_ARGUMENT;

  return (*player_path_)[step].getY();
}

inline int Api::movePossible(int dst_x, int dst_y)
{
  CHECK_PLAYER;
  CHECK_POS(dst_x, dst_y);

  // FIXME: better implementation.
  return rules_->field_->getPath(selected_player_->getPosition(),
                                 Position(dst_y, dst_x),
                                 selected_player_).empty() ? 0 : 1;
}


/*
** Accessors, for debug
*/

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

inline const CField* Api::getField() const
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
  return rules_->field_;
}

inline const char* Api::getGameStateString() const
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
