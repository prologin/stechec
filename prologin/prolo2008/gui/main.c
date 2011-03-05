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
int xpos = 0, ypos = 0;
Partie jeu;
Uint32 color_black;
TTF_Font *font_small, *font_big;
void *g_ccx = NULL;

/* These functions are defined in common stechec rules and in next_turn.cc. */
extern int api_state_is_end(void*);
extern int client_cx_process(void*);
extern void next_turn(void *client_cx);
extern int  get_state(void *api);
extern void set_event_handler(void *api, void (*fn)(void));

// defined originally in BaseRules.hh, but C++ header... :)
#define GS_END 0xffff

void	next_turn_callback(void)
{
  nouveau_tour(&jeu);
  render_map();
}

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

  SDL_EnableKeyRepeat(100, 300);

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

	case SDL_MOUSEBUTTONUP:
	  xpos = event.button.x / TAILLE_CASE;
	  ypos = event.button.y / TAILLE_CASE;
	  render_map();
	  break;

	case SDL_KEYDOWN:
	  switch(event.key.keysym.sym)
	  {
	    case SDLK_n: /* go to the next turn */
	      next_turn(g_ccx);
	      done = 1;
	      break;
	    case SDLK_q:
	    case SDLK_ESCAPE:
	      exit(0);
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

int run(void* foo, void* api, void* client_cx)
{
  init_game();
  set_event_handler(api, next_turn_callback);

  g_ccx = client_cx;
  while (get_state(api) != GS_END)
  {
    // Play a turn.
    // When this return, it means the user asked to go to
    // the next turn.
    play_turn();

    // Process our messages.
    process_messages(client_cx);
  }

  //end_game();
  foo = foo;
  return 0;
}
