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

#ifndef _CBALL_HH_
# define _CBALL_HH_

# include "Ball.hh"

class CRules;

class CBall : public Ball
{
public:
  CBall(CRules* rules);
  virtual ~CBall();

  //! @brief Set the ball position (request from the server).
  void setPosition(const MsgBallPos* m);

private:
  CRules* r_;
};


#endif /* !_CBALL_HH_ */
