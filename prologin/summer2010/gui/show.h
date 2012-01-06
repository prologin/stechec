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

#ifndef _SUMMER2010_SHOW_H__
#define _SUMMER2010_SHOW_H__


#include <SDL_ttf.h>
#include "prologin.hh"

void init();
int end();
void event_turn();
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define VIDEO_FORMAT (SDL_HWSURFACE | SDL_RESIZABLE)
#define FONT_SIZE 20

#define CASE_WIDTH 50
#define CASE_HEIGHT 50

void quit_sdl(void);
SDL_Surface*get_text_surface();
void display_map();


#endif
