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
  int i , j;
  /* fonction a completer */
  printf("Equipe: %d\nTaille Equipe %d\nMy team %d\n",
	 equipes(), taille_equipe(), mon_equipe());
  printf("Taille x %d\nTaille y %d\n", taille_ville_x (),
	 taille_ville_y());
  printf("Look at :\n\t");
  for (i = 0; i < taille_ville_x (); ++i)
    {
      for (j = 0; j < taille_ville_y (); ++j)
	printf("%c", regarde(i, j) == STANDARD ? '.' : regarde(i, j) + '0');
      printf("\n\t");
    }
  printf("score : %d\n", score(0));
  printf("Tour : %d\n", current_turn ());
  printf("Date : %d\n", current_date ());
  if (current_turn() == 1)
    deplace_joueur(2, 9, 3);
  deplace_joueur(2, current_turn() % 2 ? 8 : 9, 3);
  printf("Almanach x: %d y: %d\n", position_almanach_x(),
	 position_almanach_y());
  printf("Joueur qui a l'almanach : %d\n", qui_possede_almanach());
  printf("Delorean x: %d y: %d\n", position_delorean_x(),
	 position_delorean_y());
  printf("Joueur qui a la delorean : %d\n", qui_possede_delorean());
  printf("Equipe j2 : %d, x : %d, y : %d, etat : %d\n", equipe(2),
	 position_joueur_x(2), position_joueur_y(2),
	 etat_joueur(2));
  int id = casino(4, 0);
  printf("casino 0 0 : %d, 4 0 %d\n", casino(0, 0), casino(4, 0));
  printf("casino 4 0 ouvert ? %d jeu : %d\n", casino_ouvert(id), jeu_du_casino(id));
  printf("Total jeux : %d\n", jeux());
}

