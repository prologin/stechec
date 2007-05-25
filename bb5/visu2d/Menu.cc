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

#include "ResourceCenter.hh"
#include "Menu.hh"
#include "GuiError.hh"

Menu::Menu (const uint x, const uint y, SDL_Surface * screen,
            const ushort width, const string title) : Widget(x, y, width, 0, screen)
{
  padx = 5; pady = 5;
  fgColor = black_color;
  bgColor = darkgray_color;
  this->title = title;
  h = 2 * pady;
  font = NULL;
  if (title != "")
  {
    h += MENU_WIDGET_HEIGHT;
    font = ResourceCenter::getInst()->getFont("Vera.ttf", 18);
  }

      // recreate widget and old_creen with correct height !
  SDL_FreeSurface(widget);
  SDL_FreeSurface(old_screen);

  old_screen = SDL_CreateRGBSurface (SDL_HWSURFACE,
                                     this->w, this->h,
                                     screen->format->BitsPerPixel,
                                     screen->format->Rmask,
                                     screen->format->Gmask,
                                     screen->format->Bmask,
                                     screen->format->Amask);
  SDL_BlitSurface (screen, NULL, old_screen, NULL);

  widget = SDL_CreateRGBSurface (SDL_HWSURFACE,
                                 this->w, this->h,
                                 screen->format->BitsPerPixel,
                                 screen->format->Rmask,
                                 screen->format->Gmask,
                                 screen->format->Bmask,
                                 screen->format->Amask);
}

Menu::Menu (const uint x, const uint y, SDL_Surface * screen,
            const ushort width, const ushort height, 
            const string title) : Widget(x, y, width, height, screen)
{
  padx = 5; pady = 5;
  fgColor = black_color;
  bgColor = darkgray_color;
  this->title = title;
  font = NULL;
  if (title != "")
  {
    h += MENU_WIDGET_HEIGHT;
    font = ResourceCenter::getInst()->getFont("Vera.ttf", 18);

        // recreate widget and old_creen with correct height !
    SDL_FreeSurface(widget);
    SDL_FreeSurface(old_screen);

    old_screen = SDL_CreateRGBSurface (SDL_HWSURFACE,
                                       this->w, this->h,
                                       screen->format->BitsPerPixel,
                                       screen->format->Rmask,
                                       screen->format->Gmask,
                                       screen->format->Bmask,
                                       screen->format->Amask);
    SDL_BlitSurface (screen, NULL, old_screen, NULL);
    widget = SDL_CreateRGBSurface (SDL_HWSURFACE,
                                   this->w, this->h,
                                   screen->format->BitsPerPixel,
                                   screen->format->Rmask,
                                   screen->format->Gmask,
                                   screen->format->Bmask,
                                   screen->format->Amask);
  }
}

Menu::~Menu ()
{
  for (vector <Widget*>::iterator b = list_widget.begin ();
       b != list_widget.end (); ++b)
  {
    delete(*b);
  }
  list_widget.clear();
}

void Menu::addWidget(Widget* wt)
{
  SDL_Rect r = {get_wx(), get_wy(), w, h};    //relative to window
  list_widget.push_back(wt);
      // adapt the size of the menu
  if(wt->get_width() > w)
  {
    w = (wt->get_width() + 2 * padx);
  }
  wt->set_x((w - wt->get_width()) / 2);
  wt->set_y(getnbwidget() * (MENU_WIDGET_HEIGHT +  pady));

      // replace background screen by save one
  SDL_BlitSurface(old_screen, NULL, screen, &r);
  h += MENU_WIDGET_HEIGHT + pady;

  SDL_FreeSurface(widget);
  SDL_FreeSurface(old_screen);

      // Resizing old_screen and menu !

      // Save all the old_screen .. to erase
  old_screen = SDL_CreateRGBSurface (SDL_HWSURFACE, w, h,
                                     screen->format->BitsPerPixel,
                                     screen->format->Rmask,
                                     screen->format->Gmask,
                                     screen->format->Bmask,
                                     screen->format->Amask);
      // Create the menu
  widget = SDL_CreateRGBSurface (SDL_HWSURFACE, w, h,
                                 screen->format->BitsPerPixel,
                                 screen->format->Rmask,
                                 screen->format->Gmask,
                                 screen->format->Bmask,
                                 screen->format->Amask);

  r.h=h; r.w=w;
  SDL_BlitSurface(screen, &r, old_screen, NULL);
}

void Menu::addWidget(Widget* wt, const uint x, const uint y)
{
  list_widget.push_back(wt);
  wt->set_x(x);
  wt->set_y(y);
      // adapt the size of the menu
  if(wt->get_width() > w)
  {
    w = (wt->get_width() + 2 * padx);
    SDL_FreeSurface(widget);
    SDL_FreeSurface(old_screen);

        // Resizing old_screen and menu !

        // Save all the old_screen .. to erase
    old_screen = SDL_CreateRGBSurface (SDL_HWSURFACE, w, h,
                                       screen->format->BitsPerPixel,
                                       screen->format->Rmask,
                                       screen->format->Gmask,
                                       screen->format->Bmask,
                                       screen->format->Amask);
        // Create the menu
    widget = SDL_CreateRGBSurface (SDL_HWSURFACE, w, h,
                                   screen->format->BitsPerPixel,
                                   screen->format->Rmask,
                                   screen->format->Gmask,
                                   screen->format->Bmask,
                                   screen->format->Amask);

    SDL_Rect r = {get_wx(), get_wy(), w, h};  //relative to window
    SDL_BlitSurface(screen, &r, old_screen, NULL);
  }
}

