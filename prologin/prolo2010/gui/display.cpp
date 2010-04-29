//
// display.cpp for  in prologin_gui
// 
// Made by Hazgar
// Login   <lapie_t@epitech.net>
// 
// Started on  Thu Feb 26 10:44:38 2009 Hazgar
// Last update Sat May  2 14:20:31 2009 user
//

#include <SDL_ttf.h>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include "display.h"
#include "event.hpp"

/* Display instance (Display is a singleton) */
Display			*Display::_instance = NULL;

#define DATA(p) (PKGDATADIR p)

/* surfaces */
static SurfacesList	DisplaySurface[] =
  {
    {SFC_FLOOR, DATA("/graphics/floor.png"), NULL},
    {SFC_NONE, NULL, NULL}
  };

/* sprites */
static SpritesList	DisplaySprite[] =
  {
    {{SP_PARROT0, DATA("/graphics/parrot0.png"), 1, 900, 0, true, 116, 116, {0,0}}, NULL},
    {{SP_MONKEY0, DATA("/graphics/monkey0.png"), 1, 900, 0, true, 116, 116, {0,0}}, NULL},
    {{SP_CAT0, DATA("/graphics/cat0.png"), 1, 900, 0, true, 116, 116, {0,0}}, NULL},
    {{SP_KANGAROO0, DATA("/graphics/kangaroo0.png"), 1, 900, 0, true, 116, 116, {0,0}}, NULL},
    {{SP_KO0, DATA("/graphics/ko.png"), 1, 900, 0, true, 116, 116, {0,0}}, NULL},
    {{SP_KO1, DATA("/graphics/ko1.png"), 1, 900, 0, true, 116, 116, {0,0}}, NULL},
    {{SP_KO2, DATA("/graphics/ko2.png"), 1, 900, 0, true, 116, 116, {0,0}}, NULL},
    {{SP_KO3, DATA("/graphics/ko3.png"), 1, 900, 0, true, 116, 116, {0,0}}, NULL},
    {{SP_MOVE0, DATA("/graphics/ko3.png"), 1, 900, 0, true, 116, 116, {0,0}}, NULL},
    {{SP_MOVE1, DATA("/graphics/ko3.png"), 1, 900, 0, true, 116, 116, {0,0}}, NULL},
    {{SP_MOVE2, DATA("/graphics/ko3.png"), 1, 900, 0, true, 116, 116, {0,0}}, NULL},
    {{SP_MOVE3, DATA("/graphics/ko3.png"), 1, 900, 0, true, 116, 116, {0,0}}, NULL},
    {{SP_MOVE4, DATA("/graphics/ko3.png"), 1, 900, 0, true, 116, 116, {0,0}}, NULL},
    {{SP_MOVE5, DATA("/graphics/ko3.png"), 1, 900, 0, true, 116, 116, {0,0}}, NULL},
    {{SP_MOVE6, DATA("/graphics/ko3.png"), 1, 900, 0, true, 116, 116, {0,0}}, NULL},
    {{SP_MOVE7, DATA("/graphics/ko3.png"), 1, 900, 0, true, 116, 116, {0,0}}, NULL},
    {{SP_NONE, NULL, 0, 0, 0, false, 0, 0, {0,0}}, NULL}
  };

/* fonts */
static FontsList	DisplayFont[] =
  {
    {FT_PRICES, 20, DATA("/graphics/arialbd.ttf"), NULL},
    {FT_INFOS, 16, DATA("/graphics/arial.ttf"), NULL},
    {FT_INFOS2, 18, DATA("/graphics/arialbd.ttf"), NULL},
    {FT_NONE, 0, NULL, NULL}
  };

/*
** This function will be called at program exits to correctly
** shutdown the display.
*/
static void		DisplayCleanup(void)
{
  Display		*dsp;

  dsp = Display::GetInstance();
  delete dsp;
}

/* Display constructor. Init SDL and display window */
Display::Display(unsigned int winWidth, unsigned int winHeight)
  : Thread::Thread(), _winCaption("")
{
  SDL_Surface	*screen;
  SDL_Surface	*layout;

  atexit(DisplayCleanup);
  if (SDL_Init(SDL_INIT_VIDEO) == -1 || TTF_Init() == -1)
    throw "Display init failed";
  screen = SDL_SetVideoMode(winWidth, winHeight, 32, SDL_RESIZABLE | SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_HWACCEL
			    | SDL_ANYFORMAT | SDL_SRCCOLORKEY | SDL_RLEACCEL | SDL_SRCALPHA);
  if (screen == NULL)
    throw "Display screen init failed";

  layout = SDL_CreateRGBSurface(screen->flags, screen->w, screen->h, 32,
				0xFF000000, 0x00FF0000, 0x0000FF00,
				0x000000FF);
  if (layout == NULL)
    throw "Display layout init failed";

  SDL_EnableKeyRepeat(50, 50);

  this->_screen = new Surface(screen);
  this->_layout = new Surface(layout);
  this->_winWidth = winWidth;
  this->_winHeight = winHeight;
  this->_motions = MOTION_RESET;
  this->_display_motion[0] = winWidth >> 3;
  this->_display_motion[1] = winHeight >> 1;
  this->loadSurfaces();
  this->loadSprites();
  this->loadFonts();

  try
    {
      this->_map = new DisplayMap(*(this->_screen), this->GetSurface(SFC_FLOOR));
      this->_map->setDrawPos(this->_display_motion[0], this->_display_motion[1]);
    }
  catch (const char *str)
    {
      std::cerr << str << std::endl;
      exit(0);
    }
  std::cout << "Display engine init done" << std::endl;
}

