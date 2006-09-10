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

#include "Dice.hh"
#include "SRules.hh"
#include "SPlayer.hh"

SPlayer::SPlayer(SRules* r, const MsgPlayerCreate* m, STeam* t)
  : Player(m),
    r_(r),
    t_(t),
    f_(r->getField()),
    d_(r->getDice()),
    target_(NULL)
{
  r_->HANDLE_F_WITH(ACT_MOVE, SPlayer, this, msgMove, filterMove, GS_COACHBOTH);
  r_->HANDLE_F_WITH(ACT_STANDUP, SPlayer, this, msgStandUp, filterStandUp, GS_COACHBOTH);
  r_->HANDLE_F_WITH(ACT_BLOCK, SPlayer, this, msgBlock, filterBlock, GS_COACHBOTH);
  r_->HANDLE_F_WITH(ACT_PASS, SPlayer, this, msgPass, filterPass, GS_COACHBOTH);
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
    {
      MsgPlayerPos pkt(team_id_);
      pkt.player_id = id_;
      pkt.row = pos.row;
      pkt.col = pos.col;
      r_->sendPacket(pkt);
    }
  pos_ = pos;
  // Check for the ball
  if (r_->getBall()->getOwner() == this) 
    {
      r_->getBall()->setPosition(pos_);
    }
  
  if (!f_->intoField(pos))
		{
    	rollInjury(0);
			if (status_ == STA_STUNNED)
				setStatus(STA_RESERVE);
		}
  else
    f_->setPlayer(pos, this);
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
      LOG6("[" << id_ << "] Action _failed_, dice: " << dice_res
           << ", w/modifiers: " << dice_modif << ", required: " << required
           << ", ag: " << ag_ << ".");
      sendRoll(dice_res, modifier, required);
      return false;
    }
  LOG6("[" << id_ << "] Action succeed, dice: " << dice_res
       << ", w/modifiers: " << dice_modif << ", required: " << required
       << ", ag: " << ag_ << ".");
  sendRoll(dice_res, modifier, required);
  return true;
}

/*
** Move action
*/

int SPlayer::doMove(const ActMove* m)
{
  // Check the player has enough ma remaining
  if (ma_remain_ < m->nb_move)
    {
      LOG4("Move: not enough movement remaining");
      r_->sendIllegal(ACT_MOVE, m->client_id);
      return 0;
    }
    
  ActMove res_move(team_id_);
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
              LOG5("Player has successfully dodged out from " << pos_ << ".");
            }
          else
            {
              LOG5("Player fails to dodge out from " << pos_ << ".");
	      aim_ = aim;
	      if (reroll_enabled_) {
		if (res_move.nb_move > 0)
		  r_->sendPacket(res_move);
		return 0;
	      }

              knocked = true;
	      checkArmor(0, 0);
	      if (status_ == STA_STANDING)
		setStatus(STA_PRONE);
	      MsgPlayerKnocked pkt(m->client_id);
	      pkt.player_id = id_;
	      r_->sendPacket(pkt);
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
    r_->sendIllegal(ACT_MOVE, m->client_id);
  if (b->getPosition() != pos_ball)
    {
      MsgBallPos mesg;
      mesg.row = b->getPosition().row;
      mesg.col = b->getPosition().col;
      r_->sendPacket(mesg);
    }
  if (knocked||picking_failed)
    return 1;
	
  // Check if the player scores a touchdown
  if (b->getOwner() == this)
    {
      // I know it's ugly, but it's late...
      int row = 25 * (1 - team_id_);
      if (pos_.row == row)
	r_->touchdown();
    }	
  return 0;
}

/*
** Stand up action
*/

void SPlayer::doStandUp(const ActStandUp*)
{
  if (ma_ < 3)
    {
      ma_remain_ = 0;
      int result = d_->roll("standup");
      action_attempted_ = R_STANDUP;
      reroll_enabled_ = t_->canUseReroll();
      sendRoll(result, 0, 4);
      if (result >= 4)
	setStatus(STA_STANDING);
    }
  else
    {
      ma_remain_ = ma_ - 3;
      setStatus(STA_STANDING);
    }
}

/*
** Block action
*/

