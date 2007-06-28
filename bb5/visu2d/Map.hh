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

#ifndef GAMEFIELD_HH_
# define GAMEFIELD_HH_

# include "Constants.hh"
# include "Sprite.hh"
# include "VirtualScrollableSurface.hh"

BEGIN_NS(sdlvisu);

class Game;
class VisuPlayer;

/*!
** @brief Game map (on the left).
** @ingroup tbt_2d_gui
**
** It includes field and dugout.
*/
class Map : public VirtualScrollableSurface
{
public:
  Map(Game& g);
  virtual ~Map();

  //! @brief Check if the mouse is inside the visible game stadium.
  //! @return @c true if the mouse is inside game stadium.
  bool mouseInsideMap() const;
  //! @brief Check if the mouse is inside the visible game field.
  //! @return @c true if the mouse is inside one game square.
  bool mouseInsideField() const;
  //! @brief Convert mouse coordinate to square coordinate.
  //! @return Square coordinate (between 0 and ROWS/COLS).
  Point mouseToSquare() const;
  //! @brief Convert a square position to a map position, where to place Surface.
  //! @param pt A square coordinate (between 0 and ROWS/COLS).
  //! @param adjust Map position adjustement, when you want to center object on a square.
  Point squareToMap(const Point& pt, const Point& adjust = Point(0, 0)) const;

  void setMarker(const Point& square, int type);
  void removeMarker();
  
  void setBallPos(const Point& pos);
  void removeBall();

  /*!
  ** @brief Place a player at the given square position.
  **
  ** Use it instead of VisuPlayer::setPos() to take count of dugouts map management.
  ** @param vp Player to place into the field.
  ** @param pos Square position (between 0 and ROwS/COLS).
  */
  void placePlayer(VisuPlayer* vp, Position& pos);
  /*!
  ** @brief Remove a player from the field and place him in a dugout, according to his status.
  **
  ** Use it instead of VisuPlayer::setPos() to take count of dugouts map management.
  ** @param vp Player to remove from the field.
  ** @param s Player status.
  */
  void removePlayer(VisuPlayer* vp, enum eStatus s);

  bool getDrawTicks() const;
  void setDrawTicks(bool enable);
  
  virtual void update();

private:

  //! @brief Draw markers around squares.
  void drawTicks();
  
  Game&         g_;
  
  Surface       bg_;
  Sprite        ball_;
  Sprite        cross_black_;
  Sprite        cross_red_;

  bool          draw_ticks_;

  Surface       red_highlight_;
  Surface       blue_highlight_;
};

END_NS(sdlvisu);

#endif /* !GAMEFIELD_HH_ */
