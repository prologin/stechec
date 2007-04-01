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
** @ingroup rules
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
 
  /*!
  ** @name Game informations
  ** Following methods are all accessors... Feel free to call
  ** them whenever you want. It's cheap !
  */
  //@{

  //! @brief Get the current turn.
  //! @return The current turn in the range of [1, 8], or 0
  //!   if in other state (kickoff, ...)
  int                   turn() const;

  //! @brief Get the current half.
  //! @return The current half.
  int                   half() const;
 
  //! @brief Grab your team id.
  //! @return A team identifiant, in the range [0, 1].
  int                   myTeamId() const;
  //! @brief Get ball position.
  //! @return Return the ball position, (-1, -1) if outside.
  Point                 ball() const;

  //! @brief Get the stringified version of game status.
  //!  Mainly useful for debugging.
  //! @return The current game status.
  const char*           gameStateString() const;

  //! @brief Get the player team id at the specified position.
  //! @param pos A position on the field.
  //! @return A team id, or -1 if there is nobody at this position.
  int                   teamId(const Point& pos);
  
  //! @brief Select team to fetch information from.
  //!   Further call to all other API function will return
  //!   information for that team.
  //! @param team_id 0 or 1 to select by team identifiant,
  //!   @c US to select your team and @c THEM to select the other team.
  //! @see eSelTeam
  void                  selectTeam(int team_id);
  //! @brief Get the selected team.
  //! @return The selected team, or NULL if none was chosen.
  const CTeam*          getTeam() const;

  //! @brief Select player to fetch information from.
  //!   Further call to all other API function will return
  //!   information for that player.
  //! @param player_id Player identifier, in the range [0, MAX_PLAYER - 1].
  //! @attention It is reset when selectTeam is called.
  int                   selectPlayer(int player_id);
  
  //! @brief Get the player id at the specified position.
  //! @param pos A position on the field.
  //! @return A player id, or -1 if there is nobody at this position.
  int                   playerId(const Point& pos);

  //! @brief Get the selected player.
  //! @return The selected player.
  const CPlayer*        getPlayer() const;
  //! @brief Get a player, of the selected team.
  //! @param player_id The identifiant of this player.
  //! @return The chosen player, or NULL if it doesn't exists.
  const CPlayer*        getPlayer(int player_id);
  //! @brief Get a player on the field.
  //! @param pos A position on the field
  //! @return The player standing in this field position, or NULL if there
  //!  is nobody.
  const CPlayer*        getPlayer(const Point& pos);
  
  //! @brief Get the number of possible action for the selected player.
  //! @fixme NOT IMPLEMENTED
  //! @return The number of possible action
  int                   actionPossibleNumber() const;

  //! @brief Get a possible action for the selected player.
  //! @param index Action index, in the range [0 - actionPossibleNumber() - 1].
  //! @fixme NOT IMPLEMENTED
  //! @return Action that this player may perform.
  int                   actionPossible(int index) const;

  //! @brief Length that a move will take to the specified destination
  //!   for the for selected player.
  //! @param to Coordinate of destination.
  //! @return The number of case the selected player will have to do,
  //!   or 0 if that move can't be made.
  int                   moveLength(const Point& to);

  //! @brief Difficulty to get (FIXME) in/outside a square for a specified step.
  //! @param step The step (starting from 0 to move_length - 1) of the
  //!   current movement projection.
  //! @return The number of tackles zone for this step.
  //! @note You have to make a successful call to moveLength before.
  int                   moveDifficulty(int step);

  //! @brief Where the player will be on the field at this step.
  //! @param step The current step (starting from 0 to move_length - 1) of the
  //!   current movement projection.
  //! @return Position on the field.
  //! @note You have to make a successful call to moveLength before.
  Point                 movePath(int step);

  //! @brief Determine if a move is possible for the selected player to
  //!   the specified destination.
  //! @param to Coordinate of destination.
  //! @return 1 if this move is possible, 0 else.
  int                   movePossible(const Point& to);

  //! @brief Get the current weather.
  //! @note FIXME: currently disabled.
  const Weather*        getWeather() const;
  
  //@}


  /*!
  ** @name Actions
  ** Following methods are all possible actions.
  **
  ** Functions can return SUCCESS, or any error code defined in
  ** common/Constants.hh
  **
  ** Remember that even if they return success, an action
  ** may or may not be accepted later by the server.
  */
  //@{

  //! @brief End the current turn.
  void          doEndTurn();

  //! @brief Move the turn marker.
  void          doMoveTurnMarker();

  //! @brief Ask for an illegal procedure.
  void          doAskIllegalProcedure();

  //! @brief Choose to kick off or receive.
  //! @param kickoff
  //!   @c true if coach want to kick off.
  //!   @c false if coach want to receive.
  int           doChooseKickoff(bool kickoff);

  //! @brief Place the selected player, before a kick off.
  //! @note If there is already a player on this square, he will be placed in the reserve.
  //! @param pos Position to place the player.
  //!   @c (-1,-1) to place the player in the reserve.
  //!     Any other coordinates out of team's zone do the same.
  int           doPlacePlayer(const Point& pos);

  //! @brief End team's placement, before a kick off.
  int           doEndPlacement();

  //! @brief Place the ball, on the kick off.
  //! @param pos Position to place the ball.
  int           doPlaceBall(const Point& pos);

  //! @brief Give the ball to one of your player, if the ball goes out of field
  //!   on kick off.
  //! @param p Player id to give the ball.
  int           doGiveBall(int p);

  //! @brief Reroll or accept the last dice roll.
  //! @param reroll @c true if coach want to reroll,
  //!   @c false to accept the result.
  int           doReroll(bool reroll);

  //! @brief Declare an action for the selected player.
  //! @param action Action to declare.
  int           doDeclare(enum eDeclaredAction action);

  //! @brief After a block action, choose which block dice to use.
  //! @note Wait for the right server reponse before calling it.
  //! @param n A number between 1 and 3, depending on the previous
  //!  returned server message.
  int           doChooseBlockDice(int n);

  //! @brief After a block action, push the defender to the selected square.
  //! @note Wait for the right server reponse before calling it.
  //! @param n A number between 1 and 3, depending on the previous
  //!  returned server message.
  int           doBlockPush(int n);

  //! @brief After a block action, choose to follow the defender.
  //! @note Wait for the right server reponse before calling it.
  //! @param follow @c true if following is wanted, else @c false.
  int           doFollow(bool follow);

  //! @brief Stand up the selected player.
  //! @note Declare a DCL_MOVE or DCL_BLITZ or DCL_PASS action before.
  int           doStandUpPlayer();

  //! @brief Move the selected player.
  //! @note Declare a DCL_MOVE or DCL_BLITZ or DCL_PASS action before.
  int           doMovePlayer(const Point& to);

  //! @brief Block with the selected player.
  //! @note Declare a DCL_BLOCK or DCL_BLITZ action before.
  //! @param def_p The defender id, on the other team.
  int           doBlockPlayer(int def_p);

  //! @brief Throw the ball to the specified position.
  //! @note Declare a DCL_PASS action before.
  //! @param to Where to throw the ball.
  int           doPassPlayer(const Point& to);

  //! @brief Choose the next dice result.
  //! @param roll Next result to obtain, between 1 and number of dice faces.
  void          doCheatDice(int roll);

  //! @brief Send a chat message. Can be called every time.
  //! @param msg Message to send.
  void          sendChatMessage(const std::string& msg);

  //@}


private:
  CTeam*                selected_team_;
  CPlayer*              selected_player_;
  const PosList*        player_path_;
};

# include "Api.hxx"

#endif /* !API_HH_ */
