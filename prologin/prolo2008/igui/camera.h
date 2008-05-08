#ifndef DEF_CAMERA_H
#define DEF_CAMERA_H

#include <SDL.h>
#include "graphics.h"

#define CAMERA_WIDTH        WINDOW_WIDTH - MENU_WIDTH
#define CAMERA_HEIGHT       WINDOW_HEIGHT
#define CAMERA_MOVE_SIZE    (TAILLE_CASE / 2)

//d�place la cam�ra selon un vecteur
void moveCam(SDL_Rect * cam, int x, int y);

//retourne 1 si la cam�ra peut se placer � une position, 0 sinon
int correct_cam(const int x, const int y);

#endif
