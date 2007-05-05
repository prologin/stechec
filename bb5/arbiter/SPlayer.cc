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
#include "SRules.hh"
#include "SPlayer.hh"
#include "SPlayerMsg.hh"
#include "SActionHandler.hh"

SPlayer::SPlayer(SRules* r, const MsgPlayerCreate* m, STeam* t, SPlayerMsg* pm)
  : Player(m),
    r_(r),
    t_(t),
    f_(r->getField()),
    d_(r->getDice()),
    pm_(pm),
    ah_(r->getActionHandler()),
    target_(NULL)
{
}

/*
** Management, misc code.
*/

bool SPlayer::acceptPlayerCreation()
{
  // FIXME: some checks... ?

  // Ok, player accepted
  return true;
}

bool SPlayer::checkAndDeclareTouchdown() 
{
  if (r_->getBall()->getOwner() == this)
    {
      if (pos_.row == ((ROWS - 1) * (1 - team_id_)))
        {
          r_->touchdown();
          return true;
        }
    }
  return false;
}

bool SPlayer::canUseSkillReroll() const
{
  return false; //FIXME: waiting for skills implementation.
  return hasSkill(skill_concerned_) && !hasUsedSkill(skill_concerned_);
}

bool SPlayer::canUseAnyReroll() const
{
  return canUseSkillReroll() || t_->canUseReroll();
}

/*
** Position.
*/

void SPlayer::setPosition(const Position& pos, bool advertise_client)
{
  assert(f_->intoField(pos));
  if (f_->intoField(pos_))
    {
      f_->setPlayer(pos_, NULL);
    }
  if (advertise_client && pos_ != pos)
    {
      pos_ = pos;
      pm_->sendPosition(this);
    }
  else
    {
      pos_ = pos;
    }
  f_->setPlayer(pos, this);
}

/*
** Status.
*/

void SPlayer::setStatus(enum eStatus new_status)
{

  if (status_ != new_status)
    {
      pm_->sendStatus(new_status, this);
    }

  switch (new_status)
    {
    case STA_RESERVE:
      status_ = STA_RESERVE;
      break;
    
    case STA_STANDING:
      // ok... play again :p
      status_ = STA_STANDING;
      break;
      
    case STA_PRONE:
      status_ = STA_PRONE;
      break;
      
    case STA_STUNNED:
      status_ = STA_STUNNED;
      break;

    case STA_KO:
      status_ = STA_KO;
      break;
      
    case STA_INJURED:
      status_ = STA_INJURED;
      break;
      
    case STA_UNASSIGNED:
      WARN("Can't set player in 'unassigned' state");
      break;

    default:
      LOG3("You can't set this state from outside...");
      break;
    }
}

void SPlayer::setProne()
{
  if (will_prone_)
    setStatus(STA_PRONE);
}

void SPlayer::prepareKickoff()
{
  int dice;
  
  switch (status_)
    {
    case STA_STANDING:
    case STA_PRONE:
    case STA_STUNNED:
      f_->setPlayer(pos_, NULL);
      setStatus(STA_RESERVE);
      break;

    case STA_KO:
      dice = d_->roll("ko");
      pm_->sendMsgKO(dice, this);
      if (dice > 3)
        setStatus(STA_RESERVE);
      break;

    default:
      break;
    }
}

/*
** Agility roll.
*/

// Standard action. Launch a D6:
//  - 1 -> always fails
//  - 6 -> always succeed
//  - else -> apply modifier and compare with agility.
bool SPlayer::rollAgility(enum eRoll roll_type, int modifier)
{
  int dice_result = d_->roll(d_->stringify(roll_type));
  int dice_modified = dice_result + modifier;
  int required = 7 - std::min(ag_, 6);
  bool success = dice_result != 1 && (dice_result == 6 || dice_modified >= required);
  LOG6( "[t%1,p%2] Roll %3 %4, dice: %5, with modifiers: %6, required: %7, ag: %8.",
      team_id_, id_,
      d_->stringify(roll_type), success?"succeds":"_fails_",
      dice_result, dice_modified, required, ag_ );
  pm_->sendRoll(roll_type, dice_result, modifier, required, reroll_enabled_, this);
  return success;
}

