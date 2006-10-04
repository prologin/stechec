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

/*!
** This file demonstrates basic usage of Stechec SDL Engine.
** Basically, it opens up a window, and display one moving sprite.
*/

int main()
{
  Log log(3);
  
  SDLWindow win(NULL);
  win.init();
  VirtualSurface& screen = win.getScreen();
  // Do something with screen, like adding sprites.
 
  while (true)
    {
      Input& inp = win.getInput();
      // Do something, this is your main loop!
      // You can fetch mouse pos/keyboard state into 'inp'.
      if (win.processOneFrame())
	break;
    }
  return 0;
 }
