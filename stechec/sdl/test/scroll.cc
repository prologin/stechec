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
#include "Sprite.hh"

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
  vs.setPos(Point(100, 100));
  vs.setAutomaticAdjust(true);
  win.getScreen().addChild(&vs);
  LOG1(vs.getRect());

  // add a ground
  Surface ground("ground_1200_800.png");
  vs.addChild(&ground);

  // add a sprite
  Sprite s("ovni.png");
  s.setZ(1);
  s.splitNbFrame(4, 3);
  s.setPos(150, 50);
  vs.addChild(&s);
  
  while (true)
    {
      Input& i = win.getInput();

      if (i.key_pressed_['p'])
	vs.setZoom(vs.getZoom() * 1.2);
      if (i.key_pressed_['o'])
	vs.setZoom(vs.getZoom() * 0.8);
      if (i.key_pressed_['q'])
	break;

      if (i.button_pressed_[1])
	{
	  if (s.getScreenRect().inside(i.mouse_))
	    s.anim(50, false);
	}

      if (win.processOneFrame())
	break;
    }
  return 0;
}