bool SPlayer::rollAgility()
{
  return rollAgility(roll_attempted_, modifier_);
}

/*
** Armour, injury and casualty rolls.
*/

void SPlayer::checkArmour(int av_mod, int inj_mod)
{
  int result = d_->roll("check armour", D6, 2);
  if (result + av_mod > av_)
    rollInjury(inj_mod);
  ah_->process();
}

void SPlayer::rollInjury(int modifier)
{
  int injury = d_->roll("injury", D6, 2) + modifier;
  LOG6("[t%1,p%2] Armour passed, injury : %3.", team_id_, id_, injury);
  if (injury <= 7)
    setStatus(STA_STUNNED);
  else if (injury <= 9) 
    {
      setStatus(STA_KO);
      f_->setPlayer(pos_, NULL);
    }
  else 
    {
      setStatus(rollCasualty());
      f_->setPlayer(pos_, NULL);
    }
}

enum eStatus SPlayer::rollCasualty()
{
  switch (d_->roll("casualty"))
    {
    case 1:
    case 2:
    case 3: 
    case 4:
    case 5: 
    case 6: return STA_INJURED;
    }
  return STA_UNASSIGNED;
}

/*
** Block action.
*/

void SPlayer::tryBlock()
{
  if (getAction() == DCL_BLOCK)
    setHasPlayed();
  else
    ma_remain_ = ma_remain_ - 1;
  
  int mod_st_atk = getSt();
  int mod_st_df = target_->getSt();

  if (mod_st_atk > 2 * mod_st_df || mod_st_df > 2 * mod_st_atk)
    nb_block_dice_ = 3;
  else if (mod_st_atk != mod_st_df)
    nb_block_dice_ = 2;
  else
    nb_block_dice_ = 1;

  if (nb_block_dice_ == 1)
    strongest_team_id_ = -1;
  else if (mod_st_atk < mod_st_df)
    strongest_team_id_ = r_->getCurrentOpponentTeamId();
  else
    strongest_team_id_ = team_id_;

  choose_block_ = strongest_team_id_ != r_->getCurrentOpponentTeamId();

  roll_attempted_ = R_BLOCK;
  skill_concerned_ = SK_BLOCK;
  reroll_enabled_ = canUseAnyReroll();

  rollBlock();
}

void SPlayer::rollBlock()
{
  MsgBlockResult msg(team_id_);
  msg.player_id = id_;
  msg.opponent_id = target_->getId();
  msg.reroll = reroll_enabled_;
  msg.strongest_team_id = strongest_team_id_;
  msg.nb_dice = nb_block_dice_;

  for (int i = 0; i < nb_block_dice_; ++i)
    {
      result_[i] = (enum eBlockDiceFace) d_->roll("block", DBLOCK);
      LOG5("[t%1,p%2] Rolled block dice: %3",
         team_id_, id_,
         Dice::stringify(result_[i]));
      msg.results[i] = result_[i];
    }

  r_->sendPacket(msg);

  if (reroll_enabled_)
    {
      t_->state_ = GS_REROLL;
      ah_->putBlockRoll(this);
    }
  else
    {
      chooseBlockDice(false);
    }
}

void SPlayer::chooseBlockDice(bool reroll)
{
  if (reroll)
    {
      if (!hasUsedSkill(skill_concerned_))
        {
          useSkill(skill_concerned_);
        }
      else
        {
          t_->useReroll();
        }
      reroll_enabled_ = false;
      rollBlock();
    }
  else if (nb_block_dice_ == 1)
    {
      resolveBlockDice(0);
    }
  else if (strongest_team_id_ == team_id_)
    {
      t_->state_ = GS_BLOCK;
      t_->setNbChoices(nb_block_dice_);
      ah_->putBlockDiceChoice(this);
    }
  else
    {
      r_->getTeam(target_->getTeamId())->state_ = GS_BLOCK;
      r_->getTeam(target_->getTeamId())->setNbChoices(nb_block_dice_);
      ah_->putBlockDiceChoice(target_);
    }
}

