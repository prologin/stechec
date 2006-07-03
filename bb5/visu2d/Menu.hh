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

#ifndef MENU_H
#define MENU_H

#include <vector>
#include <SDL_gfxPrimitives.h>

#include "Colors.hh"
#include "Global.hh"
#include "Widget.hh"

#define MENU_WIDTH 200
#define MENU_ALPHA 170
#define MENU_WIDGET_HEIGHT 30
#define MENU_WIDGET_WIDTH 150

/*! @brief Menu Class
**
** A menu is a collection of Widget
** with a simple box color in background.
**
** @author poltuiu
** @date 2006-04-15
*/
class Menu: public Widget
{
  SDL_Color fgColor;		// foreground color
  SDL_Color bgColor;		// background color
  TTF_Font *font;
  ushort padx, pady;
  string title;		// title of the menu
  vector <Widget*> list_widget;

    public:

  //! @brief constructor
  //! 
  //! @param x is the x coordinate of the menu in the window
  //! @param y is the y coordiante of the menu in the window
  //! @param screen is the container father of the menu
  //! @param width is the width of the menu box
  //! @param title, string if menu has title
  Menu(const uint x, const uint y, SDL_Surface * screen,
   const ushort width = MENU_WIDTH, const string title = "");

  //! @brief constructor
  //! 
  //! @param x is the x coordinate of the menu in the window
  //! @param y is the y coordiante of the menu in the window
  //! @param screen is the container father of the menu
  //! @param width is the width of the menu box
  //! @param height is the height of the menu box
  //! @param title, string if menu has title
  Menu(const uint x, const uint y, SDL_Surface * screen,
   const ushort width, const ushort height, const string title = "");

  //! @brief destructor
  ~Menu();

  //! @brief Method to draw the menu
  //! @note The draw method is called to draw the element the first time
  void draw();

  //! @brief Method to refresh the menu
  //! @note This method is used to redraw the element
  void refresh();

  //! @brief Method to erase the menu
  void erase();

  // events
  //! @brief Method to do an action when mouse button is down
  //!
  //! @param mouusex is the coordinate x of the mouse pointer
  //! @param mousey is the coordinate y of the mouse pointer
  //! @return the action associated with the button used
  ushort mousebuttondown(const uint mousex=0, const uint mousey=0);

  //! @brief Method to do an action when the mouse is moved
  //!
  //! This method do mousemotion for each button of the menu
  //!
  //! @param mouusex is the coordinate x of the mouse pointer
  //! @param mousey is the coordinate y of the mouse pointer
  void mousemotion(const uint mousex, const uint mousey);

  // Widgets list

  //! @brief Method to add a widget in the menu
  //!
  //! @param widget is a reference to the widget to add
  //! @note menu is used like a stack : last widget added is the bottom of the menu
  void addWidget(Widget* widget);

  //! @brief Method to add a widget in the menu
  //!
  //! @param widget is a reference to the widget to add
  //! @param x is the x coordinate of the widget (relative to menu box)
  //! @param y is the y coordinate of the widget (relative to menu box)
  //! @note menu is used like a stack : last widget added is the bottom of the menu
  void addWidget(Widget* widget, const uint x, const uint y);

  //! @brief Method to delete a widget in the menu
  //!
  //! @param indice is the index of the button to delete
  void deleteWidget(const ushort indice);

  // graphic
  //! @brief Method to set the value of background color of the menu
  //! @note It's necessary to do a refresh() after to redraw the menu
  //!
  //! @param bgColor is the new background color of the menu
  void setbgColor(SDL_Color bgColor);

  //! @brief Method to get the value of the background color of the menu
  //!
  //! @return the color in SDL_Color format
  SDL_Color getbgColor()
  {
    return(bgColor);
  }

  //! @brief Method to set the value of foreground color of the menu
  //!
  //! @param fgColor is the new foreground color of the menu
  void setfgColor(SDL_Color fgColor);

  //! @brief Method to get the value of the foreground color of the menu
  //!
  //! @return the color in SDL_Color format
  SDL_Color getfgColor()
  {
    return(fgColor);
  }

  ushort getwidth()
  {
    return(w);
  }

  ushort getheight()
  {
    return(h);
  }

  ushort getnbwidget()
  {
    return(list_widget.size ());
  }

  ushort getpadx()
  {
    return(padx);
  }

  ushort getpady()
  {
    return(pady);
  }
};

#endif
