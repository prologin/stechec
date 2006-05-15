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
#define BUTTON_H

#include <SDL_gfxPrimitives.h>
#include "Global.hh"
#include "Colors.hh"

#define BUTTON_ALPHA 200

/*! @brief Master Button Class
**
** This class create a button
** Warning, a button is handled compared to its container
**
** This class is an abstract class, to create a button
** use one of derivated class : ImageButton or TextButton
**
** @author poltuiu
** @date 2006-03-16
*/
class Button
{
    protected:
  SDL_Surface *screen, *button, *old_surface;

  uint x, y; //coordinates relative to father (warning);
  uint w, h; //size
  bool visible;
  
    public:
  Button(const uint, const uint, const uint, const uint, SDL_Surface*);
  ~Button();
  
  //! @brief Method to test the position of the mouse pointer compared
  //! to the button
  //!
  //! @param mousex is the coordinate x of the mouse pointer
  //! @param mousey is the coordinate y of the mouse pointer
  //! @param dialogx is the coordiante x of the father element of the button
  //! @param dialogy is the coordiante x of the father element of the button
  //!
  // @return 1 if the pointer is inside 0 else
  ushort inside(const uint mousex, const uint mousey,
                const uint dialogx, const uint dialogy);
  
  //! @brief Virtual Method to draw the button
  virtual void draw(const uint, const uint) = 0;

  //! @brief Method to erase the button
  //!
  //! @note in this method coordinates are in the window and not in the father element
  //!
  //! @param posx coordinate x of the top-left corner of the button (in the window)
  //! @param posy coordinate y of the top-left corner of the button (in the window)
  void erase(const uint posx, const uint posy);

  //! @brief Virtual Method to do an action when mouse is moved
  virtual void mousemotion(const uint, const uint,
                           const uint, const uint)=0;

  
  //! @brief Method to do an action when mouse button is down
  //!
  //! return inside(..) value
  //!
  //! @param mousex is the coordinate x of the mouse pointer
  //! @param mousey is the coordinate y of the mouse pointer
  //! @param dialogx is the coordiante x of the father element of the button
  //! @param dialogy is the coordiante x of the father element of the button
  //!
  //! @return 1 or 0
  ushort mousebuttondown(const uint mousex, const uint mousey,
                         const uint dialogx, const uint dialogy);
  
  //! @brief Method to get the x coordinate of the button
  //!
  //! @return the x coordinate
  uint getx();
  
  //! @brief Method to get the y coordinate of the button
  //!
  //! @return the y coordinate
  uint gety();
};

/*! @brief Image Button Class
**
** This class create a button with an image
**
** @author poltuiu
** @date 2006-03-27
*/
class ImageButton : public Button
{
  SDL_Surface *image;

    public:
  ImageButton(const uint, const uint, const uint, const uint,
              SDL_Surface*,const std::string filename);
  ~ImageButton();

  //! @brief Method to draw the button
  //!
  //! @note in this method coordinates are in the window and not in the father element
  //!
  //! @param posx coordinate x of the top-left corner of the button (in the window)
  //! @param posy coordinate y of the top-left corner of the button (in the window)
  void draw(const uint posx, const uint posy);

  //! @brief Method to do an action when mouse is moved
  //!
  //! if the mouse pointer is inside the button 
  //!   if the button is visible -> invisible
  //!   if the button is invisible -> visible
  //!
  //! @param mousex is the coordinate x of the mouse pointer
  //! @param mousey is the coordinate y of the mouse pointer
  //! @param dialogx is the coordiante x of the father element of the button
  //! @param dialogy is the coordiante x of the father element of the button
  void mousemotion(const uint mousex, const uint mousey,
                   const uint dialogx, const uint dialogy);
};

/*! @brief Text Button Class
**
** This class create a button with a text
**
** @author poltuiu
** @date 2006-03-27
*/
class TextButton : public Button
{
  std::string txt; // text of the button
  TTF_Font* font;
  SDL_Color textColor, textColorMotion; // Color of the button in default or motion mode
  SDL_Color bgColor, bgColorMotion;  // Color of the background of the button in default or motion mode
  
    public:
  TextButton(const uint, const uint, const uint, const uint,
             SDL_Surface*, const std::string txt);
  ~TextButton();

  //! @brief Method to draw the button when mouse pointer is inside
  //!
  //! @note in this method coordinates are in the window and not in the father element
  //!
  //! @param posx coordinate x of the top-left corner of the button (in the window)
  //! @param posy coordinate y of the top-left corner of the button (in the window)
  //! @param textColor color of the text of the button in SDL_Color format
  //! @param bgColor color of the background of the button in SDL_Color format
  void drawmotion(const uint posx, const uint posy,
                  SDL_Color textColor, SDL_Color bgColor);

  //! @brief Method to draw the button
  //!
  //! @note in this method coordinates are in the window and not in the father element
  //!
  //! @param posx coordinate x of the top-left corner of the button (in the window)
  //! @param posy coordinate y of the top-left corner of the button (in the window)
  void draw(const uint posx, const uint posy);

  //! @brief Method to do an action when mouse is moved
  //!
  //! if the mouse pointer is inside the button 
  //!   if the button is visible -> invisible
  //!   if the button is invisible -> visible
  //!
  //! @param mousex is the coordinate x of the mouse pointer
  //! @param mousey is the coordinate y of the mouse pointer
  //! @param dialogx is the coordiante x of the father element of the button
  //! @param dialogy is the coordiante x of the father element of the button
  void mousemotion(const uint mousex, const uint mousey,
                   const uint dialogx, const uint dialogy);

};

/*! @brief Button Close Class
**
** This class create a button with a "Close" image
**
** @author poltuiu
** @date 2006-03-16
*/
class BClose:public ImageButton
{
    public:
  BClose(SDL_Surface* screen):ImageButton(62,136,114,30,screen,ADD_IMG_PATH("dialog/close.jpg")){};
};

/*! @brief Button Yes Class
**
** This class create a button with a "Yes" image
**
** @author poltuiu
** @date 2006-03-16
*/
class BYes:public ImageButton
{
    public:
  BYes(SDL_Surface* screen):ImageButton(62,135,50,31,screen,ADD_IMG_PATH("dialog/yes.jpg")){};
};

/*! @brief Button No Class
**
** This class create a button with a "No" image
**
** @author poltuiu
** @date 2006-03-16
*/
class BNo:public ImageButton
{
    public:
  BNo(SDL_Surface* screen):ImageButton(130,135,50,31,screen,ADD_IMG_PATH("dialog/no.jpg")){};
};

#endif
