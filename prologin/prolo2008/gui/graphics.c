#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <time.h>
#include "graphics.h"
#include "prologin.h"
#include "constants.h"

extern int win_xsize, win_ysize;
extern TTF_Font	*font;
extern SDL_Surface *numbers[MAX_ROBOTS];

SDL_Surface* MyIMGLoad(const char *filename)
{
    SDL_Surface *res = IMG_Load(filename);
    if(!res)
        printf("Unable to load BMP file \"%s\": %s\n", filename, IMG_GetError());
    return res;
}

SDL_Surface* MySurfaceCreate(int w, int h)
{
    SDL_Surface *res = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, BITRATE, 0, 0, 0, 0);
    if(!res)
        printf("Unable to create %d*%d RGB surface\n", w, h);
    return res;
}

SDL_Surface* window_init()
{
    SDL_Surface* screen;

    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return NULL;
    }

    win_xsize = taille_carte_x() * TAILLE_CASE + 2;
    win_ysize = taille_carte_y() * TAILLE_CASE + 2;

    screen = SDL_SetVideoMode(win_xsize, win_ysize, BITRATE,
                                SDL_HWSURFACE|SDL_DOUBLEBUF);

    if (!screen)
    {
        printf("Unable to set %dx%d video: %s\n",
                win_xsize, win_ysize, SDL_GetError());
        return NULL;
    }

    SDL_WM_SetCaption("ProloGUIne 2008", "");

    return screen;
}

SDL_Surface* background_init(const int w, const int h)
{
    SDL_Surface* result = NULL;
    SDL_Surface* tiles[NB_TILES] = {0};
    SDL_Surface* obstacles[NB_OBSTACLES] = {0};
    SDL_Rect pos;
    int i, j;

    result = MySurfaceCreate(w * TAILLE_CASE + 2, h * TAILLE_CASE + 2);

    tiles[SOL]          = MyIMGLoad(IMG_PREFIX "ground.png");
    tiles[TROU]         = MyIMGLoad(IMG_PREFIX "hole.png");
    tiles[CONTOUR_H]    = MyIMGLoad(IMG_PREFIX "hole_horiz.png");
    tiles[CONTOUR_V]    = MyIMGLoad(IMG_PREFIX "hole_vert.png");

    for(i = 0; i < NB_TILES; i++)
        if(tiles[i] == NULL)
            return NULL;

    obstacles[0] = MyIMGLoad(IMG_PREFIX "rock.png");

    for(i = 0; i < NB_OBSTACLES; i++)
        if(obstacles[i] == NULL)
            return NULL;

    //contour de la carte
    SDL_FillRect(result, NULL, SDL_MapRGB(result->format, 255, 255, 255));

    srand(time(NULL));

    //blitte les tiles
    for(i = 0; i < w; i++)
        for(j = 0; j < h; j++)
        {
            pos.x = i * TAILLE_CASE + 1;
            pos.y = j * TAILLE_CASE + 1;
            if(type_case(i, j) != TRANCHEE)
            {
                SDL_BlitSurface(tiles[SOL], NULL, result, &pos);
                if(type_case(i, j) == OBSTACLE)
                    SDL_BlitSurface(obstacles[0], NULL, result, &pos);
            }
            else
            {
                SDL_BlitSurface(tiles[TROU], NULL, result, &pos);

                //permet d'afficher des contours aux tranchées
                if(i > 0 && type_case(i - 1, j) != TRANCHEE)
                    SDL_BlitSurface(tiles[CONTOUR_V], NULL, result, &pos);

                if(i < w - 1 && type_case(i + 1, j) != TRANCHEE)
                {
                    pos.x += TAILLE_CASE - TAILLE_CONTOUR;
                    SDL_BlitSurface(tiles[CONTOUR_V], NULL, result, &pos);
                    pos.x -= TAILLE_CASE - TAILLE_CONTOUR;
                }

                if(j > 0 && type_case(i, j - 1) != TRANCHEE)
                    SDL_BlitSurface(tiles[CONTOUR_H], NULL, result, &pos);

                if(j < h - 1 && type_case(i, j + 1) != TRANCHEE)
                {
                    pos.y += TAILLE_CASE - TAILLE_CONTOUR;
                    SDL_BlitSurface(tiles[CONTOUR_H], NULL, result, &pos);
                }
            }
        }

    for(i = 0; i < NB_TILES; i++)
      SDL_FreeSurface(tiles[i]);

    return result;
}

SDL_Surface** sprites_init()
{
    int i;
    Uint32 transp_color;

    SDL_Surface** res = malloc(sizeof(*res) * NB_SPRITES);

    res[HAMSTER1] = MyIMGLoad(IMG_PREFIX "hamster_1.png");
    res[HAMSTER2] = MyIMGLoad(IMG_PREFIX "hamster_2.png");
    res[HAMSTER1_POMME] = MyIMGLoad(IMG_PREFIX "hamster_1_apple.png");
    res[HAMSTER2_POMME] = MyIMGLoad(IMG_PREFIX "hamster_2_apple.png");
    res[POMME] = MyIMGLoad(IMG_PREFIX "apple.png");
    res[PEPIN] = MyIMGLoad(IMG_PREFIX "pin.png");

    for(i = 0; i < NB_SPRITES; i++)
        if(res[i] == NULL)
            return NULL;

    char number[10];
    SDL_Color black = {200, 0, 0};
    for (i =0; i < MAX_ROBOTS; i++)
    {
      snprintf(number, 9, "%i", i);
      numbers[i] = TTF_RenderText_Solid(font, number, black);
    }
    return res;
}


void display_game(Partie *p, SDL_Surface *board, SDL_Surface **sprites, SDL_Surface *bg)
{
  int x, y;
  int i, n = p->nb_tours - 1;
  SDL_Rect blitPos;

#ifndef REPLAY
  n = 0;
#endif

  printf("Rendering the map.\n");

  //affiche l'arriere plan
  SDL_BlitSurface(bg, 0, board, NULL);
  blitPos.w = TAILLE_CASE;
  blitPos.h = TAILLE_CASE;
  //puis tous les hamsters
  for(i = 0; i < MAX_ROBOTS; i++)
  {
    printf("Drawing hamster %i\n", i);
    blitPos.x = p->tours[n].hamsters[i].x * TAILLE_CASE;
    blitPos.y = p->tours[n].hamsters[i].y * TAILLE_CASE;
    if (i < 3)
      SDL_BlitSurface(porte_pomme(i) ? sprites[HAMSTER1_POMME] : sprites[HAMSTER1], 0, board, &blitPos);
    else

    // Blit du nombre
    SDL_BlitSurface(porte_pomme(i) ? sprites[HAMSTER2_POMME] : sprites[HAMSTER2], 0, board, &blitPos);
    // Valeurs hardcodees.
    blitPos.x += TAILLE_CASE / 2 - 2;
    blitPos.y += TAILLE_CASE / 1.5 + 1;
    SDL_BlitSurface(numbers[i], 0, board, &blitPos);
  }
  for (y = 0; y < taille_carte_y(); ++y)
    for (x = 0; x < taille_carte_x(); ++x)
      if (pomme(x, y))
      {
	blitPos.x = x * TAILLE_CASE;
	blitPos.y = y * TAILLE_CASE;
        SDL_BlitSurface(sprites[POMME], 0, board, &blitPos);
      }
}


int in_gamefield(const int x, const int y)
{
  return (x >= 0 && x < (TAILLE_CASE * taille_carte_x())
      && y >= 0 && y < (TAILLE_CASE * taille_carte_y()));
}

