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
  memset(this->_unit_remind, 0xFF, sizeof(this->_unit_remind));
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
	    {
	      delete dsp;
	      exit(0);
	    }
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
  while (dsp->Read((void*)(&status), sizeof(status)))
    {
      if (status == EV_DISPLAY_END)
	{
	  dsp->Stop();
	  delete dsp;
	  exit(0);
	}
    }
}

/* */
void			GameEngine::RetrieveData(void)
{
  static int		last_game_turn;
  int			game_turn;
  SDL_Event		ev;
  Player		player;
  taille_terrain	map_size;

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

  map_size = taille_terrain_actuelle();
  ev.user.code = EV_MAPSIZE;
  ev.user.data1 = new int[1];
  *(int*)(ev.user.data1) = MAP_WIDTH - map_size.taille;
  xSDL_PushEvent(&ev);

  player.cards = mes_cartes();
  player.units = unites();
  ev.user.code = EV_PLAYER;
  ev.user.data1 = new EventPlayer(last_game_turn%2, player.cards, player.units);
  xSDL_PushEvent(&ev);

  int size = player.units.size();
  for (int i = 0; i < size; i++)
    {
      ev.user.code = EV_CASETYPE;
      for (int j = 0; j < MAP_SIZE; ++j)
	if (this->_unit_remind[j] == player.units[i].id)
	  {
	    int x = j % MAP_WIDTH;
	    int y = j / MAP_HEIGHT;
	    ev.user.data1 = NULL;
	    if (player.units[i].pos.x == x && player.units[i].pos.y < y)
	      ev.user.data1 = new EventCase(x, y, LD_MOVE0);
	    else if (player.units[i].pos.x == x && player.units[i].pos.y > y)
	      ev.user.data1 = new EventCase(x, y, LD_MOVE1);
	    else if (player.units[i].pos.x < x && player.units[i].pos.y == y)
	      ev.user.data1 = new EventCase(x, y, LD_MOVE2);
	    else if (player.units[i].pos.x > x && player.units[i].pos.y == y)
	      ev.user.data1 = new EventCase(x, y, LD_MOVE3);
	    else if (player.units[i].pos.x < x && player.units[i].pos.y < y)
	      ev.user.data1 = new EventCase(x, y, LD_MOVE4);
	    else if (player.units[i].pos.x < x && player.units[i].pos.y > y)
	      ev.user.data1 = new EventCase(x, y, LD_MOVE5);
	    else if (player.units[i].pos.x > x && player.units[i].pos.y > y)
	      ev.user.data1 = new EventCase(x, y, LD_MOVE6);
	    else if (player.units[i].pos.x > x && player.units[i].pos.y < y)
	      ev.user.data1 = new EventCase(x, y, LD_MOVE7);
	    if (ev.user.data1 != NULL)
	      xSDL_PushEvent(&ev);
	  }
      if (player.units[i].ko >= 0)
	{
	  ev.user.data1 = new EventCase(player.units[i].pos.x, player.units[i].pos.y, LD_KO0 + player.units[i].ko);
	  xSDL_PushEvent(&ev);
	}
      ev.user.data1 = new EventCase(player.units[i].pos.x, player.units[i].pos.y, player.units[i].type_unite_actuel);
      xSDL_PushEvent(&ev);
      this->_unit_remind[player.units[i].pos.x + player.units[i].pos.y * MAP_WIDTH] = player.units[i].id;
    }
}
