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
class SPlayerMsg;
class SActionHandler;

/*!
** @brief Player implementation for the server.
** 
** It symbolizes player and valids and applies action requests.
**
** You will have up to 32 instances of this class at runtime,
** handled by the STeam and SPlayerMsg class.
*/
class SPlayer : public Player
{
public:
  SPlayer(SRules* r, const MsgPlayerCreate* m, STeam* t, SPlayerMsg* pm);

  //! @brief Checks some things, if this player could be valid.
  bool acceptPlayerCreation();

  bool canUseSkillReroll() const;
  bool canUseAnyReroll() const;
  //! @brief Checks if the player scores a touchdown,
  //!   and eventually declares the touchdown.
  bool checkAndDeclareTouchdown();

  //! @brief Sets player position.
  //! @note Use this function instead of setting pos_ directly, so
  //!   the field can be updated too.
  //! @param pos Where to move.
  //! @param advertise_client Whether or not send a MSG_PLAYERPOS to clients.
  void setPosition(const Position& pos, bool advertise_client = true);

  //! @brief Sets new player status.
  void setStatus(enum eStatus new_status);
  //! @brief Sets status to prone if he has to.
  void setProne();
  //! @brief Goes in the reserve if he can.
  void prepareKickoff();

  //! @brief Checks for armour value and eventually injury.
  void checkArmour(int av_mod, int inj_mod);
 
  void considerBlockDices(bool reroll);
  //! @brief Applies block dice result
  void resolveBlockDice(int chosen_dice);

  void tryBlockPush(SPlayer* target);
  void setPusher(SPlayer* p);
  //! @brief Applies blockpush choice
  //! @brief Push next player if there is no other player behind.
  void resolveBlockPush(int chosen_square);
  //! @brief Considers if player can follow or not, during a block action.
  //! @param p Player to be pushed in an empty square or out of the field.
  void considerBlockFollow();
  //! @brief Applies follow or not decision.
  void blockFollow(bool follow);
  void finishBlockPush();
  void finishBlockAction();
  void bePushedInTheCrowd();

  //! @brief Tries to catch the ball.
  void tryCatchBall(bool accurate_pass);
  void finishCatchBall(bool reroll, bool success);
  //! @brief Tries to dodge.
  void tryDodge();
  void finishDodge(bool reroll, bool success);
  //! @brief Tries to move to an adjacent square.
  void tryMove(Position& aim);
  //! @brief Tries to pick up the ball.
  void tryPickUp();
  void finishPickUp(bool reroll, bool success);
  //! @brief Tries to stand up.
  void tryStandUp();
  void finishStandUp(bool reroll, bool success);
  //! @brief Tries to throw the ball.
  void tryThrow();
  void finishThrow(bool reroll, int success);

  // Message handlers.
  void msgPlayerPos(const MsgPlayerPos* m);
  void msgDeclare(const MsgDeclare* m);
  void msgMove(const MsgMove* m);
  void msgStandUp(const MsgStandUp* m);
  void msgBlock(const MsgBlock* m);
  void msgPass(const MsgPass* m);
  void msgSkill(const MsgSkill* m);

  int nb_block_dice_;

private:

  //! @brief Rolls for injury.
  void rollInjury(int modifier);
  //! @brief Rolls for casualty.
  enum eStatus rollCasualty();

  //! @brief Rolls and compares to player's agility.
  bool rollAgility(enum eRoll roll_type, int modifier);
  bool rollAgility();

  void rollCatchBall();
  void rollDodge();
  void rollPickUp();
  void rollStandUp();
  void rollThrow();

  //! @brief Block a player.
  //! @return non-zero if action failed.
  void tryBlock();
  void rollBlock();

  Position push_choices_[3];
  int nb_push_choices_;
  int strongest_team_id_;
  bool choose_block_;
  enum eBlockDiceFace result_[3];
  bool target_knocked_;
  bool follow_;

  Position move_aim_; ///< Where the player'd like to go to.
  enum eRoll roll_attempted_;
  enum eSkill skill_concerned_; ///< Skill related to action attempted.
  bool reroll_enabled_;
  int modifier_; ///< Roll modifier.
  Position throw_aim_;
  float distance_;

  SRules* r_; ///< Server rules.
  STeam* t_; ///< Player's team.
  SField* f_; ///< Game field (same as SRules' one, it is used quite frequently).
  Dice* d_; ///< The dice (same remark).
  SPlayerMsg* pm_;
  SActionHandler* ah_;

  SPlayer* pusher_; ///< Player who is pushing.
  SPlayer* target_; ///< Player that is the target of a block or a push.
};


#endif /* !SPLAYER_HH_ */
