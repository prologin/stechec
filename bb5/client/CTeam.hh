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

#ifndef CTEAM_HH_
# define CTEAM_HH_

# include "CPlayer.hh"
# include "Team.hh"

class CRules;

/*!
** @brief Team implementation on client side
*/
class CTeam : public Team<CPlayer>
{
public:
  CTeam(int team_id, CRules *r);
  virtual ~CTeam();

  //! @brief Load team configuration from xml.
  void loadConfig(const std::string& team_file);

  //! @brief Load player configuration from xml.
  void loadPlayerConfig(xml::XMLTeam& team, int player_id);

  //! @brief Set the position of all players on the field.
  //! @param formation_id The xml formation id to fetch position from.
  //! @throw XMLError Thrown on any xml error, including if the player position
  //!   can't be found in the xml file.
  void placeTeam(int formation_id);

  //! @brief Move a player to a position (as far as player can go)
  //!  Called from the UI.
  bool movePlayer(int player_id, const Position& to, enum eAction action);

  //! @brief Stand up a player
  //!  Called from the UI.
  bool standUpPlayer(int player_id, enum eAction action);

  //! @brief Do a block.
  //! @param player_id Player doing the block.
  //! @param opponent Player to block.
  //! @param action Action to do.
  bool blockPlayer(int player_id, CPlayer* opponent, enum eAction action);
  
  //! @brief Do a pass.
  //! @param player_id Player doing the pass.
  //! @param to The pass destination.
  bool passPlayer(int player_id, const Position& to);
	
  //! @brief Get player
  //! @note Temporary, wait for API cleaning.
  const CPlayer* getPlayerConst(int id) const;
  
private:
  void msgTeamInfo(const MsgTeamInfo* m);
  void msgPlayerCreate(const MsgPlayerCreate* m);
  void msgReroll(const MsgReroll* m);

  bool filterTeamInfo(const MsgTeamInfo* m);
  bool filterPlayerCreate(const MsgPlayerCreate* m);
  bool filterReroll(const MsgReroll* m);

  CRules* r_;
  xml::XMLTeam xml_team_;
  xml::XMLFormation xml_formation_;
};

inline const CPlayer* CTeam::getPlayerConst(int id) const
{
  if (id >= 0 && id < MAX_PLAYER)
    return player_[id];

  LOG2("Wrong player_id: %1 (team: %2)", id, team_id_);
  return NULL;
}


#endif /* !CTEAM_HH_ */
