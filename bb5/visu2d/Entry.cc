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
#include "Entry.hh"

Entry::Entry(const uint x, const uint y, const uint w, const uint h, string* ptxt,
             SDL_Surface * screen, Widget* father): Widget(x, y, w, h, screen, father)
{
  font = ResourceCenter::getInst()->getFont("Vera.ttf", 14);
  txt = ptxt;
  if(txt->empty())
  {
    (*txt) = " ";
  }
  else
  {
    size_t last = txt->length() - 1;
    if((*txt).at(last) != ' ')
    {
      (*txt) += " ";
    }
  }
      // Warning length() = last elt + 1 !
  index = txt->length() - 1;
  
  fgColor = black_color;
  bgColor = lightgray_color;
  bgColorFocus = white_color;
}

Entry::Entry(const uint x, const uint y, string* ptxt,
             SDL_Surface* screen, Widget* father): Widget(x, y, screen, father)
{
  font = ResourceCenter::getInst()->getFont("Vera.ttf", 14);
  txt = ptxt;
  (*txt) = " ";
  index = 0;
  
  fgColor = black_color;
  bgColor = lightgray_color;
  bgColorFocus = white_color;
}

Entry::~Entry ()
{
  ResourceCenter::getInst()->releaseFont(font);
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

    // Keyboard
void Entry::keydown(const SDL_keysym* keysym)
{
	if(keysym->unicode<127 and keysym->unicode>=32)
  {
  	// ASCII character
    add_char(keysym->unicode & 0x7F);
    set_index(++index);
    refresh();   
  }
  else
  {
    switch(keysym->sym)
    {
      case SDLK_DELETE:
      {
      	delete_char();
        refresh();
        break;
      }
      case SDLK_BACKSPACE:
      {
        if(index > 0)
        {
          set_index(--index);
          delete_char();
          refresh();
        }
        break;
      }
      case SDLK_LEFT:
      {
        if(index > 0)
        {
          set_index(--index);
          refresh();
        }
        break;
      }
      case SDLK_RIGHT:
      {
        set_index(++index);
        refresh(); 
        break;
      }
      default:
        break;
    }
  }
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
