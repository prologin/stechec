/*!
** This file has been generated, if you wish to
** modify it in a permanent way, please refer
** to the script file : gen/generator_c.rb
*/

#include "prologin.h"

/*!
** Fonction appell�e au d�but de la partie.
*/
void init_game(void)
{
  /* fonction a completer */
}

/*!
** Fonction appell�e pour la phase de retrait de KO.
*/
position retirer_ko(void)
{
  unite * u;
  int i;
  size_t size;
  unites(&u, &size);
  for (i = 0; i < size; i ++){
    if (u[i].ennemi && u[i].ko > 0) return u[i].pos;
  }
}

/*!
** Fonction appell�e pour la phase de jeu.
*/
void jouer(void)
{
  unite * u;
  size_t size;
  unites(&u, &size);
  afficher_position(pos_renfort(0));
  afficher_unite(u[0]);
}

/*!
** Fonction appell�e � la fin de la partie.
*/
void end_game(void)
{
  /* fonction a completer */
}

