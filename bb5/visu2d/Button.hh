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

#ifndef BUTTON_H
# define BUTTON_H

# include "Global.hh"
# include "Colors.hh"
# include "Widget.hh"

# include <SDL_gfxPrimitives.h>

# define BUTTON_ALPHA 200

/*!
** @brief Image Button Class
** @ingroup tbt_2d_gui
**
** This class create a button with an image
**
** @author poltuiu
** @date 2006-03-27
*/
class ImageButton: public Widget
{
  ushort action; // Action associate to the button
    public:
  ImageButton(const uint x, const uint y, const uint w, const uint h,
              SDL_Surface* screen, Widget* father, const string filename, const ushort action = CLOSE);
  ImageButton(const uint w, const uint h,
              SDL_Surface* screen, Widget* father, const string filename, const ushort action = CLOSE);
  ImageButton(SDL_Surface* screen, Widget* father, const string filename, const ushort action = CLOSE);
  virtual ~ImageButton(){};

  //! @brief Method to draw the button
  //!
  //! @note in this method coordinates are in the window and not in the father element
  void draw();

  void refresh(){};

  //! @brief Method to do an action when mouse is moved
  //!
  //! if the mouse pointer is inside the button 
  //!   if the button is visible -> invisible
  //!   if the button is invisible -> visible
  //!
  //! @param mousex is the coordinate x of the mouse pointer
  //! @param mousey is the coordinate y of the mouse pointer
  void mousemotion(const uint mousex, const uint mousey);

  //! @brief Public method to get the action
  //! @return the action of the Button
  ushort mousebuttondown(const uint mousex = 0, const uint mousey = 0);
};

/*!
** @brief Text Button Class
** @ingroup tbt_2d_gui
**
** This class create a button with a text
**
** @author poltuiu
** @date 2006-03-27
*/
class TextButton : public Widget
{
  string txt; // text of the button
  TTF_Font* font;
  SDL_Color textColor, textColorMotion; // Color of the button in default or motion mode
  SDL_Color bgColor, bgColorMotion;  // Color of the background of the button in default or motion mode
  ushort action; // Action associate to the button

    public:
  TextButton(const uint x, const uint y, const uint w, const uint h,
             SDL_Surface* screen, Widget* father, const string txt, const ushort action = CLOSE);
  TextButton(const uint w, const uint h,
             SDL_Surface* screen, Widget* father, const string txt, const ushort action = CLOSE);
  TextButton(SDL_Surface* screen, Widget* father, const string txt, const ushort action = CLOSE);
  virtual ~TextButton();

  //! @brief Method to draw the button when mouse pointer is inside
  //!
  //! @note in this method coordinates are in the window and not in the father element
  //!
  //! @param textColor color of the text of the button in SDL_Color format
  //! @param bgColor color of the background of the button in SDL_Color format
  void drawmotion(SDL_Color textColor, SDL_Color bgColor);

  //! @brief Method to draw the button
  //!
  //! @note in this method coordinates are in the window and not in the father element
  void draw();

  void refresh(){};

  //! @brief Method to do an action when mouse is moved
  //!
  //! if the mouse pointer is inside the button 
  //!   if the button is visible -> invisible
  //!   if the button is invisible -> visible
  //!
  //! @param mousex is the coordinate x of the mouse pointer
  //! @param mousey is the coordinate y of the mouse pointer
  void mousemotion(const uint mousex, const uint mousey);

  //! @brief Public method to get the action
  //!
  //! @param mousex is the coordinate x of the mouse pointer
  //! @param mousey is the coordinate y of the mouse pointer
  //! @return the action of the Button
  ushort mousebuttondown(const uint mousex = 0, const uint mousey = 0);
};

/*!
** @brief Button Close Class
** @ingroup tbt_2d_gui
**
** This class create a button with a "Close" image
**
** @author poltuiu
** @date 2006-03-16
*/
class BClose:public ImageButton
{
    public:
  BClose(SDL_Surface* screen, Widget* father)
    : ImageButton(62,136,114,30,screen,father,"image/dialog/close.jpg")
  {}
};

/*!
** @brief Button Yes Class
** @ingroup tbt_2d_gui
**
** This class create a button with a "Yes" image
**
** @author poltuiu
** @date 2006-03-16
*/
class BYes:public ImageButton
{
    public:
  BYes(SDL_Surface* screen, Widget* father)
    : ImageButton(62,135,50,31,screen,father,"image/dialog/yes.jpg")
  {}
};

/*!
** @brief Button No Class
** @ingroup tbt_2d_gui
**
** This class create a button with a "No" image
**
** @author poltuiu
** @date 2006-03-16
*/
class BNo:public ImageButton
{
public:
  BNo(SDL_Surface* screen, Widget* father)
    : ImageButton(130,135,50,31,screen,father, "image/dialog/no.jpg")
  {};
};

#endif
