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

#include "DialogBox.hh"
#include "GuiError.hh"

    //
    // ATTENTION ICI : faire sauter toutes les valeurs en dur !!!!!!!!!!!!!!
    //


DialogBox::DialogBox(const uint x, const uint y, const uint w, const uint h,
                     SDL_Surface *screen, const string filename):
Widget(x, y, w, h, screen)
{
  image=LoadImage(filename,1);
  if (image == NULL) 
  {
    GUIError error(string("Not found ")+filename);
    throw(error);
  }
  move_dialog = false;

      // the dialog_box_sav .. for refresh
  dialog_box_surf_sav = SDL_CreateRGBSurface(
    SDL_HWSURFACE, 
    this->w - 30,this->h - 60,
    screen->format->BitsPerPixel,
    screen->format->Rmask,
    screen->format->Gmask,
    screen->format->Bmask,
    screen->format->Amask);
}

DialogBox::~DialogBox()
{
  SDL_FreeSurface(dialog_box_surf_sav);
  SDL_FreeSurface(image);
  for (vector<ImageButton*>::iterator b = list_button.begin();
       b != list_button.end(); ++b)
  {
    delete(*b);
  }
  list_button.clear();
}

void DialogBox::refresh()
{
  static int old_x = x;
  static int old_y = y;
  SDL_Rect oldr = {old_x, old_y, w, h};

      // replace background with old background
  SDL_BlitSurface(old_screen, NULL, screen, &oldr);

  SDL_Rect newr = {x, y, w, h};
      // sav bg in old_screen first
  SDL_BlitSurface(screen, &newr, old_screen, NULL);
      // draw bg ..
  SDL_BlitSurface(screen, &newr, widget, NULL);
  SDL_BlitSurface(image, NULL, widget, NULL);
      // add sav
  SDL_Rect rsav = {20, 30, w-30, h-60};
  SDL_BlitSurface(dialog_box_surf_sav, NULL, widget, &rsav);
      // .. then flip
  SDL_SetAlpha(widget, SDL_SRCALPHA,(move_dialog) ? (DIALOG_ALPHA / 2) : 255);
  SDL_BlitSurface(widget, NULL, screen, &newr);
  SDL_Flip(screen);

  old_x = x; old_y = y;
}

void DialogBox::erase()
{
  SDL_Rect r = {x, y, w, h};
  SDL_BlitSurface(old_screen, NULL, screen, &r);
  SDL_Flip(screen);
}

ushort DialogBox::insidetitle(const uint mousex, const uint mousey)
{
  return((mousex >= x) && (mousex < (x + w)) && (mousey >= y) && (mousey < (y + 25)));
}

ushort DialogBox::mousebuttondown(const uint mousex, const uint mousey)
{
  if (insidetitle(mousex,mousey))
  {
    move_dialog = true;
    refresh();
    return 1;
  }
  else
  {
    for (vector<ImageButton*>::iterator b = list_button.begin();
         b != list_button.end(); ++b)
    {
      if((*b)->inside(mousex, mousey))
        return 1;
    }
  }
  return 0;
}

ushort DialogBox::mousebuttonup()
{
  if (move_dialog)
  {
    move_dialog = false;
    refresh();
    return 1;
  }
  return 0;
}

ushort DialogBox::mousemotion(const uint mousex, const uint mousey, const uint xrel, const uint yrel)
{
  if(move_dialog)
  {
    x += xrel;
    y += yrel;
        // Test the limits of the window
    if (x > (uint)screen->w) x = 0;
    else if ((x + w) > (uint)screen->w)
      x = screen->w - w;
    if (y > (uint)screen->h) y = 0;
    else if ((y + h) > (uint)screen->h)
      y = screen->h - h;
        // and redraw the dialogbox
    refresh();
    return 1;
  }
  else
  {
    for (vector<ImageButton*>::iterator b = list_button.begin(); 
         b != list_button.end(); ++b)
    {
      (*b)->mousemotion(mousex, mousey);
    }
  }
  return 0;
}

void DialogBox::addButton(ImageButton* new_button)
{
  list_button.push_back(new_button);
}

ushort DialogBox::getnbButton()
{
  return((unsigned short)list_button.size());
}

DInfo::DInfo(SDL_Surface* screen, const string message):
DialogBox(((screen->w - 240) / 2), ((screen->h - 170) / 2), 240, 170, screen, ADD_IMG_PATH("dialog/dialog_info.png"))
{
  this->message=message;
  font = TTF_OpenFont(ADD_FONTS_PATH("Vera.ttf"), 14);
      //If there was an error in loading the font 
  if (font == NULL)
  {
    GUIError error(string("Not found Vera.ttf"));
    throw(error);
  }
      // init icone
  icone = LoadImage(ADD_IMG_PATH("dialog/icons.jpg"),1);
  if (icone == NULL)
  {
    GUIError error(string("Not found ") + ADD_IMG_PATH("dialog/icons.jpg"));
    throw(error);
  }

  try
  {
    BClose* close = new BClose(screen, this);
    addButton(close);
  }
  catch(GUIError& error)
  {
    TTF_CloseFont(font);
    throw(error);
  }
}

DInfo::~DInfo()
{
  TTF_CloseFont(font);
}

void DInfo::draw()
{
  SDL_Rect r = {x, y, w, h};
      // Sav bg screen
  SDL_BlitSurface(screen, &r, old_screen, NULL);
      // draw bg ..
  SDL_BlitSurface(screen, &r, widget, NULL);
  SDL_BlitSurface(image, NULL, widget, NULL);
      //draw ico
      //Icone INFO is the 6 on the list
  SDL_Rect srico = {6*60,0,60,60};
  SDL_Rect drico = {25,34,60,60};
  SDL_BlitSurface(icone, &srico, widget, &drico);

      //draw txt
  SDL_Rect rt = {90,30,140,100};
  SDL_Color textColor = black_color;
  PrintStrings(widget, font, message, rt, textColor);
      // .. then flip
  SDL_BlitSurface(widget, NULL, screen, &r);
  SDL_Flip(screen);
      // sav in dialog_box_surf_sav .. for refresh
      // The sav part is not all the dialogbox but only the text and the icon part
  SDL_Rect rsav = {x+20,y+30,w-30,h-60};
  SDL_BlitSurface(screen, &rsav, dialog_box_surf_sav, NULL);
}

ushort DInfo::mousebuttondown(const uint mousex, const uint mousey)
{
  if (insidetitle(mousex, mousey))
  {
        // move it
    move_dialog = true;
    refresh();
    return 1;
  }
  else
  {
        // test if mouse pointer is inside the button
    for (vector<ImageButton*>::iterator b = list_button.begin(); 
         b != list_button.end(); ++b)
    {
      if((*b)->inside(mousex, mousey))
      {
        erase();
        return((*b)->mousebuttondown());
      }
    }
  }
  return 0;
}
