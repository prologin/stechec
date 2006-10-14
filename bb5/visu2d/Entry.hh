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

#ifndef ENTRY_H
#define ENTRY_H

#include "Global.hh"
#include "Colors.hh"
#include "GuiError.hh"
#include "Widget.hh"

#include <SDL_gfxPrimitives.h>

#define ENTRY_ALPHA 170

class Entry: public Widget
{
    protected:
  TTF_Font* font;
  string* txt;
  ushort index;

  SDL_Color fgColor;
  SDL_Color bgColor, bgColorFocus;  // Color of the background of the entry in default or focus mode

    public:
  Entry(const uint x, const uint y, const uint w, const uint h,
        string* txt, SDL_Surface* screen, Widget* father);
  Entry(const uint w, const uint h, string* txt, SDL_Surface* screen, Widget* father);
  ~Entry(); 

  // Draw
  void draw();
  void refresh();

  // Focus
  void gainfocus();
  void losefocus();

  // Mouse
  //! @brief Method to do an action when mouse is moved
  //! Nothing to do with en entry
  void mousemotion(const uint mousex, const uint mousey){};

  //keyboard
  void keydown(const SDL_keysym* keysym);

  // Txt
  void add_char(const char c);
  void delete_char();
  ushort get_index(); 
  void set_index(const ushort i);
  void set_txt(string* txt);
  string get_txt();

  // Color

  //! @brief Method to set the value of the background color
  //! @note It's necessary to do a refresh() after to redraw all
  //!
  //! @param bgColor is the new background color
  void setbgColor(SDL_Color bgColor);

  //! @brief Method to get the value of the background color
  //!
  //! @return the color in SDL_Color format
  SDL_Color getbgColor()
  {
    return(bgColor);
  }

  //! @brief Method to set the value of the foreground color of the text
  //!
  //! @param fgColor is the new foreground color of the text
  void setfgColor(SDL_Color fgColor);

  //! @brief Method to get the value of the foreground color of the text
  //!
  //! @return the color in SDL_Color format
  SDL_Color getfgColor()
  {
    return(fgColor);
  }
};

#endif
