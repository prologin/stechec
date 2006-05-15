/*!
** This file has been generated, if you wish to
** modify it in a permanent way, please refer
** to the script file : make_player/make_c.rb
*/

#include <stdlib.h>
#include <stdio.h>
#include "prologin.h"

/*!
** Fonction de début
*/
void init_game(void)
{
  fprintf(stderr, "time: %d extra: %d\n", time_remaining(), extra_time_remaining());
}

/*!
** Fonction appelée a chaque tour
*/
void play_turn(void)
{
  int i, j, k;
  while (rand() & 0x13)
    {
      fprintf(stderr, "time: %d extra: %d\n", time_remaining(), extra_time_remaining());
      for (i = 0; i < 8000; i++)
        for (j = 0; j < 8000; j++)
          k += i * j / i % j + 42;
    }
}
