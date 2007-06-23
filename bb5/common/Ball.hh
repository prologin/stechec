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

# include "Constants.hh"
# include "PacketHandler.hh"

/*!
** @struct MsgBallPos
** @brief Ball Position.
** @ingroup tbt_rules
** Client -> Server, only during a kick-off.
** Server -> Client, at any time of the game.
*/
DECLARE_PACKET(MSG_BALLPOS, MsgBallPos)
  int row;
  int col;
END_PACKET

/*!
** @struct MsgGiveBall
** @brief Changes Ball owner.
** @ingroup tbt_rules
** Client -> Server, after a touchback.
** Server -> Client, at any time of the game.
*/
DECLARE_PACKET(MSG_GIVEBALL, MsgGiveBall)
  int player_id; ///< Player of receiving team who gets the ball back.
END_PACKET;

/*!
** @brief Base class describing the ball.
** @ingroup tbt_rules
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