void SPlayer::resolveBlockDice(int chosen_dice)
{
  switch (result_[chosen_dice])
    {
    case BATTACKER_DOWN:
      if (r_->getBall()->getOwner() == this)
        {
          ah_->putBallBounce();
        }
      ah_->putArmourRoll(this, 0, 0);
      pm_->sendMsgKnocked(this);
      setStatus(STA_PRONE);
      r_->turnover(TOM_KNOCKEDDOWN);
      break;
    case BBOTH_DOWN:
      bool atk_down = !hasSkill(SK_BLOCK); //FIXME: Ask coach for Block skill usage.
      bool def_down = !hasSkill(SK_BLOCK); //FIXME: Ask opponent coach for Block skill usage.
      if ((def_down && r_->getBall()->getOwner() == target_)
          || (atk_down && r_->getBall()->getOwner() == this))
        {
          ah_->putBallBounce();
        }
      if (def_down) ah_->putArmourRoll(target_, 0, 0);
      if (atk_down) ah_->putArmourRoll(this, 0, 0);
      if (atk_down)
        {
          pm_->sendMsgKnocked(this);
          setStatus(STA_PRONE);
        }
      if (def_down)
        {
          pm_->sendMsgKnocked(target_);
          target_->setStatus(STA_PRONE);
        }
      if (atk_down)
        {
          r_->turnover(TOM_KNOCKEDDOWN);
        }
      else
        {
          ah_->process();
        }
      break;
    case BPUSHED:
      target_knocked_ = false;
      pusher_ = NULL;
      chooseBlockPush();
      break;
    case BDEFENDER_STUMBLE:
      target_knocked_ = !target_->hasSkill(SK_DODGE); //FIXME: Ask opponent coach for Dodge skill usage.
      pusher_ = NULL;
      chooseBlockPush();
      break;
    case BDEFENDER_DOWN:
      target_knocked_ = true;
      pusher_ = NULL;
      chooseBlockPush();
      break;
    }
}

void SPlayer::setPushed(SPlayer* p)
{
  target_ = p;
}

void SPlayer::setPusher(SPlayer* p)
{
  pusher_ = p;
}

void SPlayer::chooseBlockPush()
{
  Position squares[3];
  Position dt = target_->getPosition();
  Position d = dt - getPosition();
  move_aim_ = dt;

  squares[1] = dt + d;
  if (d.col == 0)
    {
      squares[0] = dt + d + Position(0, -1);
      squares[2] = dt + d + Position(0, 1);
    }
  else if (d.row == 0)
    {
      squares[0] = dt + d + Position(-1, 0);
      squares[2] = dt + d + Position(1, 0);
    }
  else
    {
      squares[0] = dt + d + Position(-d.row, 0);
      squares[2] = dt + d + Position(0, -d.col);
    }

  LOG3("Pusher pos: %1 Aimed pos: %2", pos_, target_->pos_);
  LOG3("Opposite squares: c1: %1 c2: %2 c3: %3", squares[0], squares[1], squares[2]);

  nb_push_choices_ = 0;

  // Look for empty squares into the field.
  for (int i = 0; i < 3; i++)
    if (f_->intoField(squares[i]) && f_->getPlayer(squares[i]) == NULL)
      {
        push_choices_[nb_push_choices_] = squares[i];
        nb_push_choices_++;
      }

  // No empty squares into the field -> Try to push out of the field.
  if (nb_push_choices_ == 0)
    {
      for (int i = 0; i < 3; i++)
        if (!f_->intoField(squares[i]) && nb_push_choices_ == 0)
          {
            push_choices_[nb_push_choices_] = squares[i];
            nb_push_choices_++;
          }
    }

  // Still no one possibility -> all possibilities (push other players)
  if (nb_push_choices_ == 0)
    {
      for (int i = 0; i < 3; i++)
      {
        push_choices_[nb_push_choices_] = squares[i];
        nb_push_choices_++;
      }
    }

  LOG3("Final number of choices: %1", nb_push_choices_);
  for (int i = 0; i < nb_push_choices_; i++)
    LOG3("Choice #%1: %2", i, push_choices_[i]);

  target_->setPusher(this);
  r_->getCurrentTeam()->setPusher(this);
  pm_->sendMsgBlockPush(nb_push_choices_, push_choices_, target_);

  if (nb_push_choices_ == 1)
    {
      resolveBlockPush(0);
    }
  else
    {
      t_->state_ = GS_PUSH;
      t_->setNbChoices(nb_push_choices_);
      ah_->putBlockPushChoice(this);
    }
}

