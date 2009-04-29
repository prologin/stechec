//
// display.cpp for  in prologin_gui
// 
// Made by Hazgar
// Login   <lapie_t@epitech.net>
// 
// Started on  Thu Feb 26 10:44:38 2009 Hazgar
// Last update Wed Apr 29 18:54:36 2009 user
//

#include <SDL_ttf.h>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include "display.h"
#include "font.h"
#include "event.hpp"

/* Display instance (Display is a singleton) */
Display			*Display::_instance = NULL;

/* surfaces */
static SurfacesList	DisplaySurface[] =
  {
    {SFC_FLOOR, "/opt/share/stechec/prolo2009/graphics/floor.png", NULL},
    {SFC_PANEL, "/opt/share/stechec/prolo2009/graphics/panel.png", NULL},
    {SFC_PRICE, "/opt/share/stechec/prolo2009/graphics/price.png", NULL},
    {SFC_NONE, NULL, NULL}
  };

/* sprites */
static SpritesList	DisplaySprite[] =
  {
    {{SP_HOUSE1, "/opt/share/stechec/prolo2009/graphics/house1.png", 4, 400, 0, true, 117, 104, {0,0}}, NULL},
    {{SP_HOUSE2, "/opt/share/stechec/prolo2009/graphics/house2.png", 4, 400, 0, true, 117, 104, {0,0}}, NULL},
    {{SP_HOUSE3, "/opt/share/stechec/prolo2009/graphics/house3.png", 4, 400, 0, true, 117, 104, {0,0}}, NULL},
    {{SP_HOUSE4, "/opt/share/stechec/prolo2009/graphics/house4.png", 4, 400, 0, true, 117, 104, {0,0}}, NULL},
    {{SP_HOUSE5, "/opt/share/stechec/prolo2009/graphics/house5.png", 4, 400, 0, true, 117, 104, {0,0}}, NULL},
    {{SP_ROAD1, "/opt/share/stechec/prolo2009/graphics/road1.png", 1, 0, 0, true, 117, 61, {0,0}}, NULL},
    {{SP_ROAD2, "/opt/share/stechec/prolo2009/graphics/road2.png", 1, 0, 0, true, 117, 61, {0,0}}, NULL},
    {{SP_ROAD3, "/opt/share/stechec/prolo2009/graphics/road3.png", 1, 0, 0, true, 117, 61, {0,0}}, NULL},
    {{SP_ROAD4, "/opt/share/stechec/prolo2009/graphics/road4.png", 1, 0, 0, true, 117, 61, {0,0}}, NULL},
    {{SP_ROAD5, "/opt/share/stechec/prolo2009/graphics/road5.png", 1, 0, 0, true, 117, 61, {0,0}}, NULL},
    {{SP_ROAD6, "/opt/share/stechec/prolo2009/graphics/road6.png", 1, 0, 0, true, 117, 61, {0,0}}, NULL},
    {{SP_ROAD7, "/opt/share/stechec/prolo2009/graphics/road7.png", 1, 0, 0, true, 117, 61, {0,0}}, NULL},
    {{SP_RESERVED, "/opt/share/stechec/prolo2009/graphics/floor_res.png", 1, 0, 0, true, 117, 61, {0,0}}, NULL},
    {{SP_MONUMENT1, "/opt/share/stechec/prolo2009/graphics/monument1.png", 1, 0, 0, true, 124, 62, {0,0}}, NULL},
    {{SP_MONUMENT2, "/opt/share/stechec/prolo2009/graphics/monument2.png", 1, 0, 0, true, 124, 62, {0,0}}, NULL},
    {{SP_MONUMENT3, "/opt/share/stechec/prolo2009/graphics/monument3.png", 1, 0, 0, true, 124, 62, {0,0}}, NULL},
    {{SP_MONUMENT4, "/opt/share/stechec/prolo2009/graphics/monument4.png", 1, 0, 0, true, 124, 62, {0,0}}, NULL},
    {{SP_MONUMENT5, "/opt/share/stechec/prolo2009/graphics/monument5.png", 1, 0, 0, true, 124, 62, {0,0}}, NULL},
    {{SP_MONUMENT6, "/opt/share/stechec/prolo2009/graphics/monument6.png", 1, 0, 0, true, 124, 62, {0,0}}, NULL},
    {{SP_MONUMENT7, "/opt/share/stechec/prolo2009/graphics/monument7.png", 1, 0, 0, true, 124, 62, {0,0}}, NULL},
    {{SP_MONUMENT8, "/opt/share/stechec/prolo2009/graphics/monument8.png", 1, 0, 0, true, 124, 62, {0,0}}, NULL},
    {{SP_MONUMENT9, "/opt/share/stechec/prolo2009/graphics/monument9.png", 1, 0, 0, true, 124, 62, {0,0}}, NULL},
    {{SP_MONUMENT10, "/opt/share/stechec/prolo2009/graphics/monument10.png", 1, 0, 0, true, 124, 62, {0,0}}, NULL},
    {{SP_MONUMENT11, "/opt/share/stechec/prolo2009/graphics/monument11.png", 1, 0, 0, true, 124, 62, {0,0}}, NULL},
    {{SP_MONUMENT12, "/opt/share/stechec/prolo2009/graphics/monument12.png", 1, 0, 0, true, 124, 62, {0,0}}, NULL},
    {{SP_MONUMENT13, "/opt/share/stechec/prolo2009/graphics/monument13.png", 1, 0, 0, true, 124, 62, {0,0}}, NULL},
    {{SP_MONUMENT14, "/opt/share/stechec/prolo2009/graphics/monument14.png", 1, 0, 0, true, 124, 62, {0,0}}, NULL},
    {{SP_NONE, NULL, 0, 0, 0, false, 0, 0, {0,0}}, NULL}
  };

