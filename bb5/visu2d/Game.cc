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

#include <iterator>
#include <SDL_ttf.h>

#include "Api.hh"
#include "ClientCx.hh"

#include "SDLWindow.hh"
#include "Input.hh"
#include "Surface.hh"
#include "VirtualSurface.hh"
#include "VirtualScrollableSurface.hh"
#include "TextSurface.hh"
#include "InputTextSurface.hh"
#include "Sprite.hh"
#include "Panel.hh"
#include "Field.hh"
#include "VisuPlayer.hh"
#include "ActionPopup.hh"
#include "Game.hh"

BEGIN_NS(sdlvisu);

Game::Game(SDLWindow& win, xml::XMLConfig* xml, Api* api, ClientCx* ccx)
  : win_(win),
    xml_(xml),
    api_(api),
    ccx_(ccx),
    dlg_follow_answer_(api),
    dlg_reroll_answer_(api)
{
  panel_ = new Panel(*this);
  field_ = new VisuField(*this);
  action_popup_ = new ActionPopup(*this);

  api_->setEventHandler(this);
  win_.getScreen().addChild(panel_);
  win_.getScreen().addChild(field_);
  win_.getScreen().addChild(action_popup_);
    
  txt_status_ = TextSurface("Vera.ttf", 12, 270, 65);
  txt_status_.setZ(6);
  txt_status_.setRenderMethod(eTextShaded);
  txt_status_.setPos(3, 532);
  win_.getScreen().addChild(&txt_status_);

  state_list_.insert(stNothing); // Avoid useless segv...
  setState(stWait);    // Wait game begins.

  for (int i = 0; i < 16; i++)
    {
      player_[0][i] = NULL;
      player_[1][i] = NULL;
    }

  for (int i = 0; i < 3; i++)
    {
      block_push_[i].load("image/general/push.png");
      block_push_[i].splitNbFrame(2, 1);
      block_push_[i].setZ(8);
      block_push_[i].hide();
      field_->addChild(&block_push_[i]);
    }
  
  initDialogBoxes();
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

  delete dlg_play_;
  delete dlg_touchback_;
  delete dlg_follow_;
}

