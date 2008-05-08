#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "constants.h"
#include "prologin.h"
#include "graphics.h"
#include "camera.h"
#include "players.h"

SDL_Surface *screen, *game_board, *game_background, **sprites;
SDL_Surface *numbers[MAX_ROBOTS];
SDL_Event event;
SDL_Rect board_pos;
int done;
int win_xsize = 0, win_ysize = 0;
Partie jeu;
Uint32 color_black;
TTF_Font *font;

void	render_map(void)
{
  display_game(&jeu, game_board, sprites, game_background);
  SDL_FillRect(screen, 0, color_black);
  SDL_BlitSurface(game_board, 0, screen, 0);
  SDL_Flip(screen);
}

void	init_game(void)
{
  printf("GUI Initialization...\n");

  TTF_Init();

  font = TTF_OpenFont(IMG_PREFIX "font.ttf", 10);
  if (!font)
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

	case SDL_KEYDOWN:
	  switch(event.key.keysym.sym)
	  {
	    case SDLK_n: /* go to the next turn */
	      done = 1;
	      nouveau_tour(&jeu);
	      break;
	    case SDLK_RIGHT:
	      //moveCam(&camera, CAMERA_MOVE_SIZE, 0);
	      break;
	    case SDLK_UP:
	      //moveCam(&camera, 0, - CAMERA_MOVE_SIZE);
	      break;
	    case SDLK_DOWN:
	      //moveCam(&camera, 0, CAMERA_MOVE_SIZE);
	      break;
	    case SDLK_LEFT:
	      //moveCam(&camera, - CAMERA_MOVE_SIZE, 0);
	      break;
	    default: break;
	  }
	  // We don't break in order to redraw the map.

	case SDL_VIDEOEXPOSE:
	  render_map();
	  break;
      }
      usleep(500); // Don't kill our CPU.
    }
  }
}

/* These functions are defined in common stechec rules. */
extern int api_state_is_end(void*);
extern int client_cx_process(void*);
extern void next_turn(void*);

int run(void* foo, void* api, void* client_cx)
{
  init_game();

  while (!api_state_is_end(api))
  {
    // Play a turn.
    // When this return, it means the user asked to go to
    // the next turn.
    play_turn();

    // Process our messages.
    while (client_cx_process(client_cx))
      ;
    printf("Proceeding to the next turn...\n");
    next_turn(client_cx);
  }

  //end_game();
  foo = foo;
  return 0;
}
