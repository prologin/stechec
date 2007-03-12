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

SPlayer::SPlayer(SRules* r, const MsgPlayerCreate* m, STeam* t, SPlayerMsg* pm)
  : Player(m),
    r_(r),
    t_(t),
    f_(r->getField()),
    d_(r->getDice()),
    pm_(pm),
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

void SPlayer::setPosition(const Position& pos, bool advertise_client)
{
  if (f_->intoField(pos_))
    f_->setPlayer(pos_, NULL);
  if (advertise_client && pos_ != pos)
    pm_->sendPosition(this);

  pos_ = pos;
  // Pushed off-field.
  if (!f_->intoField(pos))
    {
      rollInjury(0);
      // Go into reserve if stunned.
      if (status_ == STA_STUNNED)
        setStatus(STA_RESERVE);
      // Check for the ball.
      if (r_->getBall()->getOwner() == this)
        r_->getBall()->throwin();
    }
  else
    f_->setPlayer(pos, this);
  // Check for the ball.
  if (r_->getBall()->getOwner() == this) 
    r_->getBall()->setPosition(pos_, advertise_client);
}

// Standard action. Launch a D6:
//  - 1 -> always fails
//  - 6 -> always succeed
//  - else -> apply modifier and compare with agility.
bool SPlayer::tryAction(int modifier)
{
  int dice_res = d_->roll("action");
  int dice_modif = dice_res + modifier;
  int required = 7 - std::min(ag_, 6);
  if (dice_res != 6 && (dice_modif < required || dice_res == 1))
    {
      LOG6("[%1] Action _failed_, dice: %2, w/modifiers: %3, required: %4, ag: %5.", id_, dice_res, dice_modif,
          required, ag_ );
      pm_->sendRoll(dice_res, modifier, required, this);
      return false;
    }
  LOG6("[%1] Action succeed, dice: %2, w/modifiers: %3, required: %4, ag: %5.", id_, dice_res,
       dice_modif, required, ag_);
  pm_->sendRoll(dice_res, modifier, required, this);
  return true;
}

/*
** Move action
*/

int SPlayer::doMove(const MsgMove* m)
{
  // Check the player has enough ma remaining
  if (ma_remain_ < m->nb_move)
    {
      LOG4("Move: not enough movement remaining");
      r_->sendIllegal(MSG_MOVE, m->client_id);
      return 0;
    }
    
  MsgMove res_move(team_id_);
  SBall* b = r_->getBall();  
  int illegal = 0;
  bool knocked = false;
  bool picking_failed = false;
  Position aim;
  Position pos_ball = b->getPosition();

  res_move.player_id = m->player_id;
  res_move.nb_move = 0;
  for (int i = 0; i < m->nb_move; i++)
    {
      // Check that we can move here.
      aim.row = m->moves[i].row;
      aim.col = m->moves[i].col;
      if (!pos_.isNear(aim))
        {
          LOG4("Move: not an adjacent case");
          illegal = 1;
          break;
        }

      // Check tackle zones.
      int nb_tackles_pos = f_->getNbTackleZone(r_->getCurrentOpponentTeamId(), pos_);
      int nb_tackles_aim = f_->getNbTackleZone(r_->getCurrentOpponentTeamId(), aim);
      if (nb_tackles_pos > 0)
        {
          action_attempted_ = R_DODGE;
          reroll_enabled_ = t_->canUseReroll();
          if (tryAction(1 - nb_tackles_aim))
            {
              LOG5("Player has successfully dodged out from %1.", pos_);
            }
          else
            {
              LOG5("Player fails to dodge out from %1.", pos_);
              aim_ = aim;
              if (reroll_enabled_)
                {
                  if (res_move.nb_move > 0)
                    r_->sendPacket(res_move);
                  return 0;
                }

              r_->getCurrentTeam()->turnover(TOM_KNOCKEDDOWN);
              knocked = true;
              checkArmor(0, 0);
              if (status_ == STA_STANDING)
                setStatus(STA_PRONE);
              pm_->sendMsgKnocked(this);
            }
        }

      // Ok... Move on.
      res_move.nb_move++;
      res_move.moves[i].row = aim.row;
      res_move.moves[i].col = aim.col;
      setPosition(aim, false);
      ma_remain_--;

      // Check if can pick the ball.
      if (b->getPosition() == pos_&&b->getOwner() != this)
        if (knocked)
          {
            picking_failed = true;
            b->bounce();
          }
        else
          {
            action_attempted_ = R_PICKUP;
            reroll_enabled_ = t_->canUseReroll();
            picking_failed = !b->catchBall(this, 1);
            if (picking_failed && reroll_enabled_)
              {
                if (res_move.nb_move > 0)
                  r_->sendPacket(res_move);
                return 0;
              }
          }
      
      if (knocked || picking_failed)
        break;
    }
    
  // If he falls with the ball
  if (knocked&&b->getOwner() == this)
    {
      b->bounce();
    }
    
  // Send the results.
  if (res_move.nb_move > 0)
    r_->sendPacket(res_move);
  if (illegal)
    r_->sendIllegal(MSG_MOVE, m->client_id);
  if (b->getPosition() != pos_ball)
    {
      b->sendPosition();
    }
  if (knocked||picking_failed)
    return 1;

  // Check if the player scores a touchdown
  if (b->getOwner() == this)
    {
      // I know it's ugly, but it's late...
      int row = (ROWS - 1) * (1 - team_id_);
      if (pos_.row == row)
        r_->touchdown();
    }
  return 0;
}

