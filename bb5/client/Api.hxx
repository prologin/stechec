/*
** TowBowlTactics, an adaptation of the tabletop game Blood Bowl.
**
** Copyright (C) 2006, 2007 The TBT Team.
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

#include "Dice.hh"

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

inline int Api::doPlaceBall(const Point& pos)
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);

  Position bpos(pos);
  if (!rules_->field_->intoField(bpos))
    {
      LOG2("Kickoff rejected. Ball not on the field: %1", pos);
      return INVALID_ACTION;
    }
  if ((rules_->getTeamId() == 0 && bpos.row < 13)
      || (rules_->getTeamId() == 1 && bpos.row > 12))
    {
      LOG2("Kickoff rejected. Ball in your part of the field (%1): %2",
	   selected_team_->getTeamId(),  pos);
      return INVALID_ACTION;
    }
  MsgBallPos pkt;
  pkt.row = bpos.row;
  pkt.col = bpos.col;
  rules_->sendPacket(pkt);

  return SUCCESS;
}

inline int Api::doGiveBall(int p)
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);

  if (rules_->getState() != GS_TOUCHBACK)
    {
      LOG2("There is no touchback.");
      return INVALID_ACTION;
    }

  if (getPlayer(p) == NULL)
    {
      LOG2("Player `%1' does not exist.", p);
      return BAD_PLAYER;
    }

  if (getPlayer(p)->getStatus() != STA_STANDING)
    {
      LOG2("Player `%1' can't carry the ball.", p);
      return INVALID_ACTION;
    }
  MsgGiveBall pkt;
  pkt.player_id = p;
  rules_->sendPacket(pkt);
  return SUCCESS;
}


inline int Api::doReroll(bool reroll)
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
  if (reroll && !selected_team_->canUseReroll())
    {
      LOG2("Cannot use reroll (no one left or already use one this turn).");
      return INVALID_ACTION;
    }
  if (rules_->getState() != GS_REROLL && rules_->getState() != GS_BLOCK)
    {
      LOG2("Cannot do reroll nor accept (no dice to reroll).");
      return INVALID_ACTION;
    }

  MsgReroll msg(rules_->our_team_->getTeamId());
  msg.reroll = reroll;
  rules_->sendPacket(msg);
  return SUCCESS;
}

inline int Api::doDeclare(enum eAction action)
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
  CHECK_TEAM;
  CHECK_PLAYER;

  if (action == NONE)
    return INVALID_ACTION;
  return rules_->our_team_->declareAction(selected_player_, action);
}

inline int Api::doChooseBlockDice(int n)
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
  if (rules_->getState() != GS_BLOCK)
    {
      LOG2("You do not have the choice...");
      return INVALID_ACTION;
    }
  MsgBlockDice msg(rules_->our_team_->getTeamId());
  msg.dice = n;
  rules_->sendPacket(msg);
  return SUCCESS;
}

inline int Api::doBlockPush(int n)
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
  if (rules_->getState() != GS_PUSH)
    {
      LOG2("You do not have the choice...");
      return INVALID_ACTION;
    }
  ActBlockPush msg(rules_->our_team_->getTeamId());
  msg.square_chosen = n;
  rules_->sendPacket(msg);
  return SUCCESS;
}

inline int Api::doFollow(bool follow)
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
  if (rules_->getState() != GS_FOLLOW)
    {
      LOG2("You do not have the choice...");
      return INVALID_ACTION;
    }
  MsgFollow msg(rules_->our_team_->getTeamId());
  msg.follow = follow;
  rules_->sendPacket(msg);
  return SUCCESS;
}

inline int Api::doStandUpPlayer()
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
  CHECK_TEAM;
  CHECK_PLAYER;

  return selected_player_->standUp();
}

inline int Api::doMovePlayer(const Point& to)
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
  CHECK_TEAM;
  CHECK_PLAYER;

  return selected_player_->move(to);
}

inline int Api::doBlockPlayer(int def_p)
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
  CHECK_TEAM;
  CHECK_PLAYER;

  CPlayer* opponent = rules_->other_team_->getPlayer(def_p);
  return selected_player_->block(opponent);
}

inline int Api::doPassPlayer(const Point& to)
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
  CHECK_TEAM;
  CHECK_PLAYER;

  return selected_player_->pass(to);
}

inline void Api::sendChatMessage(const std::string& msg)
{
  if (rules_->getState() == GS_WAIT)
    return;

  MsgChat pkt;
  stringToPacket(pkt.msg, msg, sizeof(pkt.msg));
  rules_->sendPacket(pkt);
}

inline void Api::doCheatDice(int roll)
{
  MsgCheatDice pkt;
  pkt.next_result = roll;
  rules_->sendPacket(pkt);
}


/*
** Accessors, for the game
*/

