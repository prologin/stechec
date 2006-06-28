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
    field_(api, win.getInput()),
    action_popup_(api, *this),
    our_turn_(false),
    is_playing_(false),
    action_(eActNone)
{
  api_->setEventHandler(this);
  win_.getScreen().addChild(&panel_);
  win_.getScreen().addChild(&field_);

  txt_status_ = TextSurface("font/Vera", 270, 65);
  txt_status_.setZ(4);
  txt_status_.setPos(3, 532);
  win_.getScreen().addChild(&txt_status_);

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

VirtualSurface& Game::getScreen()
{
  return win_.getScreen();
}

Panel& Game::getPanel()
{
  return panel_;
}

VisuField& Game::getField()
{
  return field_;
}


void Game::evKickOff()
{
  txt_status_.addText("Kickoff. Please place the ball.");
}

void Game::evNewTurn(bool our_turn)
{
  our_turn_ = our_turn;
  is_playing_ = true;
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
      p = new VisuPlayer(api_, *this, ap);
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

void Game::selectPlayer(VisuPlayer* vp)
{
  action_popup_.hide();
  action_popup_.setVisuPlayer(vp);
}

void Game::unselectAllPlayer()
{
  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 16; j++)
      if (player_[i][j] != NULL)
        player_[i][j]->unselect();
}

void Game::addAction(eAction action)
{
  LOG2("PREPARE an action !");
  action_ = action;
}

int Game::run()
{
  int game_state = -1;

  TextSurface fps("font/Vera", 70, 25);
  fps.setZ(4);
  fps.setPos(720, 570);
  win_.getScreen().addChild(&fps);

    
  // Sit back and see what's happening...
  while (api_->getState() != GS_END)
    {
      // Process any rules incoming messages.
      while (ccx_->process())
        ;

      // Update the whole app, and print one frame.
      if (win_.processOneFrame())
        break;

      if (win_.getInput().button_pressed_[3])
        {
          action_ = eActNone;
          action_popup_.show(win_.getInput().mouse_);
        }

      if (win_.getInput().button_pressed_[1])
        {
          if (action_popup_.isVisible())
            action_popup_.hide();
          else if (action_ != eActNone
                   && action_popup_.getVisuPlayer() != NULL)
            {
              LOG2("Do an action !");
              action_popup_.getVisuPlayer()->action(action_);
              action_ = eActNone;
            }
        }


      // Print some things
      std::ostringstream os;
      os << "FPS: " << win_.getFps();
      fps.setText(os.rdbuf()->str());

      if (game_state != api_->getState())
        {
          std::string status;
          switch (api_->getState())
            {
            case GS_WAIT: status = "GS_WAIT"; break;
            case GS_INITGAME: status = "GS_INITGAME"; break;
            case GS_INITHALF: status = "GS_INITHALF"; break;
            case GS_COACH1: status = "GS_COACH1"; break;
            case GS_COACH2: status = "GS_COACH2"; break;
            case GS_PAUSE: status = "GS_PAUSE"; break;
            default: status = "other"; break;
            }
          txt_status_.addText("New game status: " + status);
          game_state = api_->getState();
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