/* fonts */
static FontsList	DisplayFont[] =
  {
    {FT_PRICES, 12, "/opt/share/stechec/prolo2009/graphics/arial.ttf", NULL},
    {FT_INFOS, 16, "/opt/share/stechec/prolo2009/graphics/arial.ttf", NULL},
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
  //  SDL_FreeSurface(layout);
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
  SfcField		scr_field;
  SDL_Event		ev;
  SDL_Surface		*screen;

  scr_field.setSize(this->_screen->getWidth(), this->_screen->getHeight());
  this->GetSurface(SFC_PANEL)->SetAlphaFlags();
  this->GetSurface(SFC_PANEL)->Blit(*(this->_layout), scr_field);
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
      this->displayMotion();
      this->_map->Blit(*(this->_screen), scr_field);
      this->_layout->Blit(*(this->_screen), scr_field);
      SDL_Flip(screen);
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

  screen = static_cast<SDL_Surface*>(this->_screen->getSurface());
  nscreen = SDL_SetVideoMode(winWidth, winHeight, 0, screen->flags);
  if (nscreen == NULL)
    throw "Display screen resize failed";
  delete this->_screen;
  this->_screen = new Surface(nscreen);
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

/* Keyboard event handler */
void			Display::keyboardEvent(unsigned int keysym)
{
  switch (keysym)
    {
    case SDLK_ESCAPE:
      exit(0);
      break;
    case SDLK_TAB:
      this->_map->ShowPrices();
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
  Font			*ft;
  SfcField		pos;

  switch (code)
    {
    case EV_CASETYPE:
      c = static_cast<EventCase*>(data);
      this->_map->setCase(c->data, c->x, c->y);
      delete c;
      break;
    case EV_CASEOWNER:
      c = static_cast<EventCase*>(data);
      this->_map->setCaseOwner(c->data, c->x, c->y);
      delete c;
      break;
    case EV_CASEPRICE:
      c = static_cast<EventCase*>(data);
      this->_map->setCasePrice(c->data, c->x, c->y);
      delete c;
      break;
    case EV_PLAYER:
      p = static_cast<EventPlayer*>(data);
      ft = this->GetFont(FT_INFOS);
      ft->setColor(0xFFFFFFFF);
      ft->Text.str("");
      ft->Text << "Player" << p->id << ": score["
	       << std::setfill('0') << std::setw(4) << p->score
	       << "] money[" << std::setfill('0') << std::setw(4) << p->money << "]";
      pos.setPos(0, 16 * p->id + 16);
      ft->Blit(*(this->_layout), pos);
      delete p;
      break;
    case EV_NEWTURN:
      i = (int*)data;
      ft = this->GetFont(FT_INFOS);
      ft->setColor(0xFFFFFFFF);
      ft->Text.str("");
      ft->Text << "Turn " << *i;
      pos.setPos(0, 0);
      ft->Blit(*(this->_layout), pos);
      delete i;
      break;
    }
}
