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

#ifndef CPLAYER_HH_
# define CPLAYER_HH_

# include "Player.hh"

class CRules;

/*!
** Player class for client.
*/
class CPlayer : public Player
{
public:
  CPlayer(CRules* r, const MsgPlayerInfo* m);
  virtual ~CPlayer();

  //! @brief Move the player on the field.
  bool move(const Position& to);
	
	//! @brief Stand up the player.
  bool standUp();

  //! @brief Block the player at the specified position.
  bool block(const Position& to);
  
  //! @brief Set the player position.
  void setPosition(const Position& pos);
  
  //! @brief Substract dep to ma_remain_.
  void subMa(int dep);

private:
  CRules* r_;

  // Message from server.
  void msgPlayerPos(const MsgPlayerPos* m);
  void msgPlayerMove(const ActMove* m);
  void msgPlayerKnocked(const MsgPlayerKnocked* m);
  void msgPlayerStatus(const MsgPlayerStatus* m);

  // Filter messages. Say yes if this message is for this player.
  bool filterPlayerPos(const MsgPlayerPos* m);
  bool filterPlayerMove(const ActMove* m);
  bool filterPlayerKnocked(const MsgPlayerKnocked* m);
  bool filterPlayerStatus(const MsgPlayerStatus* m);
};

#endif /* !CPLAYER_HH_ */
