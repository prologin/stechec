//
// gameengine.cpp for  in /goinfre/lapie_t/prolo_gui_09
// 
// Made by stephane2 lapie
// Login   <lapie_t@epitech.net>
// 
// Started on  Fri Mar  6 16:17:43 2009 stephane2 lapie
// Last update Sat Apr 25 17:08:42 2009 Hazgar
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
  while (((Api*)g_api)->getState() != GS_END)
    {
      this->RetrieveData();
      sleep(2);
      ((ClientCx*)g_client_cx)->setReady();
      while (((ClientCx*)g_client_cx)->process(true))
	;
    }
}

/*int			type_case(int x, int y)
{
  if (x == 50 && y == 50)
    return (LD_MONUMENT);
  return (LD_EMPTY);
}

int			type_monument(int x, int y)
{
  if (x == 50 && y == 50)
    return (13);
  return (0);
}*/

/* */
void			GameEngine::RetrieveData(void)
{
  static int		last_monument_bid;
  int			monument_bid;
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
	//case_type = LD_EMPTY;
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
	//case_owner = appartenance(x, y);
	case_owner = OWN_TOWNHALL;
	if (case_owner != this->_map[k].getOwner())
	  {
	    this->_map[k].setOwner(case_owner);
	    std::cout << "[Event] case owner changed in " << x << "x" << y << std::endl;
	    ev.user.code = EV_CASEOWNER;
	    ev.user.data1 = new EventCase(x, y, case_owner);
	    SDL_PushEvent(&ev);
	  }
	//case_price = valeur_case(x, y);
	case_price = 1;
	if (case_price != this->_map[k].getPrice())
	  {
	    this->_map[k].setPrice(case_price);
	    std::cout << "[Event] case price changed in " << x << "x" << y << std::endl;
	    ev.user.code = EV_CASEPRICE;
	    ev.user.data1 = new EventCase(x, y, case_price);
	    SDL_PushEvent(&ev);
	  }
      }
  //game_turn = numero_tour();
  game_turn = 0;
  if (game_turn != last_game_turn)
    {
      
    }
  //monument_bid = monument_en_cours();
  monument_bid = 0;
  if (monument_bid != last_monument_bid)
    {
      
    }
}
