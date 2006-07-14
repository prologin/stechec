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
  CPlayer(int id, int team_id, CRules* r);
  virtual ~CPlayer();

  //! @brief Load player configuration from xml.
  void loadConfig(xml::XMLTeam& team);

  //! @brief Get player information.
  void msgPlayerInfo(const MsgPlayerInfo* m);

  //! @brief Move the player on the field.
  bool move(const Position& to);

  //! @brief Set the player position.
  void setPosition(const Position& pos);

private:
  CRules* r_;
};

#endif /* !CPLAYER_HH_ */