void SPlayer::resolveBlockPush(int chosen_square)
{
  if (chosen_square != 0) push_choices_[0] = push_choices_[chosen_square];
  SPlayer* other_target = f_->getPlayer(push_choices_[0]);
  LOG2("Player `%1' of team `%2' pushes player `%3' of team `%4' from %5 to %6.",
      id_, team_id_, target_->getId(), target_->getTeamId(), move_aim_, push_choices_[0]);
  if (other_target == NULL)
    {
      if ((r_->getBall()->getPosition() == push_choices_[0])
          && (r_->getBall()->getOwner() != target_))
        {
          ah_->putBallBounce();
        }
      finishBlockPush();
    }
  else
    {
      // Oh, another player to move.
      r_->getCurrentTeam()->setPusher(target_);
      target_->setPushed(other_target);
      target_->setPusher(this);
      target_->chooseBlockPush();
    }
}

void SPlayer::finishBlockPush()
{
  //FIXME: "crowd?" rules tests fail here.
  target_->setPosition(push_choices_[0], true);
  if (!f_->intoField(push_choices_[0]))
    ah_->putPushInTheCrowd(this);
  else if (r_->getBall()->getOwner() == target_)
    r_->getBall()->setPosition(push_choices_[0], true);
  if (pusher_ != NULL)
    pusher_->finishBlockPush();
  else
    {
      t_->state_ = GS_FOLLOW;
      r_->sendPacket(MsgFollow(team_id_));
      ah_->putBlockFollowChoice(this);
    }
}

void SPlayer::bePushedInTheCrowd()
{
  rollInjury(0);
  if (status_ == STA_STUNNED)
    setStatus(STA_RESERVE);
  if (r_->getBall()->getOwner() == this)
    r_->getBall()->throwIn();
}

void SPlayer::blockFollow(bool follow)
{
  if (follow)
    {
      setPosition(move_aim_, true);
      if (r_->getBall()->getOwner() == this)
        r_->getBall()->setPosition(pos_, true);
    }
  if (target_knocked_)
    {
      if (r_->getBall()->getOwner() == target_)
        {
          ah_->putBallBounce();
        }
      ah_->putArmourRoll(target_, 0, 0);
      pm_->sendMsgKnocked(target_);
      target_->setStatus(STA_PRONE);
    }
  ah_->process();
}

/*
** Catch ball action.
*/

void SPlayer::tryCatchBall(bool accurate_pass)
{
  int nb_tackles = f_->getNbTackleZone(r_->getOpponentTeamId(team_id_), pos_);
  modifier_ = (accurate_pass?1:0) - nb_tackles;
  roll_attempted_ = R_CATCH;
  skill_concerned_ = SK_CATCH;
  reroll_enabled_ = canUseAnyReroll();
  rollCatchBall();
}

void SPlayer::rollCatchBall()
{
  bool success = rollAgility();
  if (reroll_enabled_)
    {
      t_->state_ = GS_REROLL;
      ah_->putCatchBallRoll(this, success);
    }
  else
    {
      finishCatchBall(false, success);
    }
}

