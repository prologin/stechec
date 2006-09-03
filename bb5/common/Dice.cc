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
#include "Dice.hh"

Dice::Dice(int type)
  : type_(type)
{
  static bool srand_done = false;

  if (!srand_done)
    {
      srand((unsigned)time(0));
      srand_done = true;
    }
}

Dice::~Dice()
{
}

int Dice::roll(int nb_dice)
{
  int val = 0;

  switch (type_)
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
          ERR("!!!!! DBLOCK dice weird error !!!! returned -> " << val);
          break;
        }
      break;
 
    default:
      while (nb_dice-- > 0)
        val += (1+(int) (((double)type_)*rand()/(RAND_MAX+1.0)));
      break;
    }
  return val;
}

const char* Dice::getBlockDiceString(enum eBlockDiceFace face)
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

int Dice::roll(bool reroll, int x)
{
  int result = roll(x);
  if (reroll)
    {
      // FIXME: ask current player for reroll
      //sendAcceptMessage(result);
    }
  return result;
}
