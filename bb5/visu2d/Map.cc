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
    reserve_bg_("image/general/reserve_0.jpg"),
    ball_("image/general/ball.png"),
    cross_black_("image/general/crosses_rgbbw.png"),
    cross_red_("image/general/crosses_rgbbw.png"),
    draw_ticks_(true),
    dugouts_back_(g, "image/general/reserve_navigator_down.png"),
    dugouts_forth_(g, "image/general/reserve_navigator_up.png")
{
  bg_.setZ(-1);
  addChild(&bg_);
  drawTicks();

  reserve_bg_.setZ(-1);
  reserve_bg_.setPos(619,365);
  addChild(&reserve_bg_);

  ball_.setZ(5);
  addChild(&ball_);
  ball_.hide();

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

  for (int dugout = 0; dugout < 4; dugout ++)
    for (int team = 0; team < 2; team ++)
      dugouts_page_index_[dugout][team] = 0;
  for (int zone = 0; zone < 5; zone ++)
    for (int team = 0; team < 2; team ++)
      for (int player = 0; player < MAX_PLAYER; player ++)
        players_by_zone_[zone][team][player] = NULL;

  dugouts_back_.setPos(607, 467);
  dugouts_forth_.setPos(619, 467);
  dugouts_back_.setActionHandler(new DugoutsCallback(this, false));
  dugouts_forth_.setActionHandler(new DugoutsCallback(this, true));
  addChild(&dugouts_back_);
  addChild(&dugouts_forth_);
  dugouts_back_.disable();
  dugouts_forth_.enable();
}

Map::~Map()
{
}

//
// Mouse and field squares coordinates
//

