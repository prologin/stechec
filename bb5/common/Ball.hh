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

#ifndef _BALL_HH_
# define _BALL_HH_

/*!
** @file common/Ball.hh
** @ingroup rules
*/

# include "PacketHandler.hh"

# define N 2
# define S 6
# define E 4
# define W 8
# define NE 3
# define NW 1
# define SE 5
# define SW 7

//! @brief Server -> Client. Send the position of the ball.
DECLARE_PACKET(MSG_BALLPOS, MsgBallPos)
  int row;
  int col;
END_PACKET

//! @brief Client -> Server. Send the owner of the ball.
DECLARE_PACKET(MSG_GIVEBALL, MsgGiveBall)
  int player_id;
END_PACKET


/*!
** @brief Represents the Blood Bowl Ball.
** @ingroup rules
*/
class Ball
{
public:
  Ball();
  virtual ~Ball();

  //! @brief Get the ball position.
  const Position& getPosition() const;
  //! @brief Set the ball position.
  void  setPosition(const Position& pt);

protected:
  Position pos_; ///< The ball position on the field.
};


inline Ball::Ball()
{
}

inline Ball::~Ball()
{
}

inline void Ball::setPosition(const Position& pt)
{
  pos_ = pt;
}

inline const Position& Ball::getPosition() const
{
  return pos_;
}

#endif // !_BALL_HH_
