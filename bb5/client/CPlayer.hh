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
  CPlayer(CRules* r, const MsgPlayerCreate* m);
  virtual ~CPlayer();

  //! @brief Declare an action for the player.
  int declareAction(enum eAction action);

  //! @brief Move the player on the field.
  int move(const Position& to);
	
  //! @brief Stand up the player.
  int standUp();

  //! @brief Block the player at the specified position.
  int block(CPlayer* opponent);
	
  //! @brief Block the player at the specified position.
  int pass(const Position& to);
  
  //! @brief Set the player position.
  void setPosition(const Position& pos);
  
  //! @brief Substract dep to ma_remain_.
  void subMa(int dep);

  //! @brief Graphic info (x-shift of player in the picture).
  int getPlayerPosition() const;

  //! @brief Graphic info.
  const std::string& getPlayerPicture() const;

  
private:
  CRules* r_;

  // Message from server.
  void msgDeclareAction(const MsgDeclare* m);
  void msgPlayerPos(const MsgPlayerPos* m);
  void msgPlayerMove(const MsgMove* m);
  void msgPlayerKnocked(const MsgPlayerKnocked* m);
  void msgPlayerStatus(const MsgPlayerStatus* m);
  void msgPlayerKO(const MsgPlayerKO* m);

  // Filter messages. Say yes if this message is for this player.
  bool filterDeclareAction(const MsgDeclare* m);
  bool filterPlayerPos(const MsgPlayerPos* m);
  bool filterPlayerMove(const MsgMove* m);
  bool filterPlayerKnocked(const MsgPlayerKnocked* m);
  bool filterPlayerStatus(const MsgPlayerStatus* m);
  bool filterPlayerKO(const MsgPlayerKO* m);

  // for graphic UI.
  int player_position_;
  std::string player_picture_;
};

#endif /* !CPLAYER_HH_ */
