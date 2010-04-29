//
// gameengine.cpp for  in /goinfre/lapie_t/prolo_gui_09
// 
// Made by stephane2 lapie
// Login   <lapie_t@epitech.net>
// 
// Started on  Fri Mar  6 16:17:43 2009 stephane2 lapie
// Last update Sat May  2 13:39:21 2009 user
//

#include <cstdlib>
#include <iostream>
#include <SDL.h>

#include "Api.hh"
#include "ClientCx.hh"

#include "display.h"
#include "prologin.hh"
#include "gameengine.h"
#include "event.hpp"

extern void		*g_client_cx;
extern void		*g_api;

#define GS_END 0xffff

/* GameEngine instance (GameEngine is a singleton) */
GameEngine		*GameEngine::_instance = NULL;

/*
** This function will be called at program exits to correctly
** shutdown the game engine.
*/
static void		GameEngineCleanup(void)
{
  GameEngine		*ne;

  ne = GameEngine::GetInstance();
  delete ne;
}

/* SDL_PushEvent with timer to retry */
static void		xSDL_PushEvent(SDL_Event *ev)
{
  int			i;

  for (i = 0; i < 3; i++)
    {
      if (SDL_PushEvent(ev) != -1)
	return ;
      usleep(50);
    }
 std::cout << "SDL events fatal error" << std::endl;
 exit(0);
}

/* */
GameEngine::GameEngine(void)
{
  atexit(GameEngineCleanup);
  std::cout << "Game engine init done" << std::endl;
}

/* copy constructor, not used yet */
GameEngine::GameEngine(const GameEngine &right)
{
  (void)right;
}

/* */
GameEngine::~GameEngine(void)
{
  std::cout << "Shutting down game engine...";
  std::cout << "done" << std::endl;
}

/* operator= overload, not user yet */
GameEngine		&GameEngine::operator=(const GameEngine &right)
{
  (void)right;
  return (*this);
}

/* Return GameEngine instance */
GameEngine		*GameEngine::GetInstance(void)
{
  if (!GameEngine::_instance)
    {
      try
	{
	  GameEngine::_instance = new GameEngine;
	}
      catch (const char *str)
	{
	  std::cerr << str << std::endl;
	  return (NULL);
	}
    }
  return (GameEngine::_instance);
}

/* */
void			GameEngine::Run(void)
{
  int			winner, status;
  Display		*dsp;
  SDL_Event		ev;

  dsp = Display::GetInstance();
  while (((Api*)g_api)->getState() != GS_END)
    {
      this->RetrieveData();
      status = 0;
      while (dsp->Read((void*)(&status), sizeof(status)))
	{
	  if (status == EV_DISPLAY_END)
	    exit(0);
	  else if (status == EV_DISPLAY_NEXTTURN)
	    break;
	}
      ((ClientCx*)g_client_cx)->setReady();
      while (((ClientCx*)g_client_cx)->process(true))
	;
    }
  this->RetrieveData();
  ev.type = SDL_USEREVENT;
  ev.user.code = EV_ENDGAME;
  xSDL_PushEvent(&ev);
  winner = -1;
  /*for (int i = 0; i < (int)this->_player.size(); i++)
    {
      score = this->_player[i].score + this->_player[i].money;

      ev.user.code = EV_PLAYER;
      ev.user.data1 = new EventPlayer(i, this->_player[i].score, this->_player[i].money, this->_player[i].bid);
      xSDL_PushEvent(&ev);

      if (winner == -1 || score > score_winner)
	{
	  winner = i;
	  score_winner = score;
	}
      else if (score == score_winner)
	winner = -2;
	}*/
  /*ev.user.code = EV_WINNER;
    if (winner == -2)
    ev.user.data1 = new EventPlayer(-1, 0, 0, 0);
  else
    ev.user.data1 = new EventPlayer(winner, this->_player[winner].score, this->_player[winner].money, this->_player[winner].bid);
    xSDL_PushEvent(&ev);*/
  while (dsp->Read((void*)(&status), sizeof(status)))
    {
      if (status == EV_DISPLAY_END)
	exit(0);
    }
}

/* */
void			GameEngine::RetrieveData(void)
{
  static int		last_game_turn;
  int			game_turn;
  SDL_Event		ev;
  Player		player;

  ev.type = SDL_USEREVENT;

  game_turn = tour_actuel();
  if (game_turn != last_game_turn)
    {
      ev.user.code = EV_NEWTURN;
      ev.user.data1 = new int[1];
      *(int*)(ev.user.data1) = game_turn;
      xSDL_PushEvent(&ev);
      last_game_turn = game_turn;
    }

  player.cards = mes_cartes();
  player.units = unites();
  ev.user.code = EV_PLAYER;
  ev.user.data1 = new EventPlayer(last_game_turn%2, player.cards, player.units);
  xSDL_PushEvent(&ev);

  int size = player.units.size();
  for (int i = 0; i < size; i++)
    {
      /* check units remind */
      ev.user.code = EV_CASETYPE;
      ev.user.data1 = new EventCase(player.units[i].pos.x, player.units[i].pos.y, player.units[i]);
      //      this->_unit_remind[player.units[i].pos.x + player.units[i].pos.y * MAP_WIDTH].push_back(/* ID */);
      xSDL_PushEvent(&ev);
    }
}
