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

  for (int i = 0; i < 16; i++)
    {
      player_[0][i] = NULL;
      player_[1][i] = NULL;
    }
}

Game::~Game()
{
  win_.getScreen().removeChild(&panel_);
  win_.getScreen().removeChild(&field_);

  for (int i = 0; i < 16; i++)
    {
      delete player_[0][i];
      delete player_[1][i];
    }
}

Input& Game::getInput()
{
  return win_.getInput();
}


void Game::evPlayerPos(int team_id, int player_id, const Point& pos)
{
  VisuPlayer* p = player_[team_id][player_id - 1];

  // First msg means player creation
  if (player_[team_id][player_id - 1] == NULL)
    {
      // Get added player infos.
      api_->switchTeam(team_id);  
      const CPlayer* ap = api_->getPlayer(player_id);

      // Create it.
      p = new VisuPlayer(api_, panel_, field_, *this, ap);
      player_[team_id][player_id - 1] = p;

      // Set its property
      p->load("image/figs/amazon");
      p->splitSizeFrame(40, 40);
      p->setZ(3);
      p->setFrame(ap->getPlayerPosition() * 2 + 1);
      field_.addChild(p);
    }
  p->setPos(pos * 40);
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

  TextSurface fps("font/Vera", 70, 25);
  fps.setZ(4);
  fps.setPos(720, 570);
  win_.getScreen().addChild(&fps);

  TextSurface status("font/Vera", 160, 25);
  status.setZ(4);
  status.setPos(0, 575);
  win_.getScreen().addChild(&status);
  
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

      // Print some things
      std::ostringstream os;
      os << "FPS: " << win_.getFps();
      fps.setText(os.rdbuf()->str());

      switch (api_->getState())
        {
        case GS_WAIT: status.setText("Status: GS_WAIT"); break;
        case GS_INITGAME: status.setText("Status: GS_INITGAME"); break;
        case GS_INITHALF: status.setText("Status: GS_INITHALF"); break;
        case GS_COACH1: status.setText("Status: GS_COACH1"); break;
        case GS_COACH2: status.setText("Status: GS_COACH2"); break;
        case GS_PAUSE: status.setText("Status: GS_PAUSE"); break;
        default: status.setText("Status: other"); break;
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