void Game::initDialogBoxes()
{
  // Dialog box for kickoff/play turn.
  dlg_play_ = new DialogBox(*this, eDlgBoxOk, eDlgInfo);
  dlg_play_->setPos(150, 150);

  dlg_touchback_ = new DialogBox(*this, eDlgBoxOk, eDlgInfo);
  dlg_touchback_->setPos(150, 200);

  dlg_reroll_ = new DialogBox(*this, eDlgBoxYesNo, eDlgInfo);
  dlg_reroll_->setPos(150, 200);
  dlg_reroll_->setText("Do you want to reroll ?");
  dlg_reroll_->setActionHandler(&dlg_reroll_answer_);

  dlg_follow_ = new DialogBox(*this, eDlgBoxYesNo, eDlgInfo);
  dlg_follow_->setPos(150, 200);
  dlg_follow_->setText("Do you want to follow your opponent ?");
  dlg_follow_->setActionHandler(&dlg_follow_answer_);
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

bool Game::isStateSet(enum eState s) const
{
  enum eState first = *state_list_.begin();
  if (first == stNothing)
    WARN("getStatus returns stNothing... must be a bug.");

  // Not in the current chunk.
  if (s / 10 > first / 10)
    return false;

  return state_list_.find(s) != state_list_.end();
}

void Game::setState(enum eState s)
{
  if (state_list_.find(s) != state_list_.end())
    WARN("state %1 already set!", s);
  state_list_.insert(s);

  // debug
  LOG4("Insert %1", s);
  std::copy(state_list_.begin(), state_list_.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;
}

void Game::unsetState(enum eState s)
{
  state_list_.erase(s);

  if (s == stShowDlgBox && !dlg_box_list_.empty())
    {
      LOG2("a dlgbox has been closed, display the next one.");
      dlg_box_list_.front()->enable();
      dlg_box_list_.pop_front();
    }
  else if (s == stShowDlgBox)
    LOG2("unset dlgbox, but none in the list");
    
  // debug
  LOG4("Remove %1", s);
  std::copy(state_list_.begin(), state_list_.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;
}

void Game::unselectAllPlayer()
{
  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 16; j++)
      if (player_[i][j] != NULL)
        player_[i][j]->unselect();
}

void Game::pushDialogBox(DialogBox *dlgbox)
{
  if (isStateSet(stShowDlgBox))
    {
      LOG2("push one dlgbox in the list");
      dlg_box_list_.push_back(dlgbox);
    }
  else
    {
      LOG2("display dlgbox immediately.");
      dlgbox->enable();
    }
}


/*
** Events
*/

void Game::evIllegal(int)
{
  LOG4("An illegal token was received.");
}

void Game::evNewTurn(int player_id, int cur_half, int cur_turn)
{
  std::ostringstream os;

  os << "Turn " << cur_turn << " (half " << cur_half << ")" << std::endl;
  unsetState(stWait);
  if (player_id == api_->myTeamId())
    {
      unsetState(stWaitOther);
      setState(stWaitPlay);
      os << "It is your turn, Play !";
      txt_status_.addText("It is your turn... Play !");
      dlg_play_->setText(os.str());
      pushDialogBox(dlg_play_);

      int i = api_->myTeamId();
      for (int j = 0; j < 16; j++)
	if (player_[i][j] != NULL)
	  player_[i][j]->newTurn();
    }
  else
    {
      unsetState(stWaitPlay);
      setState(stWaitOther);
      os << "Other turn. Please wait...";
      txt_status_.addText("Waiting that the other coach finish its turn.");
      dlg_play_->setText(os.str());
      pushDialogBox(dlg_play_);
    }
  panel_->setTurn(player_id, cur_turn);
}

void Game::evEndGame()
{
  txt_status_.addText("Game is finished.");
  dlg_play_->setText("Game is finished.");
  pushDialogBox(dlg_play_);
}

void Game::evMoveTurnMarker()
{
  txt_status_.addText("Move turn marker ? Ahah, not implemented yet.");
}

void Game::evTimeExceeded()
{
  txt_status_.addText("Time exceeded. You were too slow.");
}

void Game::evPlayerKnocked(int team_id, int player_id)
{
  LOG4("A player was knocked down.");
  team_id = player_id;
}

void Game::evKickOff()
{
  unsetState(stWait);
  setState(stDoKickoff);
  txt_status_.addText("Kickoff. Please place the ball.");
  dlg_play_->setText("Kickoff. Place the ball.");
  pushDialogBox(dlg_play_);
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
  VisuPlayer* p = new VisuPlayer(*this, action_popup_, ap);
  player_[team_id][player_id] = p;
  field_->addChild(p);
}

void Game::evPlayerPos(int team_id, int player_id, const Point& pos)
{
  VisuPlayer* p = player_[team_id][player_id];

  p->setPos(field_->squareToField(pos));
}

void Game::evPlayerMove(int team_id, int player_id, const Point& pos)
{
  VisuPlayer* p = player_[team_id][player_id];
  assert(p != NULL);

  p->move(field_->squareToField(pos), 35.);
}

void Game::evBallPos(const Point& pos)
{
  field_->setBallPos(pos);
}

void Game::evGiveBall()
{
  LOG4("Touchback! receiving team can give the ball to any player on the field.");
  dlg_touchback_->setText("Touchback! receiving team can give the ball to any player on the field.");
  pushDialogBox(dlg_touchback_);
}

void Game::evResult(int team_id, int player_id, enum eRoll action_type, int result, 
		    int modifier, int required, bool reroll)
{
  LOG4("Player `%1' tried an action : `%2' : roll [%3] + [%4], required : [%5].",
       player_id, Dice::stringify(action_type), result, modifier, required);
	
  if (result + modifier < required && reroll && api_->getTeamId() == team_id)
    {
      LOG4(" -> You can use a 'reroll' or 'accept' this result.");
      pushDialogBox(dlg_reroll_);
    }
}

void Game::evBlockResult(int team_id, int player_id, int opponent_id, 
			 int nb_dice, enum eBlockDiceFace result[3],
			 int choose, bool reroll)
{
  player_id = opponent_id;
  nb_dice = nb_dice;
  result = result;

  if (team_id != api_->getTeamId())
    {
      if (choose == api_->getTeamId())
	{
	  if (reroll)
	    txt_status_.addText(" Wait for opponent reroll decision");
	  else
	    LOG4(" You must choose a 'dice <n>'");
	}
    }
  else
    {
      if (choose == api_->getTeamId())
	{
	  if (reroll)
	    LOG4("You can use a 'reroll' or choose a 'dice <n>'");
	  else
	    LOG4(" You must choose a 'dice <n>'");
	}
      else if (reroll)
	{
	  txt_status_.addText("You can use a reroll or accept this result.");
	  pushDialogBox(dlg_reroll_);
	}
    }
}

void Game::evBlockPush(Position pos, int nb_choice, Position choices[])
{
  LOG4( "You can push the player from the square %1 to : ", pos);
  for (int i = 0; i < nb_choice; i++)
    {
      LOG4("  'push %1: %2", i, choices[i]);
      block_push_[i].setPos(Point(choices[i]) * 40);
      block_push_[i].show();
    }
  setState(stBlockPushChoice);
  
}

void Game::evFollow()
{
  txt_status_.addText("Follow after block choice.");
  pushDialogBox(dlg_follow_);
}







int Game::run()
{
  Input& inp = win_.getInput();
  int game_state = -1;

  InputTextSurface textbox("Vera.ttf", 160);
  textbox.setPos(50, 500);
  textbox.setZ(6);
  textbox.hide();
  win_.getScreen().addChild(&textbox);

  TextSurface fps("Vera.ttf", 12, 70, 25);
  fps.setZ(4);
  fps.setRenderMethod(eTextShaded);
  fps.setPos(720, 570);
  win_.getScreen().addChild(&fps);

  DialogBox test_dg(*this, eDlgBoxOk, eDlgInfo);
  test_dg.setText("Ceci n'est qu'un test, qui des fois peut rester en place plusieurs mois.");
  test_dg.setPos(50, 50);

  DialogBox test_dg2(*this, eDlgBoxBlock, eDlgError);
  test_dg2.setText("Mais non, il n'y a pas (encore) d'erreur.");
  test_dg2.setPos(150, 50);

  
  // Sit back and see what's happening...
  while (api_->getState() != GS_END)
    {
      // Process any rules incoming messages.
      while (ccx_->process())
        ;

      // Update the whole app, and print one frame.
      if (win_.processOneFrame() || win_.getInput().key_pressed_[(unsigned)'q'])
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

      // Block push choice
      if (isStateSet(stBlockPushChoice))
	{
	  for (int i = 0; i < 3 && block_push_[i].isShown(); i++)
	    if (block_push_[i].getScreenRect().inside(inp.mouse_))
	      {
		block_push_[i].setFrame(2);
		if (inp.button_[1])
		  {
		    LOG1("block push ok...");
		    api_->doBlockPush(i);
		    for (int j = 0; j < 3; j++)
		      block_push_[j].hide();
		    unsetState(stBlockPushChoice);
		  }
	      }
	    else
	      block_push_[i].setFrame(1);
	}
      
      // Test dlg
      if (win_.getInput().key_pressed_[SDLK_t])
        pushDialogBox(&test_dg);
      if (win_.getInput().key_pressed_[SDLK_r])
        pushDialogBox(&test_dg2);

      // Field border around squares.
      if (win_.getInput().key_pressed_[SDLK_s])
	field_->setDrawTicks(!field_->getDrawTicks());

      // End turn
      if (win_.getInput().key_pressed_[SDLK_e])
	api_->doEndTurn();
      
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
            case GS_PAUSE: status = "Pause."; break;
            }
	  if (status != "")
	    txt_status_.addText("New game status: " + status);
          game_state = api_->getState();
        }
    }
  if (api_->getState() != GS_END)
    ccx_->disconnect();
  return 0;
}


/*
** Dialog boxes callback handlers.
*/

void Game::DlgFollowAnser::clicked(int btn_index)
{
  bool follow = btn_index == 0;
  
  LOG1("follow clicker: %1", btn_index);
  api_->doFollow(follow);
}

void Game::DlgRerollAnser::clicked(int btn_index)
{
  bool reroll = btn_index == 0;
  
  LOG1("reroll clicker: %1", btn_index);
  api_->doReroll(reroll);
}

END_NS(sdlvisu);
