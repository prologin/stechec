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

#ifndef LABELENTRY_H
# define LABELENTRY_H

# include "Entry.hh"
# include "Label.hh"
# include "Global.hh"
# include "GuiError.hh"
# include "Widget.hh"

/*!
** @ingroup tbt_2d_gui
*/
class LabelEntry : public Widget
{
  Label* label;
  Entry* entry;

    public:
  LabelEntry(const uint x, const uint y, const uint w, const uint h,
             SDL_Surface* screen, Widget* father, const string Label, string* txt);
  ~LabelEntry();

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

      // Txt
  void add_char(const char c);
  void delete_char();
  ushort get_index(); 
  void set_index(const ushort i);
  void set_txt(string* txt);
  string get_txt();
};

#endif /*LABELENTRY_H*/