void SPlayer::finishCatchBall(bool reroll, bool success)
{
  if (reroll)
    {
      if (!hasUsedSkill(skill_concerned_))
        {
          useSkill(skill_concerned_);
        }
      else
        {
          t_->useReroll();
        }
      reroll_enabled_ = false;
      rollCatchBall();
    }
  else if (success)
    {
      LOG5("[t%1,p%2] succeeds to catch the ball.", team_id_, id_);
      r_->getBall()->setOwner(this);
      if (!checkAndDeclareTouchdown())
        {
          if (team_id_ == r_->getCurrentOpponentTeamId())
            {
              r_->turnover(TOM_LOSTBALL); //FIXME: Check if there was a pass or a hand-off.
            }
          else
            {
              ah_->process();
            }
        }
    }
  else
    {
      LOG5("[t%1,p%2] fails to catch the ball.", team_id_, id_);
      r_->getBall()->bounce();
    }
}

/*
** Dodge action.
*/

void SPlayer::tryDodge()
{
  int nb_tackles = f_->getNbTackleZone(r_->getOpponentTeamId(team_id_), pos_);
  modifier_ = 1 - nb_tackles;
  roll_attempted_ = R_DODGE;
  skill_concerned_ = SK_DODGE;
  reroll_enabled_ = canUseAnyReroll();
  rollDodge();
}

void SPlayer::rollDodge()
{
  bool success = rollAgility();
  if (reroll_enabled_)
    {
      t_->state_ = GS_REROLL;
      ah_->putDodgeRoll(this, success);
    }
  else
    {
      finishDodge(false, success);
    }
}

void SPlayer::finishDodge(bool reroll, bool success)
{
  if (reroll)
    {
      if (!hasUsedSkill(skill_concerned_))
        {
          useSkill(skill_concerned_);
        }
      else
        {
          t_->useReroll();
        }
      reroll_enabled_ = false;
      rollDodge();
    }
  else if (success)
    {
      LOG5("[t%1,p%2] succeeds to dodge out to %3.", team_id_, id_, pos_);
      if (r_->getBall()->getPosition() == pos_
          && r_->getBall()->getOwner() != this)
        {
          tryPickUp();
        }
      else if (!checkAndDeclareTouchdown())
        {
          ah_->process();
        }
    }
  else
    {
      LOG5("[t%1,p%2] fails to dodge out to %3.", team_id_, id_, pos_);
      pm_->sendMsgKnocked(this); //FIXME: possibly breaks client compatibility.
      setStatus(STA_PRONE);
      if (r_->getBall()->getPosition() == pos_)
        {
          ah_->putBallBounce();
        }
      ah_->putArmourRoll(this, 0, 0);
      if (team_id_ == r_->getCurrentTeamId())
        {
          r_->turnover(TOM_KNOCKEDDOWN);
        }
      else
        {
          ah_->process();
        }
    }
}

/*
** Move action.
*/

void SPlayer::tryMove(Position aim)
{
  int nb_tackles_pos = f_->getNbTackleZone(r_->getCurrentOpponentTeamId(), pos_);
  move_aim_ = aim;

  MsgMove msg(team_id_);
  msg.player_id = id_;
  msg.nb_move = 1;
  msg.moves[0].row = aim.row;
  msg.moves[0].col = aim.col;
  r_->sendPacket(msg);

  setPosition(aim, false);
  ma_remain_--;

  if (r_->getBall()->getOwner() == this)
    r_->getBall()->setPosition(pos_, true);

  if (nb_tackles_pos > 0)
    {
      tryDodge();
    }
  else if ((r_->getBall()->getOwner() != this) && (r_->getBall()->getPosition() == pos_))
    {
      tryPickUp();
    }
  else if (!checkAndDeclareTouchdown())
    {
      ah_->process();
    }
}

/*
** Pick up action.
*/

void SPlayer::tryPickUp()
{
  int nb_tackles = f_->getNbTackleZone(r_->getOpponentTeamId(team_id_), pos_);
  modifier_ = 1 - nb_tackles;
  roll_attempted_ = R_PICKUP;
  skill_concerned_ = SK_SUREHANDS;
  reroll_enabled_ = canUseAnyReroll();
  rollPickUp();
}