/* Display copy constructor, not used yet*/
Display::Display(const Display &right)
  : Thread::Thread()
{
  (void)right;
}

/* Display destructor */
Display::~Display()
{
  std::cout << "Shutting down display engine...";
  delete this->_screen;
  delete this->_layout;
  delete this->_map;
  SDL_Quit();
  std::cout << "done" << std::endl;
}

/* Display operator= overload, not used yet */
Display			&Display::operator=(const Display &right)
{
  (void)right;
  return (*this);
}

/* Return the Display instance */
Display			*Display::GetInstance(void)
{
  if (!Display::_instance)
    {
      try
	{
	  Display::_instance = new Display;
	}
      catch (const char *str)
	{
	  std::cerr << str << std::endl;
	  return (NULL);
	}
    }
  return (Display::_instance);
}

/* */
Surface			*Display::GetSurface(SurfaceID id)
{
  int			i;

  for (i = 0; DisplaySurface[i].id != SFC_NONE; i++)
    if (DisplaySurface[i].id == id)
      return (DisplaySurface[i].sfc);
  return (NULL);
}

/* */
Sprite			*Display::GetSprite(SpriteID id)
{
  int			i;

  for (i = 0; DisplaySprite[i].cfg.id != SP_NONE; i++)
    if (DisplaySprite[i].cfg.id == id)
      return (DisplaySprite[i].sprite);
  return (NULL);
}

/* */
Font			*Display::GetFont(FontID id)
{
  int			i;

  for (i = 0; DisplayFont[i].id != FT_NONE; i++)
    if (DisplayFont[i].id == id)
      return (DisplayFont[i].font);
  return (NULL);
}

/* */
void			Display::loadSurfaces(void)
{
  int			i;

  std::cout << "Loading textures...";
  for (i = 0; DisplaySurface[i].id != SFC_NONE; i++)
    DisplaySurface[i].sfc = new Surface(DisplaySurface[i].path);
  std::cout << "done" << std::endl;
}

/* */
void			Display::loadSprites(void)
{
  int			i;

  std::cout << "Loading sprites...";
  for (i = 0; DisplaySprite[i].cfg.id != SP_NONE; i++)
    DisplaySprite[i].sprite = new Sprite(DisplaySprite[i].cfg);
  std::cout << "done" << std::endl;
}

/* */
void			Display::loadFonts(void)
{
  int			i;

  std::cout << "Loading fonts...";
  for (i = 0; DisplayFont[i].id != FT_NONE; i++)
    DisplayFont[i].font = new Font(DisplayFont[i].path, DisplayFont[i].size);
  std::cout << "done" << std::endl;
}

/* Event and display loop */
void			Display::Core(void)
{
  int			status;
  int			time[3];
  SfcField		scr_field;
  SDL_Event		ev;
  SDL_Surface		*screen;
  //int			fps;

  scr_field.setSize(this->_screen->getWidth(), this->_screen->getHeight());
  screen = static_cast<SDL_Surface*>(this->_screen->getSurface());
  time[0] = SDL_GetTicks();
  /*time[2] = time[0];
    fps = 0;*/
  while (1)
    {
      /*if ((time[0] - time[2]) >= 1000)
	{
	  fps = 0;
	  time[2] = SDL_GetTicks();
	  }*/
      while (SDL_PollEvent(&ev))
	{
	  switch (ev.type)
	    {
	    case SDL_QUIT:
	      status = EV_DISPLAY_END;
	      this->Write((void*)(&status), sizeof(status));
	      break;
	    case SDL_ACTIVEEVENT:
	      this->_motions = MOTION_RESET;
	      break;
	    case SDL_KEYUP:
	      this->_motions = MOTION_RESET;
	      break;
	    case SDL_KEYDOWN:
	      this->keyboardEvent(ev.key.keysym.sym);
              break;
            case SDL_MOUSEBUTTONDOWN:
              this->mouseClickEvent(ev.button.x, ev.button.y);
              break;
	    case SDL_MOUSEMOTION:
              this->mouseMotionEvent(ev.motion.x, ev.motion.y);
              break;
	    case SDL_USEREVENT:
	      this->userEvent(ev.user.code, ev.user.data1);
	      break;
	    case SDL_VIDEORESIZE:
	      this->setWinSize(ev.resize.w, ev.resize.h);
	      break;
	    }
	}
      time[1] = SDL_GetTicks();
      if ((time[1] - time[0]) > 2)
	{
	  time[0] = SDL_GetTicks();
	  continue;
	}
      this->displayMotion();
      this->_map->Blit(*(this->_screen), scr_field);
      this->_layout->Blit(*(this->_screen), scr_field);
      SDL_Flip(screen);
      time[0] = SDL_GetTicks();
      //fps++;
    }
}

