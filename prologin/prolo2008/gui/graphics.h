#ifndef DEF_GRAPHICS_H
#define DEF_GRAPHICS_H

#include "players.h"

#define BITRATE         32

#define TAILLE_CASE     47 //taille d'une case
#define TAILLE_CONTOUR  2

#define NB_OBSTACLES    1   //toutes les textures d'obstacles

#define NB_TILES        6   //toutes les textures de terrain
enum {SOL = 0, TROU, CONTOUR_H1, CONTOUR_V1, CONTOUR_H2, CONTOUR_V2};

#define NB_SPRITES      6  //hamster, pomme, petite pomme, pepin
enum {HAMSTER1 = 0, HAMSTER2, HAMSTER1_POMME, HAMSTER2_POMME, POMME, PEPIN};

//retourne une surface � partir d'un fichier bitmap
SDL_Surface* MyBMPLoad(char *filename);

//retourne une surface d'aire w*h
SDL_Surface* MySurfaceCreate(int w, int h);

//rend la couleur (255, 0, 255) (fushia) transparente
void transparence(SDL_Surface *s);

//initialise l'�cran et retourne la surface principale
SDL_Surface* window_init();

//cr�� le fond de carte : sol tranch�es et obstacles
SDL_Surface* background_init(const int w, const int h);

//retourne un vecteur contenant tous les sprites : hamster, pomme, p�pin...
SDL_Surface **sprites_init();

//colle le fond bg et les sprites selon l'�tat de la partie p sur la surface board
void display_game(Partie *p, SDL_Surface *board, SDL_Surface **sprites, SDL_Surface *bg);

//retourne 1 si la position est (graphiquement) sur le terrain, 0 sinon
int in_gamefield(const int x, const int y);

#endif
