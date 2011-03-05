#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <time.h>
#include "graphics.h"
#include "prologin.h"
#include "constants.h"

extern int win_xsize, win_ysize;
extern TTF_Font	*font_small, *font_big;
extern SDL_Surface *numbers[MAX_ROBOTS];

static SDL_Surface *turns[MAX_TURN] = {NULL};
static SDL_Surface *pos = NULL;

extern int xpos, ypos;


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
    tiles[CONTOUR_H2]    = MyIMGLoad(IMG_PREFIX "hole_horiz2.png");
    tiles[CONTOUR_H1]    = MyIMGLoad(IMG_PREFIX "hole_horiz1.png");
    tiles[CONTOUR_V1]    = MyIMGLoad(IMG_PREFIX "hole_vert1.png");
    tiles[CONTOUR_V2]    = MyIMGLoad(IMG_PREFIX "hole_vert2.png");

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
            if(type_case(i, taille_carte_y() - j - 1) != TRANCHEE)
            {
                SDL_BlitSurface(tiles[SOL], NULL, result, &pos);
                if(type_case(i, taille_carte_y() - j - 1) == OBSTACLE)
                    SDL_BlitSurface(obstacles[0], NULL, result, &pos);
            }
            else
            {
                SDL_BlitSurface(tiles[TROU], NULL, result, &pos);

                //permet d'afficher des contours aux tranchées
                if(i > 0 && type_case(i - 1, taille_carte_y() - j - 1) != TRANCHEE)
                    SDL_BlitSurface(tiles[CONTOUR_V1], NULL, result, &pos);

                if(i < w - 1 && type_case(i + 1, taille_carte_y() - j - 1) != TRANCHEE)
                {
                    pos.x += TAILLE_CASE - TAILLE_CONTOUR;
                    SDL_BlitSurface(tiles[CONTOUR_V2], NULL, result, &pos);
                    pos.x -= TAILLE_CASE - TAILLE_CONTOUR;
                }

                if(j > 0 && type_case(i, taille_carte_y() - j - 1 - 1) != TRANCHEE)
                    SDL_BlitSurface(tiles[CONTOUR_H1], NULL, result, &pos);

                if(j < h - 1 && type_case(i, taille_carte_y() - j -1 + 1) != TRANCHEE)
                {
                    pos.y += TAILLE_CASE - TAILLE_CONTOUR;
                    SDL_BlitSurface(tiles[CONTOUR_H2], NULL, result, &pos);
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
    SDL_Color black = {0, 0, 0};

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
    SDL_Color turn_color = {100, 50, 0};
    TTF_SetFontStyle(font_big, TTF_STYLE_BOLD);
    for (i = 0; i < MAX_ROBOTS; i++)
    {
      snprintf(number, 9, "%i", i);
      TTF_SetError("No error.");
      numbers[i] = TTF_RenderText_Solid(font_small, number, black);
      if (!numbers[i])
	fprintf(stderr, "SDL_ttf error (numbers rendering): %s\n", TTF_GetError());
    }

    char buf[64];
    // Draws the current turn number
    for (i = 0; i < MAX_TURN; ++i)
    {
      TTF_SetError("No error. Pwet.");
      snprintf(buf, 64, "Tour:%i", i);
      turns[i] = TTF_RenderText_Blended(font_big, buf, black);
      if (!turns[i])
	fprintf(stderr, "SDL_ttf error (turn rendering): %s\n", TTF_GetError());
    }
    return res;
}


void display_game(Partie *p, SDL_Surface *board, SDL_Surface **sprites, SDL_Surface *bg)
{
  int x, y;
  int i, n = p->nb_tours - 1;
  SDL_Rect blitPos;
  char	buf[64];

#ifndef REPLAY
  n = 0;
#endif

  // Blits the background
  SDL_BlitSurface(bg, 0, board, NULL);
  blitPos.w = TAILLE_CASE;
  blitPos.h = TAILLE_CASE;

  // Then all the hamsters
  for(i = 0; i < MAX_ROBOTS; i++)
  {
    //    printf("Displaying hamster id %d in pos_y %d\n",
    //	   i, 
    //	   p->tours[n].hamsters[i].y );

    blitPos.x = p->tours[n].hamsters[i].x * TAILLE_CASE;
    blitPos.y = p->tours[n].hamsters[i].y * TAILLE_CASE;
    if (i < 3)
      SDL_BlitSurface(porte_pomme(i) ? sprites[HAMSTER2_POMME] : sprites[HAMSTER2], 0, board, &blitPos);
    else
      SDL_BlitSurface(porte_pomme(i) ? sprites[HAMSTER1_POMME] : sprites[HAMSTER1], 0, board, &blitPos);
    // Valeurs hardcodees.
    blitPos.x += TAILLE_CASE / 2 - 2;
    blitPos.y += TAILLE_CASE / 1.5 + 1;
    SDL_BlitSurface(numbers[(i + MAX_ROBOTS / 2) % MAX_ROBOTS], 0, board, &blitPos);
  }

  // Draws apples
  for (y = 0; y < taille_carte_y(); ++y)
    for (x = 0; x < taille_carte_x(); ++x)
      if (pomme(x, taille_carte_y() - y - 1))
      {
	blitPos.x = x * TAILLE_CASE;
	blitPos.y = y * TAILLE_CASE;
        SDL_BlitSurface(sprites[POMME], 0, board, &blitPos);
      }
    blitPos.x = 10;
    blitPos.y = 10;
    SDL_BlitSurface(turns[p->nb_tours - 1], NULL, board, &blitPos);

    SDL_Color black = {0, 0, 0};
    snprintf(buf, 64, "pos:%i,%i", xpos, ypos);
    pos = TTF_RenderText_Blended(font_big, buf, black);
    blitPos.x = 10;
    blitPos.y = 30;
    SDL_BlitSurface(pos, NULL, board, &blitPos);
    SDL_FreeSurface(pos);

}


int in_gamefield(const int x, const int y)
{
  return (x >= 0 && x < (TAILLE_CASE * taille_carte_x())
      && y >= 0 && y < (TAILLE_CASE * taille_carte_y()));
}

