#include <SDL.h>
#include "graphics.h"
#include "camera.h"
#include "constants.h"
#include "prologin.h"


void moveCam(SDL_Rect * cam, int x, int y)
{
    if(correct_cam(cam->x + x, cam->y + y))
    {
        cam->x += x;
        cam->y += y;
    }
}


int correct_cam(const int x, const int y)
{
    //return (x >= - FIELD_MARGIN) && (x + CAMERA_WIDTH <= 2 * FIELD_MARGIN + (TAILLE_CASE * taille_carte_x()))
    //    && (y >= - FIELD_MARGIN) && (y + CAMERA_HEIGHT <= 2 * FIELD_MARGIN + (TAILLE_CASE * taille_carte_y()));
}
