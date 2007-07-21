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

#include "Map.hh"

#include "Api.hh"
#include "CPlayer.hh"
#include "Game.hh"
#include "VisuPlayer.hh"

#include "ResourceCenter.hh"

#include <SDL_gfxPrimitives.h>

BEGIN_NS(sdlvisu);

Map::Map(Game& g)
  : VirtualScrollableSurface("VSMap", g.getInput(), Point(500, 600), Point(723, 1054)),
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

Map::~Map()
{
}

bool Map::mouseInsideMap() const
{
  Rect render(getPos(), getScreenRect().getSize());
  return render.inside(g_.getInput().mouse_);
}

bool Map::mouseInsideField() const
{
  Point sq(mouseToSquare());
  return mouseInsideMap() && sq.x >= 0 && sq.x < COLS && sq.y >= 0 && sq.y < ROWS;
}

Point Map::mouseToSquare() const
{
  return (g_.getInput().mouse_ - Point(getScreenRect()) - Point(7, 7)) / 40;
}

Point Map::squareToMap(const Point& pt, const Point& adjust) const
{
  return pt * 40 + Point(7, 7) + adjust;
}

void Map::setMarker(const Point& square, int type)
{
  switch (type)
    {
    case 0:
      red_highlight_.hide();
      blue_highlight_.setPos(squareToMap(square));
      blue_highlight_.show();
      break;

    case 1:
      blue_highlight_.hide();
      red_highlight_.setPos(squareToMap(square));
      red_highlight_.show();
      break;
      
    default:
      LOG2("Marker type '%d' unknown.", type);
      break;
    }
}

void Map::removeMarker()
{
  red_highlight_.hide();
  blue_highlight_.hide();
}

void Map::setBallPos(const Point& pos)
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
      cross_black_.setPos(squareToMap(pos, Point(4, 4)));
      cross_black_.show();
      ball_.setPos(squareToMap(pos, Point(3, 3)));
      ball_.show();
    }
  else
    {
      if (g_.isStateSet(VGS_WAITKICKOFF))
        {
          cross_red_.setPos(squareToMap(pos, Point(4, 4)));
          cross_red_.show();
        }
      ball_.move(squareToMap(pos, Point(3, 3)), 100.);
    }
}

void Map::removeBall()
{
  ball_.hide();
}

void Map::placePlayer(VisuPlayer* vp, Position& pos)
{
  assert(vp != NULL);
  assert(pos.col >= 0 && pos.col < COLS && pos.row >= 0 && pos.row < ROWS);

  //FIXME: to do.
  vp->setPos(squareToMap(pos));
}

void Map::removePlayer(VisuPlayer* vp, enum eStatus s)
{
  assert(vp != NULL);

  //FIXME: to do.
  switch (s)
  {
    case STA_RESERVE:
      vp->setPos(Point(654, ((vp->getPlayer()->getTeamId() == 1) ? 377 : 637)));
      break;
    case STA_KO:
      vp->setPos(Point(654, ((vp->getPlayer()->getTeamId() == 1) ? 277 : 737)));
      break;
    case STA_INJURED:
      vp->setPos(Point(654, ((vp->getPlayer()->getTeamId() == 1) ? 177 : 837)));
      break;
    case STA_SENTOFF:
      vp->setPos(Point(654, ((vp->getPlayer()->getTeamId() == 1) ? 77 : 937)));
      break;
    default:
      WARN("Player `%1' of team `%2' doesn't have to leave the field in state `%3'.",
          vp->getPlayer()->getId(), vp->getPlayer()->getTeamId(), Player::stringify(s));
      break;
  }
}

bool Map::getDrawTicks() const
{
  return draw_ticks_;
}

void Map::setDrawTicks(bool enable)
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
      bg_.load("image/general/playground_0.jpg");
    }
}

/*
** FIXME: Copy the image before modifying it, or it will be impossible to reload the original.
** Draw square borders. Modify directly bg_ by 'erasing' with black
** little arrows.
*/
void Map::drawTicks()
{
  const int square_size = 40;
  const int margin_size = 7;
  int i, j;
  SDL_Rect r;
  SDL_Surface* surf;
  Uint32 color;
  Surface img(ResourceCenter::getInst()->getImage("image/general/playground_0.jpg"));

  bg_.create(img.getSize().x, img.getSize().y, img.getSDLSurface());
  surf = bg_.getSDLSurface();
  color = SDL_MapRGB(surf->format, 0, 0, 0);

  for (i = 0; i <= COLS; i++)
    {
      for (j = 0; j <= ROWS; j++)
        {
          r.w = 1;
          r.h = 1;
          r.x = i * square_size + margin_size;
          r.y = j * square_size + margin_size;
          SDL_FillRect(surf, &r, color);
          if (i > 0)
            { 
             r.x = i * square_size + margin_size - 2;
             r.w = 2;
             SDL_FillRect(surf, &r, color);
            }
          if (i < COLS)
            { 
              r.x = i * square_size + margin_size + 1;
              r.w = 2;
              SDL_FillRect(surf, &r, color);
            }
      
          r.x = i * square_size + margin_size;
          r.w = 1;
          if (j > 0)
            { 
              r.y = j * square_size + margin_size - 2;
              r.h = 2;
              SDL_FillRect(surf, &r, color);
            }
          if (j < ROWS)
            { 
              r.y = j * square_size + margin_size + 1;
              r.h = 2;
              SDL_FillRect(surf, &r, color);
            }
        }
    }
}

void Map::update()
{
  Input& inp = g_.getInput();

  // Handle kickoff of ball - we place the ball on receiver's field side.
  if (g_.isStateSet(VGS_DOKICKOFF))
    {
      Point square(mouseToSquare());
      ball_.setPos(inp.mouse_ - Point(getScreenRect()) - Point(30, 30));
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
              LOG5("Try to place the ball at %1.", square);
              if (!g_.getApi()->doPlaceBall(square))
                {
                  ball_.setPos(squareToMap(square, Point(10, 10)));
                  g_.unsetState(VGS_DOKICKOFF);
                  g_.setState(VGS_WAITKICKOFF);
                  cross_black_.setPos(squareToMap(square, Point(4, 4)));
                  cross_black_.show();
                  removeMarker();
                }
            }
        }
      else
        removeMarker();
    }

  // Handle kickoff of ball - give the ball to one of our player.
  if (g_.isStateSet(VGS_DOTOUCHBACK))
    {
      Point square(mouseToSquare());
      ball_.setPos(inp.mouse_ - Point(getScreenRect()) - Point(30, 30));
      ball_.show();

      const CPlayer* p = g_.getApi()->getPlayer(square);
      if (p != NULL && p->getTeamId() == g_.getApi()->myTeamId())
        {
          setMarker(square, 0);
          if (inp.button_pressed_[1])
            {
              LOG4("Try to give the ball to %1 at %2", p->getId(), square);
              if (!g_.getApi()->doGiveBall(p->getId()))
                {
                  ball_.setPos(squareToMap(square, Point(10, 10)));
                  g_.unsetState(VGS_DOTOUCHBACK);
                  g_.setState(VGS_WAITKICKOFF);
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