/*
** Stand up action
*/

void SPlayer::doStandUp(const MsgStandUp*)
{
  if (ma_ < 3)
    {
      ma_remain_ = 0;
      int result = d_->roll("standup");
      action_attempted_ = R_STANDUP;
      reroll_enabled_ = t_->canUseReroll();
      pm_->sendRoll(result, 0, 4, this);
      if (result >= 4)
        setStatus(STA_STANDING);
    }
  else
    {
      ma_remain_ = ma_ - 3;
      setStatus(STA_STANDING);
    }
  //FIXME: advertise client.
}

/*
** Block action
*/

int SPlayer::doBlock(const MsgBlock* m)
{
  int other_team_id = m->client_id == 0 ? 1 : 0;
  target_ = r_->getTeam(other_team_id)->getPlayer(m->opponent_id);
  if (target_ == NULL
      || target_->getTeamId() == getTeamId()
      || target_->status_ != STA_STANDING
      || !pos_.isNear(target_->getPosition()))
    {
      LOG3("Cannot block player '%1` at %2 (status: %3).", m->opponent_id,
          target_->getPosition(), target_->status_);
      return 0;
    }

  if (getAction() == DCL_BLOCK)
    setHasPlayed();
  else
    ma_remain_ = ma_remain_ - 1;
  
  int mod_st_atk = getSt();
  int mod_st_df = target_->getSt();

  if (mod_st_atk > 2 * mod_st_df || mod_st_df > 2 * mod_st_atk)
    nb_dice_ = 3;
  else if (mod_st_atk != mod_st_df)
    nb_dice_ = 2;
  else
    nb_dice_ = 1;

  MsgBlockResult msg(team_id_);
  msg.player_id = id_;
  msg.opponent_id = m->opponent_id;
  msg.reroll = t_->canUseReroll();
  if (nb_dice_ == 1)
    msg.strongest_team_id = -1;
  else if (mod_st_atk < mod_st_df)
    msg.strongest_team_id = r_->getCurrentOpponentTeamId();
  else
    msg.strongest_team_id = team_id_;
  msg.nb_dice = nb_dice_;

  for (int i = 0; i < nb_dice_; ++i)
    {
      result_[i] = (enum eBlockDiceFace)d_->roll("block", DBLOCK);
      LOG5("Rolled block dice: %1", Dice::stringify(result_[i]));
      msg.results[i] = result_[i];
    }

  r_->sendPacket(msg);

  if (t_->canUseReroll())
    t_->setConcernedPlayer(this);
  choose_block_ = msg.strongest_team_id != r_->getCurrentOpponentTeamId();
  action_attempted_ = R_BLOCK;
  if (mod_st_atk == mod_st_df)
    {
      if (t_->canUseReroll())
        {
          t_->state_ = GS_REROLL;
        }
      else
        resolveBlock(0);
    }
  else if (mod_st_atk > mod_st_df)
    {
      t_->state_ = GS_BLOCK;
    }
  else
    {
      if (t_->canUseReroll())
        {
          t_->state_ = GS_REROLL;
        }
      else
        {
          r_->getTeam(other_team_id)->state_ = GS_BLOCK;
        }
    }
  return 0;
}

