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

SPlayer::SPlayer(SRules* r, const MsgPlayerInfo* m, STeam* t)
  : Player(m->player_id, m->client_id),
    r_(r),
    t_(t),
    target_push_(NULL)
{
  r_->HANDLE_F_WITH(ACT_MOVE, SPlayer, this, msgMove, filterMove, GS_COACHBOTH);
  r_->HANDLE_F_WITH(ACT_BLOCK, SPlayer, this, msgBlock, filterBlock, GS_COACHBOTH);
  r_->HANDLE_F_WITH(ACT_PASS, SPlayer, this, msgPass, filterPass, GS_COACHBOTH);
  
  ma_ = m->ma;
  st_ = m->st;
  ag_ = m->ag;
  av_ = m->av;
  name_ = packetToString(m->name);
  setStatus(STA_STANDING);
  will_prone_ = false;
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
  SField* f = r_->getField();

  if (f->intoField(pos_))
    f->setPlayer(pos_, NULL);
  if (advertise_client && pos_ != pos)
    {
      MsgPlayerPos pkt;
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
  
  if (!f->intoField(pos))
    rollInjury(0);
  else
    f->setPlayer(pos, this);
}

// Standard action. Launch a D6:
//  - 1 -> always fails
//  - 6 -> always succeed
//  - else -> apply modifier and compare with agility.
bool SPlayer::tryAction(int modifier)
{
  Dice d(6);
  int dice_res = d.roll();
  int dice_modif = dice_res + modifier;
  int required = 7 - std::min(ag_, 6);
  if (dice_res != 6 && (dice_modif < required || dice_res == 1))
    {
      LOG6("[" << id_ << "] Action _failed_, dice: " << dice_res
           << ", w/modifiers: " << dice_modif << ", required: " << required
           << ", ag: " << ag_ << ".");
      return false;
    }
  LOG6("[" << id_ << "] Action succeed, dice: " << dice_res
       << ", w/modifiers: " << dice_modif << ", required: " << required
       << ", ag: " << ag_ << ".");
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
  // Check the player is standing
  if (status_ != STA_STANDING)
    {
      LOG4("Move: not in a standing position");
      r_->sendIllegal(ACT_MOVE, m->client_id);
      return 0;
    }
    
  ActMove res_move(team_id_);
  SField* f = r_->getField();
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
      int nb_tackles_pos = f->getNbTackleZone(r_->getCurrentOpponentTeamId(), pos_);
      int nb_tackles_aim = f->getNbTackleZone(r_->getCurrentOpponentTeamId(), aim);
      if (nb_tackles_pos > 0)
        {
          if (tryAction(1 - nb_tackles_aim))
            {
              LOG5("Player has successfully dodged out from " << pos_ << ".");
            }
          else
            {
              LOG5("Player has been knocked out from " << pos_ << ".");
              knocked = true;
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
            picking_failed = !b->catchBall(this, 1);
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
  if (knocked)
    {
      checkArmor(0, 0);
      if (status_ == STA_STANDING)
        setStatus(STA_PRONE);
      MsgPlayerKnocked pkt(m->client_id);
      pkt.player_id = id_;
      r_->sendPacket(pkt);
      return 1;
    }
  if (picking_failed)
    return 1;
  return 0;
}

/*
** Block action
*/

int SPlayer::doBlock(const ActBlock* m)
{
  int other_team_id = m->client_id == 0 ? 1 : 0;
  SPlayer* target = r_->getTeam(other_team_id)->getPlayer(m->opponent_id);
  if (target == NULL
      || target->getTeamId() == getTeamId()
      || target->status_ != STA_STANDING
      || !pos_.isNear(target->getPosition()))
  {
    LOG3("Cannot block player '" << other_team_id << "' at "
	 << target->getPosition() << " (status: " << target->status_ << ").");
    return 0;
  }

  int mod_st_atk = getSt();
  int mod_st_df = target->getSt();
  int nb_dice;

  if (mod_st_atk > 2 * mod_st_df || mod_st_df > 2 * mod_st_atk)
    nb_dice = 3;
  else if (mod_st_atk != mod_st_df)
    nb_dice = 2;
  else
    nb_dice = 1;

  Dice d(DBLOCK);
  enum eBlockDiceFace result[3];
  enum eBlockDiceFace best_result = BATTAKER_DOWN;
  for (int i = 0; i < nb_dice; ++i)
    {
      result[i] = (enum eBlockDiceFace)d.roll();
      LOG5("Rolled block dice: " << Dice::getBlockDiceString(result[i]));
      if (result[i] > best_result)
	best_result = result[i];
    }
  
  switch (best_result)
    {
    case BATTAKER_DOWN:
      setStatus(STA_PRONE);
      return 1;

    case BBOTH_DOWN:
      target->setStatus(STA_PRONE);
      setStatus(STA_PRONE);
      return 1;

    case BPUSHED:
      blockPushChoice(target);
      return 0;

    case BDEFENDER_STUMBLE:
      blockPushChoice(target);
      target->setStatus(STA_PRONE);
      return 0;
      
    case BDEFENDER_DOWN:
      blockPushChoice(target);
      target->setStatus(STA_PRONE);
      return 0;
    }
  return 0;
}


void SPlayer::blockPushChoice(SPlayer* target)
{
  SField* f = r_->getField();
  Position choice[3];
  Position dt = target->getPosition();
  Position d = dt - getPosition();

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

  ActBlockPush pkt;
  pkt.target_row = dt.row;
  pkt.target_col = dt.col;
  pkt.nb_choice = 0;

  for (int i = 0; i < 3; i++)
    if (f->intoField(choice[i]) && f->getPlayer(choice[i]) == NULL)
      {
	pkt.choice[pkt.nb_choice].row = choice[i].row;
	pkt.choice[pkt.nb_choice].col = choice[i].col;
	pkt.nb_choice++;
      }

  // If there is no choice, add all possibilities.
  if (pkt.nb_choice == 0)
    {
      for (int i = 0; i < 3; i++)
	{
	  pkt.choice[pkt.nb_choice].row = choice[i].row;
	  pkt.choice[pkt.nb_choice].col = choice[i].col;
	  pkt.nb_choice++;
	}
    }

  target_push_ = target;
  // FIXME: may follow up move (then use setPosition).
  f->setPlayer(dt, NULL);
  
  // FIXME: asks player, instead of doing the choice ourself.
  //r_->sendPacket(pkt);
  int my_choice = Dice(pkt.nb_choice).roll();
  pkt.nb_choice = 1;
  pkt.choice[0].row = pkt.choice[my_choice - 1].row;
  pkt.choice[0].col = pkt.choice[my_choice - 1].col;

  LOG3("pkt choices: " << pkt.nb_choice << " " << pkt.choice[my_choice - 1].row
       << " " << pkt.choice[my_choice - 1].col);

  blockPush(&pkt);
}

// FIXME: keep choices into memory, to check if user is
// not sending us falsified choice.
void SPlayer::blockPush(ActBlockPush* m)
{
  SField* f = r_->getField();
  Position to(m->choice[0].row, m->choice[0].col);
  SPlayer* other_target = f->getPlayer(to);

  if (m->nb_choice != 1)
    return;

  LOG2("blockpush 2nd phase: " << to);
  
  // Kludge, don't override last pos field.
  target_push_->pos_ = Point(-1, -1);
  // Kludge, remove field warning.
  f->setPlayer(to, NULL);

  // Finally, it's over. Move to dest.
  target_push_->setPosition(to);

  // Oh, another player to move.
  if (other_target != NULL)
    target_push_->blockPushChoice(other_target);
  else
    target_push_ = NULL;
}

/*
** Pass action.
*/

int SPlayer::doPass(const ActPass* m)
{
  SField* f = r_->getField();
  SBall* b = r_->getBall();

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
      r_->sendIllegal(ACT_PASS, m->client_id);
      return 0;
    }

  int nb_tackles = f->getNbTackleZone(r_->getCurrentOpponentTeamId(), pos_);
  int modifier = dist_modifier - nb_tackles;
  int catch_mod = 0;

  if (tryAction(modifier))
    catch_mod = 1; // accurate pass

  b->setPosition(dest);
  SPlayer* p = f->getPlayer(b->getPosition());
  if (!b->catchBall(p, catch_mod))
    {
      return 1; // Turnover
    }
  return 0;
}


/*
** Player state, injuries...
*/

void SPlayer::setStatus(enum eStatus new_status)
{
  switch (new_status)
    {
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
      
    case STA_INJURIED:
      status_ = STA_INJURIED;
      break;
      
    case STA_SEVERE_INJURIED:
      status_ = STA_SEVERE_INJURIED;
      break;
      
    case STA_DEAD:
      status_ = STA_DEAD;
      break;
      
    case STA_UNASSIGNED:
      WARN("Can't set player in 'unassigned' state");
      break;

    default:
      LOG3("You can't set this state from outside...");
      break;
    }
  MsgPlayerStatus pkt(team_id_);
  pkt.player_id = id_;
  pkt.status = status_;
  r_->sendPacket(pkt);
}

void SPlayer::setProne()
{
  if (will_prone_)
    setStatus(STA_PRONE);
}

void SPlayer::checkArmor(int av_mod, int inj_mod)
{
  Dice d(D6);
  if (d.roll(2) + av_mod > av_)
    rollInjury(inj_mod);
}

void SPlayer::rollInjury(int inj_mod)
{
  Dice d(D6);

  int injury = d.roll(2) + inj_mod;
  if (injury <= 7)
    setStatus(STA_STUNNED);
  else if (injury <= 9) 
    {
      setStatus(STA_KO);
      r_->getField()->setPlayer(pos_, NULL);
    }
  else 
    {
      setStatus(rollCasualty());
      r_->getField()->setPlayer(pos_, NULL);
    }
    LOG6("[" << id_ << "] Amor Passed, Injury : " << injury << ".");
}

enum eStatus SPlayer::rollCasualty()
{
  switch (Dice(D6).roll())
    {
    case 1:
    case 2:
    case 3: return STA_INJURIED;
    case 4:
    case 5: return STA_SEVERE_INJURIED;
    case 6: return STA_DEAD;
    }
  return STA_UNASSIGNED;
}



/*
** Messages, filters
*/

void SPlayer::msgMove(const ActMove* m)
{
  if (!t_->canDoAction(m, this))
    return;
  if (doMove(m))
    r_->turnOver();
}

void SPlayer::msgBlock(const ActBlock* m)
{
  if (!t_->canDoAction(m, this))
    return;
  if (doBlock(m))
    r_->turnOver();
  has_done_block_ = true;
}

void SPlayer::msgPass(const ActPass* m)
{
  if (!t_->canDoAction(m, this))
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
