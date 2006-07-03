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

 #include "Button.hh"
 #include "GuiError.hh"

 //
 // ImageButton
 //
 ImageButton::ImageButton (const uint x, const uint y, const uint w, const uint h,
                         SDL_Surface * screen, Widget* father,
                         const string filename, const ushort action):
                         Widget(x, y, w, h, screen, father)
 {
   widget = LoadImage(filename, 1);
   if (widget == NULL)
   {
     GUIError error(string("Not found ") + filename);
     throw(error);
   }
   this->action=action;
 }

 ImageButton::ImageButton (const uint w, const uint h,
                         SDL_Surface * screen, Widget* father,
                         const string filename, const ushort action):
                         Widget(w, h, screen, father)
 {
   widget = LoadImage(filename, 1);
   if (widget == NULL)
   {
     GUIError error(string("Not found ") + filename);
     throw(error);
   }
   this->action=action;
 }

 ImageButton::ImageButton (SDL_Surface * screen, Widget* father,
                           const string filename, const ushort action):
                           Widget(screen, father)
 {
   widget = LoadImage(filename, 1);
   if (widget == NULL)
   {
     GUIError error(string("Not found ") + filename);
     throw(error);
   }
   this->action=action;
 }

 void ImageButton::draw()
 {
   SDL_Rect r = {get_wx(), get_wy(), w, h};

   // First save background .. for erase
   SDL_BlitSurface(screen, &r, old_screen, NULL);
   // Then flip button
   SDL_BlitSurface(widget, NULL, screen, &r);
   SDL_Flip(screen);
 }

 void ImageButton::mousemotion(const uint mousex, const uint mousey)
 {
   if (inside(mousex, mousey))
   {
     if (visible == false)
     {
       draw();
       visible = true;
     }
   }
   else if (visible == true)
   {
     erase();
     visible = false;
   }
 }

 ushort ImageButton::mousebuttondown(const uint mousex, const uint mousey)
 {
         return(action);
 }

 //
 // TextButton
 //

 TextButton::TextButton(const uint x, const uint y,
                        const uint w, const uint h,
                        SDL_Surface * screen, Widget* father,
                        const string txt, const ushort action):
                        Widget(x, y, w, h, screen, father)
 {
   font = TTF_OpenFont(ADD_FONTS_PATH ("Vera.ttf"), 14);
   //If there was an error in loading the font 
   if (font == NULL)
   {
     GUIError error(string ("Not found Vera.ttf"));
     throw (error);
   }
   this->txt = txt;
   textColor = black_color; textColorMotion = black_color;
   bgColor = gainsboro_color; bgColorMotion = white_color;
   this->action=action;
   Focus globfocus;
 }

 TextButton::TextButton(const uint w, const uint h,
                        SDL_Surface * screen, Widget* father,
                        const string txt, const ushort action):
                        Widget(w, h, screen, father)
 {
   font = TTF_OpenFont(ADD_FONTS_PATH ("Vera.ttf"), 14);
   //If there was an error in loading the font 
   if (font == NULL)
   {
     GUIError error(string ("Not found Vera.ttf"));
     throw (error);
   }
   this->txt = txt;
   textColor = black_color; textColorMotion = black_color;
   bgColor = gainsboro_color; bgColorMotion = white_color;
   this->action=action;
   Focus globfocus;
 }

 TextButton::TextButton(SDL_Surface * screen, Widget* father,
                        const string txt, const ushort action):
                        Widget(screen, father)
 {
   font = TTF_OpenFont(ADD_FONTS_PATH ("Vera.ttf"), 14);
   //If there was an error in loading the font 
   if (font == NULL)
   {
     GUIError error(string ("Not found Vera.ttf"));
     throw (error);
   }
   this->txt = txt;
   textColor = black_color; textColorMotion = black_color;
   bgColor = gainsboro_color; bgColorMotion = white_color;
   this->action=action;
   Focus globfocus;
 }

 TextButton::~TextButton()
 {
   TTF_CloseFont(font);
 }

 void TextButton::drawmotion(SDL_Color textColor, SDL_Color bgColor)
 {
   SDL_Rect r = {get_wx(), get_wy(), w, h};	//relative to window
   SDL_Rect txtr = {0, 0, w, h};	//relative to button
   // draw bg
   SDL_BlitSurface(old_screen, NULL, widget, NULL);
   // box
   boxRGBA(widget, 0, 0, w, h, bgColor.r, bgColor.g, bgColor.b, BUTTON_ALPHA);
   rectangleRGBA(widget, 0, 0, w - 1, h - 1, 0, 0, 0, BUTTON_ALPHA);

   //draw txt
   PrintStrings(widget, font, txt, txtr, textColor);
   //then flip
   SDL_BlitSurface(widget, NULL, screen, &r);
   SDL_Flip(screen);
 }

 void TextButton::draw()
 {
   SDL_Rect r = {get_wx(), get_wy(), w, h};	//relative to window
   SDL_Rect txtr = {0, 0, w, h};	//relative to button
   // First save background
   SDL_BlitSurface(screen, &r, old_screen, NULL);
   // draw bg
   SDL_BlitSurface(screen, &r, widget, NULL);
   // box
   boxRGBA(widget, 0, 0, w, h, bgColor.r, bgColor.g, bgColor.b, BUTTON_ALPHA);
   rectangleRGBA(widget, 0, 0, w - 1, h - 1, 0, 0, 0, BUTTON_ALPHA);
   //draw txt
   PrintStrings(widget, font, txt, txtr, textColor);
   //then flip
   SDL_BlitSurface(widget, NULL, screen, &r);
   SDL_Flip(screen);
 }

 void TextButton::mousemotion(const uint mousex, const uint mousey)
 {
   if (inside(mousex, mousey))
   {
     if (visible == false)
     {
       drawmotion(textColorMotion, bgColorMotion);
       visible = true;
     }
   }
   else if (visible == true)
   {
     drawmotion(textColor, bgColor);
     visible = false;
   }
 }

 ushort TextButton::mousebuttondown(const uint mousex, const uint mousey)
 {
     if(inside(mousex, mousey))
     {
         globfocus.set_focus(this);
         return(action);
     }
     return 0;

 }
