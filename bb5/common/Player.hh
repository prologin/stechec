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

#ifndef PLAYER_HH_
# define PLAYER_HH_

# include "Constants.hh"
# include "Field.hh"
# include "PacketHandler.hh"

//! Maximum number of moves for a single MsgMove packet.
//! @ingroup tbt_rules
const int MAX_MOVE = 16;

//! Maximum number of skills for a single MsgPlayerCreate packet.
//! @ingroup tbt_rules
const int MAX_SKILL = 16;

/*!
** @struct MsgDeclare
** @brief Declare a Player action.
** @ingroup tbt_rules
** Client <-> Server.
*/
DECLARE_PACKET(MSG_DECLARE, MsgDeclare)
  int player_id; ///< Player beginning an action.
  int action;    ///< Declared action. @see #eDeclaredAction.
END_PACKET;

/*!
** @struct MsgMove
** @brief A Player moves of one or several squares.
** @ingroup tbt_rules
** Client <-> Server.
*/
DECLARE_PACKET(MSG_MOVE, MsgMove)
  int player_id; ///< Moving Player.
  int nb_move;   ///< Length of the move.
  struct {
    int row;
    int col;
  } moves[MAX_MOVE];
END_PACKET;

/*!
** @struct MsgStandUp
** @brief A Player stands up.
** @ingroup tbt_rules
** Client <-> Server.
*/
DECLARE_PACKET(MSG_STANDUP, MsgStandUp)
  int player_id; ///< Player being prone.
END_PACKET;

/*!
** @struct MsgBlock
** @brief Player blocks an opponent.
** @ingroup tbt_rules
** Client <-> Server.
*/
DECLARE_PACKET(MSG_BLOCK, MsgBlock)
  int player_id;    ///< Attacker.
  int opponent_id;  ///< Defender.
END_PACKET;

/*!
** @struct MsgBlockPush
** @brief A Player pushes another Player, due to a block.
** @ingroup tbt_rules
** Client <-> Server.
*/
DECLARE_PACKET(MSG_BLOCKPUSH, MsgBlockPush)
  int target_row;
  int target_col;
  int nb_choice;
  struct {
    int row;
    int col;
  } choice[3];
  int square_chosen;
END_PACKET;

/*!
** @struct MsgMultiBlock
** @brief FIXME: NOT IMPLEMENTED. For extra rules.
** @ingroup tbt_rules
** Client <-> Server.
*/
DECLARE_PACKET(MSG_MULTIBLOCK, MsgMultiBlock)
  int player_id;
  int first_opponent_id;
  int second_opponent_id;
END_PACKET;

/*!
** @struct MsgPass
** @brief A Player throws the ball.
** @ingroup tbt_rules
** Client <-> Server.
*/
DECLARE_PACKET(MSG_PASS, MsgPass)
  int player_id;
  int dest_row;
  int dest_col;
END_PACKET;

/*!
** @struct MsgPlayerCreate
** @brief Create a Player at the very beginning of the game.
** @ingroup tbt_rules
** Client <-> Server.
*/
DECLARE_PACKET(MSG_PLAYERCREATE, MsgPlayerCreate)
  int player_id;
  int ma;
  int st;
  int ag;
  int av;
  int skill[MAX_SKILL]; ///< @see #eSkill.
  int skill_nb;
  int name[8];
  int position_name[8]; ///< Player position name (lineman, ...).
  int player_position;	///< Graphic info.
  int player_img[8];	///< Graphic info.
END_PACKET;

/*!
** @struct MsgPlayerPos
** @brief Player Position changed.
** @ingroup tbt_rules
** Client <-> Server.
*/
DECLARE_PACKET(MSG_PLAYERPOS, MsgPlayerPos)
  int player_id;  ///< Player id.
  int row;
  int col;
END_PACKET;

/*!
** @struct MsgPlayerKnocked
** @brief A Player is knocked down to the ground.
** @ingroup tbt_rules
** Client <-> Server.
*/
DECLARE_PACKET(MSG_PLAYERKNOCKED, MsgPlayerKnocked);
  int player_id;  ///< Knocked Player id.
END_PACKET;

/*!
** @struct MsgPlayerStatus
** @brief Player status changed.
** @ingroup tbt_rules
** Client <-> Server.
*/
DECLARE_PACKET(MSG_PLAYERSTATUS, MsgPlayerStatus);
  int player_id;  ///< Player id.
  int status;     ///< New status. @see #eStatus.
END_PACKET;

/*!
** @struct MsgPlayerKO
** @brief A KO'd Player tries to wakes up, during a break time.
** @ingroup tbt_rules
** Server -> Client.
*/
DECLARE_PACKET(MSG_PLAYERKO, MsgPlayerKO);
  int player_id;  ///< KO'd player trying to wake up.
  int dice;       ///< Dice result.
END_PACKET;

/*!
** @struct MsgResult
** @brief Dice roll result associated to a Player, often agility roll.
** @ingroup tbt_rules
** Server -> Client.
*/
DECLARE_PACKET(MSG_RESULT, MsgResult)
  int player_id;  ///< Player concerned by the roll.
  int roll_type;  ///< Roll type. @see #eRoll.
  int reroll;     ///< Whether the coach can use a team reroll or not.
  //! FIXME: Extra rules will imply up to three skills usable for a single dice roll.
  int skill;      ///< Usable skill. @see #eSkill.
  int result;     ///< Roll raw result.
  int modifier;   ///< Sum of modifiers.
  int required;   ///< Result required.
