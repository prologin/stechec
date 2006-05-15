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

#include <SDL_ttf.h>

#include "Game.hh"

Game::Game(SDLWindow& win, xml::XMLConfig* xml, Api* api, ClientCx* ccx)
  : win_(win),
    xml_(xml),
    api_(api),
    ccx_(ccx)
{
}

Game::~Game()
{
}

int Game::run()
{
  // This is just for demo. They should go into Game.hh/other classes.
  VirtualSurface panel("VPanel", 300, 600);
  panel.setPos(Point(500, 0));
  win_.getScreen().addChild(&panel);

  Surface panel_bg("image/panel/panel");
  panel.addChild(&panel_bg);

  Sprite wheel("image/panel/wheels");
  wheel.setZ(1);
  wheel.setPos(50, 50);
  wheel.splitSurfaceIntoAnim(13, 1);
  wheel.anim(250);
  panel.addChild(&wheel);

  Sprite wheel2(wheel);
  wheel2.setPos(150, 350);
  wheel2.anim(20);
  wheel2.move(Point(200, 500), 10.);
  panel.addChild(&wheel2);

  Surface bg("image/screens/title_bg");
  bg.setZ(-1);
  win_.getScreen().addChild(&bg);

  Sprite ball("image/general/ball");
  ball.setZ(2);
  ball.setPos(10,550);
  ball.move(Point(50, 50), 20.);
  win_.getScreen().addChild(&ball);

  Sprite ball2("image/general/ball");
  ball2.setZ(2);
  ball2.setPos(100, 0);
  ball2.move(Point(500, 550), 15.);
  win_.getScreen().addChild(&ball2);

  Sprite ovni("image/figs/ovni");
  ovni.setZ(3);
  ovni.splitSurfaceIntoAnim(4, 3);
  ovni.setPos(200, 100);
  ovni.move(300, 500, 15.);
  win_.getScreen().addChild(&ovni);
  
  // Sit back and see what's happening...
  while (api_->getState() != GS_END)
    {
      // Process any rules incoming messages.
      while (ccx_->process())
        ;
      // Update the whole app, and print one frame.
      if (win_.processOneFrame())
        break;

      if (win_.getInput().key_pressed_['b'])
        ovni.anim(1, false);
      if (win_.getInput().key_pressed_['p'])
        {
          LOG2("key pressed: p");
          ovni.setZoom(ovni.getZoom() + 1.);
        }
      if (win_.getInput().key_pressed_['o'])
        {
          LOG2("key pressed: o");
          ovni.setZoom(ovni.getZoom() - 1.);
        }
    }
  return 0;
}

// Library entry point. Called by the generic client 'tbt'.
extern "C" int run(xml::XMLConfig* xml, Api* api, ClientCx* ccx)
{
  // First, create and open the SDL window.
  SDLWindow win(xml);
  try {
    win.init();
  } catch (...) {
    LOG1("Sorry, I'm unable to open up a window... You're out of luck :/");
    return 1;
  }

  try {
    // Then, create our application (constructors can already use the window).
    Game padaboum(win, xml, api, ccx);
  
    // Finally, run the app' !
    return padaboum.run();
  } catch (...) {
    return 2;
  }
}
