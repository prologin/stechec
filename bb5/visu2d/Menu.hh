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

#include <SDL_gfxPrimitives.h>

#include "Button.hh"
#include "Colors.hh"
#include "Global.hh"

#define MENU_WIDTH 200
#define MENU_BUTTON_WIDTH 150
#define MENU_BUTTON_HEIGHT 30
#define MENU_ALPHA 170

/*! @brief MenuButton Class
**
** A button for  menu is a container with :
**  - an action
**  - a textbutton
**
** Buttons are handled compared to its container 
**
** @author poltuiu
** @date 2006-04-15
*/
class MenuButton
{
  TextButton* button;
  ushort action;
    public :
  //! @brief constructor
  //!
  //! @param x is the x coordinate (relative to its father)
  //! @param y is the y coordinate (relative to its father)
  //! @param w is the width of the button
  //! @param h is the height of the button
  //! @param screen is the container father of the button
  //! @param txt is the text of the button
  //! @param action is the action return by the button when it is used
  MenuButton(const uint x, const uint y, const uint w, const uint h,
             SDL_Surface* screen, const std::string txt, ushort action);
  //! @brief destructor
  ~MenuButton();

  //! @brief Public method to get the textbutton
  //! @return a pointer to the textbutton of the MenuButton
  TextButton* getButton();
  
  //! @brief Public method to get the action
  //! @return the action of the MenuButton
  ushort getAction();
};

/*! @brief Menu Class
**
** A menu is a collection of MenuButton
** with a simple box color in background.
**
** @author poltuiu
** @date 2006-04-15
*/
class Menu
{
      // screen is the father and old_screen a copy of screen
  SDL_Surface *screen, *old_screen, *menu;
  SDL_Color fgColor; // foreground color
  SDL_Color bgColor; // background color
  TTF_Font* font;

  uint x, y; // coordinates in the windows
  uint w, h; // size of the menu
  std::string title; // title of the menu
  ushort nbbutton;
  std::vector<MenuButton*> list_button;

    public:
  
  //! @brief constructor
  //! 
  //! @param x is the x coordinate of the menu in the window
  //! @param y is the y coordiante of the menu in the window
  //! @param nbbutton is the number of buttons of the menu
  //! @param screen is the container father of the menu
  //! @param width is the width of the menu box
  //! @param title, string if menu has title
  Menu(const uint x, const uint y, const ushort nbbutton, SDL_Surface* screen,
       const ushort width=MENU_WIDTH, const std::string title="");
  
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

  //! @brief Method to test the position of the mouse pointer compared
  //!  to the menu
  //!
  //! @param mousex is the coordinate x of the mouse pointer
  //! @param mousey is the coordinate y of the mouse pointer
  //!
  //! @return 1 if the pointer is inside 0 else
  ushort insidemenu(const uint mousex, const uint mousey);

      // events
  //! @brief Method to do an action when mouse button is down
  //!
  //! @param mousexis the coordinate x of the mouse pointer
  //! @param mousey is the coordinate y of the mouse pointer
  //!
  //! @return the action associated with the button used
  ushort mousebuttondown(const uint mousex, const uint mousey);

  //! @brief Method to do an action when the mouse is moved
  //!
  //! This method do mousemotion for each button of the menu
  //!
  //! @param mouusex is the coordinate x of the mouse pointer
  //! @param mousey is the coordinate y of the mouse pointer
  void mousemotion(const uint mousex, const uint mousey);

      // buttons list
  //! @brief Method to add a button in the menu
  //! @note menu is used like a stack : last button added is the bottom of the menu
  //! and x, y coordinates are automatically calculated 
  //!
  //! @param txt is the text of the button
  //! @param action is the action associated with the button
  void addButton(const std::string txt, ushort action);

  //! @brief Method to add a button in the menu
  //!
  //! @param txt is the text of the button
  //! @param action is the action associated with the button
  //! @param x is the x coordinate of the button (relative to menu box)
  //! @param y is the y coordiante of the button (relative to menu box)
  void addButton(const std::string txt, ushort action, const ushort x, const ushort y);

  //! @brief Method to delete a button in the menu
  //!
  //! @param indice is the index of the button to delete
  void deleteButton(const ushort indice);

      // graphic
  //! @brief Method to set the value of background color of the menu
  //! @note It's necessary to do a refresh() after to redraw the menu
  //!
  //! @param bgColor is the new background color of the menu
  void setbgColor(SDL_Color bgColor);
  
  //! @brief Method to get the value of the background color of the menu
  //!
  //! @return the color in SDL_Color format
  SDL_Color getbgColor();

  //! @brief Method to set the value of foreground color of the menu
  //!
  //! @param fgColor is the new foreground color of the menu
  void setfgColor(SDL_Color fgColor);
  
  //! @brief Method to get the value of the foreground color of the menu
  //!
  //! @return the color in SDL_Color format
  SDL_Color getfgColor(); 
};

#endif