void Menu::deleteWidget(const ushort indice)
{
  if (list_widget.at (indice))
  {
        // delete elt
    vector <Widget*>::iterator b = list_widget.begin () + indice;
    delete(*b);
    list_widget.erase(list_widget.begin() + indice);

    SDL_Rect r = {get_wx(), get_wy(), w, h};  //relative to window
        // replace background screen by save one
    SDL_BlitSurface(old_screen, NULL, screen, &r);
    h -= MENU_WIDGET_HEIGHT + pady;

    SDL_FreeSurface(widget);
    SDL_FreeSurface(old_screen);

        // Resizing old_screen and menu !

        // Save all the old_screen .. to erase
    old_screen = SDL_CreateRGBSurface (SDL_HWSURFACE, w, h,
                                       screen->format->BitsPerPixel,
                                       screen->format->Rmask,
                                       screen->format->Gmask,
                                       screen->format->Bmask,
                                       screen->format->Amask);
    SDL_BlitSurface (screen, NULL, old_screen, NULL);

        // Create the menu
    widget = SDL_CreateRGBSurface (SDL_HWSURFACE, w, h,
                                   screen->format->BitsPerPixel,
                                   screen->format->Rmask,
                                   screen->format->Gmask,
                                   screen->format->Bmask,
                                   screen->format->Amask);

    r.h = h;
    SDL_BlitSurface(screen, &r, old_screen, NULL);
  }
}

void Menu::draw()
{
  if (visible)
  {
    refresh();
  }
  else
  {
    SDL_Rect r = {get_wx(), get_wy(), w, h};  //relative to window
        //sav background
    SDL_BlitSurface(screen, &r, old_screen, NULL);
        //draw background
    SDL_BlitSurface(screen, &r, widget, NULL);
        // box
    boxRGBA(widget, 0, 0, w, h, bgColor.r, bgColor.g, bgColor.b, MENU_ALPHA);
    rectangleRGBA(widget, 0, 0, w - 1, h - 1, 0, 0, 0, MENU_ALPHA); // w-1 and h-1 because x,y start in 0

        // add title
    if (title != "")
    {
      SDL_Rect txtr = {0, 0, w, MENU_WIDGET_HEIGHT};  //relative to menu
      PrintStrings(widget, font, title, txtr, fgColor);
    }

        // add box
    SDL_BlitSurface(widget, NULL, screen, &r);
        // flip
    SDL_Flip(screen);

        //and add widgets ..
    for (vector <Widget*>::iterator b = list_widget.begin();
         b != list_widget.end(); ++b)
    {
      (*b)->draw();
    }
    visible = true;
  }
}

void Menu::refresh()
{
  if(!visible)
  {
    draw();
  }
  else
  {
    SDL_Rect r = {get_wx(), get_wy(), w, h};  //relative to window
        // replace background screen by save one
    SDL_BlitSurface(old_screen, NULL, widget, NULL);
        // box
    boxRGBA(widget, 0, 0, w - 1, h - 1, bgColor.r, bgColor.g, bgColor.b,
            MENU_ALPHA);
    rectangleRGBA(widget, 0, 0, w - 1, h - 1, 0, 0, 0, MENU_ALPHA); // w-1 and h-1 because x,y start in 0        

        // add title
    if (title != "")
    {
      SDL_Rect txtr = {0, 0, w, MENU_WIDGET_HEIGHT};  //relative to menu
      PrintStrings(widget, font, title, txtr, fgColor);
    }
        // add box
    SDL_BlitSurface(widget, NULL, screen, &r);

        // flip
    SDL_Flip(screen);

        //and add widgets ..
    for (vector <Widget*>::iterator b = list_widget.begin ();
         b != list_widget.end (); ++b)
    {
      (*b)->draw();
    }
  }
}

void Menu::erase()
{
  SDL_Rect r = {get_wx(), get_wy(), w, h};  //relative to window
      // replace background screen by save one
  SDL_BlitSurface (old_screen, NULL, screen, &r);
  SDL_Flip(screen);
}

void Menu::setbgColor(SDL_Color bgColor)
{
  this->bgColor = bgColor;
  refresh();
}

void Menu::setfgColor(SDL_Color fgColor)
{
  this->fgColor = fgColor;
  refresh();
}

ushort Menu::mousebuttondown(const uint mousex, const uint mousey)
{
      // test if mouse pointer is inside a widget
  for (vector <Widget*>::iterator b = list_widget.begin();
       b != list_widget.end(); ++b)
  {
    if ((*b)->inside(mousex, mousey))
    {
      return((*b)->mousebuttondown(mousex, mousey));
    }
  }
  return 0;
}

void Menu::mousemotion (const uint mousex, const uint mousey)
{
  for (vector <Widget*>::iterator b = list_widget.begin ();
       b != list_widget.end (); ++b)
  {
    ((*b)->mousemotion(mousex, mousey));
  }
}
