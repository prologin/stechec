/*
** input.c for Project in /home/deather/stechec/prologin/prolo2008/igui
**
** Made by deather
** Login   <deather@epitech.net>
**
** Started on  Thu May  8 22:17:00 2008 deather
** Last update Fri May  9 00:04:21 2008 deather
*/

#include <SDL.h>
#include "prologin.h"
#include "constants.h"
#include "graphics.h"
#include "players.h"
#include "orders.h"
#include <assert.h>

#define DISCARD() { bpos = 0; printf("Invalid input, discarding command.\n"); return ; }


static const char *directions_str[] = {
  "BAS", //inversion car tout est affiche inverse dans l'interface graphique
  "GAUCHE",
  "HAUT",
  "DROITE",

  "ICI" };

static const char *orders_str[] =
  { "attendre",
    "lacher_pomme",
    "deplacement",
    "ramasser_pomme",
    "grappin",
    "",
    "turbo",
    "trognon" };

/* Where we stores entered orders */
extern struct AwaitingOrders awaiting_orders;

/*
 * Command buffer stores commands given by the user.
 * Command syntax:
 * [robot_id][command][opt_direction]
 */

struct InputCommand
{
  char	key;
  int	action; // see constants.h
};
  
static int input[256];
static struct Order current_order;
static int input_is_init = 0;
static unsigned int bpos = 0; // position in the command buffer
static int has_arg[ACTION_SEPARATOR] = {0, 1, 1, 0, 1, 0, 0, 1};

static void	init_input(void)
{
  int	i;

  for (i = 0; i < 256; i++)
    input[i] = -1;
  input['w'] = ACTION_WAIT;
  input['l'] = ACTION_DROP_BALL;
  input['d'] = ACTION_MOVE;
  input['r'] = ACTION_PICK_UP_BALL;
  input['g'] = ACTION_HOOK_ROBOT;
  input['t'] = ACTION_BOOST_TURBO;
  input['a'] = ACTION_LAUNCH_BULLET;
  input_is_init = 1;
}

/* Commits the current order to the orders list */
static void	commit(void)
{
  printf("Commiting an action (robot id %i, action id %i)\n", current_order.robot_id, current_order.order_id);
  memcpy(&awaiting_orders.orders[awaiting_orders.next_order], &current_order, sizeof(current_order));
  awaiting_orders.next_order++;
  DisplayCurrentOrders(&awaiting_orders);
}

void	handle_input(SDL_Event *event)
{
  char	action = tolower(event->key.keysym.sym);
  SDLKey key = event->key.keysym.sym;
  switch (key)
  {
    case SDLK_KP0:
      action = '0';
      break;
    case SDLK_KP1:
      action = '1';
      break;
    case SDLK_KP2:
      action = '2';
      break;
    case SDLK_KP3:
      action = '3';
      break;
    case SDLK_KP4:
      action = '4';
      break;
    case SDLK_KP5:
      action = '5';
      break;
    case SDLK_KP6:
      action = '6';
      break;
    case SDLK_KP7:
      action = '7';
      break;
    case SDLK_KP8:
      action = '8';
      break;
    case SDLK_KP9:
      action = '9';
      break;
  }
  if (input_is_init == 0)
    init_input();
  assert(bpos >= 0 && bpos <= 3);
  if (awaiting_orders.next_order >= MAX_TOTAL_ORDERS) {
    printf("Already too much orders\n");
    return;
  }
  if (bpos == 0)
  {
    printf("Ordre en cours: ");
    fflush(stdout);
    if (!isdigit(action))
      DISCARD();
    current_order.robot_id = action - '0';
    printf("Hamster_id = %i, ", current_order.robot_id);
    fflush(stdout);
    if (current_order.robot_id < MAX_ROBOTS/2 || current_order.robot_id >= MAX_ROBOTS)
      DISCARD();
    current_order.robot_id -= MAX_ROBOTS/2;
    //    printf("real id is %d\n", current_order.robot_id); //debug
    bpos++;
    return ;
  }
  if (bpos == 1)
  {
    if (input[action] == -1)
    {
      DISCARD();
    }
    current_order.order_id = input[action];
    printf("ordre = %s", orders_str[input[action]]);
    fflush(stdout);
    if (!has_arg[current_order.order_id])
    {
      printf("\n");
      bpos = 0;
      commit();
    } else
    bpos++;
  }
  else if (bpos == 2)
  {
    if (key == SDLK_UP)
      current_order.dir = BAS;
    else if (key == SDLK_DOWN)
      current_order.dir = HAUT;
    else if (key == SDLK_RIGHT)
      current_order.dir = DROITE;
    else if (key == SDLK_LEFT)
      current_order.dir = GAUCHE;
    else if (key == SDLK_0)
      current_order.dir = ICI;
    else
    {
      DISCARD();
      bpos = 0;
      return;
    }
    printf("direction = %s\n", directions_str[current_order.dir]);
    printf("\n");
    fflush(stdout);
    commit();
    bpos = 0;
  }
  else
    bpos = 0; // should NEVER be reached
}