int SPlayer::doBlock(const ActBlock* m)
{
  int other_team_id = m->client_id == 0 ? 1 : 0;
  target_ = r_->getTeam(other_team_id)->getPlayer(m->opponent_id);
  if (target_ == NULL
      || target_->getTeamId() == getTeamId()
      || target_->status_ != STA_STANDING
      || !pos_.isNear(target_->getPosition()))
    {
      LOG3("Cannot block player '" << other_team_id << "' at "
	   << target_->getPosition() << " (status: " << target_->status_ << ").");
      return 0;
    }

  if (getAction() == BLOCK)
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
  if (nb_dice_ == 1)
    msg.choose_team_id =  - 1;
  else if (mod_st_atk < mod_st_df)
    msg.choose_team_id = r_->getCurrentOpponentTeamId();
  else
    msg.choose_team_id = team_id_;
  msg.player_id = id_;
  msg.opponent_id = m->opponent_id;
  msg.nb_dice = nb_dice_;

  for (int i = 0; i < nb_dice_; ++i)
    {
      result_[i] = (enum eBlockDiceFace)d_->roll("block", DBLOCK);
      LOG5("Rolled block dice: " << Dice::stringify(result_[i]));
      msg.results[i] = result_[i];
    }

  msg.reroll = t_->canUseReroll();
  r_->sendPacket(msg);

  if (t_->canUseReroll())
    t_->setConcernedPlayer(this);
  choose_block_ = msg.choose_team_id != r_->getCurrentOpponentTeamId();
  action_attempted_ = R_BLOCK;
  if (mod_st_atk == mod_st_df)
    {
      if (t_->canUseReroll())
	{
	  t_->state_ = GS_REROLL;
	}
      else
	resolveBlock(0, target_);
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

void SPlayer::resolveBlock(int choosen_dice)
{
  resolveBlock(choosen_dice, target_);
}

void SPlayer::resolveBlock(int chosen_dice, SPlayer* target)
{
LOG4("Resolve block.");
  MsgPlayerKnocked pkt1(team_id_);
  MsgPlayerKnocked pkt2(r_->getCurrentOpponentTeamId());
  switch(result_[chosen_dice])
    {
    case BATTAKER_DOWN :
      checkArmor(0, 0);
      if (status_ == STA_STANDING)
        setStatus(STA_PRONE);
      pkt1.player_id = id_;
      r_->sendPacket(pkt1);
			if (r_->getBall()->getOwner() == this)
			  r_->getBall()->bounce();
      r_->turnOver();
      break;
    case BBOTH_DOWN :
      checkArmor(0, 0);
      if (status_ == STA_STANDING)
	      setStatus(STA_PRONE);
      target->checkArmor(0, 0);
      if (target->getStatus() == STA_STANDING)
	      target->setStatus(STA_PRONE);
      pkt1.player_id = id_;
      r_->sendPacket(pkt1);
      pkt2.player_id = target->getId();
      r_->sendPacket(pkt2);
			if (r_->getBall()->getOwner() == this)
			  r_->getBall()->bounce();
			if (r_->getBall()->getOwner() == target)
			  r_->getBall()->bounce();
      r_->turnOver();
      break;
    case BPUSHED :
      blockPushChoice(target);
      target_knocked_ = false;
      pusher_ = NULL;
      break;
    case BDEFENDER_STUMBLE :
      blockPushChoice(target);
      target_knocked_ = true;
      pusher_ = NULL;
      break;
    case BDEFENDER_DOWN : 
      blockPushChoice(target);
      target_knocked_ = true;
      pusher_ = NULL;
      break;
    }
}

void SPlayer::blockPushChoice(SPlayer* target)
{
  Position choice[3];
  Position dt = target->getPosition();
  Position d = dt - getPosition();
  aim_ = dt;
  target_ = target;

  choice[1] = dt + d;
  if (d.col == 0)
    {
      choice[0] = dt + d + Position(0, -1);
      choice[2] = dt + d + Position(0, 1);
    }
  else if (d.row == 0)
    {
      choice[0] = dt + d + Position(-1, 0);
      choice[2] = dt + d + Position(1, 0);
    }
  else
    {
      choice[0] = dt + d + Position(-d.row, 0);
      choice[2] = dt + d + Position(0, -d.col);
    }

  LOG3("attacker: " << pos_ << "  defender: " << target->pos_);
  LOG3("c1: " << choice[0] << " c2: " << choice[1] << " c3: " << choice[2]);

  ActBlockPush pkt(r_->getCurrentTeamId());
  pkt.target_row = dt.row;
  pkt.target_col = dt.col;
  pkt.nb_choice = 0;

  for (int i = 0; i < 3; i++)
    if (f_->intoField(choice[i]) && f_->getPlayer(choice[i]) == NULL)
      {
	pkt.choice[pkt.nb_choice].row = choice[i].row;
	pkt.choice[pkt.nb_choice].col = choice[i].col;
	choices_[pkt.nb_choice] = choice[i];
	pkt.nb_choice++;
      }

  // If there is no choice -> out of the field.
  if (pkt.nb_choice == 0)
    {
      for (int i = 0; i < 3; i++)
	if (!f_->intoField(choice[i])&&pkt.nb_choice == 0)
	  {
	    pkt.choice[pkt.nb_choice].row = choice[i].row;
	    pkt.choice[pkt.nb_choice].col = choice[i].col;
	    choices_[pkt.nb_choice] = choice[i];
	    pkt.nb_choice++;
	  }
    }
		
  // Else -> all possibilities
  if (pkt.nb_choice == 0)
    {
      for (int i = 0; i < 3; i++)
	{
	  pkt.choice[pkt.nb_choice].row = choice[i].row;
	  pkt.choice[pkt.nb_choice].col = choice[i].col;
	  choices_[pkt.nb_choice] = choice[i];
	  pkt.nb_choice++;
      	}
    }

  r_->sendPacket(pkt);
	
  if (pkt.nb_choice == 1)
    blockPush(0);
  else
    {
      t_->state_ = GS_PUSH;
    }
}

void SPlayer::blockPush(int chosen_square)
{
  Position to(choices_[chosen_square].row, choices_[chosen_square].col);
  SPlayer* other_target = f_->getPlayer(to);

  LOG2("blockpush 2nd phase: " << to);
  

  if (other_target == NULL)
    {
      target_->setPosition(to, true);
      blockFollow();
    }
  else
    {
      // Oh, another player to move.
			r_->getTeam(r_->getCurrentTeamId())->setConcernedPlayer(target_);
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
			t_->setConcernedPlayer(this);
      r_->sendPacket(MsgFollow(team_id_));
    }
}

void SPlayer::follow(bool follow)
{
  if (follow)
    {
      f_->setPlayer(pos_, NULL);
      setPosition(aim_, true);
    }
  if (target_knocked_&& target_->status_ == STA_STANDING)
    {
      MsgPlayerKnocked pkt(r_->getCurrentOpponentTeamId());
      target_->checkArmor(0, 0);
      if (target_->getStatus() == STA_STANDING)
	target_->setStatus(STA_PRONE);
      pkt.player_id = target_->getId();
      r_->sendPacket(pkt);
			if (r_->getBall()->getOwner() == target_)
			  r_->getBall()->bounce();
    }
}

/*
** Pass action.
*/

int SPlayer::doPass(const ActPass* m)
{
  SBall* b = r_->getBall();

  // Player must own the ball
  if (b->getOwner() != this)
    {
      LOG2("You must own the ball");
      r_->sendIllegal(ACT_PASS, m->client_id);
      return 0;
    }
	
  ma_remain_ = 0;

  Position dest(m->dest_row, m->dest_col);
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
      r_->sendIllegal(ACT_PASS, m->client_id);
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

  MsgBallPos mesg;
  mesg.row = b->getPosition().row;
  mesg.col = b->getPosition().col;
  r_->sendPacket(mesg);

  SPlayer* p = f_->getPlayer(b->getPosition());
  if (p != NULL&&p->getStatus() == STA_STANDING)
    {
      p->action_attempted_ = R_CATCH;
      p->reroll_enabled_ = t_->canUseReroll();
      b->catchBall(p, catch_mod);
    }
  else
    b->bounce();
  
  // The ball checks if a player of the current team catch it
	
  return 0;
}

/*
** Sending roll result
*/
void SPlayer::sendRoll(int result, int modifier, int required)
{
  MsgResult msg(team_id_);
  msg.player_id = id_;
  msg.roll_type = action_attempted_;
  msg.result = result;
  msg.modifier = modifier;
  msg.required = required;	
  msg.reroll = reroll_enabled_;
  r_->sendPacket(msg);
	
  if (reroll_enabled_ && modifier + result < required)
    {					
      t_->state_ = GS_REROLL;
      t_->setConcernedPlayer(this);
    }
}

/*
** Use of reroll
*/

void SPlayer::finishAction(bool reroll)
{
  reroll_enabled_ = false;
  int result = -1;
  switch (action_attempted_)
    {
    case R_DODGE : result = finishMove(reroll);
      break;
    case R_STANDUP : finishStandUp(reroll);
      result = 0;
      break;
    case R_PICKUP : result = finishPickUp(reroll);
      break;
    case R_THROW : finishThrow(reroll);
      result = 0;
      break;
    case R_CATCH : finishPickUp(reroll);
      result = 0;
      break;
    case R_BLOCK : finishBlock(reroll);
      result = 0;
      break;
    default:
      break;
    }
  if (result == 1)
    r_->turnOver();
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
      LOG5("Player has successfully dodged out from " << pos_ << ".");
    }
  else
    {
      LOG5("Player has been knocked out from " << pos_ << ".");
      knocked = true;
      checkArmor(0, 0);
      if (status_ == STA_STANDING)
        setStatus(STA_PRONE);
      MsgPlayerKnocked pkt(team_id_);
      pkt.player_id = id_;
      r_->sendPacket(pkt);
    }

  // Ok... Move on.
  ActMove res_move(team_id_);
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
      MsgBallPos mesg;
      mesg.row = b->getPosition().row;
      mesg.col = b->getPosition().col;
      r_->sendPacket(mesg);
    }
  if (knocked||picking_failed)
    return 1;
	
  // Check if the player scores a touchdown
  if (b->getOwner() == this)
    {
      // I know it's ugly, but it's late...
      int row = 25 * (1 - team_id_);
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
      sendRoll(result, 0, 4);
      if (result >= 4)
	setStatus(STA_STANDING);
    }
}

