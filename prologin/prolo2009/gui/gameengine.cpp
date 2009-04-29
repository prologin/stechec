//
// gameengine.cpp for  in /goinfre/lapie_t/prolo_gui_09
// 
// Made by stephane2 lapie
// Login   <lapie_t@epitech.net>
// 
// Started on  Fri Mar  6 16:17:43 2009 stephane2 lapie
// Last update Wed Apr 29 17:51:57 2009 user
//

#include <cstdlib>
#include <iostream>
#include <SDL.h>

#include "Api.hh"
#include "ClientCx.hh"

#include "prologin.h"

#include "gameengine.h"
#include "event.hpp"

extern void		*g_client_cx;
extern void		*g_api;

static int		run; // XXX Debian race condition patch
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

/* */
GameEngine::GameEngine(void)
{
  atexit(GameEngineCleanup);
  run = 0;
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
  run = 0;
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
  run = 1;
  while (run && ((Api*)g_api)->getState() != GS_END)
    {
      this->RetrieveData();
      sleep(2);
      ((ClientCx*)g_client_cx)->setReady();
      while (((ClientCx*)g_client_cx)->process(true))
	;
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

  ev.type = SDL_USEREVENT;
  for (y = 0; y < MAP_HEIGHT; y++)
    for (x = 0; x < MAP_WIDTH; x++)
      {
	k = x + (y * MAP_WIDTH);
	case_type = (t_landscape)type_case(x, y);
	if (case_type != this->_map[k].getType())
	  {
	    this->_map[k].setType(case_type);
	    std::cout << "[Event] case type changed in " << x << "x" << y << std::endl;
	    ev.user.code = EV_CASETYPE;
	    if (case_type == LD_MONUMENT)
	      ev.user.data1 = new EventCase(x, y, LD_MONUMENTS + type_monument(x, y));
	    else
	      ev.user.data1 = new EventCase(x, y, case_type);
	    SDL_PushEvent(&ev);
	  }
	case_owner = (t_owner)appartenance(x, y);
	if (case_owner != this->_map[k].getOwner())
	  {
	    this->_map[k].setOwner(case_owner);
	    std::cout << "[Event] case owner changed in " << x << "x" << y << std::endl;
	    ev.user.code = EV_CASEOWNER;
	    ev.user.data1 = new EventCase(x, y, case_owner);
	    SDL_PushEvent(&ev);
	  }
	case_price = valeur_case(x, y);
	if (case_price != this->_map[k].getPrice())
	  {
	    this->_map[k].setPrice(case_price);
	    std::cout << "[Event] case price changed in " << x << "x" << y << std::endl;
	    ev.user.code = EV_CASEPRICE;
	    ev.user.data1 = new EventCase(x, y, case_price);
	    SDL_PushEvent(&ev);
	  }
      }
  game_turn = numero_tour();
  if (game_turn != last_game_turn)
    {
      std::cout << "[Event] New game turn <" << game_turn << ">" << std::endl;
      ev.user.code = EV_NEWTURN;
      ev.user.data1 = new int[1];
      *(int*)(ev.user.data1) = game_turn;
      SDL_PushEvent(&ev);
      for (x = 0; x < NB_PLAYERS; x++)
	{
	  if (score(x) == JOUEUR_INCORRECT)
	    continue;
	  ev.user.code = EV_PLAYER;
	  ev.user.data1 = new EventPlayer(x, score(x), finances(x));
	  SDL_PushEvent(&ev);
	}
      last_game_turn = game_turn;
    }
}
