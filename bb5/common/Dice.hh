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

DECLARE_PACKET(MSG_CHEATDICE, MsgCheatDice)
  int next_result;
END_PACKET

class BaseRules;


//! @ingroup rules
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
  BATTACKER_DOWN = 1,
  BBOTH_DOWN,
  BPUSHED,
  BDEFENDER_STUMBLE,
  BDEFENDER_DOWN
};


/*!
** @brief Classe représentant un dé à X faces.
*/
class Dice
{
 public:
  //! @brief Dice contructor.
  //! @param r Pointer to Rules class.
  Dice(BaseRules* r);
  //! @brief Standart destructor.
  ~Dice();

  //! @brief Roll dices of selected type, and print a debug to
  //!   see which dice/result is used.
  //! @param msg A debug message, printed.
  //! @param type Type of dice(s) to roll.
  //! @param nb_dice Number of dice to roll.
  //! @return A (maybe :p) random number, in the specified range.
  //! @note if type == DBLOCK, only one dice can be rolled.
  //! @note Predefined results are used, if any.
  int roll(const std::string& msg, enum eDiceFaceNumber type = D6, int nb_dice = 1);

  //! @brief Get a stringified version of enum eBlockDiceFace.
  static const char* stringify(enum eBlockDiceFace face);
  //! @brief Get a stringified version of enum eRoll.
  static const char* stringify(enum eRoll roll);
  
private:
  //! @brief Pour effectuer un lancer de plusieurs des.
  //! @param x Nombre de des a lancer.
  //! @return Resultat cumule des X des.
  int roll(enum eDiceFaceNumber type = D6, int nb_dice = 1);

  void msgCheatDice(const MsgCheatDice* m);
  
  std::deque<int> cheat_dice_;
};

//@}

#endif // !_DICE_HH_