void SPlayer::rollPickUp()
{
  int success = rollAgility();
  if (reroll_enabled_)
    {
      t_->state_ = GS_REROLL;
      ah_->putPickUpRoll(this, success);
    }
  else
    {
      finishPickUp(false, success);
    }
}

void SPlayer::finishPickUp(bool reroll, bool success)
{
  if (reroll)
    {
      if (!hasUsedSkill(skill_concerned_))
        {
          useSkill(skill_concerned_);
        }
      else
        {
          t_->useReroll();
        }
      reroll_enabled_ = false;
      rollPickUp();
    }
  else if (success)
    {
      LOG5("[t%1,p%2] succeds to pick up the ball.", team_id_, id_);
      r_->getBall()->setOwner(this); //FIXME: possibly breaks client compatibility.
      if (!checkAndDeclareTouchdown())
        {
          ah_->process();
        }
    }
  else
    {
      LOG5("[t%1,p%2] fails to pick up the ball.", team_id_, id_);
      ah_->putBallBounce();
      if (team_id_ == r_->getCurrentTeamId())
        {
          r_->turnover(TOM_FAILEDPICKUP);
        }
      else
        {
          ah_->process();
        }
    }
}

/*
** Stand up action.
*/

void SPlayer::tryStandUp()
{
  if (ma_ < 3)
    {
      ma_remain_ = 0;
      roll_attempted_ = R_STANDUP;
      skill_concerned_ = SK_NONE;
      reroll_enabled_ = canUseAnyReroll();
      rollStandUp();
    }
  else
    {
      ma_remain_ = ma_ - 3;
      setStatus(STA_STANDING);
      ah_->process();
    }
}

void SPlayer::rollStandUp()
{
  int dice_result = d_->roll(d_->stringify(roll_attempted_));
  int required = 4;
  bool success = (dice_result >= required);
  LOG6( "[t%1,p%2] Roll %3 %4, dice: %5, required: %7.",
      team_id_, id_,
      d_->stringify(roll_attempted_), success?"succeds":"_fails_",
      dice_result, required, ag_ );
  pm_->sendRoll(roll_attempted_,dice_result, 0, required, reroll_enabled_, this);
  if (reroll_enabled_)
    {
      t_->state_ = GS_REROLL;
      ah_->putStandUpRoll(this, success);
    }
  else
    {
      finishStandUp(false, success);
    }
}

void SPlayer::finishStandUp(bool reroll, bool success)
{
  if (reroll)
    {
      if (!hasUsedSkill(skill_concerned_))
        {
          useSkill(skill_concerned_);
        }
      else
        {
          t_->useReroll();
        }
      reroll_enabled_ = false;
      rollStandUp();
    }
  else if (success)
    {
      LOG5("[t%1,p%2] succeds to stand up.", team_id_, id_);
      setStatus(STA_STANDING);
    }
  else
    {
      LOG5("[t%1,p%2] fails to stand up.", team_id_, id_);
    }
  ah_->process();
}

/*
** Throw action.
*/

void SPlayer::tryThrow()
{
  int nb_tackles = f_->getNbTackleZone(r_->getCurrentOpponentTeamId(), pos_);
  int dist_modifier;
  if (distance_ < 4.f)
    dist_modifier = 1; // quick pass
  else if (distance_ < 8.f)
    dist_modifier = 0; // short pass
  else if (distance_ < 12.f)
    dist_modifier = -1; // long pass
  else
    dist_modifier = -2; // long bomb

  modifier_ = dist_modifier - nb_tackles;
  roll_attempted_ = R_THROW;
  skill_concerned_ = SK_PASS;
  reroll_enabled_ = canUseAnyReroll();
  ma_remain_ = 0;
  has_played_ = true;
  rollThrow();
}

void SPlayer::rollThrow()
{
  bool success = rollAgility();
  if (reroll_enabled_)
    {
      t_->state_ = GS_REROLL;
      ah_->putThrowRoll(this, success);
    }
  else
    {
      finishThrow(false, success);
    }
}

