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
** @ingroup rules
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
 
  /**
  ** @name Actions
  ** Following methods are all possible actions.
  ** Remember that even if they return success, an action
  ** may or may not be accepted later by the server.
  **
  */
  //@{

  void          doEndTurn();
  void          doMoveTurnMarker();
  void          doAskIllegalProcedure();
	bool 					doReroll();
	bool 					doAccept();
  bool          doPlaceBall(const Point& pos);
	bool          doGiveBall(int p);
	bool					doChooseBlockDice(int n);
	bool          doBlockPush(int n);
	bool          doFollow(bool follow);
  void          sendChatMessage(const std::string& msg);

	bool doMovePlayer(int p, const Point& to);
	bool doBlitzMovePlayer(int p, const Point& to);
	bool doPassMovePlayer(int p, const Point& to);

	bool doMoveStandUpPlayer(int p);
	bool doBlockStandUpPlayer(int p);
	bool doBlitzStandUpPlayer(int p);
	bool doPassStandUpPlayer(int p);

	bool doBlockPlayer(int p, const Point& to);
	bool doBlitzBlockPlayer(int p, const Point& to);

	bool doPassPlayer(int p, const Point& to);

  //@}

  /**
  ** @name Game informations
  ** Following methods are all accessors... Feel free to call
  ** them whenever you want. It's cheap !
  */
  //@{

  //! @brief Grab your team id.
  //! @return A team identifiant, in the range [0, 1].
  int                   myTeamId() const;
  //! @brief Get ball X position.
  //! @brief Return the ball X position, -1 if outside.
  int                   ballX() const;
  //! @brief Get ball Y position.
  //! @brief Return the ball Y position, -1 if outside.
  int			ballY() const;

  
  //! @brief Select team to fetch information from.
  //!   Further call to all other API function will return
  //!   information for that team.
  //! @param team_id 0 or 1 to select by team identifiant,
  //!   @c US to select your team and @c THEM to select the other team.
  //! @see eSelTeam
  void                  selectTeam(int team_id);

  //! @brief Select player to fetch information from.
  //!   Further call to all other API function will return
  //!   information for that player.
  //! @param player_id Player identifier, in the range [0, MAX_PLAYER - 1].
  //! @attention It is reset when selectTeam is called.
  int                   selectPlayer(int player_id);

  //! @brief Length that a move will take to the specified destination
  //!   for the for selected player.
  //! @param dst_x X coordinate of destination.
  //! @param dst_y Y coordinate of destination.
  //! @return The number of case the selected player will have to do,
  //!   or 0 if that move can't be made.
  int                   moveLength(int dst_x, int dst_y);

  //! @brief Difficulty to get (FIXME) in/outside a square for a specified step.
  //! @param step The current step (starting from 0 to move_lenght - 1) of the
  //!   current movement projection.
  //! @return The number of tackles zone for this step.
  //! @note You have to make a successful call to moveLength before.
  int                   moveDifficulty(int step);

  //! @brief Where the player will be on the field at this step.
  //! @param step The current step (starting from 0 to move_lenght - 1) of the
  //!   current movement projection.
  //! @return X coordinate on the field.
  //! @note You have to make a successful call to moveLength before.
  int                   movePathX(int step);

  //! @brief Where the player will be on the field at this step.
  //! @param step The current step (starting from 0 to move_lenght - 1) of the
  //!   current movement projection.
  //! @return Y coordinate on the field.
  //! @note You have to make a successful call to moveLength before.
  int                   movePathY(int step);

  //! @brief Determine if a move is possible for the selected player to
  //!   the specified destination.
  //! @param dst_x X coordinate of destination.
  //! @param dst_y Y coordinate of destination.
  //! @return 1 if this move is possible, 0 else.
  int                   movePossible(int dst_x, int dst_y);

  //@}


  /**
  ** @name Debug information
  ** Accessors that are not scrictly needed for the game, but
  ** may be useful for debugging or getting string about particular
  ** states.
  **
  ** There is mainly a rest of methods that expose the entire C* object
  ** for the UI. It was not that I wanted first, but saved me a lot of
  ** typing :)
  **
  ** These function may or may not be accessible through C interface.
  */
  //@{
  
  const char*		getGameStateString() const;
  const std::string&    getCoachName() const;
  const std::string&    getTeamName() const;

  const CTeam*          getTeam() const;
  const CPlayer*        getPlayer(int player_id) const;
  const CPlayer*        getPlayer(const Point& pos) const;
  const Weather*        getWeather() const;
  const CField*         getField() const;

  //@}
  
private:
  CTeam*                selected_team_;
  CPlayer*              selected_player_;
  const PosList*        player_path_;
};

# include "Api.hxx"

#endif /* !API_HH_ */
