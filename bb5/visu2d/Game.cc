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
#include "ActionDlg.hh"
#include "Game.hh"

BEGIN_NS(sdlvisu);

Game::Game(SDLWindow& win, xml::XMLConfig* xml, Api* api, ClientCx* ccx)
  : win_(win),
    xml_(xml),
    api_(api),
    ccx_(ccx)
{
  panel_ = new Panel(*this);
  field_ = new VisuField(*this);
  action_popup_ = new ActionPopup(*this);
  game_dlg_ = new ActionDlg(*this);

  api_->setEventHandler(this);
  win_.getScreen().addChild(panel_);
  win_.getScreen().addChild(field_);
  win_.getScreen().addChild(action_popup_);
    
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
}

Game::~Game()
{
  win_.getScreen().removeChild(panel_);
  win_.getScreen().removeChild(field_);

  for (int i = 0; i < 16; i++)
    {
      delete player_[0][i];
      delete player_[1][i];
    }

  delete panel_;
  delete field_;
  delete action_popup_;
  delete game_dlg_;
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
    LOG5("getStatus returns stNothing... must be a bug.");

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

  return;
  // debug
  LOG4("Insert %1", s);
  std::copy(state_list_.begin(), state_list_.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;
}

void Game::unsetState(enum eState s)
{
  state_list_.erase(s);

  if (s == stShowDlgBox)
    game_dlg_->pop();

  return;
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


/*
** Events
*/

void Game::evIllegal(int team_id, int token)
{
  LOG4("An illegal action (token: %1) has tried by the coach `%2'.",
      bb5_token_str[token], team_id);
}

void Game::evNewTurn(int team_id, int cur_half, int cur_turn)
{
  std::ostringstream os;

  os << "Turn " << cur_turn << " (half " << cur_half << ")" << std::endl;

  int other_team_id = (team_id + 1) % 2;
  for (int j = 0; j < 16; j++)
    {
      if (player_[team_id][j] != NULL)
        player_[team_id][j]->newTurn();
      if (player_[other_team_id][j] != NULL)
        player_[other_team_id][j]->finishedTurn();
    }
  
  unsetState(stWaitKoffBall);
  if (team_id == api_->myTeamId())
    {
      unsetState(stWaitOther);
      setState(stWaitPlay);
      os << "It is your turn, Play !";
      game_dlg_->push(eDlgActInfo);
      game_dlg_->setText(os.str());
    }
  else
    {
      unsetState(stWaitPlay);
      setState(stWaitOther);
      os << "Other turn. Please wait...";
      game_dlg_->push(eDlgActInfo);
      game_dlg_->setText(os.str());
    }
  panel_->setTurn(team_id, cur_turn);
}

void Game::evEndGame()
{
  game_dlg_->push(eDlgActInfo);
}

void Game::evMoveTurnMarker()
{
  LOG2("Move turn marker ? Ahah, not implemented yet.");
}

void Game::evTurnOver(enum eTurnOverMotive motive)
{
  switch(motive)
    {
      case TOM_KNOCKEDDOWN:
        LOG2("Turnover (Player is knocked down).");
        game_dlg_->push(eDlgActInfo);
        game_dlg_->setText("Turnover (Player is knocked down).");
        break;
      case TOM_LOSTBALL:
        LOG2("Turnover (Ball is not caught).");
        game_dlg_->push(eDlgActInfo);
        game_dlg_->setText("Turnover (Ball is not caught).");
        break;
      case TOM_FAILEDPICKUP:
        LOG2("Turnover (Player fails to pick up the ball).");
        game_dlg_->push(eDlgActInfo);
        game_dlg_->setText("Turnover (Player fails to pick up the ball).");
        break;
      case TOM_TOUCHDOOOWN:
        LOG2("TOUCHDOOOWN!");
        game_dlg_->push(eDlgActInfo);
        game_dlg_->setText("Touchdooown!");
        break;
      case TOM_TIMEEXCEEDED:
        LOG2("Turnover (Time exceeded).");
        game_dlg_->push(eDlgActInfo);
        game_dlg_->setText("Turnover (Time exceeded).");
        break;
      case TOM_FUMBLEDPASS:
        LOG2("Turnover (Pass attempt is fumbled).");
        game_dlg_->push(eDlgActInfo);
        game_dlg_->setText("Turnover (Pass attempt is fumbled).");
        break;
      case TOM_THROWNTMFAILED:
        LOG2("Turnover (Team mate's throw fails).");
        game_dlg_->push(eDlgActInfo);
        game_dlg_->setText("Turnover (Team mate's throw fails).");
        break;
      case TOM_EJECTEDFORAFOUL:
        LOG2("Turnover (Referee ejects a player).");
        game_dlg_->push(eDlgActInfo);
        game_dlg_->setText("Turnover (Referee ejects a player).");
        break;
      default:
        WARN("Received turnover message with unknown motive.");
        game_dlg_->push(eDlgActInfo);
        game_dlg_->setText("Turnover (Unknown motive).");
        break;
    }
}

void Game::evPlayerKnocked(int, int player_id)
{
  LOG4("A player was knocked down.");

  std::ostringstream os;
  os << "Player " << (player_id + 1) << " has been knocked down";
  game_dlg_->push(eDlgActInfo);
  game_dlg_->setText(os.str());
}

void Game::evDrawKicker(int team_id)
{
  //FIXME: to do.
  if (team_id == api_->myTeamId())
    api_->doChooseKickoff(false); // Choose to receive.
}

void Game::evKickOff(int team_id, bool place_team)
{
  if (place_team)
    if (team_id == api_->myTeamId())
    {
      //FIXME: allow coach to replace his players.
      api_->doEndPlacement();
    }
    else
    {
      //FIXME: (un)setState...
      // game_dlg_->push(eDlgActInfo);
      // game_dlg_->setText("Wait that the other team sets up on the field.");
    }
  else
  {
    unsetState(stWait);
    if (team_id == api_->myTeamId())
    {
      setState(stDoKoffBall);
      game_dlg_->push(eDlgActInfo);
      game_dlg_->setText("Kickoff. Place the ball.");
    }
    else
    {
      setState(stWaitKoffBall);
      game_dlg_->push(eDlgActInfo);
      game_dlg_->setText("Waiting that the other team places the ball.");
    }
  }
}

void Game::evChat(const std::string& msg)
{
  LOG1(std::string("<chat> ") + msg);
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

void Game::evGiveBall(int team_id)
{
  LOG4("Touchback! receiving team (team id %1) can give the ball to any player on the field.",
       team_id);
  if (team_id == api_->getTeamId())
    game_dlg_->push(eDlgActTouchback);
  else
    {
      game_dlg_->push(eDlgActInfo);
      game_dlg_->setText("Touchback! Wait that other team choose to which to give ball");
    }
}

void Game::evResult(int team_id, int player_id, enum eRoll action_type, int result, 
                    int modifier, int required, bool reroll)
{
  LOG4("Player `%1' tried an action : `%2' : roll [%3] + [%4], required : [%5].",
       player_id, Dice::stringify(action_type), result, modifier, required);

  if (api_->getTeamId() == team_id && reroll)
    {
      LOG4(" -> You can use a 'reroll' or 'accept' this result.");
      game_dlg_->push(eDlgActReroll);
    }
}

void Game::evBlockResult(int team_id, int player_id, int opponent_player_id,
                         int nb_dice, enum eBlockDiceFace result[3],
                         int strongest_team_id, bool can_reroll)
{
  player_id = opponent_player_id;

  LOG2("Game::evBlockResult: team_id: %1, opponent_id: %2 strongest: %3 can_reoll %4 nb_dice %5",
       team_id, opponent_player_id, strongest_team_id, can_reroll, nb_dice);

  // show the dices.
  game_dlg_->push(eDlgActBlockDice);
  for (int i = 0; i < nb_dice; i++)
    game_dlg_->front()->addBlockButton(result[i]);

  if (strongest_team_id == api_->getTeamId())
    {
      assert(nb_dice > 1);
      LOG2("I have the strongest player, I can choose dice");
      game_dlg_->setBlockChoice();
    }
  
  if (api_->getTeamId() == team_id &&
      !(strongest_team_id != -1 && api_->getTeamId() == strongest_team_id)
      && can_reroll)
    {
      LOG2("I have the strongest player or I did the block, I can choose to reroll");
      game_dlg_->addRerollLabel();
    }

  if (api_->getTeamId() != team_id && api_->getTeamId() != strongest_team_id)
    {
      // Wait other decision, but show results.
      game_dlg_->setText("Wait for opponent to choose dice");
    }
}

void Game::evBlockPush(const Position& pos, int nb_choice, const Position choices[])
{
  LOG4( "You can push the player from the square %1 to : ", pos);
  for (int i = 0; i < nb_choice; i++)
    {
      LOG4(" - push %1: %2", i, choices[i]);
      block_push_[i].setPos(Point(choices[i]) * 40);
      block_push_[i].show();
    }
  setState(stBlockPushChoice);
  
}

void Game::evFollow()
{
  LOG2("Follow after block choice.");
  game_dlg_->push(eDlgActFollow);
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
  test_dg2.addBlockButton(2);
  test_dg2.addBlockButton(6);

  
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
      
      // Field border around squares.
      if (win_.getInput().key_pressed_[SDLK_s])
        field_->setDrawTicks(!field_->getDrawTicks());

      // End turn
      if (win_.getInput().key_pressed_[SDLK_e])
        api_->doEndTurn();
      
      // Print FPS
      std::ostringstream os;
      os << "FPS: " << win_.getFps();
      fps.setText(os.rdbuf()->str());
    }
  if (api_->getState() != GS_END)
    ccx_->disconnect();
  return 0;
}


END_NS(sdlvisu);
