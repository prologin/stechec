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

#include "SActionHandler.hh"
#include "SRules.hh"

SActionHandler::SActionHandler(SRules* r)
  : r_(r),
  d_(r->getDice())
{
}

SActionHandler::~SActionHandler()
{
  //FIXME: to do.
}

bool SActionHandler::isEmpty() const
{
  return eal_.empty();
}

SPlayer* SActionHandler::getPlayer() const
{
  if (eal_.empty())
    {
      return NULL;
    }
  else
    {
      return eal_.back().player;
    }
}

enum eRoll SActionHandler::getRollType() const
{
  if (eal_.empty())
    {
      return R_NONE;
    }
  else
    {
      return eal_.back().roll_type;
    }
}

inline void SActionHandler::put(ElementaryAction ea)
{
  eal_.push_back(ea);
}

void SActionHandler::putArmourRoll(SPlayer* p, int av_mod, int inj_mod)
{
  ElementaryAction ea;
  ea.type = EA_ROLL;
  ea.roll_type = R_ARMOUR;
  ea.player = p;
  ea.modifier = av_mod;
  ea.next_mod = inj_mod;
  put(ea);
}

void SActionHandler::putBallBounce()
{
  ElementaryAction ea;
  ea.type = EA_BALL;
  ea.roll_type = R_NONE;
  ea.player = NULL;
  put(ea);
}

void SActionHandler::putBlockDiceChoice(SPlayer* p)
{
  ElementaryAction ea;
  ea.type = EA_BLOCKDICE;
  ea.roll_type = R_NONE;
  ea.player = p;
  put(ea);
}

void SActionHandler::putBlockFollowChoice(SPlayer* p)
{
  ElementaryAction ea;
  ea.type = EA_BLOCKFOLLOW;
  ea.roll_type = R_NONE;
  ea.player = p;
  put(ea);
}

void SActionHandler::putBlockPushChoice(SPlayer* p)
{
  ElementaryAction ea;
  ea.type = EA_BLOCKPUSH;
  ea.roll_type = R_NONE;
  ea.player = p;
  put(ea);
}

void SActionHandler::putBlockRoll(SPlayer* p)
{
  ElementaryAction ea;
  ea.type = EA_ROLL;
  ea.roll_type = R_BLOCK;
  ea.player = p;
  put(ea);
}

void SActionHandler::putCatchBallRoll(SPlayer* p, bool success)
{
  ElementaryAction ea;
  ea.type = EA_ROLL;
  ea.roll_type = R_CATCH;
  ea.player = p;
  ea.success = success;
  put(ea);
}

void SActionHandler::putDodgeRoll(SPlayer* p, bool success)
{
  ElementaryAction ea;
  ea.type = EA_ROLL;
  ea.roll_type = R_DODGE;
  ea.player = p;
  ea.success = success;
  put(ea);
}

void SActionHandler::putMove(SPlayer* p, Position aim)
{
  ElementaryAction ea;
  ea.type = EA_MOVE;
  ea.roll_type = R_NONE;
  ea.player = p;
  ea.aim_position = aim;
  put(ea);
}

void SActionHandler::putPickUpRoll(SPlayer* p, bool success)
{
  ElementaryAction ea;
  ea.type = EA_ROLL;
  ea.roll_type = R_PICKUP;
  ea.player = p;
  ea.success = success;
  put(ea);
}

void SActionHandler::putPushResolution(SPlayer* p)
{
  ElementaryAction ea;
  ea.type = EA_PUSH;
  ea.roll_type = R_NONE;
  ea.player = p;
  put(ea);
}

void SActionHandler::putStandUpRoll(SPlayer* p, bool success)
{
  ElementaryAction ea;
  ea.type = EA_ROLL;
  ea.roll_type = R_STANDUP;
  ea.player = p;
  ea.success = success;
  put(ea);
}

void SActionHandler::putThrowRoll(SPlayer* p, int success)
{
  ElementaryAction ea;
  ea.type = EA_ROLL;
  ea.roll_type = R_THROW;
  ea.player = p;
  ea.success = success;
  put(ea);
}

void SActionHandler::process(bool reroll, int choice)
{
  ElementaryAction ea;
  if (eal_.empty())
    {
      if (r_->isTurnover())
        {
          r_->nextTurn();
        }
      else if (r_->getState() == GS_KICKOFF)
        {
          r_->finishKickoff();
        }
      return;
    }
  ea = eal_.back();
  eal_.pop_back();
  switch (ea.type)
    {
      case EA_BALL:
        r_->getBall()->bounce();
        break;
      case EA_BLOCKDICE:
        ea.player->resolveBlockDice(choice);
        break;
      case EA_BLOCKFOLLOW:
        ea.player->blockFollow(reroll);
        break;
      case EA_BLOCKPUSH:
        ea.player->resolveBlockPush(choice);
        break;
      case EA_MOVE:
        if (r_->isTurnover())
          process();
        else
          ea.player->tryMove(ea.aim_position);
        break;
      case EA_PUSH:
        ea.player->finishBlockPush();
        break;
      case EA_ROLL:
        switch (ea.roll_type)
          {
            case R_ARMOUR:
              ea.player->checkArmour(ea.modifier, ea.next_mod);
              break;
            case R_BLOCK:
              ea.player->considerBlockDices(reroll);
              break;
            case R_CATCH:
              ea.player->finishCatchBall(reroll, ea.success);
              break;
            case R_DODGE:
              ea.player->finishDodge(reroll, ea.success);
              break;
            case R_INJURY:
              WARN("Injury roll should never be put off.");
              break;
            case R_PICKUP:
              ea.player->finishPickUp(reroll, ea.success);
              break;
            case R_STANDUP:
              ea.player->finishStandUp(reroll, ea.success);
              break;
            case R_THROW:
              ea.player->finishThrow(reroll, ea.success);
              break;
            default:
              WARN("Unknown roll (%1) for elementary action (%2).", ea.roll_type, ea.type);
              break;
          }
        break;
      default:
        WARN("Unknown elementary action (%1).", ea.type);
        return;
    }
}