void SPlayer::finishThrow(bool reroll, int success)
{
  SBall* b = r_->getBall();
  if (reroll)
    {
      if (!hasUsedSkill(skill_concerned_))
        {
          useSkill(skill_concerned_);
        }
      else
        {
          t_->useReroll();
        }
      reroll_enabled_ = false;
      rollThrow();
    }
  else
    {
      b->setPosition(throw_aim_);
      if (!success)
        {
          b->scatter(1);
          b->scatter(1);
          b->scatter(1);
        }
      b->setThrown();
      b->sendPosition();
      SPlayer* p = f_->getPlayer(b->getPosition());
      if (p != NULL && p->getStatus() == STA_STANDING)
        {
          p->tryCatchBall(success);
        }
      else
        {
          b->bounce();
        }
    }
}

/*
** Message handlers.
*/

void SPlayer::msgBlock(const MsgBlock* m)
{
  if (!t_->canDoAction(m, this))
    return;
  target_ = r_->getOpponentTeam(team_id_)->getPlayer(m->opponent_id);
  if (target_ == NULL
      || target_->getTeamId() == getTeamId()
      || status_ != STA_STANDING
      || target_->status_ != STA_STANDING
      || !pos_.isNear(target_->getPosition()))
    {
      LOG3("Cannot block player '%1` at %2 (status: %3).", m->opponent_id,
          target_->getPosition(), target_->status_);
      r_->sendIllegal(m->token, m->client_id);
      return;
    }
  tryBlock();
}

void SPlayer::msgDeclare(const MsgDeclare* m)
{
  if (!t_->canDeclareAction(m))
    return;
  action_ = (enum eDeclaredAction) m->action;
  r_->sendPacket(*m);
}

void SPlayer::msgMove(const MsgMove* m)
{
  if (!t_->canDoAction(m, this))
    {
      return;
    }
  // Checks that the player has enough ma remaining
  if (ma_remain_ < m->nb_move)
    {
      LOG4("Move: not enough movement remaining.");
      r_->sendIllegal(m->token, m->client_id, ERR_NOTENOUGHMOVEMENT);
      return;
    }
  // Checks that the whole move is allowed.
  Position from = getPosition();
  Position to;
  for (int i = 0; i < m->nb_move; i++)
    {
      to = Position(m->moves[i].row, m->moves[i].col);
      if (!from.isNear(to))
        {
          LOG4("Move: not an adjacent square.");
          r_->sendIllegal(MSG_MOVE, m->client_id, ERR_NOTADJACENTSQUARE);
          return;
        }
      if (f_->getPlayer(to) != NULL)
        {
          LOG4("Move: not an empty square.");
          r_->sendIllegal(MSG_MOVE, m->client_id, ERR_NOTEMPTYSQUARE);
          return;
        }
      from = to;
    }
  // Stacks move actions.
  for (int i = m->nb_move - 1; i >= 0; i--)
    {
      ah_->putMove(this, Position(m->moves[i].row, m->moves[i].col));
    }
  ah_->process();
}

void SPlayer::msgPass(const MsgPass* m)
{
  if (!t_->canDoAction(m, this))
    {
      return;
    }
  if (r_->getBall()->getOwner() != this)
    {
      LOG2("Player [t%1,p%2] doesn't own the ball.", team_id_, id_);
      r_->sendIllegal(MSG_PASS, m->client_id, ERR_DOESNTOWNTHEBALL);
      return;
    }
  throw_aim_ = Position (m->dest_row, m->dest_col);
  if (!f_->intoField(throw_aim_))
    {
      LOG2("Player [t%1,p%2] can not willingly pass the ball to the crowd.", team_id_, id_);
      r_->sendIllegal(MSG_PASS,m->client_id, ERR_CANTPASSTOTHECROWD);
      return;
    }
  distance_ = pos_.distance(throw_aim_);
  if (distance_ >= 16.f)
    {
      LOG2("Player [t%1,p%2] can not throw the ball that far away.", team_id_, id_);
      r_->sendIllegal(MSG_PASS, m->client_id, ERR_CANTPASSTHATFARAWAY);
      return;
    }
  tryThrow();
}

