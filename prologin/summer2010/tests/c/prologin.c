/*!
** This file has been generated, if you wish to
** modify it in a permanent way, please refer
** to the script file : gen/generator_c.rb
*/

#include "prologin.h"
#include <stdio.h>
/*!
** Fonction appellée au début de la partie.
*/
void init_game(void)
{
  /* fonction a completer */
}

/*!
** Fonction appellée pour la phase de jeu.
*/
void jouer(void)
{
  size_t s;
  piece *p;
  int i;
  pieces_a_vennir(&p, &s);
  printf("\n\n\nt=%d\n", tour_actuel());
  for (i = 0; i < s; i ++){
    printf("%3d - %3d - %3d\n", i, p[i].tour_apparition, p[i].valeur);
  }
  free(p);
}

/*!
** Fonction appellée à la fin de la partie.
*/
void end_game(void)
{
  /* fonction a completer */
}

