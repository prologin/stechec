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

#include "SPlayer.hh"

#include "SActionHandler.hh"
#include "SBall.hh"
#include "SPlayerMsg.hh"
#include "SRules.hh"
#include "STeam.hh"

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

bool SPlayer::checkAndDeclareTouchdooown() 
{
  if (r_->getBall()->getOwner() == this)
    {
      if (pos_.row == ((ROWS - 1) * (1 - team_id_)))
        {
          LOG5("Player `%1' of team `%2' scores a TOUCHDOOOWN at %3!", id_, team_id_, pos_);
          pm_->sendTouchdooown(this);
          t_->incrementScore();
          r_->touchdooown(this);
          return true;
        }
    }
  return false;
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
  if (status_ == new_status)
    {
      WARN("Don't assign twice the same status to player `%1' of team `%2'.", id_, team_id_);
      return;
    }
  pm_->sendStatus(new_status, this);
  switch (new_status)
    {
    case STA_RESERVE:
      if (status_ == STA_PRONE || status_ == STA_STANDING || STA_STUNNED)
        {
          f_->setPlayer(pos_, NULL);
          pos_ = Position(-1, -1);
        }
      status_ = STA_RESERVE;
      break;
    case STA_STANDING:
      status_ = STA_STANDING;
      break;
    case STA_PRONE:
      status_ = STA_PRONE;
      break;
    case STA_STUNNED:
      status_ = STA_STUNNED;
      break;
    case STA_KO:
      if (status_ == STA_PRONE || status_ == STA_STANDING || STA_STUNNED)
        {
          f_->setPlayer(pos_, NULL);
          pos_ = Position(-1, -1);
        }
      status_ = STA_KO;
      break;
    case STA_INJURED:
      if (status_ == STA_PRONE || status_ == STA_STANDING || STA_STUNNED)
        {
          f_->setPlayer(pos_, NULL);
          pos_ = Position(-1, -1);
        }
      status_ = STA_INJURED;
      break;
    case STA_UNASSIGNED:
      WARN("Can't assign unassigned status to player `%1' of team `%2'.", id_, team_id_);
      break;
    default:
      WARN("Can't set unknown status `%1' to player `%2' of team `%3'.",
          new_status, id_, team_id_);
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
** Skills.
*/

void SPlayer::setRerollAvailability()
{
  setUsableSkills();
  reroll_enabled_ = t_->canUseReroll() || !usable_skills_.empty();
}

void SPlayer::setUsableSkills()
{
  usable_skills_.clear();
  switch (roll_attempted_)
    {
      case R_ARMOUR:
        break;
      case R_BLOCK:
        break;
      case R_CATCH:
        setSkillAvailability(SK_CATCH);
        break;
      case R_DODGE:
        setSkillAvailability(SK_DODGE);
        break;
      case R_INJURY:
        break;
      case R_PICKUP:
        setSkillAvailability(SK_SUREHANDS);
        break;
      case R_STANDUP:
        break;
      case R_THROW:
        setSkillAvailability(SK_PASS);
        break;
      default:
        WARN("Can't set usable skills for unknown roll type (%1).",
            Dice::stringify(roll_attempted_));
        break;
    }
}

void SPlayer::setUsableSkill(enum eSkill skill)
{
  usable_skills_.clear();
  if (skill != SK_UNASSIGNED)
    usable_skills_.push_back(skill);
}

void SPlayer::setSkillAvailability(enum eSkill skill)
{
  if (hasSkill(skill) && !hasUsedSkill(skill))
    usable_skills_.push_back(skill);
}

inline bool SPlayer::canUseSkill(enum eSkill skill) const
{
  return std::find(usable_skills_.begin(), usable_skills_.end(), skill) != usable_skills_.end();
}

enum eSkill SPlayer::getUsableSkill() const
{
  if (usable_skills_.empty())
    return SK_UNASSIGNED;
  else
    return usable_skills_.back();
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
  int dice_result = d_->roll(roll_type);
  int dice_modified = dice_result + modifier;
  int required = 7 - std::min(ag_, 6);
  bool success = dice_result != 1 && (dice_result == 6 || dice_modified >= required);
  LOG6("Player `%1' of team `%2' %3 `%4'.", id_, team_id_,
      success?"succeeds":"_fails_", Dice::stringify(roll_type));
  LOG6("Dice result: `%1', with modifiers: `%2', required: `%3', ag: `%4'.",
      dice_result, dice_modified, required, ag_);
  pm_->sendRoll(roll_type, dice_result, modifier, required,
      (reroll_enabled_?t_->canUseReroll():false), getUsableSkill(), this);
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
  int result = d_->roll(R_ARMOUR, D6, 2);
  if (result + av_mod > av_)
    rollInjury(inj_mod);
  ah_->process();
}

void SPlayer::bePushedInTheCrowd()
{
  ah_->putPushResolution(pusher_);
  rollInjury(0);
  if (status_ == STA_STUNNED)
    setStatus(STA_RESERVE);
  if (r_->getBall()->getOwner() == this)
    r_->getBall()->throwIn();
  else
    ah_->process();
}

void SPlayer::rollInjury(int modifier)
{
  int injury = d_->roll(R_INJURY, D6, 2) + modifier;
  LOG5("Player `%1' of team `%2' is injured: `%3' (on Injury table).",
      id_, team_id_, injury);
  if (injury <= 7)
    setStatus(STA_STUNNED);
  else if (injury <= 9) 
    setStatus(STA_KO);
  else 
    setStatus(rollCasualty());
}

enum eStatus SPlayer::rollCasualty()
{
  int result = (10 * d_->roll("casualty ten")) + d_->roll("casualty one", D8);
  LOG5("Player `%1' of team `%2' suffers a casualty: `%3' (on Casualty table).",
      id_, team_id_, result);
  //FIXME: Casualty table details are needed for extra rules.
  return STA_INJURED;
}

/*
** Block action.
*/

void SPlayer::tryBlock()
{
  ma_remain_ = ma_remain_ - 1;
  if (getAction() == DCL_BLOCK)
    setHasPlayed();
  else
    setHasBlocked();
  
  int mod_st_atk = getSt();
  int mod_st_df = target_->getSt();

  if (mod_st_atk > 2 * mod_st_df || mod_st_df > 2 * mod_st_atk)
    nb_block_dices_ = 3;
  else if (mod_st_atk != mod_st_df)
    nb_block_dices_ = 2;
  else
    nb_block_dices_ = 1;

  if (nb_block_dices_ == 1)
    strongest_team_id_ = -1;
  else if (mod_st_atk < mod_st_df)
    strongest_team_id_ = r_->getCurrentOpponentTeamId();
  else
    strongest_team_id_ = team_id_;

  roll_attempted_ = R_BLOCK;
  setRerollAvailability();

  rollBlock();
}

void SPlayer::rollBlock()
{
  LOG5("Player `%1' of team `%2' tries to block player `%3' of team `%4': %5 dices to roll.",
      id_, team_id_, target_->getId(), target_->getTeamId());
  for (int i = 0; i < nb_block_dices_; ++i)
    {
      block_results_[i] = (enum eBlockDiceFace) d_->roll(R_BLOCK, DBLOCK);
      LOG5("Block dice #%1 result: `%2'.", i, Dice::stringify(block_results_[i]));
    }
  pm_->sendBlockResult(reroll_enabled_, strongest_team_id_,
      nb_block_dices_, block_results_, target_, this);
  if (reroll_enabled_)
    {
      t_->state_ = GS_REROLL;
      ah_->putBlockRoll(this);
    }
  else
    {
      considerBlockDices(false);
    }
}

void SPlayer::considerBlockDices(bool reroll)
{
  reroll_enabled_ = false;
  if (reroll)
    {
      rollBlock();
    }
  else if (nb_block_dices_ == 1)
    {
      resolveBlockDice(0);
    }
  else if (strongest_team_id_ == team_id_)
    {
      t_->state_ = GS_BLOCK;
      t_->setNbChoices(nb_block_dices_);
      pm_->sendBlockResult(reroll_enabled_, strongest_team_id_,
          nb_block_dices_, block_results_, target_, this);
      ah_->putBlockDiceChoice(this);
    }
  else
    {
      r_->getTeam(target_->getTeamId())->state_ = GS_BLOCK;
      r_->getTeam(target_->getTeamId())->setNbChoices(nb_block_dices_);
      pm_->sendBlockResult(reroll_enabled_, strongest_team_id_,
          nb_block_dices_, block_results_, target_, this);
      ah_->putBlockDiceChoice(target_);
    }
}

void SPlayer::resolveBlockDice(int chosen_dice)
{
  switch (block_results_[chosen_dice])
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
      if (target_->hasSkill(SK_BLOCK))
        {
          target_->setUsableSkill(SK_BLOCK);
          pm_->sendSkillQuestion(SK_BLOCK, target_);
          t_->state_ = GS_SKILL;
          ah_->putBlockBothDownDefender(this);
        }
      else
        resolveBlockBothDownDefender(false);
      break;
    case BPUSHED:
      pusher_ = NULL;
      target_knocked_ = false;
      tryBlockPush(target_);
      break;
    case BDEFENDER_STUMBLE:
      pusher_ = NULL;
      if (target_->hasSkill(SK_DODGE))
        {
          target_->setUsableSkill(SK_DODGE);
          pm_->sendSkillQuestion(SK_DODGE, target_);
          t_->state_ = GS_SKILL;
          ah_->putBlockDefenderStumble(this);
        }
      else
        resolveBlockDefenderStumble(false);
      break;
    case BDEFENDER_DOWN:
      pusher_ = NULL;
      target_knocked_ = true;
      tryBlockPush(target_);
      break;
    }
}

void SPlayer::resolveBlockBothDownDefender(bool block)
{
  target_->setUsableSkill(SK_UNASSIGNED);
  target_knocked_ = !block;
  if (hasSkill(SK_BLOCK))
    {
      setUsableSkill(SK_BLOCK);
      pm_->sendSkillQuestion(SK_BLOCK, this);
      t_->state_ = GS_SKILL;
      ah_->putBlockBothDownAttacker(this);
    }
  else
    resolveBlockBothDownAttacker(false);
}

void SPlayer::resolveBlockBothDownAttacker(bool block)
{
  setUsableSkill(SK_UNASSIGNED);
  if ((target_knocked_ && r_->getBall()->getOwner() == target_)
      || ((!block) && r_->getBall()->getOwner() == this))
    {
      ah_->putBallBounce();
    }
  if (target_knocked_) ah_->putArmourRoll(target_, 0, 0);
  if (!block) ah_->putArmourRoll(this, 0, 0);
  if (!block)
    {
      pm_->sendMsgKnocked(this);
      setStatus(STA_PRONE);
    }
  if (target_knocked_)
    {
      pm_->sendMsgKnocked(target_);
      target_->setStatus(STA_PRONE);
    }
  if (!block)
    {
      r_->turnover(TOM_KNOCKEDDOWN);
    }
  else
    {
      ah_->process();
    }
}

void SPlayer::resolveBlockDefenderStumble(bool dodge)
{
  target_->setUsableSkill(SK_UNASSIGNED);
  target_knocked_ = !dodge;
  tryBlockPush(target_);
}

void SPlayer::setPusher(SPlayer* p)
{
  pusher_ = p;
}

void SPlayer::tryBlockPush(SPlayer* target)
{
  Position squares[3];
  Position direction;
  target_ = target;
  move_aim_ = target_->getPosition();
  direction = move_aim_ - pos_;

  squares[1] = move_aim_ + direction;
  if (direction.col == 0)
    {
      squares[0] = squares[1] + Position(0, -1);
      squares[2] = squares[1] + Position(0, 1);
    }
  else if (direction.row == 0)
    {
      squares[0] = squares[1] + Position(-1, 0);
      squares[2] = squares[1] + Position(1, 0);
    }
  else
    {
      squares[0] = squares[1] + Position(-direction.row, 0);
      squares[2] = squares[1] + Position(0, -direction.col);
    }

  LOG5("Player `%1' of team `%2' at %3 must push player `%4' of team `%5' from %6.",
      id_, team_id_, pos_, target_->getId(), target_->getTeamId(), move_aim_);
  LOG5("Opposite squares: c1: %1 c2: %2 c3: %3.", squares[0], squares[1], squares[2]);

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

  LOG5("Final number of choices: `%1'.", nb_push_choices_);
  for (int i = 0; i < nb_push_choices_; i++)
    LOG5("Choice #%1: `%2'.", i, push_choices_[i]);

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
  LOG5("Player `%1' of team `%2' pushes player `%3' of team `%4' from %5 to %6.",
      id_, team_id_, target_->getId(), target_->getTeamId(), move_aim_, push_choices_[0]);
  if (other_target == NULL)
    r_->getCurrentTeam()->getActivePlayer()->considerBlockFollow();
  else
    {
      // Oh, another player to move.
      r_->getCurrentTeam()->setPusher(target_);
      r_->getCurrentTeam()->state_ = GS_PUSH;
      target_->setPusher(this);
      target_->tryBlockPush(other_target);
    }
}

void SPlayer::considerBlockFollow()
{
  t_->state_ = GS_FOLLOW;
  r_->waitForCurrentOpponentChoice(team_id_);
  r_->sendPacket(MsgFollow(team_id_));
  ah_->putBlockFollowChoice(this);
}

void SPlayer::finishBlockFollow(bool follow)
{
  follow_ = follow;
  finishBlockPush();
}

void SPlayer::finishBlockPush()
{
  if ((push_choices_[0] != target_->getPosition())
      && f_->intoField(target_->getPosition()))
    {
      if (!f_->intoField(push_choices_[0]))
        target_->bePushedInTheCrowd();
      else if (f_->getPlayer(push_choices_[0]) != NULL)
        target_->finishBlockPush();
      else
        {
          target_->setPosition(push_choices_[0], true);
          if (r_->getBall()->getOwner() == target_)
            r_->getBall()->setPosition(push_choices_[0], true);
          else if (r_->getBall()->getPosition() == push_choices_[0])
            ah_->putBallBounce();
          if (pusher_ != NULL)
            pusher_->finishBlockPush();
          else
            this->finishBlockPush();
        }
    }
  else
    {
      if (pusher_ != NULL)
        pusher_->finishBlockPush();
      else
        finishBlockAction();
    }
}

void SPlayer::finishBlockAction()
{
  if (follow_)
    {
      setPosition(move_aim_, true);
      if (r_->getBall()->getOwner() == this)
        r_->getBall()->setPosition(pos_, true);
    }
  if (target_knocked_)
    {
      if (r_->getBall()->getOwner() == target_)
        ah_->putBallBounce();
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
  int nb_tackles = f_->getNbTackleZones(r_->getOpponentTeamId(team_id_), pos_);
  modifier_ = (accurate_pass?1:0) - nb_tackles;
  roll_attempted_ = R_CATCH;
  setRerollAvailability();
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
  usable_skills_.clear();
  if (reroll)
    {
      reroll_enabled_ = false;
      rollCatchBall();
    }
  else if (success)
    {
      LOG5("Player `%1' of team `%2' succeeds to catch the ball.", id_, team_id_);
      r_->getBall()->setOwner(this);
      if (!checkAndDeclareTouchdooown())
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
      LOG5("Player `%1' of team `%2' _fails_ to catch the ball.", id_, team_id_);
      r_->getBall()->bounce();
    }
}

/*
** Dodge action.
*/

void SPlayer::tryDodge()
{
  int nb_tackles = f_->getNbTackleZones(r_->getOpponentTeamId(team_id_), pos_);
  modifier_ = 1 - nb_tackles;
  roll_attempted_ = R_DODGE;
  setRerollAvailability();
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
  usable_skills_.clear();
  if (reroll)
    {
      reroll_enabled_ = false;
      rollDodge();
    }
  else if (success)
    {
      LOG5("Player `%1' of team `%2' succeeds to dodge out to %3.", id_, team_id_, pos_);
      if (r_->getBall()->getPosition() == pos_
          && r_->getBall()->getOwner() != this)
        {
          tryPickUp();
        }
      else if (!checkAndDeclareTouchdooown())
        {
          ah_->process();
        }
    }
  else
    {
      LOG5("Player `%1' of team `%2' _fails_ to dodge out to %3.", id_, team_id_, pos_);
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

void SPlayer::tryMove(Position& aim)
{
  int nb_tackles_pos = f_->getNbTackleZones(r_->getCurrentOpponentTeamId(), pos_);
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
  else if (!checkAndDeclareTouchdooown())
    {
      ah_->process();
    }
}

/*
** Pick up action.
*/

void SPlayer::tryPickUp()
{
  int nb_tackles = f_->getNbTackleZones(r_->getOpponentTeamId(team_id_), pos_);
  modifier_ = 1 - nb_tackles;
  roll_attempted_ = R_PICKUP;
  setRerollAvailability();
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
  usable_skills_.clear();
  if (reroll)
    {
      reroll_enabled_ = false;
      rollPickUp();
    }
  else if (success)
    {
      LOG5("Player `%1' of team `%2' succeeds to pick up the ball.", id_, team_id_);
      r_->getBall()->setOwner(this); //FIXME: possibly breaks client compatibility.
      if (!checkAndDeclareTouchdooown())
        {
          ah_->process();
        }
    }
  else
    {
      LOG5("Player `%1' of team `%2' _fails_ to pick up the ball.", id_, team_id_);
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
      setRerollAvailability();
      rollStandUp();
    }
  else
    {
      LOG5("Player `%1' of team `%2' stands up.");
      ma_remain_ = ma_ - 3;
      setStatus(STA_STANDING);
      ah_->process();
    }
}

void SPlayer::rollStandUp()
{
  int dice_result = d_->roll(roll_attempted_);
  int required = 4;
  bool success = (dice_result >= required);
  LOG6("Player `%1' of team `%2' %3 `%4'.", id_, team_id_,
      success?"succeeds":"_fails_", Dice::stringify(roll_attempted_));
  LOG6("Dice result: `%1', required: `%3', ag: `%4'.",
      dice_result, required, ag_);
  pm_->sendRoll(roll_attempted_,dice_result, 0, required,
      (reroll_enabled_?t_->canUseReroll():false), getUsableSkill(), this);
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
  usable_skills_.clear();
  if (reroll)
    {
      reroll_enabled_ = false;
      rollStandUp();
    }
  else if (success)
    {
      LOG5("Player `%1' of team `%2' succeeds to stand up.", id_, team_id_);
      setStatus(STA_STANDING);
    }
  else
    {
      LOG5("Player `%1' of team `%2' _fails_ to stand up.", id_, team_id_);
    }
  ah_->process();
}

/*
** Throw action.
*/

void SPlayer::tryThrow()
{
  int nb_tackles = f_->getNbTackleZones(r_->getCurrentOpponentTeamId(), pos_);
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
  setRerollAvailability();
  ma_remain_ = 0;
  setHasPlayed();
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
  SBall* b;
  Position ball_last_pos;
  int i;
  SPlayer* p;

  usable_skills_.clear();
  if (reroll)
    {
      reroll_enabled_ = false;
      rollThrow();
    }
  else
    {
      b = r_->getBall();
      b->setThrown();
      b->setPosition(throw_aim_);
      if (!success)
        {
          i = 3;
          while (f_->intoField(b->getPosition()) && i > 0)
            {
              // Kludge: SBall::scatter(...) must NOT advertise clients here.
              ball_last_pos = b->getPosition();
              b->scatter(1);
              i --;
            }
          if (!f_->intoField(b->getPosition()))
            {
              b->setPosition(ball_last_pos);
              b->throwIn();
              return;
            }
          b->sendPosition();
        }
      p = f_->getPlayer(b->getPosition());
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
  if (has_blocked_)
    {
      LOG3("Player `%1' of team `%2' can not block twice in the same turn.",
          id_, team_id_);
      r_->sendIllegal(m->token, m->client_id);
      return;
    }
  if (ma_remain_ < 1)
    {
      LOG3("Player `%1' of team `%2' doesn't have enough movement remaining to block.",
          id_, team_id_);
      r_->sendIllegal(m->token, m->client_id);
      return;
    }
  SPlayer* target = r_->getOpponentTeam(team_id_)->getPlayer(m->opponent_id);
  if (target == NULL)
    {
      LOG3("Player `%1' of team `%2' can not block non-existent player `%3'.",
          id_, team_id_, m->opponent_id);
      r_->sendIllegal(m->token, m->client_id);
      return;
    }
  if (status_ != STA_STANDING)
    {
      LOG3("Player `%1' of team `%2' must be standing to do a block (status: `%3').",
          id_, team_id_, status_);
      r_->sendIllegal(m->token, m->client_id);
      return;
    }
  if (target->status_ != STA_STANDING)
    {
      LOG3("Player `%1' of team `%2' can not block non-standing player `%3' (status: `%4').",
          id_, team_id_, target->getId(), target->status_);
      r_->sendIllegal(m->token, m->client_id);
      return;
    }
  if (!pos_.isNear(target->getPosition()))
    {
      LOG3("Player `%1' of team `%2' at %3 can not block player `%4' at %5.",
          id_, team_id_, Player::stringify(status_), pos_,
          target_->getId(), target->getPosition());
      r_->sendIllegal(m->token, m->client_id);
      return;
    }
  target_ = target;
  r_->sendPacket(*m);
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
  if (status_ != STA_STANDING)
    {
      LOG3("Player `%1' of team `%2' must standing to do a move (status: `%3').",
          id_, team_id_, status_);
      r_->sendIllegal(m->token, m->client_id);
      return;
    }
  // Checks that the player has enough ma remaining
  if (ma_remain_ < m->nb_move)
    {
      LOG3("Player `%1' of team `%2' doesn't have enough movement remaining to do such move.",
          id_, team_id_);
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
          LOG3("Player `%1' of team `%2' can not move from %3 to %4 which are not adjacents.",
              id_, team_id_, from, to);
          r_->sendIllegal(MSG_MOVE, m->client_id, ERR_NOTADJACENTSQUARE);
          return;
        }
      if (f_->getPlayer(to) != NULL)
        {
          LOG3("Player `%1' of team `%2' can not move to non-empty square %3.",
              id_, team_id_, to);
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
      LOG3("Player `%1' of team `%2' must own the ball to throw it.", id_, team_id_);
      r_->sendIllegal(MSG_PASS, m->client_id, ERR_DOESNTOWNTHEBALL);
      return;
    }
  throw_aim_ = Position (m->dest_row, m->dest_col);
  if (!f_->intoField(throw_aim_))
    {
      LOG3("Player `%1' of team `%2' can not willingly pass the ball to the crowd at %3.",
          id_, team_id_, throw_aim_);
      r_->sendIllegal(MSG_PASS,m->client_id, ERR_CANTPASSTOTHECROWD);
      return;
    }
  distance_ = pos_.distance(throw_aim_);
  if (distance_ >= 16.f)
    {
      LOG3("Player `%1' of team `%2' can not throw the ball as far as %3 (distance: `%4').",
          id_, team_id_, throw_aim_, distance_);
      r_->sendIllegal(MSG_PASS, m->client_id, ERR_CANTPASSTHATFARAWAY);
      return;
    }
  tryThrow();
}

void SPlayer::msgPlayerPos(const MsgPlayerPos* m)
{
  Position new_pos;
  SPlayer* out_goer;
  if (t_->state_ != GS_INITKICKOFF)
    {
      LOG3("Player `%1' of team `%2' can not enter in play, while his team is in state `%3'.",
          id_, team_id_, t_->state_);
      r_->sendIllegal(m->token, m->client_id, ERR_WRONGCONTEXT);
      return;
    }
  if (status_ != STA_RESERVE && status_ != STA_STANDING)
    {
      LOG3("Player `%1' of team `%2' can not enter in play, due to his status `%3'.",
          team_id_, id_, stringify(status_));
      r_->sendIllegal(m->token, m->client_id, ERR_CANNOTENTERINPLAY);
      return;
    }
  new_pos = Position(m->row, m->col);
  if (status_ == STA_STANDING && pos_ == new_pos)
    {
      LOG3("Player `%1' of team `%2' is already standing at %3.", id_, team_id_, pos_);
      r_->sendIllegal(m->token, m->client_id);
      return;
    }
  // Places player in the reserve if placement is out of the field, or in the wrong field side.
  if (!f_->intoField(new_pos)
      || t_->getTeamId() == 0 && new_pos.row >= ROWS / 2
      || t_->getTeamId() == 1 && new_pos.row < ROWS / 2)
    {
      // Does nothing if the player is already in the reserve.
      if (status_ != STA_RESERVE)
        setStatus(STA_RESERVE);
    }
  else
    {
      out_goer = f_->getPlayer(new_pos);
      if (out_goer != NULL)
        out_goer->setStatus(STA_RESERVE);
      if (status_ != STA_STANDING)
        setStatus(STA_STANDING);
      setPosition(new_pos, false);
      r_->sendPacket(*m);
    }
}

void SPlayer::msgSkill(const MsgSkill* m)
{
  enum eSkill skill = (enum eSkill) m->skill;
  if (t_->state_ != GS_REROLL && r_->getCurrentTeam()->state_ != GS_SKILL)
    {
      LOG3("Player `%1' of team `%2' can not use the skill `%3' now.",
          id_, team_id_, stringify(skill));
      r_->sendIllegal(m->token, m->client_id, ERR_WRONGCONTEXT);
    }
  if (!hasSkill(skill))
    {
      LOG3("Player `%1' of team `%2' doesn't have the skill `%3'.",
          id_, team_id_, stringify(skill));
      r_->sendIllegal(m->token, m->client_id);
      return;
    }
  if (!canUseSkill(skill))
    {
      LOG3("Player `%1' of team `%2' is not allowed to use his skill `%3'.",
          id_, team_id_, stringify(skill));
      r_->sendIllegal(m->token, m->client_id);
      return;
    }
  if (t_->state_ == GS_REROLL)
    {
      if (m->choice == 1)
        {
          LOG5("Player `%1' of team `%2' uses the skill `%3' to reroll `%4'.",
            id_, team_id_, stringify(skill), Dice::stringify(ah_->getRollType()));
          r_->checkForCurrentOpponentChoice(m->client_id);
          r_->sendPacket(*m);
          t_->state_ = m->client_id == 0 ? GS_COACH1 : GS_COACH2;
          useSkill(skill);
          ah_->process(true);
        }
      else
        {
          LOG5("Player `%1' of team `%2' doesn't use the skill `%3' to reroll `%4'.",
            id_, team_id_, stringify(skill), Dice::stringify(ah_->getRollType()));
          r_->checkForCurrentOpponentChoice(m->client_id);
          r_->sendPacket(*m);
          t_->state_ = m->client_id == 0 ? GS_COACH1 : GS_COACH2;
          ah_->process(false);
        }
    }
  else if (r_->getCurrentTeam()->state_ == GS_SKILL)
    {
      LOG5("Player `%1' of team `%2' chooses to%3 use the skill `%4'.",
          id_, team_id_, ((m->choice == 1) ? "" : " NOT"), Player::stringify(skill));
      r_->checkForCurrentOpponentChoice(m->client_id);
      r_->sendPacket(*m);
      r_->getCurrentTeam()->state_ = m->client_id == 0 ? GS_COACH1 : GS_COACH2;
      ah_->process(m->choice == 1);
    }
}

void SPlayer::msgStandUp(const MsgStandUp* m)
{
  if (!t_->canDoAction(m, this))
    return;
  if (status_ != STA_PRONE)
    {
      LOG3("Player `%1' of team `%2' must be prone (not `%3') to stand up.",
          id_, team_id_, status_);
      r_->sendIllegal(m->token, m->client_id);
      return;
    }
  if (ma_remain_ != ma_)
    {
      LOG3("Player `%1' of team `%2' cannot try to stand up more than once.",
          id_, team_id_);
      r_->sendIllegal(m->token, m->client_id);
      return;
    }
  tryStandUp();
}