void SPlayer::resolveBlock(int chosen_dice)
{
  switch (result_[chosen_dice])
    {
    case BATTACKER_DOWN :
      r_->getCurrentTeam()->turnover(TOM_KNOCKEDDOWN);
      checkArmor(0, 0);
      if (status_ == STA_STANDING)
        setStatus(STA_PRONE);
      pm_->sendMsgKnocked(this);
      if (r_->getBall()->getOwner() == this)
        r_->getBall()->bounce();
      r_->turnOver();
      break;
    case BBOTH_DOWN :
      r_->getCurrentTeam()->turnover(TOM_KNOCKEDDOWN);
      checkArmor(0, 0);
      if (status_ == STA_STANDING)
        setStatus(STA_PRONE);
      target_->checkArmor(0, 0);
      if (target_->getStatus() == STA_STANDING)
        target_->setStatus(STA_PRONE);
      pm_->sendMsgKnocked(this);
      pm_->sendMsgKnocked(target_);
      if (r_->getBall()->getOwner() == this)
        r_->getBall()->bounce();
      if (r_->getBall()->getOwner() == target_)
        r_->getBall()->bounce();
      r_->turnOver();
      break;
    case BPUSHED :
      target_knocked_ = false;
      pusher_ = NULL;
      blockPushChoice(target_);
      break;
    case BDEFENDER_STUMBLE :
      target_knocked_ = true;
      pusher_ = NULL;
      blockPushChoice(target_);
      break;
    case BDEFENDER_DOWN : 
      target_knocked_ = true;
      pusher_ = NULL;
      blockPushChoice(target_);
      break;
    }
}

void SPlayer::blockPushChoice(SPlayer* target)
{
  Position squares[3];
  Position dt = target->getPosition();
  Position d = dt - getPosition();
  aim_ = dt;
  target_ = target;

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

  LOG3("Pusher pos: %1 Aimed pos: %2", pos_, target->pos_);
  LOG3("Opposite squares: c1: %1 c2: %2 c3: %3", squares[0], squares[1], squares[2]);

  nb_push_choices_ = 0;

  // Look for empty squares into the field.
  for (int i = 0; i < 3; i++)
    if (f_->intoField(squares[i]) && f_->getPlayer(squares[i]) == NULL)
      {
        push_choices_[nb_push_choices_] = squares[i];
        nb_push_choices_++;
      }

  // No choice -> Try to push out of the field.
  if (nb_push_choices_ == 0)
    {
      for (int i = 0; i < 3; i++)
        if (!f_->intoField(squares[i]) && nb_push_choices_ == 0)
          {
            push_choices_[nb_push_choices_] = squares[i];
            nb_push_choices_++;
          }
    }

  // Still no one possibility -> all possibilities
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

  target_->pusher_ = this;
  r_->getTeam(getTeamId())->setPusher(this);
  pm_->sendMsgBlockPush(nb_push_choices_, push_choices_, target_);

  if (nb_push_choices_ == 1)
    blockPush(0);
  else
    {
      t_->state_ = GS_PUSH;
    }
}

