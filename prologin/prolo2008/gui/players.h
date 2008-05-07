#ifndef DEF_PLAYERS_H
# define DEF_PLAYERS_H

# include <SDL.h>

typedef SDL_Rect Hamster;

typedef struct
{
    Hamster hamsters[6];
    int     ordre[6]; //stoqués pour le replays
} Tour;

typedef struct
{
    Tour    *tours;
    int     nb_tours;
    int     cur_hamster; //permet de retenir quel hamster on est en train d'animer
} Partie;

//récupère différentes données du serveur pour initialiser
void init_jeu(Partie *p);

//modifie p pour un nouveau tour
void nouveau_tour(Partie *p);

#endif
