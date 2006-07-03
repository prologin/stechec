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

#include "Widget.hh"

Focus Widget::globfocus;

Widget::Widget(SDL_Surface* screen, Widget* father)
{
  this->screen = screen;
  this->x = 0;  this->y = 0;
  this->w = WIDGET_WIDTH;  this->h = WIDGET_HEIGHT;
  visible = false;
  focus = false;
  this->father = father;

  old_screen = SDL_CreateRGBSurface (SDL_HWSURFACE,
                                     this->w, this->h,
                                     screen->format->BitsPerPixel,
                                     screen->format->Rmask,
                                     screen->format->Gmask,
                                     screen->format->Bmask,
                                     screen->format->Amask);

  widget = SDL_CreateRGBSurface (SDL_HWSURFACE,
                                 this->w, this->h,
                                 screen->format->BitsPerPixel,
                                 screen->format->Rmask,
                                 screen->format->Gmask,
                                 screen->format->Bmask,
                                 screen->format->Amask);
}

Widget::Widget(const uint w, const uint h, SDL_Surface * screen, Widget* father)
{
  this->screen = screen;
  this->x = 0;  this->y = 0;
  this->w = w;  this->h = h;
  visible = false;
  Widget::focus = false;
  this->father = father;
  Focus globfocus();

  old_screen = SDL_CreateRGBSurface (SDL_HWSURFACE,
                                     this->w, this->h,
                                     screen->format->BitsPerPixel,
                                     screen->format->Rmask,
                                     screen->format->Gmask,
                                     screen->format->Bmask,
                                     screen->format->Amask);

  widget = SDL_CreateRGBSurface (SDL_HWSURFACE,
                                 this->w, this->h,
                                 screen->format->BitsPerPixel,
                                 screen->format->Rmask,
                                 screen->format->Gmask,
                                 screen->format->Bmask,
                                 screen->format->Amask);
}

Widget::Widget(const uint x, const uint y, const uint w, const uint h,
               SDL_Surface * screen, Widget* father)
{
  this->screen = screen;
  this->x = x;  this->y = y;
  this->w = w;  this->h = h;
  visible = false;
  Widget::focus = false;
  this->father = father;
  Focus globfocus();

  old_screen = SDL_CreateRGBSurface (SDL_HWSURFACE,
                                     this->w, this->h,
                                     screen->format->BitsPerPixel,
                                     screen->format->Rmask,
                                     screen->format->Gmask,
                                     screen->format->Bmask,
                                     screen->format->Amask);

  widget = SDL_CreateRGBSurface (SDL_HWSURFACE,
                                 this->w, this->h,
                                 screen->format->BitsPerPixel,
                                 screen->format->Rmask,
                                 screen->format->Gmask,
                                 screen->format->Bmask,
                                 screen->format->Amask);
}

Widget::~Widget()
{
  if(hasfocus())
  {
    globfocus.set_focus(NULL);
  }
  SDL_FreeSurface(widget);
  SDL_FreeSurface(old_screen);
}

void Widget::erase()
{
  SDL_Rect r = {get_wx(), get_wy(), w, h};
  SDL_BlitSurface(old_screen, NULL, screen, &r);
  SDL_Flip(screen);
}

ushort Widget::inside(const uint mousex, const uint mousey)
{
  const uint wx = get_wx();
  const uint wy = get_wy();
  return((mousex >= wx) && (mousex <= (wx + w)) && (mousey >= wy) && (mousey <= (wy + h)));
}

ushort Widget::mousebuttondown(const uint mousex, const uint mousey)
{
  if(inside(mousex, mousey))
  {
    globfocus.set_focus(this);
    return 1;
  }
  return 0;
}

void Widget::gainfocus()
{
  focus=true;
}

void Widget::losefocus()
{
  focus=false;
}

bool Widget::hasfocus()
{
  return(focus);
}

Focus& Widget::get_focus()
{ 
  return globfocus;
}

uint Widget::get_x()
{
  return x;
}

uint Widget::get_wx()
{
  if(father != NULL)
  {
    return((father->get_wx()) + x);
  }
  else
  {
    return(x);
  }
}

void Widget::set_x(const uint x)
{
  this->x = x;
}

uint Widget::get_y()
{
  return y;
}

void Widget::set_y(const uint y)
{
  this->y = y;
}

uint Widget::get_wy()
{
  if(father != NULL)
  {
    return((father->get_wy()) + y);
  }
  else
  {
    return(y);
  } 
}

uint Widget::get_height()
{
  return h;
}

void Widget::set_height(const uint height)
{
  h = height;
}

uint Widget::get_width()
{
  return w;
}

void Widget::set_width(const uint width)
{
  w = width;
}
