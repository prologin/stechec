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

#include "Entry.hh"

#include <iostream>

Entry::Entry(const uint x, const uint y, const uint w, const uint h, string* ptxt,
             SDL_Surface * screen, Widget* father): Widget(x, y, w, h, screen, father)
{
  font = TTF_OpenFont (ADD_FONTS_PATH ("Vera.ttf"), 14);
      //If there was an error in loading the font 
  if (font == NULL)
  {
    GUIError error (string ("Not found Vera.ttf"));
    throw (error);
  }
  txt = ptxt;
  (*txt) = " ";
  index = 0;

  fgColor = black_color;
  bgColor = lightgray_color;
  bgColorFocus = white_color;
}

Entry::Entry(const uint x, const uint y, string* ptxt,
             SDL_Surface* screen, Widget* father): Widget(x, y, screen, father)
{
  font = TTF_OpenFont (ADD_FONTS_PATH ("Vera.ttf"), 14);
      //If there was an error in loading the font 
  if (font == NULL)
  {
    GUIError error (string ("Not found Vera.ttf"));
    throw (error);
  }
  txt = ptxt;(*txt) = " ";
      //  (this->txt)->insert((this->txt)->size(),1,' ');
  index = 0;

  fgColor = black_color;
  bgColor = lightgray_color;
  bgColorFocus = white_color;
}

Entry::~Entry ()
{
  TTF_CloseFont (font);
}

void Entry::draw()
{
  SDL_Rect r = {get_wx(), get_wy(), w, h};

      // First save background
  SDL_BlitSurface(screen, &r, old_screen, NULL);
      // draw bg
  SDL_BlitSurface(screen, &r, widget, NULL);
      // Draw box entry
  boxRGBA(widget, 0, 0, w, h, bgColor.r, bgColor.g, bgColor.b, ENTRY_ALPHA);
  rectangleRGBA(widget, 0, 0, w - 1, h - 1, 0, 0, 0, ENTRY_ALPHA);

      // draw text
  int width = 0, height = 0;
  TTF_SizeText(font, txt->c_str(), &width, &height);
  SDL_Surface *temp = TTF_RenderText_Shaded(font, txt->c_str(), bgColor, fgColor);
  SDL_Rect rc = {5, height/ 2, width, height};
  SDL_BlitSurface(temp, NULL, widget, &rc);
  SDL_FreeSurface(temp);

      // Then flip entry
  SDL_BlitSurface(widget, NULL, screen, &r);
  SDL_Flip(screen);
}

void Entry::refresh()
{
  SDL_Rect r = {get_wx(), get_wy(), w, h};
      // First draw save background
  SDL_BlitSurface (old_screen, NULL, widget, NULL);
      // Draw box entry
  SDL_Color* bg = &bgColor;
  if(focus)
  {
    bg = &bgColorFocus;
  }
  boxRGBA(widget, 0, 0, w, h, (*bg).r, (*bg).g, (*bg).b, ENTRY_ALPHA);
  rectangleRGBA(widget, 0, 0, w - 1, h - 1, 0, 0, 0, ENTRY_ALPHA);
      // draw text
  if(txt->size() > 0)
  {
    SDL_Surface *temp = NULL;
    int width = 0, height = 0;
    if(index == 0)
    {
      string st1((*txt), 0, 1);
      TTF_SizeText(font, st1.c_str(), &width, &height);
      temp = TTF_RenderText_Shaded(font, st1.c_str(), bgColor, fgColor);
      SDL_Rect rc = {5, height/ 2, width, height};
      SDL_BlitSurface(temp, NULL, widget, &rc);
      if(txt->size() > 1)
      {
        rc.x += width;
        string st2((*txt), 1, txt->size() - 1);
        TTF_SizeText(font, st2.c_str(), &width, &height);
        temp = TTF_RenderText_Solid(font, st2.c_str(), fgColor);
        rc.w=width; rc.h=height;
        SDL_BlitSurface(temp, NULL, widget, &rc);
      }
    }
    else if(index == txt->size() - 1)
    {
      string st1((*txt), 0, index);
      TTF_SizeText(font, st1.c_str(), &width, &height);
      temp = TTF_RenderText_Solid(font, st1.c_str(), fgColor);
      SDL_Rect rc = {5, height / 2, width, height};
      SDL_BlitSurface(temp, NULL, widget, &rc);
      rc.x += width;
      string st2((*txt), index, 1);
      TTF_SizeText(font, st2.c_str(), &width, &height);
      temp = TTF_RenderText_Shaded(font, st2.c_str(), bgColor, fgColor);
      rc.w=width; rc.h=height;
      SDL_BlitSurface(temp, NULL, widget, &rc);
    }
    else
    {
      string st1((*txt), 0, index);
      TTF_SizeText(font, st1.c_str(), &width, &height);
      temp = TTF_RenderText_Solid(font, st1.c_str(), fgColor);
      SDL_Rect rc = {5, height / 2, width, height};
      SDL_BlitSurface(temp, NULL, widget, &rc);
      rc.x += width;
      string st2((*txt), index, 1);
      TTF_SizeText(font, st2.c_str(), &width, &height);
      temp = TTF_RenderText_Shaded(font, st2.c_str(), bgColor, fgColor);
      rc.w=width; rc.h=height;
      SDL_BlitSurface(temp, NULL, widget, &rc);
      rc.x += width;
      string st3((*txt), index + 1, txt->size() - index);
      temp = TTF_RenderText_Solid(font, st3.c_str(), fgColor);
      rc.w=width; rc.h=height;
      SDL_BlitSurface(temp, NULL, widget, &rc);
    }
    SDL_FreeSurface(temp);
  }
      // Then flip entry
  SDL_BlitSurface(widget, NULL, screen, &r);
  SDL_Flip(screen);
}

    // Focus
void Entry::gainfocus()
{
  focus = true;
  refresh();
}

void Entry::losefocus()
{
  focus = false;
  refresh();
}

    // Txt
void Entry::add_char(const char c)
{
  txt->insert(index, 1, c);
}

void Entry::delete_char()
{
  if(index != (txt->size() - 1))
  {
    txt->erase(index , 1);
  }
}

ushort Entry::get_index()
{
  return(index);
}

void Entry::set_index(const ushort i)
{
  index = (i >= txt->size()) ? (txt->size() - 1) : i;
}

void Entry::set_txt(string* txt)
{
  this->txt=txt;
}

string Entry::get_txt()
{
  return (*txt);
}

    // Color
void Entry::setbgColor(SDL_Color bgColor)
{
  this->bgColor=bgColor;
}

void Entry::setfgColor(SDL_Color fgColor)
{
  this->fgColor=fgColor;
}
