
/*
** simple.c for foo in /home/victor/serv/src
**
** Made by olivier gournet
** Login   <gourne_o@epita.fr>
**
** Started on  Sun Dec  5 00:57:42 2004 olivier gournet
** Last update Wed Apr 13 18:56:09 2005 olivier gournet
*/

#include <stdio.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include "prologin.h"

#define p(...) fprintf(stderr, ## __VA_ARGS__)

void init_game()
{
  p("Bonjour, monde !\n");
  p("je suis %d.\n", get_my_team());
  p("ma base est en %d:%d, j'ai %d fourmis.\n",
    get_anthill_pos_x(get_my_team()),
    get_anthill_pos_y(get_my_team()),
    get_nb_ant(get_my_team()));
  p("le terrain fait %d:%d\n", get_landscape_width(), get_landscape_height());
  p("on est parti !.\n");
}


void print_map()
{
  int i, j;

  for (i = 0; i < get_landscape_width(); i++)
    {
      for (j = 0; j < get_landscape_height(); j++)
	{
	  char c = get_land_type(i, j);
	  switch (c)
	    {
	    case UNKNOWN: c = '?'; break;
	    case NOTHING: c = '.'; break;
	    case OBSTACLE: c = 'x'; break;
	    case ANTHILL: c = '#'; break;
	    case FOOD: c = 'B'; break;
	    }
	  if (get_land_ant(i, j) != NO_ANT)
	    c = 'F';
	  p("%c", c);
	}
      p("\n");
    }
}

struct point_s
{
  int x, y;
} direction[4] =
  {
    {-1, 0},
    {0, -1},
    {1, 0},
    {0, 1}
  };

void play_turn()
{
  int i;

  for (i = 0; i < 4; i++)
    {
      int x = get_ant_pos_x(i);
      int y = get_ant_pos_y(i);

      // nourriture !
      if (get_land_type(x, y) == FOOD)
	{
	  carry_food(i);
	  continue;
	}

      // on est arrive a destination ?
      if (get_anthill_pos_x(get_my_team()) == x
	  && get_anthill_pos_y(get_my_team()) == y)
	{
	  if (get_ant_luggage(i) > 0)
	    {
	      fprintf(stderr, "Weeeeee, manger !!! (%d)\n", i);
	      let_food(i);
	      continue;
	    }
	  // demi tour !
	  direction[i].x *= -1;
	  direction[i].y *= -1;
	}

      x += direction[i].x;
      y += direction[i].y;
      
      // obstacle, demi tour (et on y perd un tour) :/
      if (get_land_type(x, y) == OBSTACLE
	  || get_land_type(x, y) == BAD_ARGUMENT
	  || (get_land_type(x, y) == ANTHILL
	      && !(get_anthill_pos_x(get_my_team()) == x
	      && get_anthill_pos_y(get_my_team()) == y)))
	{
	  direction[i].x *= -1;
	  direction[i].y *= -1;
	}

      move_ant(i, x, y);
    }
  print_map();
}
