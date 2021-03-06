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
#include "prologin.h"
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
  int score;
  int score_winner = 0;
  for (int i = 0; i < (int)this->_player.size(); i++)
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
    }
  ev.user.code = EV_WINNER;
  if (winner == -2)
    ev.user.data1 = new EventPlayer(-1, 0, 0, 0);
  else
    ev.user.data1 = new EventPlayer(winner, this->_player[winner].score, this->_player[winner].money, this->_player[winner].bid);
  xSDL_PushEvent(&ev);
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
  t_landscape		case_type;
  t_owner		case_owner;
  int			case_price;
  int			x, y, k;
  SDL_Event		ev;
  Player		player;

  ev.type = SDL_USEREVENT;
  for (y = 0; y < MAP_HEIGHT; y++)
    for (x = 0; x < MAP_WIDTH; x++)
      {
	k = x + (y * MAP_WIDTH);
	case_type = (t_landscape)type_case(x, y);
	if (case_type != this->_map[k].getType())
	  {
	    ev.user.code = EV_CASETYPE;
	    if (case_type == LD_MONUMENT)
	      ev.user.data1 = new EventCase(x, y, LD_MONUMENTS + type_monument(x, y));
	    else
	      ev.user.data1 = new EventCase(x, y, case_type);
	    xSDL_PushEvent(&ev);
	    this->_map[k].setType(case_type);
	  }
	case_owner = (t_owner)appartenance(x, y);
	if (case_owner != this->_map[k].getOwner())
	  {
	    ev.user.code = EV_CASEOWNER;
	    ev.user.data1 = new EventCase(x, y, case_owner);
	    xSDL_PushEvent(&ev);
	    this->_map[k].setOwner(case_owner);
	  }
	case_price = valeur_case(x, y);
	if (case_price != this->_map[k].getPrice())
	  {
	    ev.user.code = EV_CASEPRICE;
	    ev.user.data1 = new EventCase(x, y, case_price);
	    xSDL_PushEvent(&ev);
	    this->_map[k].setPrice(case_price);
	  }
      }
  game_turn = numero_tour();
  if (game_turn != last_game_turn)
    {
      ev.user.code = EV_NEWTURN;
      ev.user.data1 = new int[1];
      *(int*)(ev.user.data1) = game_turn;
      xSDL_PushEvent(&ev);
      last_game_turn = game_turn;
    }
  for (x = 0; x < NB_PLAYERS; x++)
    {
      player.score = score(x);
      player.money = finances(x);
      player.bid = montant_encheres(x);
      if (player.score == JOUEUR_INCORRECT)
	continue;
      ev.user.code = EV_PLAYER;
      ev.user.data1 = new EventPlayer(x, player.score, player.money, player.bid);
      xSDL_PushEvent(&ev);
      this->_player[x] = player;
    }
  /* map dump */
  /*
  std::string color[] = {"[31m", "[32m", "[33m", "[37m"};
  for (y = 0; y < MAP_HEIGHT; y++)
    {
      for (x = 0; x < MAP_WIDTH; x++)
	{
	  k = x + (y * MAP_WIDTH);
	  if (this->_map[k].getType() == 0)
	    printf(" ");
	  else
	    printf("%s%i", color[this->_map[k].getOwner()].c_str(), this->_map[k].getType());
	}
      printf("\n");
    }
  */
}
