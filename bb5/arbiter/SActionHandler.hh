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

#ifndef SACTIONHANDLER_HH_
# define SACTIONHANDLER_HH_

# include "Constants.hh"

class SPlayer;
class SRules;

/*!
** @brief Elementary action handler for the server.
** @ingroup tbt_server_rules
**
** It memorizes the sequence of pending elementary actions.
**
** You will have only 1 instance of this class at runtime,
** handled by the SRules and SPlayer classes.
*/
class SActionHandler
{
public:
  SActionHandler(SRules* r);
  virtual ~SActionHandler();

  bool isEmpty() const;
  SPlayer* getPlayer() const;
  enum eRoll getRollType() const;

  //! @brief Adds an elementary action to the stack.
  void putArmourRoll(SPlayer* p, int av_mod = 0, int inj_mod = 0);
  void putBallBounce();
  void putBlockBothDownDefender(SPlayer* p);
  void putBlockBothDownAttacker(SPlayer* p);
  void putBlockDefenderStumble(SPlayer* p);
  void putBlockDiceChoice(SPlayer* p);
  void putBlockFollowChoice(SPlayer* p);
  void putBlockPushChoice(SPlayer* p);
  void putBlockRoll(SPlayer* p);
  void putCatchBallRoll(SPlayer* p, bool success);
  void putDodgeRoll(SPlayer* p, bool success);
  void putMove(SPlayer* p, Position aim);
  void putPickUpRoll(SPlayer* p, bool success);
  void putPushResolution(SPlayer* p);
  void putStandUpRoll(SPlayer* p, bool success);
  void putThrowRoll(SPlayer* p, int success);

  //! @brief Processes elementary actions from the stack.
  void process(bool reroll = false, int choice = -1);

private:
  enum eElementaryAction {
    EA_BALL,          ///< Bounce the ball.
    EA_BLOCKBDD,      ///< Choose to either use or ignore Block skill for the defender.
    EA_BLOCKBDA,      ///< Choose to either use or ignore Block skill for the attacker.
    EA_BLOCKDICE,     ///< Choose a block dice.
    EA_BLOCKFOLLOW,   ///< Choose to follow or not after a block-push.
    EA_BLOCKPUSH,     ///< Choose where to push.
    EA_BLOCKSTUMBLE,  ///< Choose to either use or ignore Dodge skill during a block.
    EA_MOVE,          ///< Move a player.
    EA_PUSH,          ///< Apply push decisions.
    EA_ROLL,          ///< Apply dice roll, or roll dice.
  };
  struct ElementaryAction {
    enum eElementaryAction type;
    enum eRoll roll_type;
    SPlayer* player;
    int success;
    int modifier;
    int next_mod;
    Position aim_position;
    /* Still unused:
    enum eSkill skill;
    enum eStatus status;
    SPlayer* target_player;
    */
  };
  typedef std::vector<ElementaryAction> ElementaryActionList;

  void put(ElementaryAction ea);

  SRules* r_;
  ElementaryActionList eal_;
};

#endif /* !SACTIONHANDLER_HH_ */
