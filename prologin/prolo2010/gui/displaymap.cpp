//
// displaymap.cpp for  in /home/lapie_t/prologin_gui
// 
// Made by Hazgar
// Login   <lapie_t@epitech.net>
// 
// Started on  Fri Mar 13 15:06:27 2009 Hazgar
// Last update Sat May  2 13:14:22 2009 user
//

#include <SDL.h>
#include <SDL_rotozoom.h>
#include <SDL_gfxPrimitives.h>
#include <algorithm>
#include <cstring>
#include "prologin.hh"
#include "game.h"
#include "font.h"
#include "displaymap.h"
#include "display.h"

static MapCaseType	CaseType[] =
  {
    {LD_PARROT		, SP_PARROT0},
    {LD_MONKEY		, SP_MONKEY0},
    {LD_CAT		, SP_CAT0},
    {LD_KANGAROO	, SP_KANGAROO0},
    {LD_KO0		, SP_KO0},
    {LD_KO1		, SP_KO1},
    {LD_KO2		, SP_KO2},
    {LD_KO3		, SP_KO3},
    {LD_MOVE0		, SP_MOVE0},
    {LD_MOVE1		, SP_MOVE1},
    {LD_MOVE2		, SP_MOVE2},
    {LD_MOVE3		, SP_MOVE3},
    {LD_MOVE4		, SP_MOVE4},
    {LD_MOVE5		, SP_MOVE5},
    {LD_MOVE6		, SP_MOVE6},
    {LD_MOVE7		, SP_MOVE7},
    {LD_EMPTY		, SP_NONE}
  };

DisplayMap::DisplayMap(const Surface &display, Surface *texture)
{
  this->_floor = texture;
  this->_zoom = 0;
  this->_oldZoom = 0;
  this->_viewField = 100;
  this->_zsfc = NULL;
  this->_draw_pos[0] = 0;
  this->_draw_pos[1] = 0;
  this->BuildFrom(display);
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
  this->InitFloorSfc();
}

void			DisplayMap::Refresh(void)
{
  int			mw, mh, scope_start, map_case;
  int			x, y;
  SfcField		pos;
  Display		*dsp;
  std::vector<Sprite*>	sprites;


  dsp = Display::GetInstance();

  pos.setSize(this->_sfc->getWidth(), this->_sfc->getHeight());
  this->_floor_sfc->Blit(*(this->_sfc), pos);

  mw = (int)(MAP_WIDTH * (this->_viewField / 100.0));
  mh = (int)(MAP_HEIGHT * (this->_viewField / 100.0));
  x = (MAP_WIDTH >> 1) - (mw >> 1);
  y = (MAP_HEIGHT >> 1) - (mh >> 1);
  scope_start = x + y * MAP_WIDTH;
  for (int i = 0; i < mh; i++)
    for (int j = mw - 1; j >= 0; j--)
      {
	map_case = j + i * MAP_WIDTH + scope_start;
	for (unsigned int l = 0; l < this->_case[map_case].size(); ++l)
	  {
	    for (int k = 0; CaseType[k].type != LD_EMPTY; k++)
	      if (this->_case[map_case][l] == CaseType[k].type)
		{
		  sprites.push_back(dsp->GetSprite(CaseType[k].spr_id));
		  break;
		}
	  }
	if (!sprites.empty())
	  {
	    x = (j * (this->_floor->getWidth() >> 1)) + (i * (this->_floor->getWidth() >> 1));
	    y = (i * (this->_floor->getHeight() >> 1)) - (j * (this->_floor->getHeight() >> 1)) - (this->_floor->getHeight() >> 1);
	    x += this->_draw_pos[0];
	    y += this->_draw_pos[1];
	    while (!(sprites.empty()))
	      {
		int ys = y - sprites.back()->getFieldHeight() - this->_floor->getHeight();
		pos.setPos(x, ys);
		sprites.back()->action();
		sprites.back()->Blit(*(this->_sfc), pos);
		sprites.pop_back();
	      }

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
  mw = (int)(MAP_WIDTH * (this->_viewField / 100.0));
  mh = (int)(MAP_HEIGHT * (this->_viewField / 100.0));
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

void		DisplayMap::setZoom(int zoom)
{
  if (zoom > 100)
    return ;
  this->_oldZoom = this->_zoom;
  this->_zoom = zoom;
}

void		DisplayMap::setViewField(int size)
{
  this->_viewField = size;
  this->InitFloorSfc();
}

int		DisplayMap::getZoom(void) const
{
  return (this->_zoom);
}

int		DisplayMap::getViewField(void) const
{
  return (this->_viewField);
}

void		DisplayMap::ZoomIn(void)
{
  if ((this->_zoom + 10) <= 100)
    this->setZoom(this->_zoom + 10);
}

void		DisplayMap::ZoomOut(void)
{
  if ((this->_zoom - 10) >= 0)
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
  int		px, py;
  int		sx, sy, ex, ey, mw, mh;

  if ((x >= 0 && x < MAP_WIDTH) && (y >= 0 && y < MAP_HEIGHT))
    this->_case[x + y * MAP_WIDTH].push_back(type);
  this->InitFloorSfc();
}

void		DisplayMap::setDrawPos(int x, int y)
{
  this->_draw_pos[0] = x;
  this->_draw_pos[1] = y;
  this->InitFloorSfc();
}

int		DisplayMap::GetRealWidth(void) const
{
  int		mw, z;

  mw = (int)(MAP_WIDTH * (this->_viewField / 100.0));
  z = (int)(this->_floor->getWidth() * (this->_zoom / 100.0));
  mw = mw * (this->_floor->getWidth() + z);
  return (mw);
}

int		DisplayMap::GetRealHeight(void) const
{
  int		mh, z;

  mh = (int)(MAP_HEIGHT * (this->_viewField / 100.0));
  z = (int)(this->_floor->getHeight() * (this->_zoom / 100.0));
  mh = mh * (this->_floor->getHeight() + z);
  return (mh);
}

void		DisplayMap::FlushCases(void)
{
  for (int i = 0; i < MAP_SIZE; ++i)
    this->_case[i].clear();
}