void SPlayer::blockPush(const MsgBlockPush* m)
{
  if (nb_push_choices_ -1 < m->square_chosen || m->square_chosen < 0)
    {
      LOG2("Invalid push choice.");
      r_->sendIllegal(MSG_BLOCKPUSH, m->client_id);
      return;
    }

  if (pusher_ == NULL && (team_id_ != r_->getCurrentTeamId() || this != r_->getTeam(team_id_)->getActivePlayer()))
    {
      LOG2("This player shouldn't push anyone.");
      r_->sendIllegal(MSG_BLOCKPUSH, m->client_id);
      return;
    }

  r_->sendPacket(*m);

  blockPush(m->square_chosen);
}

void SPlayer::blockPush(int chosen_square)
{
  Position to(push_choices_[chosen_square].row, push_choices_[chosen_square].col);
  SPlayer* other_target = f_->getPlayer(to);

  LOG2("Blockpush 2nd phase: Try to move player %1 (team %2) to %3", target_->getId(), target_->getTeamId(), to);

  if (other_target == NULL)
    {
      target_->setPosition(to, true); //FIXME: ball must bounce if end player is pushed on its square.
      blockFollow();
    }
  else
    {
      // Oh, another player to move.
      r_->getCurrentTeam()->setPusher(target_);
      target_->aim_ = to;
      target_->pusher_ = this;
      target_->blockPushChoice(other_target);
    }
}

void SPlayer::blockFollow()
{
  if (pusher_ != NULL) 
    {
      setPosition(aim_, true);
      pusher_->blockFollow();
    }
  else
    {
      t_->state_ = GS_FOLLOW;
      r_->sendPacket(MsgFollow(team_id_));
    }
}

void SPlayer::follow(bool follow)
{
  if (follow)
    {
      setPosition(aim_, true);
    }
  if (target_knocked_ && target_->status_ == STA_STANDING)
    {
      target_->checkArmor(0, 0);
      if (target_->getStatus() == STA_STANDING)
        target_->setStatus(STA_PRONE);
      pm_->sendMsgKnocked(target_);
      if (r_->getBall()->getOwner() == target_)
        r_->getBall()->bounce();
    }
}

/*
** Pass action.
*/

int SPlayer::doPass(const MsgPass* m)
{
  SBall* b = r_->getBall();

  // Player must own the ball
  if (b->getOwner() != this)
    {
      LOG2("You must own the ball");
      r_->sendIllegal(MSG_PASS, m->client_id);
      return 0;
    }

  ma_remain_ = 0;

  Position dest(m->dest_row, m->dest_col);
  
  if (!f_->intoField(dest))
    {
      LOG2("You must aim at a square in the field.");
      r_->sendIllegal(MSG_PASS,m->client_id);
      return 0;
    }

  float dist = pos_.distance(dest);
  int dist_modifier;

  if (dist < 4.f)
    dist_modifier = 1; // quick pass
  else if (dist < 8.f)
    dist_modifier = 0; // short pass
  else if (dist < 12.f)
    dist_modifier = -1; // long pass
  else if (dist < 16.f)
    dist_modifier = -2; // long bomb
  else
    {
      LOG2("Too far");
      r_->sendIllegal(MSG_PASS, m->client_id);
      return 0;
    }

  has_played_ = true;
  int nb_tackles = f_->getNbTackleZone(r_->getCurrentOpponentTeamId(), pos_);
  int modifier = dist_modifier - nb_tackles;
  int catch_mod = 1;

  b->setPosition(dest);
  action_attempted_ = R_THROW;
  reroll_enabled_ = t_->canUseReroll();
  if (!tryAction(modifier))
    {
      if (reroll_enabled_) {
        return 0;
      }
      catch_mod = 0;
      b->scatter(1);
      b->scatter(1);
      b->scatter(1);
    }

  b->setThrown();

  b->sendPosition();

  SPlayer* p = f_->getPlayer(b->getPosition());
  if (p != NULL&&p->getStatus() == STA_STANDING)
    {
      p->action_attempted_ = R_CATCH;
      p->reroll_enabled_ = t_->canUseReroll();
      b->catchBall(p, catch_mod);
    }
  else
    {
      b->bounce();
      return 1;
    }
  
  // The ball checks if a player of the current team catch it

  return 0;
}