int SPlayer::finishPickUp(bool reroll)
{
  SBall* b = r_->getBall();  

  if (reroll)
    {
      if (b->catchBall(this, 1))
	{
	  // I know it's ugly, but it's late...
	  int row = 25 * (1 - team_id_);
	  if (pos_.row == row)
	    r_->touchdown();
	  return 0;
	}
				
      return 1;
    }
	
  b->bounce();
  return 1;
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

  MsgBallPos mesg;
  mesg.row = b->getPosition().row;
  mesg.col = b->getPosition().col;
  r_->sendPacket(mesg);

  SPlayer* p = f_->getPlayer(b->getPosition());
  if (p != NULL&&p->getStatus() == STA_STANDING)
    b->catchBall(p, catch_mod);
  else
    b->bounce();
  
  // The ball checks if a player of the current team catch it
	
  return 0;
}

void SPlayer::finishBlock(bool reroll)
{
  MsgBlockResult msg(team_id_);
  if (nb_dice_ == 1)
    msg.choose_team_id =  - 1;
  else if (choose_block_)
    msg.choose_team_id = team_id_;
  else
    msg.choose_team_id = r_->getCurrentOpponentTeamId();

  msg.player_id = id_;
  msg.opponent_id = target_->id_;
  msg.nb_dice = nb_dice_;

  if (reroll)
    {
      for (int i = 0; i < nb_dice_; ++i)
	{
	  result_[i] = (enum eBlockDiceFace)d_->roll("finishblock", DBLOCK);
	  LOG5("Rolled block dice: " << Dice::stringify(result_[i]));
	  msg.results[i] = result_[i];
	}
    }
  else
    {
      for (int i = 0; i < nb_dice_; ++i)
	{
	  msg.results[i] = result_[i];
	}
    }

  msg.reroll = false;
  r_->sendPacket(msg);
  if (nb_dice_ == 1)
    {
      t_->state_ = team_id_ == 0 ? GS_COACH1 : GS_COACH2;
      resolveBlock(0, target_);
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
      MsgPlayerStatus pkt(team_id_);
      pkt.player_id = id_;
      pkt.status = new_status;
      r_->sendPacket(pkt);
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
  MsgPlayerKO msg;
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
      msg.player_id = id_;
      msg.dice = dice;
      r_->sendPacket(msg);
      if (dice > 3)
	setStatus(STA_RESERVE);
      break;

    default:
      WARN("case not handled...");
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
  LOG6("[" << id_ << "] Amor Passed, Injury : " << injury << ".");
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

void SPlayer::msgMove(const ActMove* m)
{
  if (!t_->canDoAction(m, this, (enum eAction)m->action))
    return;
  if (doMove(m))
    r_->turnOver();
}

void SPlayer::msgStandUp(const ActStandUp* m)
{
  if (!t_->canDoAction(m, this, (enum eAction)m->action))
    return;
  doStandUp(m);
}

void SPlayer::msgBlock(const ActBlock* m)
{
  if (!t_->canDoAction(m, this, (enum eAction)m->action))
    return;
  if (doBlock(m))
    r_->turnOver();
}

void SPlayer::msgPass(const ActPass* m)
{
  if (!t_->canDoAction(m, this, PASS))
    return;
  if (doPass(m))
    r_->turnOver();
}



bool SPlayer::filterMove(const ActMove* m)
{
  if (m->client_id != team_id_ || m->player_id != id_)
    return false;
  return true;
}

bool SPlayer::filterStandUp(const ActStandUp* m)
{
  if (m->client_id != team_id_ || m->player_id != id_)
    return false;
  return true;
}

bool SPlayer::filterBlock(const ActBlock* m)
{
  if (m->client_id != team_id_ || m->player_id != id_)
    return false;
  return true;
}

bool SPlayer::filterPass(const ActPass* m)
{
  if (m->client_id != team_id_ || m->player_id != id_)
    return false;
  return true;
}
