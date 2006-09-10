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

#include "tools.hh"
#include "BaseRules.hh"
#include "Dice.hh"

Dice::Dice(BaseRules* r)
{
  srand((unsigned)time(0));

  r->HANDLE_WITH(MSG_CHEATDICE, Dice, this, msgCheatDice, GS_ALL);
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
        case 1: val=BATTAKER_DOWN; break;
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

int Dice::roll(const std::string& msg, enum eDiceFaceNumber type, int nb_dice)
{
  int res;

  if (cheat_dice_.empty())
    {
      res = roll(type, nb_dice);
      LOG3("+ Roll " << nb_dice << " D" << type << ": `" << res << "' (" << msg << ")");
    }
  else
    {
      res = cheat_dice_.front();
      cheat_dice_.pop_front();
      LOG3("+ Cheat " << nb_dice << " D" << type << ": `" << res << "' (" << msg << ")");
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
    case BATTAKER_DOWN:
      return "BATTAKER_DOWN";
    case BBOTH_DOWN:
      return "BBOTH_DOWN";
    case BPUSHED:
      return "BPUSHED";
    case BDEFENDER_STUMBLE:
      return "BDEFENDER_STUMBLE";
    case BDEFENDER_DOWN:
      return "BDEFENDER_DOWN";
    }
  return "kikoolol";
}

const char* Dice::stringify(enum eRoll roll)
{
  switch (roll)
    {
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
  return "kikoolol";
}
