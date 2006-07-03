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

 #include "Label.hh"

 #include <iostream>

 Label::Label(const uint x, const uint y, SDL_Surface * screen, Widget* father, const string txt):
             Widget(x, y, 0, 0, screen, father)
 {
   font = TTF_OpenFont (ADD_FONTS_PATH ("Vera.ttf"), 14);
   //If there was an error in loading the font 
   if (font == NULL)
   {
     GUIError error (string ("Not found Vera.ttf"));
     throw (error);
   }
   fgColor = black_color;
   bgColor = fgColor;
   set_txt(txt);
 }

 Label::Label(SDL_Surface * screen, Widget* father, const string txt):
             Widget(screen, father)
 {
   font = TTF_OpenFont (ADD_FONTS_PATH ("Vera.ttf"), 14);
   //If there was an error in loading the font 
   if (font == NULL)
   {
     GUIError error (string ("Not found Vera.ttf"));
     throw (error);
   }
   fgColor = black_color;
   bgColor = fgColor;
   set_txt(txt);
 }

 Label::~Label()
 {
     TTF_CloseFont(font);
 }

 // Draw
 void Label::draw()
 {
     if(visible)
     {
         refresh();
     }
     else
     {
         SDL_Rect r = {get_wx(), get_wy(), w, h};
         // First save background
         SDL_BlitSurface(screen, &r, old_screen, NULL);
         // draw bg
         SDL_BlitSurface(screen, &r, widget, NULL);
         // if bgcolor : add a box under the text
         if(bgColor.r != fgColor.r && bgColor.g != fgColor.g && bgColor.b != fgColor.b)
         {
             // box
             boxRGBA(widget, 0, 0, w, h, bgColor.r, bgColor.g, bgColor.b, 0); 
         }
         // draw text
         SDL_Rect txtr = {0, 0, w, h};
         PrintStrings(widget, font, txt, txtr, fgColor);
         // Then flip label
         SDL_BlitSurface(widget, NULL, screen, &r);
         SDL_Flip(screen);
         visible = true;
     }
 }

 void Label::refresh()
 {
     if(!visible)
     {
         draw();
     }
     else
     {
         SDL_Rect r = {get_wx(), get_wy(), w, h};
         // First draw save background
         SDL_BlitSurface (old_screen, NULL, widget, NULL);
         // draw bg
         SDL_BlitSurface(screen, &r, widget, NULL);
         // if bgcolor : add a box under the text
         if(bgColor.r != fgColor.r && bgColor.g != fgColor.g && bgColor.b != fgColor.b)
         {
             // box
             boxRGBA(widget, 0, 0, w, h, bgColor.r, bgColor.g, bgColor.b, 170); 
         }
         // draw text
         SDL_Rect txtr = {0, 0, w, h};
         PrintStrings(widget, font, txt, txtr, fgColor);
         // Then flip label
         SDL_BlitSurface(widget, NULL, screen, &r);
         SDL_Flip(screen);
     }
 }

 // Label
 void Label::set_txt(const string txt)
 {
     this->txt=txt;

     SDL_Rect r = {get_wx(), get_wy(), w, h};
     // replace background screen by save one
     SDL_BlitSurface(old_screen, NULL, screen, &r);
     // Recalculate width and heigth
     int txtwidth=0, txtheight=0;
     TTF_SizeText(font, txt.c_str(), &txtwidth, &txtheight);
     this->w=txtwidth;
     this->h=txtheight;

     // Recreate widget and old_creen with correct height !
     SDL_FreeSurface(widget);
     SDL_FreeSurface(old_screen);

     old_screen = SDL_CreateRGBSurface (SDL_HWSURFACE,
                      this->w, this->h,
                      screen->format->BitsPerPixel,
                      screen->format->Rmask,
                      screen->format->Gmask,
                      screen->format->Bmask,
                      screen->format->Amask);

     widget = SDL_CreateRGBSurface (SDL_HWSURFACE,
                    this->w, this->h,
                    screen->format->BitsPerPixel,
                    screen->format->Rmask,
                    screen->format->Gmask,
                    screen->format->Bmask,
                    screen->format->Amask);

     r.w=w; r.h=h;
     SDL_BlitSurface(screen, &r, old_screen, NULL);
 }

 string Label::get_txt()
 {
     return(txt);
 }

 // Color
 void Label::setfgColor(SDL_Color fgColor)
 {
     this->fgColor = fgColor;
 }

 void Label::setbgColor(SDL_Color bgColor)
 {
     this->bgColor = bgColor;
 }
