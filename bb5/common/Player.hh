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

# include "PacketHandler.hh"
# include "Constants.hh"

DECLARE_PACKET(MSG_PLAYERINFO, MsgPlayerInfo)
  int player_id;
  int ma;
  int st;
  int ag;
  int av;
  int name[8];
END_PACKET

DECLARE_PACKET(MSG_PLAYERPOS, MsgPlayerPos)
  int player_id;
  int row;
  int col;
END_PACKET

DECLARE_PACKET(MSG_PLAYERKNOCKED, MsgPlayerKnocked);
  int player_id;
END_PACKET


/*!
** Base class describing a player.
**
*/
class Player
{
public:
  Player(int id, int team_id);
  virtual ~Player();

  int getId() const;
  int getTeamId() const;
  const Position& getPosition() const;
  const std::string& getName() const;

  int getMa() const;
  int getSt() const;
  int getAg() const;
  int getAv() const;

  bool hasDoneAction() const;
  void resetTurn();

  friend std::ostream& operator<< (std::ostream& os, const Player& p);

protected:
  int         id_;      ///< Player unique identifier.
  int         team_id_; ///< Team that this player belongs to (0 or 1).
  Position    pos_;     ///< Position on the field. Read-only, please.
  std::string name_;    ///< Player name.
  int         status_;  ///< current player status (standing/prone/Stunned/KO...).

  int ma_;
  int st_;
  int ag_;
  int av_;

  int ma_remain_;
  bool action_done_;
};

# include "Player.hxx"

#endif /* !PLAYER_HH_ */
