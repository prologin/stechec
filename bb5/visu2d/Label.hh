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

#ifndef LABEL_H
# define LABEL_H

# include "Colors.hh"
# include "Global.hh"
# include "GuiError.hh"
# include "Widget.hh"

# include <SDL_gfxPrimitives.h>

/*!
** @ingroup tbt_2d_gui
*/
class Label: public Widget
{
    protected:
  TTF_Font* font;
  string txt;
  SDL_Color fgColor;
  SDL_Color bgColor;

    public:
  Label(const uint x, const uint y, SDL_Surface * screen, Widget * father, const string txt = "");
  Label(SDL_Surface * screen, Widget * father, const string txt = "");
  ~Label();

      // Draw
  void draw();
  void refresh();

      // Mouse
  void mousemotion(const uint mousex, const uint mousey){};

      // Label
  void set_txt(const string txt);
  string get_txt();

      // Color

      //! @brief Method to set the value of background color of the text
      //! @note It's necessary to do a refresh() after to redraw the label
      //!
      //! @param bgColor is the new background color of the text
  void setbgColor(SDL_Color bgColor);

      //! @brief Method to get the value of the background color of the text
      //!
      //! @return the color in SDL_Color format
  SDL_Color getbgColor()
    {
      return(bgColor);
    }

      //! @brief Method to set the value of foreground color of the txt
      //!
      //! @param fgColor is the new foreground color of the txt
  void setfgColor(SDL_Color fgColor);

      //! @brief Method to get the value of the foreground color of the txt
      //!
      //! @return the color in SDL_Color format
  SDL_Color getfgColor()
    {
      return(fgColor);
    }
};

#endif /*LABEL_H */
