//
// display.cpp for  in prologin_gui
// 
// Made by Hazgar
// Login   <lapie_t@epitech.net>
// 
// Started on  Thu Feb 26 10:44:38 2009 Hazgar
// Last update Sat Apr 25 17:04:35 2009 Hazgar
//

#include <cstdlib>
#include <iostream>
#include "display.h"
#include "event.hpp"

/* Display instance (Display is a singleton) */
Display			*Display::_instance = NULL;

/* surfaces */
static SurfacesList	DisplaySurface[] =
  {
    {SFC_FLOOR, "./image/floor.png", NULL},
    {SFC_NONE, NULL, NULL}
  };

/* sprites */
static SpritesList	DisplaySprite[] =
  {
    {{SP_HOUSE, "./image/house.png", 1, 0, 0, true, 124, 62, {0,0}}, NULL},
    {{SP_RESERVED, "./image/floor_res.png", 1, 0, 0, true, 124, 62, {0,0}}, NULL},
    {{SP_MONUMENT1, "./image/monument1.png", 1, 0, 0, true, 124, 62, {0,0}}, NULL},
    {{SP_MONUMENT2, "./image/monument2.png", 1, 0, 0, true, 124, 62, {0,0}}, NULL},
    {{SP_MONUMENT3, "./image/monument3.png", 1, 0, 0, true, 124, 62, {0,0}}, NULL},
    {{SP_MONUMENT4, "./image/monument4.png", 1, 0, 0, true, 124, 62, {0,0}}, NULL},
    {{SP_MONUMENT5, "./image/monument5.png", 1, 0, 0, true, 124, 62, {0,0}}, NULL},
    {{SP_MONUMENT6, "./image/monument6.png", 1, 0, 0, true, 124, 62, {0,0}}, NULL},
    {{SP_MONUMENT7, "./image/monument7.png", 1, 0, 0, true, 124, 62, {0,0}}, NULL},
    {{SP_MONUMENT8, "./image/monument8.png", 1, 0, 0, true, 124, 62, {0,0}}, NULL},
    {{SP_MONUMENT9, "./image/monument9.png", 1, 0, 0, true, 124, 62, {0,0}}, NULL},
    {{SP_MONUMENT10, "./image/monument10.png", 1, 0, 0, true, 124, 62, {0,0}}, NULL},
    {{SP_MONUMENT11, "./image/monument11.png", 1, 0, 0, true, 124, 62, {0,0}}, NULL},
    {{SP_MONUMENT12, "./image/monument12.png", 1, 0, 0, true, 124, 62, {0,0}}, NULL},
    {{SP_MONUMENT13, "./image/monument13.png", 1, 0, 0, true, 124, 62, {0,0}}, NULL},
    {{SP_MONUMENT14, "./image/monument14.png", 1, 0, 0, true, 124, 62, {0,0}}, NULL},
    {{SP_NONE, NULL, 0, 0, 0, false, 0, 0, {0,0}}, NULL}
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

  atexit(DisplayCleanup);
  if (SDL_Init(SDL_INIT_VIDEO) == -1)
    throw "Display init failed";
  screen = SDL_SetVideoMode(winWidth, winHeight, 32, SDL_RESIZABLE | SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_HWACCEL
			    | SDL_ANYFORMAT | SDL_SRCCOLORKEY | SDL_RLEACCEL | SDL_SRCALPHA);
  if (screen == NULL)
    throw "Display screen init failed";

  SDL_EnableKeyRepeat(50, 50);

  this->_screen = new Surface(screen);
  this->_winWidth = winWidth;
  this->_winHeight = winHeight;
  this->_motions = MOTION_RESET;
  this->_display_motion[0] = winWidth >> 3;
  this->_display_motion[1] = winHeight >> 1;
  this->loadSurfaces();
  this->loadSprites();
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

/* Event and display loop */
void			Display::Core(void)
{
  SfcField		scr_field;
  SDL_Event		ev;
  SDL_Surface		*screen;

  scr_field.setSize(this->_screen->getWidth(), this->_screen->getHeight());
  screen = static_cast<SDL_Surface*>(this->_screen->getSurface());
  while (1)
    {
      while (SDL_PollEvent(&ev))
	{
	  switch (ev.type)
	    {
	    case SDL_QUIT:
	      exit(0);
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
      this->displayMotion();
      this->_map->Blit(*(this->_screen), scr_field);
      SDL_Flip(screen);
    }
}

/* */
void			Display::displayMotion(void)
{
  if (this->_motions & MOTION_LEFT)
    this->_display_motion[0] += 24;
  if (this->_motions & MOTION_RIGHT)
    this->_display_motion[0] -= 24;
  if (this->_motions & MOTION_UP)
    this->_display_motion[1] += 24;
  if (this->_motions & MOTION_DOWN)
    this->_display_motion[1] -= 24;
  if (this->_motions != MOTION_RESET)
    this->_map->setDrawPos(this->_display_motion[0], this->_display_motion[1]);
}

/* Set display window size */
void			Display::setWinSize(unsigned int winWidth, unsigned int winHeight)
{
  SDL_Surface		*screen;
  SDL_Surface		*nscreen;

  screen = static_cast<SDL_Surface*>(this->_screen->getSurface());
  nscreen = SDL_SetVideoMode(winWidth, winHeight, 0, screen->flags);
  if (nscreen == NULL)
    throw "Display screen resize failed";
  delete this->_screen;
  this->_screen = new Surface(nscreen);
  this->_winWidth = winWidth;
  this->_winHeight = winHeight;
}

/* Set display window caption */
void			Display::setWinCaption(std::string caption)
{
  SDL_WM_SetCaption(caption.c_str(), NULL);
  this->_winCaption = caption;
}

/* Keyboard event handler */
void			Display::keyboardEvent(unsigned int keysym)
{
  switch (keysym)
    {
    case SDLK_ESCAPE:
      exit(0);
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
  EventCase		*c;

  switch (code)
    {
    case EV_CASETYPE:
      c = static_cast<EventCase*>(data);
      this->_map->setCase(c->data, c->x, c->y);
      break;
    case EV_CASEOWNER:
      c = static_cast<EventCase*>(data);
      break;
    case EV_CASEPRICE:
      c = static_cast<EventCase*>(data);
      break;
    case EV_NEWBID:
      break;
    case EV_NEWTURN:
      break;
    }
}
