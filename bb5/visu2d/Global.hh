/*
** TowBowlTactics, an adaptation of the tabletop game Blood Bowl.
** 
** Copyright (C) 2006 The TBT Team.
** 
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
** 
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
** 
** The TBT Team consists of people listed in the `AUTHORS' file.
*/
#ifndef GENERAL_H
#define GENERAL_H

using namespace std;

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#ifndef MAX
#define MAX(a,b)    (((a) > (b)) ? (a) : (b))
#endif

#ifndef TBTHOME
#define TBTHOME ".."
#endif

#define TEAM_PATH TBTHOME "/data/team/"
#define IMG_PATH TBTHOME "/data/image/"
#define SAVES_PATH TBTHOME "/saves/"
#define VIDEOS_PATH TBTHOME "/data/video/"
#define SOUNDS_PATH TBTHOME "/data/sound/"
#define I18N_PATH TBTHOME "/data/i18n/"
#define FORMATIONS_PATH TBTHOME "/data/formatios/"
#define FONTS_PATH TBTHOME "/data/font/"

#define CONFIG_FILE TBTHOME "/config.xml"

#define ADD_IMG_PATH(x) IMG_PATH x
#define ADD_TEAM_PATH(x) TEAM_PATH x
#define ADD_SAVES_PATH(x) SAVES_PATH x
#define ADD_VIDEOS_PATH(x) VIDEOS_PATH x
#define ADD_SOUNDS_PATH(x) SOUNDS_PATH x
#define ADD_I18N_PATH(x) I18N_PATH x
#define ADD_FORMATIONS_PATH(x) FORMATIONS_PATH x
#define ADD_FONTS_PATH(x) FONTS_PATH x

#define DIALOG_ALPHA 234

// dialog button
#define YES 1
#define NO 2
#define CLOSE 3

// menu button
#define OK 6
#define EXIT 8
#define BACK 10
#define NETWORK 11
#define STANDALONE 12
#define SERVER 13
#define OPTIONS 14

// FIXME: appear to be defined to DialogBoxA and LoadImageA on windows...
#undef DialogBox
#undef LoadImage

//FIXME: 
typedef unsigned uint;
typedef unsigned short ushort;

/*! @brief Generiq Load Image function
**
** This function load an image in a SDL_Surface and return it.
** If tansparency is choose, sets the color transparent pixel
**
** @author poltuiu
** @date 2006-03-15
*/
SDL_Surface *LoadImage(const std::string filename, int transparency); 

/*! @brief To print text in SDL_Surface
**
** Split string in lines with '\n' caracter,
** Create a TTF_RenderText_Solid with the result and
** Blit it
**
** @author poltuiu
** @date 2006-03-15
*/
void PrintStrings( SDL_Surface *sDest, TTF_Font *fnt, std::string str,
                   SDL_Rect &rc, SDL_Color clrFg);
#endif
