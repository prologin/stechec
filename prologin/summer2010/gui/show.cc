/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2005, 2006 Prologin
** author : Maxime AUDOUIN : <coucou747@gmail.com>
*/

#include "show.h"
#include <assert.h>
#include <iostream>
#include <map>
#include <vector>

bool operator<(position p1, position p2){
  return p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y);
}

#ifndef __MINGW32__
void get_path(char* buffer, size_t len, const char* name)
{
    snprintf(buffer, len, PKGDATADIR "/graphics/%s", name);
}

#else
# include <windows.h>

void get_path(char* buffer, size_t len, const char* name)
{
    char* it = buffer;

    GetModuleFileName(GetModuleHandle(NULL), buffer, len);
    while (*it) ++it;
    while (*it != '\\') --it;
    *it = 0;
    strncat(buffer, "\\summer2010\\graphics\\", len);
    strncat(buffer, name, len);
}
#endif

SDL_Surface* load_image(const char* name)
{
    char buffer[256];
    get_path(buffer, 256, name);
    return SDL_LoadBMP(buffer);
}

TTF_Font* load_ttf(const char* name, int fontsize)
{
    char buffer[256];
    get_path(buffer, 256, name);
    return TTF_OpenFont(buffer, fontsize);
}

enum
{
  IMG_VIDE = 0,
  IMG_PLAYER1,
  IMG_PLAYER2,
  IMG_PLAYER1_KO,
  IMG_PLAYER2_KO,
  IMG_OR1,
  IMG_OR2,
  IMG_OR3,
  IMG_OR4,
  IMG_OR5,
  IMG_OR6,
  IMG_OR7,
  IMG_OR8,
  IMG_OR9,
  NUMBER_OF_IMAGES
};
static SDL_Surface* images[NUMBER_OF_IMAGES] = {NULL};
static const char* images_path[NUMBER_OF_IMAGES] = {
  "vide.bmp",
  "player1.bmp",
  "player2.bmp",
  "player1_ko.bmp",
  "player2_ko.bmp",
  "or_1.bmp",
  "or_2.bmp",
  "or_3.bmp",
  "or_4.bmp",
  "or_5.bmp",
  "or_6.bmp",
  "or_7.bmp",
  "or_8.bmp",
  "or_9.bmp",
};

TTF_Font* font;
typedef int color;
static color background;
static color forground;
int alpha = 200;
int transparent;

SDL_Color textcolor(){
  static SDL_Color c ={ 0, 100, 250 };
  return c;
}

SDL_Surface* get_screen()
{
  static SDL_Surface* screen = NULL;
  if (screen == NULL){
    if (SDL_Init(SDL_INIT_VIDEO)) {
      fprintf(stderr, "Error: unable to initialize SDL\n");
      return NULL;
    }
    if (TTF_Init() == -1) {
      fprintf(stderr, "Error: unable to initialize SDL_ttf\n");
      SDL_Quit();
      return NULL;
    }
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, VIDEO_FORMAT);
    if (!screen)
      {
	fprintf(stderr, "Error: unable to set video mode\n");
	quit_sdl();
	}
    SDL_WM_SetCaption("SUMMER 2010", NULL);
  }
  return screen;
}

void text_surface_clear(){
  SDL_Surface * txt = get_text_surface();
  SDL_FillRect(txt, NULL, transparent);
  SDL_SetColorKey(txt, SDL_SRCCOLORKEY, transparent);
  SDL_SetAlpha( txt, SDL_SRCALPHA | SDL_RLEACCEL, alpha );
  SDL_Flip(txt);
}

bool reset_text = false;

SDL_Surface*get_text_surface(){
  static SDL_Surface* text_surface = NULL;
  if (text_surface == NULL || reset_text){
    reset_text = false;
    SDL_Surface * screen = get_screen();
    text_surface = SDL_CreateRGBSurface(SDL_HWSURFACE, screen->w, screen->h, 32, 0,0,0,0);
    text_surface_clear();
  }
  return text_surface;
}

void draw_text(char *t, int x, int y){
  SDL_Surface *s = get_text_surface();
  SDL_Rect pos;
  SDL_Surface * message = TTF_RenderText_Blended(font, t, textcolor());
  SDL_Surface * background = SDL_CreateRGBSurface(SDL_HWSURFACE, message->w, message->h, 32, 0,0,0,0);
  SDL_FillRect(background, NULL, SDL_MapRGB(background->format, 190, 190, 170));
  pos.x = x;
  pos.y = y;
  SDL_BlitSurface(background, NULL, s, &pos);
  SDL_BlitSurface(message, NULL, s, &pos);
  SDL_FreeSurface(message);
  SDL_Flip(s);
}

void draw_text(char *t, SDL_Rect pos){
  draw_text(t, pos.x, pos.y);
}

void quit_sdl()
{
    TTF_Quit();
    SDL_Quit();
}

