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

#ifndef WIDGET_H
#  define WIDGET_H
#include "Global.hh"
#include "Focus.hh"

#define WIDGET_HEIGHT 30
#define WIDGET_WIDTH 150
class Focus;

class Widget
{
    protected:
  // screen is the father and old_screen a copy of screen
  SDL_Surface *screen, *old_screen, *widget;
  Widget* father; // father widget
  uint x, y; // in the father coordinate system
  uint w, h; // size
  bool visible;
  bool focus;
  static Focus globfocus;

    public:
  Widget(SDL_Surface* screen, Widget* father = NULL);
  Widget(const uint w, const uint h, SDL_Surface* screen, Widget* father = NULL);
  Widget(const uint x, const uint y, const uint w, const uint h, SDL_Surface* screen, Widget* father=NULL);
  ~Widget();

  // Draw
  virtual void draw() = 0;
  virtual void refresh() = 0;
  void erase();

  // Mouse
  virtual void mousemotion(const uint mousex, const uint mousey) = 0;
  ushort inside(const uint mousex, const uint mousey);
  virtual ushort mousebuttondown(const uint mousex, const uint mousey);

  // Focus
  virtual void gainfocus();
  virtual void losefocus();
  virtual bool hasfocus();
  static Focus& get_focus(); 

  // Geometry
  //! @brief Method to get the x coordinate of the widget
  //!  in the father coordinate system
  //!
  //! @return the x coordinate
  uint get_x();

  //! @brief Method to get the x coordinate of the widget
  //!  in the window coordinate system
  //!
  //! @return the x coordinate
  uint get_wx();

  //! @brief Method to set the x coordinate of the widget
  //!  in the father coordinate system
  void set_x(const uint x);

  //! @brief Method to get the y coordinate of the widget
  //!  in the father coordinate system
  //!
  //! @return the y coordinate
  uint get_y();

  //! @brief Method to get the y coordinate of the widget
  //!  in the window coordinate system
  //!
  //! @return the y coordinate
  uint get_wy();

  //! @brief Method to set the y coordinate of the widget
  //!  in the father coordinate system
  void set_y(const uint y);

  //! @brief Method to get the width the widget
  //!
  //! @return the width of the widget
  uint get_width();

  void set_width(const uint width);

  //! @brief Method to get the height of the widget
  //!
  //! @return the height of the wdget
  uint get_height();

  void set_height(const uint height);

  // Txt .. for textuals widgets
  virtual void add_char(const char c){};
  virtual void delete_char(){};
  virtual ushort get_index(){return 0;}; 
  virtual void set_index(const ushort i){};
};
#endif
