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
#include "VirtualScrollableSurface.hh"

/*!
** This file demonstrates an example of VirtualScrollableSurface,
** used in bb5 to display the field.
*/

int main()
{
  Log log(3);
  
  SDLWindow win(NULL);
  win.init();

  ResourceCenter::getInst()->setResourcePrefix("./");

  // create our VSSurface.
  VirtualScrollableSurface vs("VStest", win.getInput(), Point(400, 300), Point(1200, 800));
  vs.setPos(100, 100);
  win.getScreen().addChild(&vs);

  // add a ground
  Surface ground("ground_1200_800.png");
  vs.addChild(&ground);
  
  while (true)
    {
      
      if (win.processOneFrame())
	break;
    }
  return 0;
}
