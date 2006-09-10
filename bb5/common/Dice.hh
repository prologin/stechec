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

# include "PacketHandler.hh"
# include "Constants.hh"

/*!
** @file dice.hh
** @ingroup rules
*/
//@{

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

//! @brief Faces of block dice.
enum eBlockDiceFace {
  BATTAKER_DOWN,
  BBOTH_DOWN,
  BPUSHED,
  BDEFENDER_STUMBLE,
  BDEFENDER_DOWN
};

DECLARE_PACKET(MSG_CHEATDICE, MsgCheatDice)
  int next_result;
END_PACKET

class BaseRules;

/*!
** @brief Classe représentant un dé à X faces.
*/
class Dice
{
 public:
  //! @brief Constructeur
  //! @param t type du de a fabriquer
  Dice(BaseRules* r);
  ~Dice();

  //! @brief Pour effectuer un lancer de plusieurs des.
  //! @param x Nombre de des a lancer.
  //! @return Resultat cumule des X des.
  int roll(enum eDiceFaceNumber type = D6, int nb_dice = 1);

  //! @brief Same as roll(x), but print 'msg' as debug to
  //!   see why this dice is rolled, and can use some previous
  //!   cheating results given instead of 'rand'.
  int roll(const std::string& msg, enum eDiceFaceNumber type = D6, int nb_dice = 1);


  static const char* stringify(enum eBlockDiceFace face);
  static const char* stringify(enum eRoll roll);
  
private:
  void msgCheatDice(const MsgCheatDice* m);
  
  std::deque<int> cheat_dice_;
};

//@}

#endif // !_DICE_HH_
