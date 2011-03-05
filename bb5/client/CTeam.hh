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

#ifndef CTEAM_HH_
# define CTEAM_HH_

# include "CPlayer.hh"
# include "Team.hh"
# include "xml/xml_team.hh"
# include "xml/xml_formation.hh"

class CRules;
class CPlayerMsg;

/*!
** @brief Team implementation on client side
** @ingroup tbt_client_api
*/
class CTeam : public Team<CPlayer>
{
public:
  CTeam(int team_id, CRules *r, CPlayerMsg *pm);
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

  //! @brief Declare an action for the player
  //!  Called from the UI.
  int declareAction(CPlayer* p, enum eDeclaredAction action);

  // Called by CTeamMsg.
  void msgTeamInfo(const MsgTeamInfo* m);
  void msgPlayerCreate(const MsgPlayerCreate* m);
  void msgReroll(const MsgReroll* m);
  void msgTouchdooown(const MsgTouchdooown* m);

private:
  CRules* r_;
  CPlayerMsg* pm_;
  xml::XMLTeam xml_team_;
  xml::XMLFormation xml_formation_;
};

#endif /* !CTEAM_HH_ */
