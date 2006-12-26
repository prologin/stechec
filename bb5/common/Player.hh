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

/*!
** @file bb5/common/Player.hh
** @ingroup rules
*/

# include "PacketHandler.hh"
# include "Constants.hh"
# include "Field.hh"

// Hard limit of move/skill number, for a single packet.
const int MAX_MOVE = 16;
const int MAX_SKILL = 16;

DECLARE_PACKET(ACT_DECLARE, ActDeclare)
  int player_id;
  int action;
END_PACKET

DECLARE_PACKET(ACT_MOVE, ActMove)
  int player_id;
  int nb_move;
  struct {
    int row;
    int col;
  } moves[MAX_MOVE];
END_PACKET

DECLARE_PACKET(ACT_STANDUP, ActStandUp)
  int player_id;
END_PACKET

DECLARE_PACKET(ACT_BLOCK, ActBlock)
  int player_id;
  int opponent_id;
END_PACKET

DECLARE_PACKET(ACT_BLOCKPUSH, ActBlockPush)
  int target_row;
  int target_col;
  int nb_choice;
  struct {
    int row;
    int col;
  } choice[3];
	int square_chosen;
END_PACKET

DECLARE_PACKET(ACT_MULTIBLOCK, Act_MultiBlock)
  int player_id;
  int target_location;
  int second_location;
END_PACKET

DECLARE_PACKET(ACT_PASS, ActPass)
  int player_id;
  int dest_row;
  int dest_col;
END_PACKET

// Packet sent only once at the beginning (to create player)
DECLARE_PACKET(MSG_PLAYERCREATE, MsgPlayerCreate)
  int player_id;
  int ma;
  int st;
  int ag;
  int av;
  int skill[MAX_SKILL];
  int skill_nb;
  int name[8];
  int position_name[8]; ///< Player position name (lineman, ...).
  int player_position;	///< Graphic info.
  int player_img[8];	///< Graphic info.
END_PACKET

DECLARE_PACKET(MSG_PLAYERPOS, MsgPlayerPos)
  int player_id;
  int row;
  int col;
END_PACKET

DECLARE_PACKET(MSG_PLAYERKNOCKED, MsgPlayerKnocked);
  int player_id;
END_PACKET

DECLARE_PACKET(MSG_PLAYERSTATUS, MsgPlayerStatus);
  int player_id;
  int status;
END_PACKET

DECLARE_PACKET(MSG_PLAYERKO, MsgPlayerKO);
  int player_id;
  int dice;
END_PACKET

DECLARE_PACKET(MSG_RESULT, MsgResult)
  int player_id;
  int roll_type;
  int result;
  int modifier;
  int required;
  int reroll;
END_PACKET

DECLARE_PACKET(MSG_BLOCKRESULT, MsgBlockResult)
  int choose_team_id;
  int player_id;
  int opponent_id;
  int nb_dice;
  int results[3];
  int reroll;
END_PACKET

DECLARE_PACKET(MSG_BLOCKDICE, MsgBlockDice)
  int dice;
END_PACKET

DECLARE_PACKET(MSG_FOLLOW, MsgFollow)
  int follow;
END_PACKET



/*!
** @ingroup rules
** @brief Base class describing a player.
**
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

  //! @brief Return the action of this player.
  enum eAction getAction() const;

  //! @brief Set the action.
  void setAction(enum eAction action);

  //! @brief Check if this player has this skill.
  //! @param skill Skill to check for.
  bool hasSkill(enum eSkill skill) const;

  //! @brief Get the list of skills.
  //! @note Don't use this method in rules, you should better use @c hasSkill.
  const SkillList& getSkillList() const;

  //! @brief Called on new turn.
  void resetTurn();

  static const char* stringify(enum eStatus status);
  static const char* stringify(enum eAction action);
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
  bool has_played_;
  enum eAction action_;
  bool will_prone_;

private:
  SkillList	skill_list_; ///< List of skills that this player have.
};

# include "Player.hxx"

#endif /* !PLAYER_HH_ */