void SPlayer::msgPlayerPos(const MsgPlayerPos* m)
{
  if (t_->state_ != GS_INITKICKOFF)
    {
      WARN("Bad team state (%1).", t_->state_);
      r_->sendIllegal(m->token, m->client_id, ERR_WRONGCONTEXT);
      return;
    }
  if (status_ != STA_RESERVE && status_ != STA_STANDING)
    {
      WARN("Player [t%1,p%2] can not enter in play (%3).", team_id_, id_, stringify(status_));
      r_->sendIllegal(m->token, m->client_id, ERR_CANNOTENTERINPLAY);
      return;
    }
  const Position new_pos(m->row, m->col);
  SPlayer* out_goer = f_->getPlayer(new_pos);
  // Places player in the reserve if placement is out of the field, or in the wrong field side.
  if (!f_->intoField(new_pos)
      || t_->getTeamId() == 0 && new_pos.row >= ROWS / 2
      || t_->getTeamId() == 1 && new_pos.row < ROWS / 2)
    {
      // Does nothing if the player is already in the reserve.
      if (status_ != STA_RESERVE)
        {
          f_->setPlayer(pos_, NULL);
          setStatus(STA_RESERVE);
        }
    }
  // Does nothing if the player is already standing at this place.
  else if (! (status_ == STA_STANDING && pos_ == new_pos))
    {
      if (out_goer != NULL)
        {
          f_->setPlayer(new_pos,NULL);
          out_goer->setStatus(STA_RESERVE);
        }
      if (status_ == STA_RESERVE)
        {
          setStatus(STA_STANDING);
        }
      // Note: SPlayer::setPosition(new_pos, true) doesn't advertise the client
      // in case of the player was already at this position and in the reserve.
      setPosition(new_pos, false);
      r_->sendPacket(*m);
    }
}

void SPlayer::msgSkill(const MsgSkill* m)
{
  enum eSkill skill = (enum eSkill) m->skill;
  if (!t_->canDoAction(m, this))
    return;
  if (!hasSkill(skill))
    {
      LOG2("Player `%1' of team `%2' doesn't have the skill `%3'.",
          id_, team_id_, stringify(skill));
      r_->sendIllegal(m->token, m->client_id);
    }
  else if (hasUsedSkill(skill))
    {
      LOG2("Player `%1' of team `%2' has already used the skill `%3'.",
          id_, team_id_, stringify(skill));
      r_->sendIllegal(m->token, m->client_id);
    }
  else if (ah_->getPlayer() != this)
    {
      LOG2("Player `%1' of team `%2' doesn't have to use a skill now.", id_, team_id_);
      r_->sendIllegal(m->token, m->client_id);
    }
  else if ((t_->state_ == GS_REROLL)
      && (((ah_->getRollType() == R_CATCH) && (skill == SK_CATCH))
        || ((ah_->getRollType() == R_DODGE) && (skill == SK_DODGE))
        || ((ah_->getRollType() == R_PICKUP) && (skill == SK_SUREHANDS))
        || ((ah_->getRollType() == R_THROW) && (skill == SK_PASS))))
    {
      useSkill(skill);
      ah_->process(true);
    }
  else if ((t_->state_ == GS_BLOCK) //FIXME: to do.
      && ((/*(ah_->getFoo() == bar) &&*/ (skill == SK_BLOCK))
        || (/*(ah_->getFoo() == bar) &&*/ (skill == SK_DODGE))))
    {
      ah_->process(/*skill*/);
    }
  else
    {
      LOG2("Player `%1' of team `%2' can not use skill `%3' now.",
          id_, team_id_, stringify(skill));
      r_->sendIllegal(m->token, m->client_id);
    }
}

void SPlayer::msgStandUp(const MsgStandUp* m)
{
  if (!t_->canDoAction(m, this))
    return;
  tryStandUp();
}

