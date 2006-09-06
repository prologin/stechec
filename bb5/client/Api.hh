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

#ifndef API_HH_
# define API_HH_

# include "CRules.hh"
# include "BaseApi.hh"

/*!
** @brief User interface for graphical interface, AI, etc.
**
** This class is really the "bridge" between the "coach" (whether it
** would be a human or a computer) and the rules engine.
**
** For all methods asking for an action (eg. moving player), remember
** that even if they return success, an action may or may not be
** accepted later by the server.
*/
class Api : public BaseApi<CRules>
{
public:
  Api(CRules* rules);
  ~Api();
 
  /*
  ** Following methods are all possible actions.
  ** Remember that even if they return success, an action
  ** may or may not be accepted later by the server.
  */
  void          doEndTurn();
  void          doMoveTurnMarker();
  void          doAskIllegalProcedure();
  bool          doPlaceBall(const Point& pos);
	bool          doGiveBall(int p);
  bool          doMovePlayer(int p, const Point& to);
  bool          doStandUpPlayer(int p);
  bool					doBlockPlayer(int p, const Point& to);
  void          sendChatMessage(const std::string& msg);

  /*
  ** Following methods are all accessors... Feel free to call
  ** them whenever you want. It's cheap !
  */

  //! @brief Select team to fetch information from.
  //!   Further call to all other API function will return
  //!   information from that team.
  //! @see eSelTeam
  void                  select_team(int team_id);

  const char*		getStateString() const;
  const std::string&    getCoachName() const;
  const std::string&    getTeamName() const;
  const CTeam*          getTeam() const;
  int                   getTeamId() const;
  const CPlayer*        getPlayer(int player_id) const;
  const CPlayer*        getPlayer(const Point& pos) const;
  const Weather*        getWeather() const;
  Point                 getBallPosition() const;
  const CField*         getField() const;

  
  int                   select_player(int player_id);
  int                   move_lenght(int dst_x, int dst_y);
  int                   move_difficulty(int step);
  int                   move_path_x(int step);
  int                   move_path_y(int step);
  int                   move_possible(int dst_x, int dst_y);
  
private:
  CTeam*                selected_team_;
  CPlayer*              selected_player_;
  const PosList*        player_path_;
};

# include "Api.hxx"

#endif /* !API_HH_ */
