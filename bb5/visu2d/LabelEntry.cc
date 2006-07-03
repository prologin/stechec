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

#include "LabelEntry.hh"
#include <iostream>

LabelEntry::LabelEntry(const uint x, const uint y, const uint w, const uint h, 
                       SDL_Surface* screen, Widget* father, const string labeltxt, string* txt):
Widget(x, y, w, h, screen, father)
{
  try
  {
    label=new Label(screen, this, labeltxt);
  }
  catch(GUIError &lberror)
  {
    GUIError error(lberror.getMessage());
    throw(error);
  }
  if(label->get_width() > w)
  {
    GUIError error(string ("The width of LabelEntry is lower than the width of the Label !!"));
    throw(error);
  }
      // Now it's possible to know the width of the label
      // and we can to adjust the x coordinate of the entry and the width of the entry
  try
  {
    entry = new Entry(label->get_width(), 0, (w - (label->get_width())), h, txt, screen, this);
  }
  catch(GUIError &enterror)
  {
    GUIError error(enterror.getMessage());
    throw(error);
  }
}

LabelEntry::~LabelEntry()
{
  delete(label);
  delete(entry);
}

void LabelEntry::draw()
{
  if(visible)
  {
    refresh();
  }
  else
  {
    SDL_Rect r = {get_wx(), get_wy(), w, h};
        // First save background
    SDL_BlitSurface(screen, &r, old_screen, NULL);
        // add Label
    label->draw();
        // add entry
    entry->draw();
    visible = true;
  }
}

void LabelEntry::refresh()
{
  if(!visible)
  {
    draw();
  }
  else
  {
    SDL_Rect r = {get_wx(), get_wy(), w, h};
        // Draw save background
    SDL_BlitSurface (old_screen, NULL, widget, NULL);
        // Flip LabelEntry
    SDL_BlitSurface(widget, NULL, screen, &r);
    SDL_Flip(screen);
        // add Label
    label->refresh();
        // add entry
    entry->refresh();
  }
}

    // Focus
void LabelEntry::gainfocus()
{
  focus = true;
  entry->gainfocus();
}

void LabelEntry::losefocus()
{
  focus = false;
  entry->losefocus();
}

    // Txt
void LabelEntry::add_char(const char c)
{
  entry->add_char(c);
}

void LabelEntry::delete_char()
{
  entry->delete_char();
}

ushort LabelEntry::get_index()
{
  return(entry->get_index());
}

void LabelEntry::set_index(const ushort i)
{
  entry->set_index(i);
}

void LabelEntry::set_txt(string* txt)
{
  entry->set_txt(txt);
}

string LabelEntry::get_txt()
{
  return(entry->get_txt());
}