/* */
void			Display::displayMotion(void)
{
  if (this->_motions & MOTION_LEFT)
    {
      if (this->_display_motion[0] < 0)
	this->_display_motion[0] += 20;
    }
  if (this->_motions & MOTION_RIGHT)
    {
      if ((this->_display_motion[0] + this->_map->GetRealWidth()) > (int)(this->_screen->getWidth()))
	this->_display_motion[0] -= 20;
    }
  if (this->_motions & MOTION_UP)
    {
      if (this->_display_motion[1] < (this->_map->GetRealHeight() >> 1))
	this->_display_motion[1] += 20;
    }
  if (this->_motions & MOTION_DOWN)
    {
      if (this->_display_motion[1] > ((int)(this->_screen->getHeight()) - (this->_map->GetRealHeight() >> 1)))
	this->_display_motion[1] -= 20;
    }
  if (this->_motions != MOTION_RESET)
    this->_map->setDrawPos(this->_display_motion[0], this->_display_motion[1]);
}

/* Set display window size */
void			Display::setWinSize(unsigned int winWidth, unsigned int winHeight)
{
  SDL_Surface		*screen;
  SDL_Surface		*nscreen;
  SDL_Surface		*layout;

  screen = static_cast<SDL_Surface*>(this->_screen->getSurface());
  nscreen = SDL_SetVideoMode(winWidth, winHeight, 0, screen->flags);
  layout = SDL_CreateRGBSurface(screen->flags, winWidth, winHeight, 32,
				0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
  if (nscreen == NULL || layout == NULL)
    throw "Display screen resize failed";
  delete this->_screen;
  delete this->_layout;
  this->_screen = new Surface(nscreen);
  this->_layout = new Surface(layout);
  this->_winWidth = winWidth;
  this->_winHeight = winHeight;
  this->_map->BuildFrom(*(this->_screen));
}

/* Set display window caption */
void			Display::setWinCaption(std::string caption)
{
  SDL_WM_SetCaption(caption.c_str(), NULL);
  this->_winCaption = caption;
}

/* */
void			Display::setDisplayMotion(int x, int y)
{
  this->_display_motion[0] = x;
  this->_display_motion[1] = y;
}

/* Keyboard event handler */
void			Display::keyboardEvent(unsigned int keysym)
{
  int			status;

  switch (keysym)
    {
    case SDLK_ESCAPE:
      status = EV_DISPLAY_END;
      this->Write((void*)(&status), sizeof(status));
      break;
    case SDLK_RETURN:
      status = EV_DISPLAY_NEXTTURN;
      this->Write((void*)(&status), sizeof(status));
      break;
    case SDLK_KP_MINUS:
      this->_map->ZoomOut();
      break;
    case SDLK_KP_PLUS:
      this->_map->ZoomIn();
      break;
    case SDLK_LEFT:
      this->_motions |= MOTION_LEFT;
      break;
    case SDLK_RIGHT:
      this->_motions |= MOTION_RIGHT;
      break;
    case SDLK_UP:
      this->_motions |= MOTION_UP;
      break;
    case SDLK_DOWN:
      this->_motions |= MOTION_DOWN;
      break;
    }
}

/* Mouse motions event handler */
void			Display::mouseMotionEvent(unsigned int x, unsigned int y)
{
  this->_motions = MOTION_RESET;

  if (x < MOUSE_MOTION_MARGIN)
    this->_motions |= MOTION_LEFT;
  else if (x > (this->_winWidth - MOUSE_MOTION_MARGIN))
    this->_motions |= MOTION_RIGHT;

  if (y < MOUSE_MOTION_MARGIN)
    this->_motions |= MOTION_UP;
  else if (y > (this->_winHeight - MOUSE_MOTION_MARGIN))
    this->_motions |= MOTION_DOWN;
}

/* Mouse click event handler */
/* Not used yet */
void			Display::mouseClickEvent(unsigned int x, unsigned int y)
{
  (void)x;
  (void)y;
}

void			Display::userEvent(unsigned int code, void *data)
{
  int			*i;
  EventCase		*c;
  EventPlayer		*p;
  SfcField		pos;

  switch (code)
    {
    case EV_CASETYPE:
      c = static_cast<EventCase*>(data);
      printf("# %i %i\n", c->x, c->y);
      this->_map->setCase(c->data, c->x, c->y);
      delete c;
      break;
    case EV_MAPSIZE:
      printf("$ %i\n", *((int*)data));
      this->_map->setViewField(*((int*)data));
      delete c;
      break;
    case EV_PLAYER:
      p = static_cast<EventPlayer*>(data);
      delete p;
      break;
    case EV_NEWTURN:
      i = (int*)data;
      this->_map->FlushCases();
      delete i;
      break;
    case EV_WINNER:
      p = static_cast<EventPlayer*>(data);
      delete p;
      break;
    case EV_ENDGAME:
      break;
    }
}
