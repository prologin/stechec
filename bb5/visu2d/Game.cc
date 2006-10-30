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
    our_turn_(false),
    is_playing_(false),
    action_(eActNone)
{
  panel_ = new Panel(*this);
  field_ = new VisuField(*this);
  action_popup_ = new ActionPopup(*this);

  api_->setEventHandler(this);
  win_.getScreen().addChild(panel_);
  win_.getScreen().addChild(field_);

  txt_status_ = TextSurface("Vera.ttf", 12, 270, 65);
  txt_status_.setZ(6);
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
  win_.getScreen().removeChild(panel_);
  win_.getScreen().removeChild(field_);
  win_.getScreen().removeChild(&txt_status_);

  for (int i = 0; i < 16; i++)
    {
      delete player_[0][i];
      delete player_[1][i];
    }

  delete panel_;
  delete field_;
  delete action_popup_;
}

Api* Game::getApi()
{
  return api_;
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
  return *panel_;
}

VisuField& Game::getField()
{
  return *field_;
}

void Game::evKickOff()
{
  txt_status_.addText("Kickoff. Please place the ball.");
  field_->playerDoingKickoff();
}

void Game::evNewTurn(bool our_turn)
{
  our_turn_ = our_turn;
  if (our_turn)
    txt_status_.addText("It is your turn... Play !");
    
  is_playing_ = true;
}

void Game::evChat(const std::string& msg)
{
  txt_status_.addText(std::string("<chat> ") + msg);
}

void Game::evPlayerCreate(int team_id, int player_id)
{
  if (player_[team_id][player_id] != NULL)
    {
      WARN("Player was already created! (team_id: %1, id:%2)", team_id, player_id);
      return;
    }
      
  // Get added player api info.
  api_->selectTeam(team_id);  
  const CPlayer* ap = api_->getPlayer(player_id);
  if (ap == NULL)
    {
      ERR("Can't get player api info!");
      return;
    }

  // Create it, and add it to the field.
  VisuPlayer* p = new VisuPlayer(api_, *this, ap);
  player_[team_id][player_id] = p;
  field_->addChild(p);
}

void Game::evPlayerPos(int team_id, int player_id, const Point& pos)
{
  VisuPlayer* p = player_[team_id][player_id];

  p->setPos(pos * 40);
}

void Game::evPlayerMove(int team_id, int player_id, const Point& pos)
{
  VisuPlayer* p = player_[team_id][player_id];
  assert(p != NULL);

  p->move(pos, 20.);
}

void Game::evBallPos(const Point& pos)
{
  field_->setBallPos(pos);
}


void Game::selectPlayer(VisuPlayer* vp)
{
  action_popup_->hide();
  action_popup_->setVisuPlayer(vp);
}

void Game::unselectAllPlayer()
{
  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 16; j++)
      if (player_[i][j] != NULL)
        player_[i][j]->unselect();
}

void Game::addAction(eVisuAction action)
{
  LOG2("PREPARE an action !");
  action_ = action;
}

int Game::run()
{
  int game_state = -1;

  Sprite test("image/general/map-move.png");
  test.setTransparency(160); // FIXME: global effect for this picture.
  test.splitNbFrame(3, 2);
  test.setFrame(3);
  test.setPos(40, 40);
  test.setZ(3);
  field_->addChild(&test);
  
  Sprite test2("image/general/map-move.png");
  test2.splitNbFrame(3, 2);
  test2.setFrame(4);
  test2.setPos(80, 40);
  test2.setZ(3);
  field_->addChild(&test2);

  InputTextSurface textbox("Vera.ttf", 160);
  textbox.setPos(50, 500);
  textbox.setZ(6);
  textbox.hide();
  win_.getScreen().addChild(&textbox);

  TextSurface fps("Vera.ttf", 12, 70, 25);
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

      // Chat box.
      if (win_.getInput().key_pressed_[(unsigned)'y'])
        {
          textbox.acquireInput("mytchat");
          textbox.show();
        }
      if (textbox.isAcquireFinished())
        {
          api_->sendChatMessage(textbox.getText());
          textbox.resetAcquire();
          textbox.clearText();
          textbox.hide();
        }
      
      // Action popup window
      if (is_playing_ && win_.getInput().button_pressed_[3])
        {
          action_ = eActNone;
          action_popup_->show(win_.getInput().mouse_);
        }

      if (is_playing_ && win_.getInput().button_pressed_[1])
        {
          if (action_popup_->isVisible())
            action_popup_->hide();
          else if (action_ != eActNone
                   && action_popup_->getVisuPlayer() != NULL)
            {
              LOG2("Do an action !");
              action_popup_->getVisuPlayer()->action(action_);
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
            case GS_INITKICKOFF: status = "GS_INITKICKOFF"; break;
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