bool Map::mouseInsideMap() const
{
  return getRenderRect().inside(g_.getInput().mouse_);
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

//
// Marker
//

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

//
// Ball
//

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

//
// Players and dugouts
//

void Map::addPlayer(VisuPlayer* vp)
{
  assert(vp != NULL);
  const CPlayer* p = vp->getPlayer();
  addChild(vp);
  players_by_zone_[MZ_RESERVE][p->getTeamId()][p->getId()] = vp;
  updateDugout(MZ_RESERVE, p->getTeamId(), p->getId());
}

void Map::movePlayer(enum eStatus old_s, enum eStatus new_s, int team_id, int player_id)
{
  enum eMapZone from, to;
  from = statusToZone(old_s);
  to = statusToZone(new_s);
  if (from != to)
    movePlayer(from, to, team_id, player_id);
}

inline enum eMapZone Map::statusToZone(enum eStatus s)
{
  switch (s)
  {
    case STA_SENTOFF:     return MZ_EJECTED;  break;
    case STA_INJURED:     return MZ_OUT;      break;
    case STA_KO:          return MZ_KO;       break;
    case STA_UNASSIGNED:
    case STA_RESERVE:     return MZ_RESERVE;  break;
    case STA_STANDING:
    case STA_PRONE:
    case STA_STUNNED:     return MZ_FIELD;    break;
  }
  ERR("Unable to determine map zone according to player status `%1'.", s);
  return MZ_RESERVE;
}

void Map::movePlayer(enum eMapZone from, enum eMapZone to, int team_id, int player_id)
{
  assert (from != to);
  VisuPlayer* vp = players_by_zone_[from][team_id][player_id];
  enum eMapZone by = (to == MZ_FIELD) ? from : to;

  if (team_id == 0)
    switch (by)
    {
      case MZ_EJECTED:  vp->setPos(Point(607,  69));  break;
      case MZ_OUT:      vp->setPos(Point(607, 177));  break;
      case MZ_KO:       vp->setPos(Point(607, 285));  break;
      case MZ_RESERVE:  vp->setPos(Point(607, 435));  break;
      case MZ_FIELD:    ERR("Field is not a dugout"); break;
    }
  else
    switch (by)
    {
      case MZ_EJECTED:  vp->setPos(Point(607, 983));  break;
      case MZ_OUT:      vp->setPos(Point(607, 835));  break;
      case MZ_KO:       vp->setPos(Point(607, 727));  break;
      case MZ_RESERVE:  vp->setPos(Point(607, 517));  break;
      case MZ_FIELD:    ERR("Field is not a dugout"); break;
    }
  vp->enable();

  players_by_zone_[from][team_id][player_id] = NULL;
  players_by_zone_[to][team_id][player_id] = vp;

  if (from != MZ_FIELD)
    updateDugout(from, team_id, player_id);
  if (to != MZ_FIELD)
    updateDugout(to, team_id, player_id);
}

void Map::scrollDugout(enum eMapZone dugout, int team_id, unsigned int page_index)
{
  assert(dugout != MZ_FIELD);
  VisuPlayer** players = players_by_zone_[dugout][team_id];
  unsigned int old_page_index = dugouts_page_index_[dugout][team_id];
  unsigned int min_page_index = std::min(old_page_index, page_index);
  unsigned int max_page_index = std::max(old_page_index, page_index);
  unsigned int nb_players = 0;
  int i = 0;
  Point pos;
  if (team_id == 0)
    switch (dugout)
    {
      case MZ_EJECTED:  pos = (Point(630,  45));  break;
      case MZ_OUT:      pos = (Point(630, 153));  break;
      case MZ_KO:       pos = (Point(630, 261));  break;
      case MZ_RESERVE:  pos = (Point(630, 387));  break;
      case MZ_FIELD:    ERR("Field is not a dugout"); break;
    }
  else
    switch (dugout)
    {
      case MZ_EJECTED:  pos = (Point(630, 959));  break;
      case MZ_OUT:      pos = (Point(630, 811));  break;
      case MZ_KO:       pos = (Point(630, 703));  break;
      case MZ_RESERVE:  pos = (Point(630, 575));  break;
      case MZ_FIELD:    ERR("Field is not a dugout"); break;
    }

  if (dugout == MZ_EJECTED && team_id == 0) // there is only one button for all dugouts
    {
      if (page_index > 0)
        {
          dugouts_back_.enable();
          if (page_index == ((MAX_PLAYER - 1) / 4))
            dugouts_forth_.disable();
        }
      if (page_index < ((MAX_PLAYER - 1) / 4))
        {
          dugouts_forth_.enable();
          if (page_index == 0)
            dugouts_back_.disable();
        }
    }

  while ((nb_players / 4 < min_page_index) && (i < MAX_PLAYER))
    {
      if (players[i] != NULL)
        nb_players ++;
      i ++;
    }
  while ((nb_players / 4 <= max_page_index) && (i < MAX_PLAYER))
    {
      if (players[i] != NULL)
        {
          if ((nb_players / 4) != page_index)
            {
              players[i]->disable();
            }
          else
            {
              switch (nb_players % 4)
              {
                case 0: players[i]->setPos(pos);                  break;
                case 1: players[i]->setPos(pos + Point( 0, 48));  break;
                case 2: players[i]->setPos(pos + Point(40,  0));  break;
                case 3: players[i]->setPos(pos + Point(40, 48));  break;
              }
              players[i]->enable();
            }
          nb_players ++;
        }
      i ++;
    }
  dugouts_page_index_[dugout][team_id] = page_index;
}

void Map::updateDugout(enum eMapZone dugout, int team_id, int player_id)
{
  assert(dugout != MZ_FIELD);
  VisuPlayer** players = players_by_zone_[dugout][team_id];
  unsigned int page_index = dugouts_page_index_[dugout][team_id];
  unsigned int nb_players = 0;
  int i = 0;
  Point pos;
  if (team_id == 0)
    switch (dugout)
    {
      case MZ_EJECTED:  pos = (Point(630,  45));  break;
      case MZ_OUT:      pos = (Point(630, 153));  break;
      case MZ_KO:       pos = (Point(630, 261));  break;
      case MZ_RESERVE:  pos = (Point(630, 387));  break;
      case MZ_FIELD:    ERR("Field is not a dugout"); break;
    }
  else
    switch (dugout)
    {
      case MZ_EJECTED:  pos = (Point(630, 959));  break;
      case MZ_OUT:      pos = (Point(630, 811));  break;
      case MZ_KO:       pos = (Point(630, 703));  break;
      case MZ_RESERVE:  pos = (Point(630, 575));  break;
      case MZ_FIELD:    ERR("Field is not a dugout"); break;
    }
  while (i < player_id)
    {
      if (players[i] != NULL)
        nb_players ++;
      i ++;
    }
  while (i < MAX_PLAYER)
    {
      if (players[i] != NULL)
        {
          if ((nb_players / 4) != page_index)
            {
              players[i]->disable();
            }
          else
            {
              switch (nb_players % 4)
              {
                case 0: players[i]->setPos(pos);                  break;
                case 1: players[i]->setPos(pos + Point( 0, 48));  break;
                case 2: players[i]->setPos(pos + Point(40,  0));  break;
                case 3: players[i]->setPos(pos + Point(40, 48));  break;
              }
              players[i]->enable();
            }
          nb_players ++;
        }
      i ++;
    }
}

//
// Field grid ticks
//

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
  Surface img;

  // FIXME: Copy the image before modifying it,
  // or it will be impossible to reload the original image.

  //bg_.create(bg_.getSize().x, bg_.getSize().y, bg_.getSDLSurface());
  //img.load("image/general/playground_0.jpg");
  //img.blit(bg_);

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

//
// Update routine
//

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

void Map::scrollDugouts(bool forth)
{
  assert((forth && (dugouts_page_index_[0][0] < ((MAX_PLAYER -1) / 4)))
      || ((!forth) && (dugouts_page_index_[0][0] > 0)));
  unsigned int new_page_index = dugouts_page_index_[0][0] + (forth ? 1 : -1);
  for (enum eMapZone dugout = MZ_EJECTED; dugout <= MZ_RESERVE;
      dugout = static_cast<enum eMapZone>(dugout + 1))
    for (int team = 0; team < 2; team ++)
      scrollDugout(dugout, team, new_page_index);
}

void Map::DugoutsCallback::clicked()
{
  map_->scrollDugouts(forth_);
}

END_NS(sdlvisu);
