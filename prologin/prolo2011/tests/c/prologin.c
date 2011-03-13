/*!
** This file has been generated, if you wish to
** modify it in a permanent way, please refer
** to the script file : gen/generator_c.rb
*/

#include "prologin.h"
#include <stdio.h>

int tour;

/*!
** Fonction appellée au début de la partie
*/
void init_game(void)
{
  /* fonction a completer */
  tour = 0;
}


void show_map(){
  int x, y;
  for (y = 0; y < TAILLE_TERRAIN; y ++) {
    for (x = 0; x < TAILLE_TERRAIN; x ++){
      position p;
      p.x = x;
      p.y = y;
      type_case c = regarder_type_case(p);
      printf("%d", c);
    }
    printf("\n");
  }
}

/*!
** Fonction appellée pour la phase de jeu
*/
void jouer(void)
{
  printf("equipe = %d\n", mon_equipe());
  tour ++;
  int t = tour_actuel();
  if (t != tour){
    printf("t != tour : %d, %d", t, tour);
    abort();
  }
  position from;
  position to;
  from.x = (mon_equipe() == 0) ? 0 : TAILLE_TERRAIN - 1;
  
  from.y = (mon_equipe() == 0) ? tour - 1 : TAILLE_TERRAIN - tour;
  to.x = (mon_equipe() == 0) ? 0 : TAILLE_TERRAIN - 1;
  to.y =  (mon_equipe() == 0) ? tour : TAILLE_TERRAIN - tour - 1;

  int indice = mon_equipe() == 0 ? 0 : 1;

  erreur err = deplacer(indice, from, to);

  cancel();

  afficher_position(from);
  afficher_position(to);
  printf("tour : %d\n", tour);
  afficher_erreur(err);
  printf("\n");
  
  //show_map();
}

/*!
** Fonction appellée à la fin de la partie
*/
void end_game(void)
{
  /* fonction a completer */
}

/*!
** Affiche le contenu d'une valeur de type erreur
*/

/*!
** Affiche le contenu d'une valeur de type type_case
*/

/*!
** Affiche le contenu d'une valeur de type type_bonus
*/

/*!
** Affiche le contenu d'une valeur de type position
*/

/*!
** Affiche le contenu d'une valeur de type source_energie
*/

/*!
** Affiche le contenu d'une valeur de type trainee_moto
*/

