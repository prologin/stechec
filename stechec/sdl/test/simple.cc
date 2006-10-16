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

#include "tools.hh"
#include "SDLWindow.hh"
#include "Sprite.hh"
#include "TextSurface.hh"

/*!
** This file demonstrates basic usage of Stechec SDL Engine.
** Basically, it opens up a window, and display:
**  * one static sprite.
**  * one moving sprite.
**  * some text.
*/

int main(int, char**)
{
  // Choose verbosity. 0 is pretty quiet, 8 pretty chatty.
  Log log(3);

  // Initialize window.
  SDLWindow win(NULL);
  win.init();

  // To get images from the current directory.
  ResourceCenter::getInst()->setResourcePrefix("./");
  
  // Get the main VirtualSurface.
  VirtualSurface& screen = win.getScreen();
  
  // Create a static sprite, add it to the screen.
  Surface s1("push.png");
  s1.setPos(250, 100);
  s1.setZ(1);
  screen.addChild(&s1);

  // Create a moving sprite, add it to the screen.
  Sprite s2("push.png");
  s2.setPos(100, 50);
  s2.setZ(1);
  screen.addChild(&s2);

  // Kludge. Get font from installed version of stechec.
  ResourceCenter::getInst()->setResourcePrefix();

  // Some text to draw
  TextSurface text("Vera.ttf", 300, 30);
  text.setText("'lala' production(tm)");
  text.setPos(75, 120);
  screen.addChild(&text);
  
  while (true)
    {
      // Tell the sprite to move back and forth.
      if (s2.getPos() == Point(100, 50))
	s2.move(Point(500, 50), 30.);
      if (s2.getPos() == Point(500, 50))
	s2.move(Point(100, 50), 15.);

      // Hack. Redraw the whole screen each frame.
      // This is because we don't have 'background', so the system
      // can't refresh properly.
      win.clearScreen();
      
      if (win.processOneFrame())
	break;
    }
  return 0;
 }
