#include <SDL.h>
#include "players.h"
#include "prologin.h"
#include "constants.h"

void init_jeu(Partie *p)
{
  printf("Allocating memory for %i turns...\n", nombre_tours());
#ifdef REPLAY
  p->tours = malloc(nombre_tours() * sizeof(Tour));
#else
  p->tours = malloc(sizeof(Tour)); 
#endif
  p->nb_tours = 0;
  p->cur_hamster = 0;
}

// updates every position
void nouveau_tour(Partie *p)
{
    int i;
    int n = p->nb_tours;

#ifndef REPLAY
    n = 0;
#endif

    printf("Retrieving data for new turn...\n");
    for(i = 0; i < MAX_ROBOTS; i++)
     {
	 p->tours[n].hamsters[i].w = p->tours[n].hamsters[i].x;
	 p->tours[n].hamsters[i].h = p->tours[n].hamsters[i].y;
	 p->tours[n].hamsters[i].x = pos_x(i);
	 p->tours[n].hamsters[i].y = pos_y(i);
	 printf("Hamster %i is in [%i,%i]\n", i, p->tours[n].hamsters[i].x, p->tours[n].hamsters[i].y);
     }
     p->nb_tours++;
     p->cur_hamster = 0;
}
