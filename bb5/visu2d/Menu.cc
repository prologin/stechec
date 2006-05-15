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

#include "Menu.hh"
#include "GuiError.hh"

using namespace std;

MenuButton::MenuButton(const uint x, const uint y, const uint w, const uint h,
                       SDL_Surface* screen, const string txt, ushort action)
{
  button = new TextButton(x, y, w, h, screen, txt);
  this->action = action;
}

MenuButton::~MenuButton()
{
  delete(button);
}

TextButton* MenuButton::getButton()
{
  return button;
}

ushort MenuButton::getAction()
{
  return action;
}

Menu::Menu(const uint x, const uint y, const ushort nbbutton,
           SDL_Surface* screen, const ushort width, const string title)
{
  this->screen = screen;
  this->x = x;
  this->y = y;
  this->nbbutton = nbbutton;
  this->fgColor = black_color;
  this->bgColor = darkgray_color;
  this->title = title;
  this->w = width;
  this->h = (MENU_BUTTON_HEIGHT * nbbutton) + (nbbutton + 1) * 5;
  this->font = NULL;
      // the height depends if there is a title
  if(title != "")
  {
    this->h += MENU_BUTTON_HEIGHT;
    this->font =  TTF_OpenFont(ADD_FONTS_PATH("Vera.ttf"), 18);
        //If there was an error in loading the font 
    if (font == NULL)
    {
      GUIError error(string("Not found Vera.ttf"));
      throw(error);
    }
  }

  // Save all the old_screen .. to erase
  old_screen = SDL_CreateRGBSurface(
    SDL_HWSURFACE, 
    this->w, this->h,
    screen->format->BitsPerPixel,
    screen->format->Rmask,
    screen->format->Gmask,
    screen->format->Bmask,
    screen->format->Amask);
  SDL_BlitSurface(screen,NULL,old_screen,NULL);
  
      // Create the menu
  menu = SDL_CreateRGBSurface(
    SDL_HWSURFACE,
    this->w, this->h,
    screen->format->BitsPerPixel,
    screen->format->Rmask,
    screen->format->Gmask,
    screen->format->Bmask,
    screen->format->Amask);
}

Menu::~Menu()
{
  SDL_FreeSurface(menu);
  SDL_FreeSurface(old_screen);
  list_button.clear();
  
}

void Menu::addButton(const string txt, ushort action)
{
  list_button.push_back(new MenuButton((w - MENU_BUTTON_WIDTH) / 2 , (list_button.size() * (MENU_BUTTON_HEIGHT +  5)) + 5 + ((title == "") ? 0 : MENU_BUTTON_HEIGHT), 
                                       MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, screen, txt, action));
}

void Menu::addButton(const string txt, ushort action, const ushort x, const ushort y)
{
  list_button.push_back(new MenuButton(x, y, 
                                       MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, screen, txt, action));
}

void Menu::deleteButton(const ushort indice)
{
  if(list_button.at(indice))
  {
    list_button.erase(list_button.begin()+indice);
  }
}

void Menu::draw()
{
  SDL_Rect r = {x, y, w, h}; //relative to window
      //sav background
  SDL_BlitSurface(screen, &r, old_screen, NULL);
      //draw background
  SDL_BlitSurface(screen, &r, menu, NULL);
      // box
  boxRGBA(menu, 0, 0, w, h,  bgColor.r, bgColor.g, bgColor.b, MENU_ALPHA);
  rectangleRGBA(menu, 0, 0, w - 1, h - 1, 0, 0, 0, MENU_ALPHA); // w-1 and h-1 because start x,y in 0
      
      // add title
  if(title != "")
  {
    SDL_Rect txtr = {0, 0, w, MENU_BUTTON_HEIGHT}; //relative to menu
    PrintStrings(menu, font, title, txtr, fgColor);
  }

      // add box
  SDL_BlitSurface(menu, NULL, screen, &r);
      // flip
  SDL_Flip(screen);
  
      //and add buttons .. warning not to draw button with index > nbbutton
  ushort indice=0;
  for(vector<MenuButton*>::iterator b = list_button.begin();
      b != list_button.end() and (indice < nbbutton); ++b)
  {
    indice++;
    ((*b)->getButton())->draw(x + ((*b)->getButton())->getx(), y + ((*b)->getButton())->gety());
  }
}

void Menu::refresh()
{
  SDL_Rect r = {x, y, w, h}; //relative to window
      // replace background screen by save one
  SDL_BlitSurface(old_screen, NULL, menu, NULL);
      // box
  boxRGBA(menu, 0, 0, w - 1, h - 1,  bgColor.r, bgColor.g, bgColor.b, MENU_ALPHA);
  rectangleRGBA(menu, 0, 0, w - 1, h - 1, 0, 0, 0, MENU_ALPHA); // w-1 and h-1 because start x,y in 0        
      // add title
  if(title != "")
  {
    SDL_Rect txtr = {0, 0, w, MENU_BUTTON_HEIGHT}; //relative to menu
    PrintStrings(menu, font, title, txtr, fgColor);
  }
  // add box
   SDL_BlitSurface(menu, NULL, screen, &r);

      // flip
  SDL_Flip(screen);
  
      //and add buttons .. warning not to draw button with index > nbbutton
  ushort indice=0;
  for(vector<MenuButton*>::iterator b = list_button.begin();
      b != list_button.end() and (indice < nbbutton); ++b)
  {
    indice++;
    ((*b)->getButton())->draw(x + ((*b)->getButton())->getx(), y + ((*b)->getButton())->gety());
  }
}

void Menu::erase()
{
  SDL_Rect r = {x, y, w, h}; //relative to window
      // replace background screen by save one
  SDL_BlitSurface(old_screen, NULL, screen, &r);
  SDL_Flip(screen);
}

void Menu::setbgColor(SDL_Color bgColor)
{
  this->bgColor=bgColor;
  refresh();
}

SDL_Color Menu::getbgColor()
{
  return bgColor;
}

void Menu::setfgColor(SDL_Color fgColor)
{
  this->fgColor=fgColor;
  refresh();
}

SDL_Color Menu::getfgColor()
{
  return(fgColor);
}

ushort Menu::insidemenu(const uint mousex, const uint mousey)
{
  return((mousex >= x) && (mousex <= (x + w)) &&
         (mousey >= y) && (mousey <= (y + h)));
}

ushort Menu::mousebuttondown(const uint mousex, const uint mousey)
{
  if(insidemenu(mousex, mousey))
  {
        // test if mouse pointer is inside a button
    for (vector<MenuButton*>::iterator b = list_button.begin(); 
         b != list_button.end(); ++b)
    {
      if(((*b)->getButton())->mousebuttondown(mousex, mousey, x, y))
      {
        return((*b)->getAction());
      }
    }
  }
  return 0;
}

void Menu::mousemotion(const uint mousex,const uint mousey)
{
  for (vector<MenuButton*>::iterator b = list_button.begin(); 
       b != list_button.end(); ++b)
  {
    ((*b)->getButton())->mousemotion(mousex, mousey, x, y);
  }
}

