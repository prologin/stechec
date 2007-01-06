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

#include <SDL_gfxPrimitives.h>
#include "Api.hh"
#include "Game.hh"
#include "Field.hh"

BEGIN_NS(sdlvisu);

VisuField::VisuField(Game& g)
  : VirtualScrollableSurface("VVisuField", g.getInput(), Point(500, 600), Point(725, 1100)),
    g_(g),
    bg_("image/general/playground_0.jpg"),
    ball_("image/general/ball.png"),
    cross_black_("image/general/crosses_rgbbw.png"),
    cross_red_("image/general/crosses_rgbbw.png"),
    draw_ticks_(true)
{
  bg_.setZ(-1);
  addChild(&bg_);
  addChild(&ball_);
  ball_.setZ(5);
  ball_.hide();
  drawTicks();

  red_highlight_.create(40, 40);
  blue_highlight_.create(40, 40);
  boxRGBA(red_highlight_.getSDLSurface(), 0, 0, 40, 40, 255, 0, 0, 200); 
  boxRGBA(blue_highlight_.getSDLSurface(), 0, 0, 40, 40, 0, 0, 255, 200);
  SDL_SetAlpha(red_highlight_.getSDLSurface(), SDL_SRCALPHA, 120);
  SDL_SetAlpha(blue_highlight_.getSDLSurface(), SDL_SRCALPHA, 120);
  red_highlight_.hide();
  blue_highlight_.hide();
  addChild(&red_highlight_);
  addChild(&blue_highlight_);
  cross_black_.splitNbFrame(5, 1);
  cross_black_.setFrame(4);
  cross_black_.hide();
  cross_red_.splitNbFrame(5, 1);
  cross_red_.setFrame(1);
  cross_red_.hide();
  addChild(&cross_black_);
  addChild(&cross_red_);
}

VisuField::~VisuField()
{
}

bool VisuField::mouseInsideField() const
{
  Point sq(mouseToSquare());
  return sq.x >= 0 && sq.x < COLS && sq.y >= 0 && sq.y < ROWS;
}

Point VisuField::mouseToSquare() const
{
  return (g_.getInput().mouse_ - Point(getScreenRect()) - Point(7, 7)) / 40;
}

Point VisuField::squareToField(const Point& pt, const Point& adjust) const
{
  return pt * 40 + Point(7, 7) + adjust;
}

void VisuField::setMarker(const Point& square, int type)
{
  switch (type)
    {
    case 0:
      red_highlight_.hide();
      blue_highlight_.setPos(squareToField(square));
      blue_highlight_.show();
      break;

    case 1:
      blue_highlight_.hide();
      red_highlight_.setPos(squareToField(square));
      red_highlight_.show();
      break;
      
    default:
      LOG2("Marker type '%d' unknown.", type);
      break;
    }
}

void VisuField::removeMarker()
{
  red_highlight_.hide();
  blue_highlight_.hide();
}

void VisuField::setBallPos(const Point& pos)
{
  if (pos == Point(-1, -1))
    {
      ball_.hide();
      return;
    }

  if (!ball_.isShown())
    {
      // This case happen when other team does the kickoff, on reception
      // of the first MSG_BALLPOS
      cross_black_.setPos(squareToField(pos, Point(4, 4)));
      cross_black_.show();
      ball_.setPos(squareToField(pos, Point(3, 3)));
      ball_.show();
    }
  else
    {
      if (g_.isStateSet(stWaitKoffBall))
	{
	  cross_red_.setPos(squareToField(pos, Point(4, 4)));
	  cross_red_.show();
	}
      ball_.move(squareToField(pos, Point(3, 3)), 35.);
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
** little arrows.
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

  // Handle kickoff of ball - we place the ball on receiver's field side.
  if (g_.isStateSet(stDoKoffBall))
    {
      Point square(mouseToSquare());
      ball_.setPos(g_.getInput().mouse_ - Point(getScreenRect()) - Point(30, 30));
      ball_.show();

      if (mouseInsideField())
	{
	  if (square.y < ROWS / 2 && g_.getApi()->myTeamId() == 0
	      || square.y >= ROWS / 2 && g_.getApi()->myTeamId() == 1)
	    setMarker(square, 1);
	  else
	    setMarker(square, 0);
	    
	  if (inp.button_pressed_[1])
	    {
	      LOG(4, "Try to place the ball at " << square);
	      if (!g_.getApi()->doPlaceBall(square))
		{
		  ball_.setPos(squareToField(square, Point(10, 10)));
		  g_.unsetState(stDoKoffBall);
		  g_.setState(stWaitKoffBall);
		  cross_black_.setPos(squareToField(square, Point(4, 4)));
		  cross_black_.show();
		  removeMarker();
		}
	    }
	}
      else
	removeMarker();
    }

  // Handle kickoff of ball - give the ball to one of our player.
  if (g_.isStateSet(stDoKoffGiveBall))
    {
      Point square(mouseToSquare());
      ball_.setPos(g_.getInput().mouse_ - Point(getScreenRect()) - Point(30, 30));
      ball_.show();

      const CPlayer* p = g_.getApi()->getPlayer(square);
      if (p != NULL)
	{
	  setMarker(square, 0);
	  if (inp.button_pressed_[1])
	    {
	      LOG4("Try to give the ball to %1 at %2", p->getId(), square);
	      if (!g_.getApi()->doGiveBall(p->getId()))
		{
		  ball_.setPos(squareToField(square, Point(10, 10)));
		  g_.unsetState(stDoKoffGiveBall);
		  g_.setState(stWaitKoffBall);
		  removeMarker();
		}
	    }
	}
      else if (mouseInsideField() &&
	       (square.y < ROWS / 2 && g_.getApi()->myTeamId() == 0
		|| square.y >= ROWS / 2 && g_.getApi()->myTeamId() == 1))
	setMarker(square, 1);
      else
	removeMarker();
    }
  
  VirtualScrollableSurface::update();
}

END_NS(sdl_visu);
