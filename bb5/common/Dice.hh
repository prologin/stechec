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

#ifndef _DICE_HH_
# define _DICE_HH_

/*!
** @file dice.hh
*/

// FIXME: to move to arbiter. Client rules doesn't need it.

//! @brief Some Dice number of face.
enum eDiceFaceNumber {
  D2 = 2,
  D3,
  D4,
  DBLOCK,
  D6,
  D8 = 8,
  D10 = 10,
  D12 = 12,
  D16 = 16,
  D66 = 666 // :)
};

enum eBlockDiceFace {
  BATTAKER_DOWN,
  BBOTH_DOWN,
  BPUSHED,
  BDEFENDER_STUMBLE,
  BDEFENDER_DOWN
};

//DECLARE_PACKET(MSG_ROLLINFO, MsgRollInfo)
//    int dice_result[3]; //maximum 3 result dices
//    bool rerollable;
//END_PACKET

/*!
** Classe représentant un dé à X faces.
*/
class Dice
{
 public:
  //! @brief Constructeur
  //! @param t type du de a fabriquer
  Dice(int t = D6);
  ~Dice();

  //! @brief Pour effectuer un lancer de plusieurs des.
  //! @param x Nombre de des a lancer.
  //! @return Resultat cumule des X des.
  int roll(int x = 1);

  int roll(bool reroll, int x = 1);

private:
  int type_; 
};

#endif // !_DICE_HH_
