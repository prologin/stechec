#include <iostream>
#include "Gui.hpp"
#include "Utils.hpp"
#include "SpriteMgr.hpp"

Gui::Gui(const char *wnd_name, unsigned int xsize, unsigned int ysize)
{
  if (SDL_Init(SDL_INIT_VIDEO) == -1)
    throw Error("Unable to initialize SDL: ", SDL_GetError(), NULL);
  _wnd = NULL;
  if (xsize && ysize)
  {
    _xsize = xsize;
    _ysize = ysize;
    _wnd = SDL_SetVideoMode(_xsize, _ysize, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!_wnd)
      throw Error("Unable to create a window: ", SDL_GetError(), NULL);
    std::cout << "[info] Using " << (_wnd->flags & SDL_HWSURFACE ? "hardware" : "software") << " rendering" << std::endl;
    std::cout << "[info] Double-buffering " << (_wnd->flags & SDL_DOUBLEBUF ? "enabled" : "disabled") << std::endl;
    _bpp = _wnd->format->BytesPerPixel;
    SDL_WM_SetCaption(wnd_name, NULL);
  }
  else
  {
    _xsize = _ysize = _bpp = 0;
    _wnd = NULL;
  }
}

Gui::~Gui(void)
{
  SDL_Quit();
}

void	Gui::PutPixel(unsigned int x, unsigned int y, unsigned int color)
{
  SDL_Color	c;
  Uint32	p;
  char		*buf;	// We use a char* to perform arithmetic operations.

  if (x >= _xsize || y >= _ysize)
    return ;
  c.r = (color & 0x00FF0000) >> 16;
  c.g = (color & 0x0000FF00) >> 8;
  c.b = (color & 0x000000FF);
  p = SDL_MapRGB(_wnd->format, c.r, c.g, c.b);
  buf = (char *)_wnd->pixels + y * _wnd->pitch + x * _wnd->format->BytesPerPixel;

  switch (_wnd->format->BytesPerPixel)
  {
    case 1:
      *(Uint8 *)buf = p;
      break;

    case 2:
      *(Uint16 *)buf = p;
      break;

    case 3:
    case 4:
      *(Uint32 *)buf = p;
      break;
  }
}

void		Gui::DrawRect(unsigned int x, unsigned int y, unsigned int xsize, unsigned int ysize, unsigned int color)
{
  unsigned int	i;

  for (i = x; i < x + xsize; ++i)
  {
    this->PutPixel(i, y, color);
    this->PutPixel(i, y + ysize, color);
  }
  for (i = y; i < y + ysize; ++i)
  {
    this->PutPixel(x, i, color);
    this->PutPixel(x + xsize, i, color);
  }
}

void	Gui::Refresh(void)
{
  SDL_Flip(_wnd);
}

void	Gui::PutSprite(unsigned int x, unsigned int y, Sprite *s)
{
  SDL_Rect	dst;

  if (!s)
    return ;
  dst.x = x;
  dst.y = y;
  SDL_BlitSurface(s->GetSprite(), NULL, _wnd, &dst);
}

void	Gui::SetCallback(Uint8 event, GuiCallback f)
{
  _callbacks[event] = f;
}

void	Gui::Loop(void)
{
  SDL_Event e;
  std::map<Uint8, GuiCallback>::iterator iter;

  while (SDL_WaitEvent(&e))
  {
    iter = _callbacks.find(e.type);
    if (iter != _callbacks.end())
      ((*iter).second)(&e);
    if (e.type == SDL_VIDEOEXPOSE)
      this->Refresh();
    else if (e.type == SDL_QUIT)
      return ;
  }
}

void	Gui::Clear(void)
{
  memset(_wnd->pixels, 0, _xsize * _ysize * _wnd->format->BytesPerPixel);
}
