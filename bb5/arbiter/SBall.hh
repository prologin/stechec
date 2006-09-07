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

class SRules;
class SPlayer;

/*!
** @brief Blood Bowl Ball for the server.
*/
class SBall : public Ball
{
public:
  SBall(SRules* r);

  //! @brief Bounce the ball around.
  void bounce(int nb = 1);
  //! @brief Player try to catch the ball.
  bool catchBall(SPlayer *p, int modifier);
  //! @brief Return the ball owner, NULL if nobody
  SPlayer* getOwner();
	//! @brief remove ball from field (before a kickoff)
	void removeFromField();
	void scatter(int nb);
	void setThrown();

private:
  void msgPlaceBall(const MsgBallPos* m);
  void msgGiveBall(const MsgGiveBall* m);
  bool invalidBallPlacement();
  

  void afterBounce(const Position& delta, int amplitude);
  //! @brief throwin in case the ball leave the field.
  void throwin();

  SRules* r_;
  SPlayer* owner_; ///< Ball owner, or NULL if nobody
	bool thrown_;

};
  
#endif /* !SBALL_H_ */
