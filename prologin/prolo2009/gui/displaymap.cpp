//
// displaymap.cpp for  in /home/lapie_t/prologin_gui
// 
// Made by Hazgar
// Login   <lapie_t@epitech.net>
// 
// Started on  Fri Mar 13 15:06:27 2009 Hazgar
// Last update Sat Apr 25 13:01:24 2009 Hazgar
//

#include <SDL.h>
#include <SDL_rotozoom.h>
#include <cstring>
#include "game.h"
#include "displaymap.h"
#include "display.h"

static MapCaseType	CaseType[] =
  {
    {LD_HOUSE		, SP_HOUSE},
    {LD_RESERVED	, SP_RESERVED},
    {LD_MONUMENTS	, SP_MONUMENT1},
    {LD_MONUMENTS + 1	, SP_MONUMENT2},
    {LD_MONUMENTS + 2	, SP_MONUMENT3},
    {LD_MONUMENTS + 3	, SP_MONUMENT4},
    {LD_MONUMENTS + 4	, SP_MONUMENT5},
    {LD_MONUMENTS + 5	, SP_MONUMENT6},
    {LD_MONUMENTS + 6	, SP_MONUMENT7},
    {LD_MONUMENTS + 7	, SP_MONUMENT8},
    {LD_MONUMENTS + 8	, SP_MONUMENT9},
    {LD_MONUMENTS + 9	, SP_MONUMENT10},
    {LD_MONUMENTS + 10	, SP_MONUMENT11},
    {LD_MONUMENTS + 11	, SP_MONUMENT12},
    {LD_MONUMENTS + 12	, SP_MONUMENT13},
    {LD_MONUMENTS + 13	, SP_MONUMENT14},
    {LD_EMPTY		, SP_NONE}
  };

DisplayMap::DisplayMap(const Surface &display, Surface *texture)
{
  this->_floor = texture;
  this->_zoom = 0;
  this->_oldZoom = 0;
  this->_viewField = 11;
  this->_zsfc = NULL;
  this->_draw_pos[0] = 0;
  this->_draw_pos[1] = 0;
  this->BuildFrom(display);
  memset(this->_case, LD_EMPTY, sizeof(this->_case));
  this->InitFloorSfc();
}

DisplayMap::~DisplayMap(void)
{
}

void		DisplayMap::BuildFrom(const Surface &display)
{
  SDL_Surface	*dsp;
  SDL_Surface	*sfc;
  SDL_Surface	*floor_sfc;

  dsp = static_cast<SDL_Surface*>(display.getSurface());
  sfc = SDL_CreateRGBSurface(dsp->flags, dsp->w, dsp->h, 32,
			     dsp->format->Rmask, dsp->format->Gmask,
			     dsp->format->Bmask, dsp->format->Amask);
  floor_sfc = SDL_CreateRGBSurface(dsp->flags, dsp->w, dsp->h, 32,
				   dsp->format->Rmask, dsp->format->Gmask,
				   dsp->format->Bmask, dsp->format->Amask);
  if (sfc == NULL || floor_sfc == NULL)
    throw "DisplayMap build error.";

  this->_sfc = new Surface(sfc);
  this->_floor_sfc = new Surface(floor_sfc);
}

void		DisplayMap::Refresh(void)
{
  int		mw, mh, scope_start, map_case;
  int		i, j, k, x, y;
  SfcField	pos;
  Display	*dsp;
  Sprite	*sprite;


  dsp = Display::GetInstance();

  pos.setSize(this->_sfc->getWidth(), this->_sfc->getHeight());
  this->_floor_sfc->Blit(*(this->_sfc), pos);

  mw = MAP_WIDTH * (this->_viewField / 100.0);
  mh = MAP_HEIGHT * (this->_viewField / 100.0);
  x = (MAP_WIDTH >> 1) - (mw >> 1);
  y = (MAP_HEIGHT >> 1) - (mh >> 1);
  scope_start = x + y * MAP_WIDTH;
  for (i = 0; i < mh; i++)
    for (j = 0; j < mw; j++)
      {
	map_case = j + i * MAP_WIDTH + scope_start;
	sprite = NULL;
	if (this->_case[map_case] == LD_ROAD)
	  {
	  }
	else
	  {
	    for (k = 0; CaseType[k].type != LD_EMPTY; k++)
	      if (this->_case[map_case] == CaseType[k].type)
		{
		  sprite = dsp->GetSprite(CaseType[k].spr_id);
		  break;
		}
	  }
	if (sprite != NULL)
	  {
	    x = (j * (this->_floor->getWidth() >> 1)) + (i * (this->_floor->getWidth() >> 1));
	    y = (i * (this->_floor->getHeight() >> 1)) - (j * (this->_floor->getHeight() >> 1)) - (this->_floor->getHeight() >> 1);
	    x += this->_draw_pos[0];
	    y += this->_draw_pos[1];
	    pos.setPos(x, y);
	    sprite->Blit(*(this->_sfc), pos);
	  }
      }
}

void		DisplayMap::InitFloorSfc(void)
{
  int		mw, mh;
  int		i, j, x, y;
  SfcField	pos;

  if (this->_floor == NULL)
    return ;
  SDL_FillRect((SDL_Surface*)(this->_floor_sfc->getSurface()), NULL, 0x0);
  mw = MAP_WIDTH * (this->_viewField / 100.0);
  mh = MAP_HEIGHT * (this->_viewField / 100.0);
  for (i = 0; i < mh; i++)
    for (j = 0; j < mw; j++)
      {
        x = (j * (this->_floor->getWidth() >> 1)) + (i * (this->_floor->getWidth() >> 1));
        y = (i * (this->_floor->getHeight() >> 1)) - (j * (this->_floor->getHeight() >> 1)) - (this->_floor->getHeight() >> 1);
	x += this->_draw_pos[0];
	y += this->_draw_pos[1];
	pos.setPos(x, y);
	this->_floor->Blit(*(this->_floor_sfc), pos);
      }
}

void		DisplayMap::setZoom(unsigned short zoom)
{
  if (zoom > 100)
    return ;
  this->_oldZoom = this->_zoom;
  this->_zoom = zoom;
}

void		DisplayMap::setViewField(unsigned short size)
{
  this->_viewField = size;
}

void		DisplayMap::ZoomIn(void)
{
  if ((this->_zoom + 2) <= 100)
    this->setZoom(this->_zoom + 10);
}

void		DisplayMap::ZoomOut(void)
{
  if ((this->_zoom - 2) >= 0)
    this->setZoom(this->_zoom - 10);
}

void		DisplayMap::Blit(Surface &dst, SfcField &pos)
{
  this->Refresh();
  if (this->_zoom != this->_oldZoom && this->_zoom >= 0)
    {
      if (this->_zsfc != NULL)
	delete this->_zsfc;
      this->_zsfc = new Surface(rotozoomSurface((SDL_Surface*)this->_sfc->getSurface(), 0.0, (this->_zoom / 100.0) + 1, 0));
    }
  if (this->_zsfc != NULL)
    this->_zsfc->Blit(dst, pos);
  else
    this->_sfc->Blit(dst, pos);
}

void		DisplayMap::setCase(int type, int x, int y)
{
  this->_case[x + y * MAP_WIDTH] = type;
}

void		DisplayMap::setDrawPos(int x, int y)
{
  this->_draw_pos[0] = x;
  this->_draw_pos[1] = y;
  this->InitFloorSfc();
}
