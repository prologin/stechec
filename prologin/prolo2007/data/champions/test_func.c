/*!
** This file has been generated, if you wish to
** modify it in a permanent way, please refer
** to the script file : gen/generator_c.rb
*/

#include <stdio.h>
#include "prologin.h"

/*!
** Fonction appele au debut de la partie
*/
void init_game(void)
{
  /* fonction a completer */

}

/*!
** Fonction appele a chaque tour
*/
void play_turn(void)
{
  int x = taille_corps_x();
  int y = taille_corps_y();
  int i, j, type;

  printf("my_eq: %d,  taille_equipe: %d, x: %d, y:%d\n",
	 mon_equipe(), taille_equipe(mon_equipe()), x, y);

  printf("%d %d\n", score(mon_equipe()),
	 current_beat());

  for (j = 0; j < y; ++j)
    {
      for (i = 0; i < x; ++i)
	{
	  type = regarde(i, j);
	  if (type == FLESH)
	    printf(".");
	  else if (type == VESSEL)
	    printf("_");
	  else if (type == VIRUS)
	    printf("V");
	  else if (type == CELL)
	    printf("C");
	  else if (type == WHITE_CELL)
	    printf("O");
	  else
	    printf("%d", type);
	}
      printf("\n");
    }
  for (j = 0; j < y; ++j)
    {
      for (i = 0; i < x; ++i)
	if (visible(i, j))
	  printf(".");
	else
	  printf("X");
      printf("\n");
    }
}