void init() {
  int i;
  get_screen();
  SDL_Surface *text = get_text_surface();
  background = SDL_MapRGB(SDL_GetVideoSurface()->format, 70,0,0);
  forground = SDL_MapRGB(SDL_GetVideoSurface()->format, 255,255,255);
  for (i = 0; i < NUMBER_OF_IMAGES; ++i)
    {
      images[i] = load_image(images_path[i]);
      if (images[i] == NULL){
	std::cout << "fail while loading : " << images_path[i] << " in " << PKGDATADIR << std::endl;
	abort();
      }
    }
  font = load_ttf("FreeSerif.ttf", FONT_SIZE);
  assert(font != NULL);
  transparent = SDL_MapRGB(text->format, 0,0,0);
  text_surface_clear();
  draw_text("appuyez sur espace pour faire avancer le jeu", 10, 150);
  display_map();
  SDL_Delay(3000);
  text_surface_clear();
}

int end() {
  for (int i = 0; i < NUMBER_OF_IMAGES; ++i)
    SDL_FreeSurface(images[i]);
  TTF_CloseFont(font);
  quit_sdl();
  return 0;
}
int viewpoint_y = 0;
int viewpoint_x = 0;

void resized(SDL_Event event){
  SDL_Surface *old_text_surface = get_text_surface();
  SDL_SetVideoMode(event.resize.w, event.resize.h,0, VIDEO_FORMAT);
  reset_text = true;
  SDL_Surface *text_surface = get_text_surface();
  SDL_BlitSurface(old_text_surface, NULL, text_surface, NULL);
  SDL_FreeSurface(old_text_surface);
  display_map();
}

void event_turn() {
  static int noshow = 0;
  if (!noshow){
    SDL_Event event;
    display_map();
    while(1){
      SDL_WaitEvent(&event);
      switch(event.type){
      case SDL_VIDEORESIZE:
	resized(event);
	break;
      case SDL_QUIT:
	noshow = 1;
	return;
      case SDL_KEYDOWN:
	switch (event.key.keysym.sym){
	case SDLK_ESCAPE:
	  noshow = 1;
	  return;
	case SDLK_SPACE:
	case SDLK_RETURN:
	  return;
	case SDLK_DOWN:
	  viewpoint_y = viewpoint_y + 1;
	  break;
	case SDLK_UP:
	  viewpoint_y = viewpoint_y - 1;
	  break;
	case SDLK_LEFT:
	  viewpoint_x = viewpoint_x - 1;
	  break;
	case SDLK_RIGHT:
	  viewpoint_x = viewpoint_x + 1;
	  break;
	default:
	  break;
	}
	display_map();
	break;
      default:
	display_map();
	break;
      }
    }
  }
}


void display_map(){

  SDL_Surface *screen = get_screen();
  int ncasesx = screen->w / CASE_WIDTH + (screen->w % CASE_WIDTH != 0);
  int ncasesy = screen->h / CASE_HEIGHT + (screen->h % CASE_HEIGHT != 0);
  SDL_FillRect(screen, NULL, background);
  text_surface_clear();
  std::vector<piece> vect_pej = pieces_en_jeu();
  std::vector<unite> vect_u = unites();
  std::vector<piece> vect_pav = pieces_a_vennir();
  std::map<position, int> map;
  for (int i = 0, s = vect_pej.size(); i < s; i ++) {
    piece p = vect_pej[i];
    map[p.pos_piece] = IMG_OR1 - 1 + p.valeur;
  }
  for (int i = 0, s = vect_pav.size(); i < s; i ++) {
    piece p = vect_pav[i];
    //map[p.pos_piece] = IMG_OR1 - 1 + p.valeur;
  }
  for (int i = 0, s = vect_u.size(); i < s; i ++) {
    unite u = vect_u[i];
    map[u.pos_unite] = IMG_PLAYER1 + u.team + ( (u.ko != 0) ? 2 : 0 );
  }
  for (int x = 0; x < ncasesx; x++)
    {
      for (int y = 0; y < ncasesy; y++)
        {
	  SDL_Rect pos;
	  pos.x = x * CASE_WIDTH;
	  pos.y = y * CASE_HEIGHT;
	  int rx = x + viewpoint_x;
	  int ry = y + viewpoint_y;
	  position pos_i;
	  pos_i.x = rx;
	  pos_i.y = ry;
	  int pic = -1;
	  if (rx >= 0 && ry >= 0 && rx < TAILLE_TERRAIN && ry < TAILLE_TERRAIN){
	    if (map.count(pos_i)){
	      pic = map[pos_i];
	    }else{
	      pic = IMG_VIDE;
	    }
	  }
	  if (pic != -1) SDL_BlitSurface(images[pic], NULL, screen, &pos);
        }
    }
  { // clean text surface
    SDL_Rect pos;
    pos.x = 0;
    pos.y = 0;
    pos.w = 200;
    pos.h = 150;
    SDL_FillRect(screen, &pos, forground);
  }
  for (int i = 0; i <= 1 ; i ++){
    int score_ = score(i);
    SDL_Rect pos;
    pos.x = 0;
    pos.y = i * 50;
    SDL_BlitSurface(images[IMG_PLAYER1 + i], NULL, screen, &pos);
    pos.x = 100;
    char buffer[255];
    sprintf(buffer, "score : %d", score_);
    draw_text(buffer, pos);
  }

  {
    SDL_Rect pos;
    pos.x = 0;
    pos.y = 100;
    int tour = tour_actuel();
    char buffer[255];
    sprintf(buffer, "tour : %d", tour);
    draw_text(buffer, pos);
  }
  SDL_BlitSurface(get_text_surface(), NULL, screen, NULL);
  SDL_Flip(screen);
}
