/*
** TowBowlTactics, an adaptation of the tabletop game Blood Bowl.
**
** Copyright (C) 2006, 2007 The TBT Team.
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
#include "InputTextSurface.hh"

/*!
** @file text.cc
** This file demonstrates some usages of TextSurface.
*/

int main(int, char**)
{
  Log log(5);

  SDLWindow win;
  win.init(NULL);
  Input& inp = win.getInput();

  // To get images from the current directory.
  ResourceCenter::getInst()->setResourcePrefix("./");

  // Get the main VirtualSurface.
  VirtualSurface& screen = win.getScreen();

  // Background
  Surface ground("ground_1200_800.png");
  ground.setPos(-30, -30);
  screen.addChild(&ground);

  // Kludge. Get font from installed version of stechec.
  ResourceCenter::getInst()->setResourcePrefix();

  // Some text to draw
  TextSurface text("Vera.ttf", 24, 300, 50);
  text.setText("'lala' production(tm).");
  text.setPos(75, 120);
  text.setZ(1);
  screen.addChild(&text);

  TextSurface text2("Vera.ttf", 12, 300, 50);
  text2.addText("par ici, il fait beau");
  text2.addText("tres beau.");
  text2.addText("tres tres beau.");
  text2.setPos(75, 400);
  text2.setZ(1);
  screen.addChild(&text2);

  TextSurface text3("Bobtag.ttf", 34, 300, 50);
  text3.addText("BOB tag");
  text3.setPos(475, 200);
  text3.setZ(1);
  screen.addChild(&text3);

  TextSurface text4("Tolo.ttf", 26, 350, 200);
  text4.addText("TOLOlololo looks like that, isn't is funny ?");
  text4.setPos(400, 400);
  text4.setZ(1);
  screen.addChild(&text4);

  static SDL_Color white_color = { 255, 255, 255, SDL_ALPHA_OPAQUE };
  InputTextSurface it("Vera.ttf", 300);
  it.setPos(10, 10);
  it.setZ(2);
  it.setTextColor(white_color);
  it.setRenderMethod(eTextSolid);
  screen.addChild(&it);

  Timer t1(3);
  t1.start();

  while (true)
    {
      if (it.getRect().inside(inp.mouse_) && inp.button_pressed_[1])
	it.acquireInput("lala");
      if (it.isAcquireFinished())
	it.resetAcquire();

      if (win.processOneFrame())
	break;
      if (t1.isTimeElapsed())
	{
	  if (text3.getText() == "BOB tag")
	    text3.setText("Boby !!!");
	  else
	    text3.setText("BOB tag");
	  t1.restart();
	}
    }
  return 0;
 }
