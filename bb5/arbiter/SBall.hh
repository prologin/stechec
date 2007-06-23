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

#ifndef SBALL_H_
# define SBALL_H_

# include "Ball.hh"

class SPlayer;
class SRules;

/*!
** @brief Ball for the server.
** @ingroup tbt_server_rules
**
** It symbolizes the ball, checks all ball manipulations.
**
** You will have only one instance of this class at runtime,
** handled by SRules class.
*/
class SBall : public Ball<SPlayer>
{
public:
  SBall(SRules* r);

  //! @brief Sets ball position and advertises the client if it changed.
  //! @note Use this function instead of setPosition(pos), if you
  //!   want to advertise the client in the same time.
  //! @param pos Where to place the ball.
  //! @param advertise_client Whether or not send a MSG_BALLPOS to clients.
  void setPosition(const Position& pos, bool advertise_client = true);

  //! @brief Sets ball owner and advertises the client if it has changed.
  void setOwner(SPlayer* p);

  //! @brief Removes the ball from the field.
  //! @note Use it before a kickoff.
  void removeFromField();

  //! @brief Bounces the ball around.
  void bounce(int nb = 1);
  //! @brief Scatters the ball in a random direction.
  //! @param nb Number of squares covered.
  void scatter(int nb);

  //! @brief Makes spectators throw the ball in the field.
  void throwIn();
  void setThrown();
  bool hasBeenThrown();

  void resetTurn();

  void sendPosition();

private:
  //! @brief Makes land the ball on the field.
  //! @param delta Direction.
  //! @param amplitude Distance.
  void afterBounce(const Position& delta, int amplitude);

  void msgPlaceBall(const MsgBallPos* m);
  void msgGiveBall(const MsgGiveBall* m);
  bool invalidKickoffDestination(const Position& pos) const;
  void touchback();
  
  SRules* r_;
  bool thrown_;
};

#endif /* !SBALL_H_ */
