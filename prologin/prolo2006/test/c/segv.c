/*!
** This file has been generated, if you wish to
** modify it in a permanent way, please refer
** to the script file : make_player/make_c.rb
*/

#include <stdlib.h>
#include <stdio.h>
#include "prologin.h"

/*!
** Fonction de d�but
*/
void init_game(void)
{
}

/*!
** Fonction appel�e a chaque tour
*/
void play_turn(void)
{
  int *c = (int*)42;
  if ((rand() & 0x11) == 0x11)
    {
      *c = 42;
    }
}
