/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2006 Prologin
*/

#ifndef INFOBOX_HH_
# define INFOBOX_HH_

# include <SDL_ttf.h>
# include <SDL_gfxPrimitives.h>

class InfoBox
{
public:
  InfoBox(Api* api)
    : screen_(SDL_GetVideoSurface()),
      api_(api),
      date_(-1),
      delorean_power_(-1)
  {
    for (int i = 0; i < MAX_TEAM; i++)
      score_[i] = -1;
    
    font_ = TTF_OpenFont((std::string(PKGDATADIR "font/") + "Vera.ttf").c_str(), 12);
    if (font_ == NULL)
      {
        ERR("Error loading the font: " << TTF_GetError());
        assert(false);
      }
    line_skip_ = TTF_FontLineSkip(font_);


    // Create the dialog box
    info_box_surf_ =
      SDL_CreateRGBSurface(SDL_HWSURFACE, 
                           160,
                           100,
                           screen_->format->BitsPerPixel,
                           screen_->format->Rmask,
                           screen_->format->Gmask,
                           screen_->format->Bmask,
                           screen_->format->Amask);
    assert(info_box_surf_ != NULL);
  }

  ~InfoBox()
  {
    TTF_CloseFont(font_);
    SDL_FreeSurface(info_box_surf_);
  }
  
  bool update(bool have_to_blit)
  {
    // Determine if we have to redraw something
    if (!have_to_blit)
      {
        if (date_ != api_->current_date()
            || delorean_power_ != api_->autonomie_delorean())
          have_to_blit = true;
        else
          for (int i = 0; i < api_->equipes(); i++)
            if (score_[i] != api_->score(i))
              have_to_blit = true;
      }

    if (!have_to_blit)
      return false;
    
    // Copy screen to info box.
    SDL_Rect src = { 600, 10, 160, 100 };
    SDL_BlitSurface(screen_, &src, info_box_surf_, NULL);

    // Apply transparency.
    boxRGBA(info_box_surf_, 0, 0, 160, 100, 127, 255, 212, 170);
    rectangleRGBA(info_box_surf_, 1, 1, 159, 99, 127, 255, 212, 170);

    // Print our strings.
    int line = 0;
    std::ostringstream os;

    date_ = api_->current_date();
    os << "Date: " << date_;
    printString(os.str(), line++);
    os.rdbuf()->str("");

    delorean_power_ = api_->autonomie_delorean();
    os << "Autonomie delorean: " << delorean_power_;
    printString(os.str(), line++);
    os.rdbuf()->str("");

    for (int i = 1; i <= api_->equipes(); i++)
      {
        score_[i - 1] = api_->score(i - 1);
        os << "Score team " << i << ": " << score_[i - 1];
        printString(os.str(), line++);
        os.rdbuf()->str("");
      }
    
    // Blit back.
    SDL_Rect dst = { 600, 10, 0, 0 };
    SDL_BlitSurface(info_box_surf_, NULL, screen_, &dst);
    return true;
  }

private:

  void printString(const std::string& str, int line_number = 0)
  {
    const SDL_Color darkmagenta_color = { 139, 0, 139, SDL_ALPHA_OPAQUE };
    SDL_Surface *temp = NULL;

    temp = TTF_RenderText_Solid(font_, str.c_str(), darkmagenta_color);
    SDL_Rect dst = { 5, line_skip_ * line_number, 0, 0};
    SDL_BlitSurface(temp, NULL, info_box_surf_, &dst);
    SDL_FreeSurface(temp);
  }
  
  TTF_Font* font_;
  SDL_Surface* screen_;
  SDL_Surface* info_box_surf_;
  int line_skip_;

  // Saved information from the API.
  // Used to know if we must redraw or not.
  Api* api_;
  int date_;
  int delorean_power_;
  int score_[MAX_TEAM];
};

#endif /* !INFOBOX_HH_ */
