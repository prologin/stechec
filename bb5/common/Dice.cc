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

Dice::Dice(BaseRules* r)
{
  srand((unsigned)time(0));

  r->HANDLE_WITH(MSG_CHEATDICE, Dice, this, msgCheatDice, 0);
}

Dice::~Dice()
{
}

int Dice::roll(enum eDiceFaceNumber type, int nb_dice)
{
  int val = 0;

  switch (type)
    {
    case D66:
      while (nb_dice-- > 0)
        val += (1+(int) (6.0*rand()/(RAND_MAX+1.0)))*10 + (1+(int) (6.0*rand()/(RAND_MAX+1.0)));
      break;

    case DBLOCK:
      // We want to throw only one dice, and return a valid
      // block dice value.
      // FIXME: throw an exception instead.
      if (nb_dice > 1)  
        return -42;

      val = 1 + (int)(6.0 * rand() / (RAND_MAX + 1.0));
      switch (val)
        {
        case 1: val=BATTACKER_DOWN; break;
        case 2: val=BBOTH_DOWN; break;
        case 3:
        case 4: val=BPUSHED; break;
        case 5: val=BDEFENDER_STUMBLE; break;
        case 6: val=BDEFENDER_DOWN; break;
        default:
	  return -42;
          break;
        }
      break;
 
    default:
      while (nb_dice-- > 0)
        val += (1+(int) (((double)type)*rand()/(RAND_MAX+1.0)));
      break;
    }
  return val;
}

int Dice::roll(enum eRoll bb_roll_type, enum eDiceFaceNumber type, int nb_dice)
{
  return roll(Dice::stringify(bb_roll_type), type, nb_dice);
}

int Dice::roll(const std::string& msg, enum eDiceFaceNumber type, int nb_dice)
{
  int res = 0;
  int nb_dice_start = nb_dice;

  if (cheat_dice_.empty())
    {
      res = roll(type, nb_dice);
      LOG3("+ Roll %1D%2: `%3' (%4)", nb_dice, type, res, msg);
    }
  else
    {
      // Cheat as many dices as we can.
      while (nb_dice-- > 0 && !cheat_dice_.empty())
	{
	  res += cheat_dice_.front();
	  cheat_dice_.pop_front();
	}
      if (nb_dice > 0)
	{
	  LOG3("+ Cheat some dices: %1D%2: temporary result: `%3' (%4)",
	       nb_dice_start - nb_dice, type, res, msg);
	  res += roll(type, nb_dice);
	  LOG3("  Roll remaining dices: %1D%2: `%3' (%4)", nb_dice, type, res, msg);
	}
      else
	LOG3("+ Cheat %1D%2: `%3' (%4)", nb_dice_start, type, res, msg);
    }
  return res;
}

void Dice::msgCheatDice(const MsgCheatDice* m)
{
  cheat_dice_.push_back(m->next_result);
}

const char* Dice::stringify(enum eBlockDiceFace face)
{
  switch (face)
    {
    case BATTACKER_DOWN:
      return "BATTACKER_DOWN";
    case BBOTH_DOWN:
      return "BBOTH_DOWN";
    case BPUSHED:
      return "BPUSHED";
    case BDEFENDER_STUMBLE:
      return "BDEFENDER_STUMBLE";
    case BDEFENDER_DOWN:
      return "BDEFENDER_DOWN";
    }
  return "bdf_kikoolol";
}

const char* Dice::stringify(enum eRoll roll)
{
  switch (roll)
    {
    case R_UNASSIGNED:
      return "R_UNASSIGNED";
    case R_DODGE:
      return "R_DODGE";
    case R_STANDUP:
      return "R_STANDUP";
    case R_PICKUP:
      return "R_PICKUP";
    case R_ARMOUR:
      return "R_ARMOUR";
    case R_INJURY:
      return "R_INJURY";
    case R_THROW:
      return "R_THROW";
    case R_CATCH:
      return "R_CATCH";
    case R_BLOCK:
      return "R_BLOCK";
    }
  return "r_kikoolol";
}
