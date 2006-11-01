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

#include "Api.hh"
#include "Game.hh"
#include "Field.hh"

BEGIN_NS(sdlvisu);

VisuField::VisuField(Game& g)
  : VirtualScrollableSurface("VVisuField", g.getInput(), Point(500, 600), Point(725, 1100)),
    g_(g),
    bg_("image/general/playground_0.jpg"),
    ball_("image/general/ball.png"),
    draw_ticks_(true)
{
  bg_.setZ(1);
  addChild(&bg_);
  addChild(&ball_);
  ball_.setZ(5);
  ball_.hide();
  drawTicks();
}

VisuField::~VisuField()
{
}

void VisuField::playerDoingKickoff()
{
  ball_.setPos(g_.getInput().mouse_ - Point(getScreenRect()) - Point(30, 30));
  ball_.show();
}

void VisuField::setBallPos(const Point& pos)
{
  if (ball_.isShown())
    ball_.move(pos * 40, 25.);
  else
    {
      ball_.setPos(pos * 40);
      ball_.show();
    }
}

bool VisuField::getDrawTicks() const
{
  return draw_ticks_;
}

// FIXME: doesn't work right now.
// I don't handle surface alteration very well.
void VisuField::setDrawTicks(bool enable)
{
  draw_ticks_ = enable;
  if (draw_ticks_)
    {
      LOG3("Show tick border");
      drawTicks();
    }
  else
    {
      // To erase ticks borders, reload image.
      LOG3("Remove tick border");
    }
}

/*
** Draw square borders. Modify directly bg_ by 'erasing' with black
** color little arrows.
*/
void VisuField::drawTicks()
{
  const int square_size = 40;
  const int margin_size = 7;
  int i, j;
  SDL_Rect r;
  SDL_Surface* surf = bg_.getSDLSurface();

  for (i = 0; i <= COLS; i++)
    {
      for (j = 0; j <= ROWS; j++)
	{
	  r.w = 1;
	  r.h = 1;
	  r.x = i * square_size + margin_size;
	  r.y = j * square_size + margin_size;
	  SDL_FillRect(surf, &r, 0xFF00);
	  if (i > 0)
	    { 
	     r.x = i * square_size + margin_size - 2;
	     r.w = 2;
	     SDL_FillRect(surf, &r, 0xFF00);
	    }
	  if (i < COLS)
	    { 
	      r.x = i * square_size + margin_size + 1;
	      r.w = 2;
	      SDL_FillRect(surf, &r, 0xFF00);
	    }
      
	  r.x = i * square_size + margin_size;
	  r.w = 1;
	  if (j > 0)
	    { 
	      r.y = j * square_size + margin_size - 2;
	      r.h = 2;
	      SDL_FillRect(surf, &r, 0xFF00);
	    }
	  if (j < ROWS)
	    { 
	      r.y = j * square_size + margin_size + 1;
	      r.h = 2;
	      SDL_FillRect(surf, &r, 0xFF00);
	    }
	}
    }
}

void VisuField::update()
{
  Input& inp = g_.getInput();

  // Handle kickoff.
  if (g_.isStateSet(stDoKickoff))
    {
      ball_.setPos(g_.getInput().mouse_ - Point(getScreenRect()) - Point(30, 30));
  
      if (inp.button_pressed_[1])
	{
	  Point to((inp.mouse_ - Point(getScreenRect())) / 40);
	  ball_.setPos(to * 40);
	  LOG(4, "Place the ball at " << to);
	  if (g_.getApi()->doPlaceBall(to))
	    g_.unsetState(stDoKickoff);
	}
    }

  VirtualScrollableSurface::update();
}

END_NS(sdl_visu);
