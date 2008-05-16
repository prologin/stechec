#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "constants.h"
#include "prologin.h"
#include "graphics.h"
#include "camera.h"
#include "players.h"
#include "orders.h"
#include "input.h"

SDL_Surface *screen, *game_board, *game_background, **sprites;
SDL_Surface *numbers[MAX_ROBOTS];
SDL_Event event;
SDL_Rect board_pos;
int done;
int win_xsize = 0, win_ysize = 0;
Partie jeu;
int xpos = 0, ypos = 0;
Uint32 color_black;
TTF_Font *font_small, *font_big;

struct AwaitingOrders awaiting_orders;

void	render_map(void)
{
  display_game(&jeu, game_board, sprites, game_background);
  SDL_FillRect(screen, 0, color_black);
  SDL_BlitSurface(game_board, 0, screen, 0);
  SDL_Flip(screen);
}

void	init_game(void)
{

  printf(" ********* Touches permettant le controle des hamsters de la GUI *******\n"
	 "Chaque ordre est entre par une sequence de deux ou trois touches :\n"
	 "1) 3,4 ou 5 pour le hamster a controler\n"
	 "2) w,r,l,d,t,g,a pour respectivement: attendre, ramasser_pomme,\n"
	 " lacher_pomme, deplacement,  turbo, grappin, trognon (attaque)\n"
	 "3) Pour les ordres qui demandent une direction, entrer la direction avec\n"
	 "les touches de direction, ou '0' pour la direction ICI\n\n"
	 "Pour aller au tour suivant: touche 'n', pour quitter, 'q'\n\n"
	 );

  printf("GUI Initialization...\n");

  awaiting_orders.next_order = 0;

  if (TTF_Init() == -1)
  {
    fprintf(stderr, "TTF Initlization error: %s\n", TTF_GetError());
    exit(1);
  }

  font_small = TTF_OpenFont(IMG_PREFIX "font.ttf", 10);
  font_big = TTF_OpenFont(IMG_PREFIX "font.ttf", 14);
  if (!font_small || !font_big)
  {
    fprintf(stderr, "Unable to open TTF font: %s\n", TTF_GetError());
    exit(1);
  }

  screen = window_init();

  if(!screen)
  {
    fprintf(stderr, "Unable to open display.  Aborting.\n");
    exit(1);
  }

  sprites = sprites_init();
  if(!sprites)
  {
    fprintf(stderr, "Unable to initalize sprites.  Aborting.\n");
    exit(1);
  }

  color_black = SDL_MapRGB(screen->format, 0, 0, 0);

  game_board = MySurfaceCreate(taille_carte_x() * TAILLE_CASE + 2, taille_carte_y() * TAILLE_CASE + 2);

  game_background = background_init(taille_carte_x(), taille_carte_y());

  if(!game_background)
  {
    fprintf(stderr, "Unable to load background.\n");
    exit(1);
  }

  init_jeu(&jeu);
  sprites = sprites_init();

  nouveau_tour(&jeu);
  render_map();
}

void	play_turn(void)
{

  printf("Playing a turn.\n");
  //main loop
  done = 0;
  nouveau_tour(&jeu);
  render_map();
  while (!done)
  {
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
	case SDL_QUIT:
	  printf("Exit request caught, exiting.\n");
	  exit(1);
	  break;
	  
	case SDL_MOUSEBUTTONUP:
	  xpos = event.button.x / TAILLE_CASE;
	  ypos = event.button.y / TAILLE_CASE;
	  render_map();
	  break;

	case SDL_KEYDOWN:
	  switch(event.key.keysym.sym)
	  {
	    case SDLK_n: /* go to the next turn */
	      SendOrders(&awaiting_orders);
	      done = 1;
	      break;
	    case SDLK_q:
	    case SDLK_ESCAPE:
	      exit(0);
	      break;
	    default: handle_input(&event);
	  }
	  break;

	case SDL_VIDEOEXPOSE:
	  render_map();
	  break;
      }
      usleep(500); // Don't kill our CPU.
    }
  }
}