END_PACKET;

/*!
** @struct MsgBlockResult
** @brief Block dices roll result.
** @ingroup tbt_rules
** Server -> Client.
*/
DECLARE_PACKET(MSG_BLOCKRESULT, MsgBlockResult)
  int player_id;          ///< Attacker id.
  int opponent_id;        ///< Defender id.
  bool reroll;            ///< Attacker's coach can reroll.
  int strongest_team_id;  ///< Team id of the strongest Player, can choose which dice to use.
  int nb_dice;            ///< Number of block dices rolled.
  int results[3];         ///< Results of block dices rolls.
END_PACKET;

/*!
** @struct MsgBlockDice
** @brief Coach chooses a block dice roll result.
** @ingroup tbt_rules
** Client -> Server.
*/
DECLARE_PACKET(MSG_BLOCKDICE, MsgBlockDice)
  int dice; ///< Chosen block dice index.
END_PACKET;

/*!
** @struct MsgFollow
** @brief Attacker follows the defender after having pushed him.
** @ingroup tbt_rules
** Client <-> Server.
*/
DECLARE_PACKET(MSG_FOLLOW, MsgFollow)
  //! Whether or not follow after a block.
  //!   @c -1 Server gives the coach his choice to follow or not.
  //!   @c 0 Player doesn't follow his victim.
  //!   @c 1 Player follows his victim.
  int follow;
END_PACKET;

/*!
** @struct MsgSkill
** @brief A player use one of his skill.
** @ingroup tbt_rules
** Client <-> Server.
*/
DECLARE_PACKET(MSG_SKILL, MsgSkill)
  int player_id; ///< Skilled player.
  int skill;  ///< Skill concerned. @see #eSkill.
              ///< FIXME: Several skills may be used at the same time, in extra rules.
  //! Whether or not use the skill.
  //!   @c -1 Server gives the coach his choice of using or not the skill.
  //!   @c 0 Player player_id doesn't use the skill.
  //!   @c 1 Player player_id uses the skill.
  int choice;
END_PACKET;

/*!
** @brief Base class describing a BB player.
** @ingroup tbt_rules
*/
class Player
{
public:
  typedef std::vector<enum eSkill> SkillList;
  
  Player(const MsgPlayerCreate* m);
  virtual ~Player();

  int getId() const;
  int getTeamId() const;
  const Position& getPosition() const;
  const std::string& getName() const;
  const std::string& getPositionName() const;

  //! @brief Get Movement Allowance.
  int getMa() const;
  //! @brief Get Movement Allowance Remaining.
  int getMaRemain() const;
  //! @brief Get Strenght.
  int getSt() const;
  //! @brief Get Agility.
  int getAg() const;
  //! @brief Get Armour Value.
  int getAv() const;

  //! @brief Get player status.
  //! @see Constant.hh
  enum eStatus getStatus() const;

  //! @brief Check if the player has performed an action this turn.
  bool hasPlayed() const;
  //! @brief Set the 'has played this turn' marker.
  void setHasPlayed();
  //! @brief Check if the player has performed a block during a blitz action.
  bool hasBlocked() const;
  //! @brief Set the 'has blocked during this blitz action' marker.
  void setHasBlocked();

  //! @brief Return the action of this player.
  enum eDeclaredAction getAction() const;
  //! @brief Set the action.
  void setAction(enum eDeclaredAction action);

  //! @brief Check if this player has this skill.
  //! @param skill Skill to check for.
  bool hasSkill(enum eSkill skill) const;
  //! @brief Get the list of skills.
  //! @note Don't use this method in rules, you should better use @c hasSkill.
  const SkillList& getSkillList() const;
  //! @brief Check if this player has used this skill.
  //! @param skill Skill to check for.
  bool hasUsedSkill(enum eSkill skill) const;
  //! @brief Mark the skill as used for this turn.
  void useSkill(enum eSkill skill);

  //! @brief Called on new turn.
  void resetTurn();

  static const char* stringify(enum eStatus status);
  static const char* stringify(enum eDeclaredAction action);
  static const char* stringify(enum eRealAction action);
  static const char* stringify(enum eSkill skill);

  friend std::ostream& operator<< (std::ostream& os, const Player& p);

protected:
  int		id_;      ///< Player unique identifier.
  int		team_id_; ///< Team that this player belongs to (0 or 1).
  Position	pos_;     ///< Position on the field. Read-only, please.
  std::string	name_;    ///< Player name.
  std::string	position_name_; ///< Player position name (lineman, ...).
  enum eStatus	status_;  ///< Current player status (standing/prone/Stunned/KO...).

  int ma_;
  int st_;
  int ag_;
  int av_;

  int ma_remain_;
  bool has_blocked_;
  bool has_played_;
  enum eDeclaredAction action_;
  bool will_prone_;

private:
  SkillList	skill_list_; ///< List of skills that this player have.
  SkillList	used_skill_list_; ///< List of skills that this player have used this turn.
};

# include "Player.hxx"

#endif /* !PLAYER_HH_ */