/*
** Use of reroll
*/

void SPlayer::finishAction(bool reroll)
{
  reroll_enabled_ = false;
  int result = -1; //FIXME: Useless with the new turnover flag.
  switch (action_attempted_)
    {
    case R_DODGE : result = finishMove(reroll);
      break;
    case R_STANDUP : finishStandUp(reroll);
      result = 0;
      break;
    case R_PICKUP : result = finishPickUp(reroll, true);
      break;
    case R_THROW : result = finishThrow(reroll);
      break;
    case R_CATCH : finishPickUp(reroll, false);
      result = 0;
      break;
    case R_BLOCK : finishBlock(reroll);
      result = 0;
      break;
    default:
      break;
    }
  if (result == 1)
    {
      r_->getCurrentTeam()->turnover(TOM_EJECTEDFORAFOUL); //FIXME: Inconsistent or useless.
      r_->turnOver();
    }
  assert(result != -1);
}

int SPlayer::finishMove(bool reroll)
{
  SBall* b = r_->getBall();  

  bool knocked = false;
  bool picking_failed = false;

  Position pos_ball = b->getPosition();

  // Check tackle zones.
  int nb_tackles_aim = f_->getNbTackleZone(r_->getCurrentOpponentTeamId(), aim_);
  if (reroll&&tryAction(1 - nb_tackles_aim))
    {
      LOG5("Player has successfully dodged out from %1.", pos_);
    }
  else
    {
      LOG5("Player has been knocked out from %1.", pos_);
      r_->getCurrentTeam()->turnover(TOM_KNOCKEDDOWN);
      knocked = true;
      checkArmor(0, 0);
      if (status_ == STA_STANDING)
        setStatus(STA_PRONE);
      pm_->sendMsgKnocked(this);
    }

  // Ok... Move on.
  MsgMove res_move(team_id_);
  res_move.player_id = id_;
  res_move.nb_move = 1;
  res_move.moves[0].row = aim_.row;
  res_move.moves[0].col = aim_.col;

  setPosition(aim_, true);
  ma_remain_--;

  // Check if can pick the ball.
  if (b->getPosition() == pos_&&b->getOwner() != this)
    if (knocked)
      {
        picking_failed = true;
        b->bounce();
      }
    else
      {
        picking_failed = !b->catchBall(this, 1);
      }
          
  // If he falls with the ball
  if (knocked&&b->getOwner() == this)
    {
      b->bounce();
    }
    
  // Send the result.
  r_->sendPacket(res_move);
  if (b->getPosition() != pos_ball)
    {
      b->sendPosition();
    }
  if (knocked||picking_failed)
    return 1;
  
  // Check if the player scores a touchdown
  if (b->getOwner() == this)
    {
      // I know it's ugly, but it's late...
      int row = (ROWS - 1) * (1 - team_id_);
      if (pos_.row == row)
        r_->touchdown();
    }
  return 0;
}

void SPlayer::finishStandUp(bool reroll)
{
  if (reroll)
    {
      int result = d_->roll("finishstandup");
      action_attempted_ = R_STANDUP;
      pm_->sendRoll(result, 0, 4, this);
      if (result >= 4)
        setStatus(STA_STANDING);
      //FIXME: advertise client.
    }
}

//FIXME: Separate pickup and catch resolution.
int SPlayer::finishPickUp(bool reroll, bool pickup)
{
  SBall* b = r_->getBall();  

  if (reroll)
    {
      if (b->catchBall(this, 1))
        {
          // I know it's ugly, but it's late...
          int row = (ROWS - 1) * (1 - team_id_);
          if (pos_.row == row)
            r_->touchdown();
          return 0;
        }
      else
        {
          if (pickup)
            r_->getCurrentTeam()->turnover(TOM_FAILEDPICKUP);
          return 1;
        }
    }
  else
    {
      if (pickup)
        r_->getCurrentTeam()->turnover(TOM_FAILEDPICKUP);
      b->bounce();
      return 1;
    }
}

