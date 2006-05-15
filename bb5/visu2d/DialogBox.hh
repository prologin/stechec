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

#ifndef DIALOGBOX_H
#define DIALOGBOX_H

#include "Global.hh"
#include "Button.hh"
#include "Colors.hh"
#include <vector>

/*! @brief Master DialogBox Class
**
** This class is a virtual class.
** You need to use a class which inherit from
** it to do a dialogbox.
**
** @author poltuiu
** @date 2006-03-15
*/
class DialogBox
{
protected:
  SDL_Surface *screen, *old_screen, *dialog_box_surf, *dialog_box_surf_sav;
  SDL_Surface *image; //background image
  uint x, y;          //coordinates
  uint w, h;          //sizes
  std::vector<ImageButton*> list_button;
  bool move_dialog;
  
  //! @brief Method to redraw the dialogbox
  void refresh();

  //! @brief Method to erase dialogbox
  //!
  //! replace dialogbox clip by previous sav screen
  void erase();
  
  //! @brief Method to test the position of the mouse pointer compared 
  //! to the title of the dialogbox
  //!
  //! @param mx is the mouse pointer x coordinate
  //! @param my is the mouse pointer y coordinate
  //
  //! @return 1 if in and 0 else
  ushort insidetitle(const uint mx, const uint my);
  
public:
  DialogBox(const uint, const uint, SDL_Surface*, const std::string);
  virtual ~DialogBox();
 
  //! @brief Method to do an action when the mouse button is up
  //!
  //! @param mx is the mouse pointer x coordinate
  //! @param my is the mouse pointer y coordinate
  //!
  //! @return 1 if do something, 0 else
  ushort mousebuttonup(const uint mx, const uint my);

  //! @brief Virtual Method to do an action when the mouse button is down
  //!
  //! @param mx is the mouse pointer x coordinate
  //! @param my is the mouse pointer y coordinate
  //!
  //! @return 1 if do something, 0 else
  virtual ushort mousebuttondown(const uint mx, const uint my);

  //! @brief Method to do an action when the mouse is moved
  //!
  //! @note this function has twho way :
  //!   - if the dialogbox is in move mode, then move it
  //!   - if not, look at if its buttons must react
  //!
  //! @param mousex is the mouse pointer x coordinate
  //! @param mousey is the mouse pointer y coordinate
  //! @param xrel is the relative motion in x direction
  //! @param yrel is the relative motion in y direction
  //!
  //! @return 1 if do something, 0 else
  ushort mousemotion(const uint mousex, const uint mousey,
                  const uint xrel, const uint yrel);

  //! @brief Virtual Method to draw the dialogbox
  virtual void draw() = 0;

  //! @brief Method to add a button in button list
  //!
  //! @param new_button is the button to add
  void addButton(ImageButton* new_button);
  
  //! @brief Method to return the number of buttons in list_button
  ushort nbrButton();
};

/*! @brief Dinfo Class : a simple Info DialogBox
**
** This class create a dialogbox with :
**  - an icon
**  - an ImageButton to close
**  - a text
** 
** This class inherit from DialogBox
**
** @author poltuiu
** @date 2006-03-15
*/
class DInfo : public DialogBox
{
  std::string message;
  TTF_Font* font;
  SDL_Surface* icone;
  
    public:
  DInfo(SDL_Surface*, const std::string);
  ~DInfo();
  
  //! @brief Method to do an action when mouse button is down
  //!
  //! @note this function has two way :
  //!  - if mouse pointer is inside the title, box pass on move mode
  //!  - if mouse pointer is inside the button, close the box (button close)
  //!
  //! @param mx is the mouse pointer x coordinate
  //! @param my is the mouse pointer y coordinate
  //!
  //! @return 1 if do something, 0 else
  ushort mousebuttondown(const uint mx, const uint my);

  //! @brief Method to draw the dialogbox
  //!
  //! @note to draw the dialogbox, we need to draw all elements
  //! step by step
  void draw();
};

#endif
