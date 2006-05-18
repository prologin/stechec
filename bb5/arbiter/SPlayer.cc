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

SPlayer::SPlayer(int id, int team_id, SRules* r)
  : Player(id, team_id),
    r_(r)
{
}

// sig... wanted it to be on Player.
void SPlayer::setPosition(const Position& pos)
{
  SField* f = r_->getField();
  if (f->intoField(pos_))
    f->setPlayer(pos_, NULL);
  pos_ = pos;
  f->setPlayer(pos_, this);
}


void SPlayer::msgPlayerInfo(const MsgPlayerInfo* m)
{
  ma_ = m->ma;
  st_ = m->st;
  ag_ = m->ag;
  av_ = m->av;
  name_ = packetToString(m->name);

  // Ok, tell everybody we have accepted this player.
  r_->sendPacket(*m);
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
  int required = 6 - std::min(ag_, 6) + 1;
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

bool SPlayer::pickBall(int modifier)
{
  SField* f = r_->getField();
  SBall* b = r_->getBall();  

  assert(pos_ == b->getPosition());
  int nb_tackles = f->getNbTackleZone(r_->getCurrentOpponentTeamId(), pos_);
  if (!tryAction(modifier - nb_tackles))
    {
      LOG5("Player has failed to pick up ball at " << pos_);
      b->bounce();
      return false;
    }
  LOG5("Player successfully take the ball at " << pos_);
  return true;
}

int SPlayer::doMove(const ActMove* m)
{
  ActMove res_move(team_id_);
  SField* f = r_->getField();
  SBall* b = r_->getBall();  
  int illegal = 0;
  bool knocked = false;
  bool picking_failed = false;
  Position aim;

  res_move.player_id = m->player_id;
  res_move.nb_move = 0;
  for (int i = 0; i < m->nb_move; i++)
    {
      // Check that we can move here.
      aim.row = m->moves[i].row;
      aim.col = m->moves[i].col;
      if (!pos_.isNear(aim))
        {
          illegal = 1;
          break;
        }

      // Check tackle zones.
      int nb_tackles = f->getNbTackleZone(r_->getCurrentOpponentTeamId(), pos_);
      if (nb_tackles > 0)
        {
          if (tryAction(1 - nb_tackles))
            {
              LOG5("Player has successfully dodged out from " << pos_ << ".");
        }
          else
            {
              LOG5("Player has been knocked out from " << pos_ << ".");
              knocked = true;
            }
        }

      // Check if can pick the ball.
      if (b->getPosition() == aim)
        if (knocked)
          {
            picking_failed = true;
          }
        else
          {
            picking_failed = !pickBall(1);
          }

      // Ok... Move on.
      res_move.nb_move++;
      res_move.moves[i].row = aim.row;
      res_move.moves[i].col = aim.col;
      setPosition(aim);
      if (knocked || picking_failed)
        break;
    }

  // Send the results.
  if (res_move.nb_move > 0)
    r_->sendPacket(res_move);
  if (illegal)
    r_->sendIllegal(ACT_MOVE, m->client_id);
  if (knocked)
    {
      MsgPlayerKnocked pkt(m->client_id);
      pkt.player_id = id_;
      r_->sendPacket(pkt);
    }
  if (picking_failed)
    b->bounce();
  action_done_ = true;
  return 0;
}

int SPlayer::doBlock(const ActBlock* m)
{
  SPlayer* target = r_->getField()->getPlayer(m->target_location);
  if ((target == NULL)
      || (target->getTeamId() == getTeamId()) //prevent same team block??
      || (target->status_ != STA_STANDING))
  {
    LOG3("cannot block player from " << m->target_location << ".");
    return 0;
  }
  int mod_st_atk = getSt();
  int mod_st_df = target->getSt();
  int nb_dice = 0;
  if (mod_st_atk == mod_st_df)
    {
      nb_dice = 1;
    }
  else if ((mod_st_atk >= 2*mod_st_df))
    {
      nb_dice = 3;
    }
  else if ((mod_st_df >= 2*mod_st_atk))
    {
      nb_dice = -3;
    }
  else
    {
      nb_dice=(mod_st_atk>mod_st_df)?2:-2;
      int result[nb_dice];
      Dice d(DBLOCK);
      for (int i=0; i<nb_dice; ++i)
        {
          result[i] = d.roll();
          LOG5("rolled block dice "+ result[i]);
        }
    }
  // FIXME: apply results
  action_done_ = true;
  return 0;
}

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
  if (!p->pickBall(catch_mod))
    {
      while (true)
        {
          p = f->getPlayer(b->getPosition());
          if (p == NULL || p->pickBall(0))
            break;
        }
    }
  action_done_ = true;
  // FIXME: turnover
  return 0;
}

void SPlayer::checkArmor(int av_mod, int inj_mod)
{
  Dice d(D6);
  if (d.roll(2) + av_mod > av_)
    rollInjury(inj_mod);
}

//FIXME: setStatus
inline void SPlayer::rollInjury(int inj_mod)
{
  Dice d(D6);
  int injury = d.roll(2) + inj_mod;
  if (injury <= 7)
    status_ = STA_STUNED;
  else if (injury <= 9)
    status_ = STA_KO;
  else
    status_ = rollCasualty();
}

inline int SPlayer::rollCasualty()
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
}

// Explicit instanciation ? Hum :)
#include "Field.cc"
void foo()
{
  Field<SPlayer> f;
}
