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

/*!@class Widget
 * @brief Master Widget Class
 *
 * This class is a virtual class.
 * You need to use a class which inherit from it to do a widget.
 *
 * @author poltuiu
 * @date 2006-10-27
 */
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
/*! Constructor
 *
 * @param screen is the SDL_Surface in which the widget will be printed
 * @param father is the widget father
 */
  Widget(SDL_Surface* screen, Widget* father = NULL);
  
/*! Constructor
 *
 * @param w is the width of the widget
 * @param h is the height of the widget 
 * @param screen is the SDL_Surface in which the widget will be printed
 * @param father is the widget father
 */
  Widget(const uint w, const uint h, SDL_Surface* screen, Widget* father = NULL);
  
/*! Constructor
 *
 * @param x is the x coordinate of the widget
 * @param y is the y coordinate of the widget
 * @param w is the width of the widget
 * @param h is the height of the widget 
 * @param screen is the SDL_Surface in which the widget will be printed
 * @param father is the widget father
 */
  Widget(const uint x, const uint y, const uint w, const uint h, SDL_Surface* screen, Widget* father=NULL);

/*! Destructor
 */
  virtual ~Widget();

// Drawing methods
 /*! @brief virtual method to draw the widget.
  */
  virtual void draw() = 0;

  /*! @brief virtual method to refresh the widget
   */
  virtual void refresh() = 0;
  
 /*! @brief Method to erase the widget
  * 
  * Replace the screen by the save screen.
  */
  void erase();

// Mouse methods
  /*! @brief virtual method to do something when the mouse is moved
   * 
   * @param mousex is the mouse pointer x coordinate
   * @param mousey is the mouse pointer y coordinate
   */
  virtual void mousemotion(const uint mousex, const uint mousey) = 0;
  
  /*! @brief method to test if the mouse pointer is inside the widget
   * 
   * @param mousex is the mouse pointer x coordinate
   * @param mousey is the mouse pointer y coordinate
   * @return 0 or 1
   */
  ushort inside(const uint mousex, const uint mousey);
  
  /*! @brief virtual method to do something when a button of the mouse is pressed
   * 
   * @param mousex is the mouse pointer x coordinate
   * @param mousey is the mouse pointer y coordinate
   * @return 0 or 1
   */
  virtual ushort mousebuttondown(const uint mousex, const uint mousey);

//keyboard methods
  /*! @brief virtual method to do something when a key of the keyboard is pressed
   * 
   * @note method for textuals widgets
   * @param keysym is the key pressed in Keysym structure
   */
  virtual void keydown(const SDL_keysym* keysym){};

// Focus methods
  /*! @brief virtual method used when the widget gain the focus
   * 
   * The method set focus variable to true ..
   */
  virtual void gainfocus();
  
  /*! @brief virtual method used when the widget lose the focus
   * 
   *  The method set focus variable to false ..
   */
  virtual void losefocus();
  
  /*! @brief virtual method to test tha value of the focus variable
   * 
   *  @return the boolean value of the focus
   */
  virtual bool hasfocus();
  
  /*! @brief Method to return the globfocus
   * 
   * globfocus is a static variable which stores the reference
   * of the widget which has focus
   */
  static Focus& get_focus(); 

// Geometry
  /*! @brief Method to get the x coordinate of the widget in the father coordinate system
   *
   * @return the x coordinate
   */
  uint get_x();

  /*! @brief Method to get the x coordinate of the widget in the window coordinate system
   * 
   * @return the x coordinate
   */
  uint get_wx();

  /*! @brief Method to set the x coordinate of the widget in the father coordinate system
   * 
   * @param x is the value to set
   */
  void set_x(const uint x);

  /*! @brief Method to get the y coordinate of the widget in the father coordinate system
   * 
   *  @return the y coordinate
   */
  uint get_y();

  /*! @brief Method to get the y coordinate of the widget in the window coordinate system
   * 
   *  @return the y coordinate
   */
  uint get_wy();

  /*! @brief Method to set the y coordinate of the widget in the father coordinate system
   * 
   *  @param y is the value to set
   */
  void set_y(const uint y);

  /*! @brief Method to get the width of the widget
   * 
   *  @return the width of the widget
   */
  uint get_width();

  /*! @brief Method to set the width of the widget
   * 
   * @param width is the value to set
   */
  void set_width(const uint width);

  /*! @brief Method to get the height of the widget
   * 
   *  @return the height of the wdget
   */
  uint get_height();

  /*! @brief Method to set the heigth ot the widget
   * 
   * @param height is the value to set
   */
  void set_height(const uint height);

};
#endif
