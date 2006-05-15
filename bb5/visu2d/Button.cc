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

Button::Button(const uint x, const uint y, const uint w, const uint h,
               SDL_Surface* screen)
{
  this->screen = screen;
  this->x = x; this->y = y;
  this->w = w; this->h = h;
  visible = false;
  
  old_surface = SDL_CreateRGBSurface(
    SDL_HWSURFACE, 
    this->w, this->h,
    screen->format->BitsPerPixel,
    screen->format->Rmask,
    screen->format->Gmask,
    screen->format->Bmask,
    screen->format->Amask);
  button = SDL_CreateRGBSurface(
    SDL_HWSURFACE, 
    this->w, this->h,
    screen->format->BitsPerPixel,
    screen->format->Rmask,
    screen->format->Gmask,
    screen->format->Bmask,
    screen->format->Amask);
}

Button::~Button()
{
  SDL_FreeSurface(button);
  SDL_FreeSurface(old_surface);
}

void Button::erase(const uint posx, const uint posy)
{
  SDL_Rect r = {posx, posy, w, h};
  SDL_BlitSurface(old_surface, NULL, screen, &r);
  SDL_Flip(screen);
}

ushort Button::mousebuttondown(const uint mousex,const uint mousey,
                               const uint dialogx,const uint dialogy)
{
  return(inside(mousex, mousey, dialogx, dialogy));
}

ushort Button::inside(const uint mousex, const uint mousey,
                      const uint dialogx, const uint dialogy)
{
  return((mousex >= (dialogx + x)) && (mousex <= (dialogx + x + w)) &&
         (mousey >= (dialogy + y)) && (mousey <= (dialogy + y + h)));
}


uint Button::getx()
{
  return x;
}

uint Button::gety()
{
  return y;
}

//
// ImageButton
//

ImageButton::ImageButton(uint x, uint y, uint w, uint h,
                         SDL_Surface* screen, const std::string filename):
Button(x,y,w,h,screen)
{
  image = LoadImage(filename,1);
  if (image == NULL) 
  {
    GUIError error(string("Not found ")+filename);
    throw(error);
  }
}

ImageButton::~ImageButton()
{
  SDL_FreeSurface(image);
}

void ImageButton::draw(const uint posx, const uint posy)
{
  SDL_Rect r = {posx, posy, w, h};
  
      // First save background
  SDL_BlitSurface(screen, &r, old_surface, NULL);
      // Then flip button
  SDL_BlitSurface(image, NULL, button, NULL);
  SDL_BlitSurface(button, NULL, screen, &r);
  SDL_Flip(screen);
}

void ImageButton::mousemotion(const uint mousex, const uint mousey,
                              const uint dialogx, const uint dialogy)
{
  if (inside(mousex, mousey, dialogx, dialogy))
  {
    if (visible == false)
    {
      draw(dialogx + x, dialogy + y);
      visible = true;
    }
  }
  else if(visible == true)
  {
    erase(dialogx + x, dialogy + y);
    visible = false;
  }
}

//
// TextButton
//

TextButton::TextButton(const uint x, const uint y, const uint w, const uint h,
                       SDL_Surface* screen, const std::string txt):
Button(x, y, w, h, screen)
{
  font = TTF_OpenFont(ADD_FONTS_PATH("Vera.ttf"), 14);
      //If there was an error in loading the font 
  if (font == NULL)
  {
    GUIError error(string("Not found Vera.ttf"));
    throw(error);
  }
  this->txt = txt;
  textColor = black_color;
  textColorMotion = black_color;
  bgColor = gainsboro_color;
  bgColorMotion = white_color;
}

TextButton::~TextButton()
{
  TTF_CloseFont(font);
}

void TextButton::drawmotion(const uint posx, const uint posy, SDL_Color textColor, SDL_Color bgColor)
{
  SDL_Rect r = {posx, posy, w, h}; //relative to window
  SDL_Rect txtr = {0, 0, w, h}; //relative to button
      // draw bg
  SDL_BlitSurface(old_surface, NULL, button, NULL);
      // box
  boxRGBA(button, 0, 0, w, h,  bgColor.r, bgColor.g, bgColor.b, BUTTON_ALPHA);
  rectangleRGBA(button, 0, 0, w - 1, h - 1, 0, 0, 0, BUTTON_ALPHA);

      //draw txt
  PrintStrings(button, font, txt, txtr, textColor);
      //then flip
  SDL_BlitSurface(button, NULL, screen, &r);
  SDL_Flip(screen);
}

void TextButton::draw(const uint posx, const uint posy)
{
  SDL_Rect r = {posx, posy, w, h}; //relative to window
  SDL_Rect txtr = {0, 0, w, h}; //relative to button
      // First save background
  SDL_BlitSurface(screen, &r, old_surface, NULL);
      // draw bg
  SDL_BlitSurface(screen, &r, button, NULL);
      // box
  boxRGBA(button, 0, 0, w, h,  bgColor.r, bgColor.g, bgColor.b, BUTTON_ALPHA);
  rectangleRGBA(button, 0, 0, w - 1, h - 1, 0, 0, 0, BUTTON_ALPHA);
      //draw txt
  PrintStrings(button, font, txt, txtr, textColor);
      //then flip
  SDL_BlitSurface(button, NULL, screen, &r);
  SDL_Flip(screen);
}

void TextButton::mousemotion(const uint mousex, const uint mousey,
                             const uint dialogx, const uint dialogy)
{
  if (inside(mousex, mousey, dialogx, dialogy))
  {
    if (visible == false)
    {
      drawmotion(x + dialogx, y + dialogy, textColorMotion, bgColorMotion);
      visible = true;
    }
  }
  else if(visible == true)
  {
    drawmotion(x + dialogx, y + dialogy, textColor, bgColor);
    visible = false;
  }
}
