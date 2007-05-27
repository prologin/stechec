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
** @file bb5/common/Ball.hh
** @ingroup rules
*/

# include "PacketHandler.hh"

/*!
** @struct MsgBallPos
** @brief Ball Position.
** Client -> Server, only during a kick-off.
** Server -> Client, at any time of the game.
*/
DECLARE_PACKET(MSG_BALLPOS, MsgBallPos)
  int row;
  int col;
END_PACKET

/*!
** @struct MsgGiveBall
** @brief Give the Ball to a Player.
** Client -> Server, after a touchback.
*/
DECLARE_PACKET(MSG_GIVEBALL, MsgGiveBall)
  int player_id; ///< Player of receiving team who gets the ball back.
END_PACKET;

//! @brief Scatter template.
//! @note It is slightly different from BB's template.
//!  123
//!  8B4
//!  765
enum eScatterDirection {
  N = 2,
  S = 6,
  E = 4,
  W = 8,
  NE = 3,
  NW = 1,
  SE = 5,
  SW = 7,
};


/*!
** @brief Represents the Blood Bowl Ball.
** @ingroup rules
*/
template <typename T>
class Ball
{
public:
  Ball();
  virtual ~Ball();

  //! @brief Get the ball position.
  const Position& getPosition() const;
  //! @brief Set the ball position.
  void  setPosition(const Position& pt);

  //! @brief Returns the ball's owner, NULL if nobody.
  T* getOwner() const;
  //! @brief Sets the ball owner.
  void setOwner(T* player);

protected:
  Position pos_; ///< The ball position on the field.
  T* owner_; ///< Ball's owner, or NULL if nobody.
};

# include "Ball.hxx"

#endif // !_BALL_HH_
