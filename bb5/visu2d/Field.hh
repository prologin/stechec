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

# include "VirtualScrollableSurface.hh"
# include "Sprite.hh"

BEGIN_NS(sdlvisu);

class Game;

/*!
** Game field (on the left).
*/
class VisuField : public VirtualScrollableSurface
{
public:
  VisuField(Game& g);
  virtual ~VisuField();

  //! @brief Check if the mouse is inside the visible game field.
  //! @return @c true if the mouse is inside one game square.
  bool mouseInsideField() const;
  //! @brief Convert mouse coordinate to square coordinate.
  //! @return Square coordinate (between 0 and ROWS/COLS).
  Point mouseToSquare() const;
  //! @brief Convert a square position to a field position, where to place Surface.
  //! @param pt A square coordinate (between 0 and ROWS/COLS).
  //! @param adjust Field position adjustement, when you want to center object on a square.
  Point squareToField(const Point& pt, const Point& adjust = Point(0, 0)) const;

  void setMarker(const Point& square, int type);
  void removeMarker();
  
  void setBallPos(const Point& pos);

  bool getDrawTicks() const;
  void setDrawTicks(bool enable);
  
  virtual void update();

private:

  //! @brief Draw markers around squares.
  void drawTicks();
  
  Game&         g_;
  
  Surface       bg_;
  Sprite        ball_;
  Sprite	cross_black_;
  Sprite	cross_red_;

  bool	        draw_ticks_;

  Surface	red_highlight_;
  Surface	blue_highlight_;
};

END_NS(sdlvisu);

#endif /* !GAMEFIELD_HH_ */
