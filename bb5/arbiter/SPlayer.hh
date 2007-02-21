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

#ifndef SPLAYER_HH_
# define SPLAYER_HH_

# include "Dice.hh"
# include "Team.hh"
# include "Player.hh"

class SRules;
class STeam;
typedef Field<SPlayer> SField;

class SPlayer : public Player
{
public:
  SPlayer(SRules* r, const MsgPlayerCreate* m, STeam* t);

  //! @brief Check some thing, if this player could be valid.
  bool acceptPlayerCreation();

  //! @brief Set player position.
  //! @note Use this function instead of setting pos_ directly, so
  //!   the field can be updated too.
  //! @param pos Where to move.
  //! @param advertise_client Whether or not send a MSG_PLAYERPOS to clients.
  void setPosition(const Position& pos, bool advertise_client = true);

  //! @brief Set new player status.
  void setStatus(enum eStatus new_status);

  //! @brief Set status to prone if he has to
  void setProne();
  
  //! @brief player goes in reserve if he can.
  //   KO players try to regain consciousness
  void prepareKickoff();

  //! @brief Try some action, consider player's agility.
  bool tryAction(int modifier);

  //! @brief Finish the current action
  void finishAction(bool reroll);

  //! @brief Apply block dice result
  void resolveBlock(int chosen_dice);

  //! @brief Apply blockpush choice
  void blockPush(const MsgBlockPush* m);

  //! @brief Apply follow or not decision.
  void follow(bool follow);

  enum eRoll action_attempted_;
  bool reroll_enabled_;
  int nb_dice_;
  SPlayer* pusher_; ///< Player who pushes you.

  void msgDeclare(const MsgDeclare* m);
  void msgMove(const MsgMove* m);
  void msgStandUp(const MsgStandUp* m);
  void msgBlock(const MsgBlock* m);
  void msgPass(const MsgPass* m);

private:

  void sendRoll(int result, int modifier, int required);

  //! @brief Move this player.
  //! @return non-zero if action failed.
  int doMove(const MsgMove* m);
  //! @brief Stand up this player.
  void doStandUp(const MsgStandUp* m);
  //! @brief Block a player.
  //! @return non-zero if action failed.
  int doBlock(const MsgBlock* m);
  //! @brief Do a pass.
  //! @return non-zero if action failed.
  int doPass(const MsgPass* m);

  void blockPushChoice(SPlayer* target);
  
  void blockFollow();

  int finishMove(bool reroll);
  void finishStandUp(bool reroll);
  int finishPickUp(bool reroll, bool pickup);
  int finishThrow(bool reroll);
  void finishBlock(bool reroll);
  
  //! @brief Check for armor and eventually injury.
  void checkArmor(int arMod, int injMod);
  //! @brief Roll for injury.
  void rollInjury(int injMod);
  //! @brief Roll for casualty.
  enum eStatus rollCasualty();

  Position aim_; ///< Where the player'd like to go to.

  bool choose_block_;
  enum eBlockDiceFace result_[3];

  //! @brief Push next player if there is no other player behind.
  void blockPush(int chosen_square);
  Position push_choices_[3];
  int nb_push_choices_;

  bool target_knocked_;

  SRules* r_; ///< Server rules.
  STeam* t_; ///< Player's team.
  SField* f_; ///< Game field (same as SRules' one, it is used quite frequently).
  Dice* d_; ///< The dice (same remark).
  
  SPlayer* target_; ///< Player that is the target of a block or a push.

};


#endif /* !SPLAYER_HH_ */
