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
    ccx_(ccx),
    panel_(api, win.getInput()),
    field_(api, win.getInput())
{
  api_->setEventHandler(this);
  win_.getScreen().addChild(&panel_);
  win_.getScreen().addChild(&field_);
}

Game::~Game()
{
  win_.getScreen().removeChild(&panel_);
  win_.getScreen().removeChild(&field_);
}

void Game::evPlayerPos(int team_id, int player_id, const Point& pos)
{
  field_.addPlayer(team_id, player_id, pos);
}
void Game::evBallPos(const Point& pos)
{
  field_.setBallPos(pos);
}

int Game::run()
{
  Sprite ball("image/general/ball");
  ball.setZ(2);
  ball.setPos(10, 550);
  ball.move(Point(50, 50), 20.);
  field_.addChild(&ball);

  Sprite ball2("image/general/ball");
  ball2.setZ(2);
  ball2.setPos(100, 0);
  ball2.move(Point(500, 550), 15.);
  field_.addChild(&ball2);

  Sprite ovni("image/figs/ovni");
  ovni.setZ(2);
  ovni.splitNbFrame(4, 3);
  ovni.setPos(200, 100);
  ovni.move(300, 500, 15.);
  //win_.getScreen().addChild(&ovni);
  
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
