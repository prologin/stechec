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

  //! @brief Checks if the player scores a touchdooown,
  //!   and eventually declares the touchdooown.
  bool checkAndDeclareTouchdooown();

  //! @brief Sets player position.
  //! @note Use this function instead of setting pos_ directly,
  //!   so the field can be updated too.
  //! @param pos Where to move.
  //! @param advertise_client Whether or not send a MSG_PLAYERPOS to clients.
  void setPosition(const Position& pos, bool advertise_client = true);
  //! @brief Sets player status and sends a MSG_STATUS to clients.
  //! @note Use this function instead of setting pos_ directly,
  //!   so the field can be updated too.
  //! @param new_status New status, it has to be different from the current one.
  void setStatus(enum eStatus new_status);
  //! @brief Sets status to prone if he has to.
  //! @note Call this function at the end of his team turn.
  void setProne();
  //! @brief Goes in the reserve if he can.
  void prepareKickoff();

  //! @brief Sets the given skill as the only usable skill.
  void setUsableSkill(enum eSkill skill);

  //! @brief Checks for armour value and eventually injury.
  void checkArmour(int av_mod, int inj_mod);
  //! @brief Rolls for injury and sets player status.
  void bePushedInTheCrowd();
 
  //! @brief Rerolls the block dices if needed, otherwise gives the
  //!   choice (if any) to a coach for which block dice to apply.
  void considerBlockDices(bool reroll);
  //! @brief Applies block dice roll result.
  void resolveBlockDice(int chosen_dice);
  //! @brief Applies BBOTH_DOWN block dice roll result.
  void resolveBlockBothDownDefender(bool block);
  //! @brief Applies BBOTH_DOWN block dice roll result.
  void resolveBlockBothDownAttacker(bool block);
  //! @brief Applies BDEFENDER_STUMBLE block dice roll result.
  void resolveBlockDefenderStumble(bool dodge);
  //! @brief Sets pusher of this player.
  void setPusher(SPlayer* p);
  //! @brief Tries to push the targetted player to a square. Eventually
  //!   gives a coach his choice of which square to push the player to.
  void tryBlockPush(SPlayer* target);
  //! @brief Selects chosen square to push the targetted player.
  void resolveBlockPush(int chosen_square);
  //! @brief Eventually gives the coach his choice to follow or not after a block push.
  void considerBlockFollow();
  //! @brief Registers follow decision and moves to pushes application.
  void finishBlockFollow(bool follow);
  //! @brief Applies pushes decisions, beginning by the last pushed player.
  void finishBlockPush();
  //! @brief Applies follow decision, eventually knocks down the defender.
  void finishBlockAction();

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

  // Message handlers called by SPlayerMsg.
  void msgPlayerPos(const MsgPlayerPos* m);
  void msgDeclare(const MsgDeclare* m);
  void msgMove(const MsgMove* m);
  void msgStandUp(const MsgStandUp* m);
  void msgBlock(const MsgBlock* m);
  void msgPass(const MsgPass* m);
  void msgSkill(const MsgSkill* m);

private:

  //! @brief Checks if a team reroll or a skill reroll
  //!   is available, according to the roll attempted.
  void setRerollAvailability();
  //! @brief Sets the list of usable skills,
  //!   according to the roll attempted.
  void setUsableSkills();
  void setSkillAvailability(enum eSkill skill);
  //! @brief Checks if the given skill can be used now.
  bool canUseSkill(enum eSkill skill) const;
  //! @brief Gets one of the usable skills if any.
  //FIXME: This will have to be deleted for extra rules,
  // because up to three skills could be usable for the same roll.
  enum eSkill getUsableSkill() const;

  //! @brief Rolls a six-sided dice, compares the result to
  //!   player's agility, and sends MSG_RESULT to clients.
  bool rollAgility(enum eRoll roll_type, int modifier);
  bool rollAgility();

  //! @brief Rolls for injury.
  void rollInjury(int modifier);
  //! @brief Rolls for casualty.
  enum eStatus rollCasualty();

  //! @brief Tries to block the targetted player.
  void tryBlock();
  //! @brief Rolls block dices, according to modifiers.
  void rollBlock();

  void rollCatchBall();
  void rollDodge();
  void rollPickUp();
  void rollStandUp();
  void rollThrow();

  int nb_block_dices_;
  Position push_choices_[3];
  int nb_push_choices_;
  int strongest_team_id_;
  bool choose_block_;
  enum eBlockDiceFace result_[3];
  bool target_knocked_;
  bool follow_;

  Position move_aim_; ///< Where the player'd like to go to.
  enum eRoll roll_attempted_;
  SkillList usable_skills_; ///< Skills which can be used.
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
