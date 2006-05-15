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

/*! @brief Blood Bowl Ball for the server.
**
** @author victor
** @date 30/12/2005
*/
class SBall : public Ball
{
public:
  SBall(SRules* r);

  static void scatter(Position& pos, int nb=1);
  //! @brief Bounce the ball around.
  void bounce(int nb=1);
  //! @brief throwin in case the ball leave the field.
  void throwin(const Position& pos, const Position& dest);


private:
  void msgPlaceBall(const MsgBallPos* m);
  void moveDelta(const Position& delta);

  SRules* r_;
};
  
#endif /* !SBALL_H_ */
