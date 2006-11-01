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

  void playerDoingKickoff();
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

  bool	        draw_ticks_;
};

END_NS(sdlvisu);

#endif /* !GAMEFIELD_HH_ */
