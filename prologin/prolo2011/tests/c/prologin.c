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
  printf("tour : %d\n", tour);
  show_map();
  trainee_moto * arr;
  int nb, i;
  trainees_moto(&arr, &nb);
  for (i = 0; i < nb; i++){
    afficher_trainee_moto(arr[i]);
  }
}

/*!
** Fonction appellée à la fin de la partie
*/
void end_game(void)
{
  /* fonction a completer */
}