int SPlayer::finishThrow(bool reroll)
{
  SBall* b = r_->getBall();

  float dist = pos_.distance(b->getPosition());
  int dist_modifier;

  if (dist < 4.f)
    dist_modifier = 1; // quick pass
  else if (dist < 8.f)
    dist_modifier = 0; // short pass
  else if (dist < 12.f)
    dist_modifier = -1; // long pass
  else if (dist < 16.f)
    dist_modifier = -2; // long bomb

  int nb_tackles = f_->getNbTackleZone(r_->getCurrentOpponentTeamId(), pos_);
  int modifier = dist_modifier - nb_tackles;
  int catch_mod = 1;

  if (!reroll||!tryAction(modifier))
    {
      catch_mod = 0;
      b->scatter(1);
      b->scatter(1);
      b->scatter(1);
    }
  b->setThrown();

  b->sendPosition();

  SPlayer* p = f_->getPlayer(b->getPosition());
  if (p != NULL && p->getStatus() == STA_STANDING)
    b->catchBall(p, catch_mod);
  else
    b->bounce();
  
  // The ball checks if a player of the current team catch it

  return r_->getCurrentTeam()->isTurnover();
}

void SPlayer::finishBlock(bool reroll)
{
  MsgBlockResult msg(team_id_);
  msg.player_id = id_;
  msg.opponent_id = target_->id_;
  msg.reroll = 0;
  if (nb_dice_ == 1)
    msg.strongest_team_id = -1;
  else if (choose_block_)
    msg.strongest_team_id = team_id_;
  else
    msg.strongest_team_id = r_->getCurrentOpponentTeamId();
  msg.nb_dice = nb_dice_;

  if (reroll)
    {
      for (int i = 0; i < nb_dice_; ++i)
        {
          result_[i] = (enum eBlockDiceFace)d_->roll("finishblock", DBLOCK);
          LOG5("Rolled block dice: %1", Dice::stringify(result_[i]));
          msg.results[i] = result_[i];
        }
    }
  else
    {
      for (int i = 0; i < nb_dice_; ++i)
        msg.results[i] = result_[i];
    }

  r_->sendPacket(msg);

  if (nb_dice_ == 1)
    {
      t_->state_ = team_id_ == 0 ? GS_COACH1 : GS_COACH2;
      resolveBlock(0);
    }
  else if (!choose_block_)
    {
      t_->state_ = team_id_ == 0 ? GS_COACH1 : GS_COACH2;
      r_->getTeam(target_->getTeamId())->state_ = GS_BLOCK;
    }
}

/*
** Player state, injuries...
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

void SPlayer::checkArmor(int av_mod, int inj_mod)
{
  int result = d_->roll("checkarmor", D6, 2);
  if (result + av_mod > av_)
    rollInjury(inj_mod);
}

void SPlayer::rollInjury(int inj_mod)
{
  int injury = d_->roll("injury", D6, 2) + inj_mod;
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
  LOG6("[%1] Amor Passed, Injury : %2.", id_, injury);
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
** Messages, filters
*/

void SPlayer::msgDeclare(const MsgDeclare* m)
{
  if (!t_->canDeclareAction(m))
    return;
  action_ = (enum eDeclaredAction )m->action;
  r_->sendPacket(*m);
}

void SPlayer::msgMove(const MsgMove* m)
{
  if (!t_->canDoAction(m, this))
    return;
  if (doMove(m))
    r_->turnOver();
}

void SPlayer::msgStandUp(const MsgStandUp* m)
{
  if (!t_->canDoAction(m, this))
    return;
  doStandUp(m);
}

void SPlayer::msgBlock(const MsgBlock* m)
{
  if (!t_->canDoAction(m, this))
    return;
  if (doBlock(m))
    r_->turnOver();
}

void SPlayer::msgPass(const MsgPass* m)
{
  if (!t_->canDoAction(m, this))
    return;
  if (doPass(m))
    r_->turnOver();
}