inline int Api::turn() const
{
  return rules_->cur_turn_;
}

inline int Api::half() const
{
  return rules_->cur_half_;
}

inline int Api::myTeamId() const
{
  return rules_->getTeamId();
}

inline Point Api::ball() const
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
  return rules_->ball_->getPosition();
}

inline int Api::teamId(const Point& pos)
{
  CHECK_POS(pos.x, pos.y);
  CPlayer* p = rules_->field_->getPlayer(pos);
  if (p != NULL)
    return p->getTeamId();
  return -1;
}

inline int Api::playerId(const Point& pos)
{
  CHECK_POS(pos.x, pos.y);
  CPlayer* p = rules_->field_->getPlayer(pos);
  if (p != NULL)
    return p->getId();
  return -1;
}

inline void Api::selectTeam(int team_id)
{
  selected_team_ = NULL;
  selected_player_ = NULL;

  if (rules_->getState() == GS_WAIT)
    return;
  if (team_id == US || (team_id <= 1 && team_id == rules_->getTeamId()))
    selected_team_ = rules_->our_team_;
  if (team_id == THEM || ((team_id == 0 || team_id == 1) && team_id != rules_->getTeamId()))
    selected_team_ = rules_->other_team_;
}

inline int Api::selectPlayer(int player_id)
{
  CHECK_TEAM;
  selected_player_ = selected_team_->getPlayer(player_id);
  return selected_player_ == NULL ? BAD_PLAYER : SUCCESS;
}

inline int Api::actionPossibleNumber() const
{
  CHECK_TEAM;
  CHECK_PLAYER;
  // FIXME: todo
  return 0;
}

inline int Api::actionPossible(int index) const
{
  CHECK_TEAM;
  CHECK_PLAYER;
  // FIXME: todo
  return index = 0;
}

inline int Api::moveLength(const Point& to)
{
  CHECK_TEAM;
  CHECK_PLAYER;
  CHECK_POS(to.x, to.y);

  player_path_ = &rules_->field_->getPath(selected_player_->getPosition(),
                                          to,
                                          selected_player_);
  return player_path_->size();
}

inline int Api::moveDifficulty(int step)
{
  if (player_path_ == NULL || step < 0 || step >= (int)player_path_->size())
    return BAD_ARGUMENT;

  Position step_pos((*player_path_)[step]);
  int nbt = rules_->field_->getNbTackleZone(selected_team_->getTeamId(), step_pos);
  if (nbt == 0)
    return 0;
  return (7 - std::min(selected_player_->getAg(), 6)) + nbt - 1;
}

inline Point Api::movePath(int step)
{
  if (player_path_ == NULL || step < 0 || step >= (int)player_path_->size())
    return Point(-1, -1);

  return (*player_path_)[step];
}

inline int Api::movePossible(const Point& to)
{
  CHECK_TEAM;
  CHECK_PLAYER;
  CHECK_POS(to.x, to.y);

  // FIXME: do a better implementation.
  return rules_->field_->getPath(selected_player_->getPosition(),
                                 to,
                                 selected_player_).empty() ? 0 : 1;
}


inline const char* Api::gameStateString() const
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
    case GS_TOUCHBACK:
      return "GS_TOUCHBACK";
    case GS_REROLL:
      return "GS_REROLL";
    case GS_BLOCK:
      return "GS_BLOCK";
    case GS_PUSH:
      return "GS_PUSH";
    case GS_FOLLOW:
      return "GS_FOLLOW";
    }
  return BaseApi<CRules>::getStateString();
}

inline const CTeam* Api::getTeam() const
{
  return selected_team_;
}

inline const CPlayer* Api::getPlayer() const
{
  return selected_player_;
}

inline const CPlayer* Api::getPlayer(int player_id)
{
  if (selected_team_ == NULL)
    return NULL;
  return selected_team_->getPlayer(player_id);
}

inline const CPlayer* Api::getPlayer(const Point& pos)
{
  return rules_->field_->getPlayer(pos);
}

inline const Weather* Api::getWeather() const
{
  assert(rules_->getState() != GS_WAIT && rules_->getState() != GS_INITGAME);
  return rules_->weather_;
}
