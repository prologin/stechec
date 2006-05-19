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

#include <SDL_TTF.h>
#include <SDL_gfxPrimitives.h>

#include "TextSurface.hh"

TextSurface::TextSurface(const std::string& font_name, int surf_width, int surf_height)
  : Surface(surf_width, surf_height)
{
  font_ = ResourceCenter::getInst()->getFont(font_name, 12);
  line_skip_ = TTF_FontLineSkip(font_);
  ref_count_ = 1;
}

TextSurface::~TextSurface()
{
  if (--ref_count_ == 0)
    ResourceCenter::getInst()->releaseFont(font_);
}

TextSurface::TextSurface(const TextSurface& ts)
{
  font_ = ts.font_;
  ref_count_ = ts.ref_count_ + 1;
}

TextSurface& TextSurface::operator= (const TextSurface& rhs)
{
  font_ = rhs.font_;
  ref_count_ = rhs.ref_count_ + 1;  
}

void TextSurface::setText(const std::string& text)
{
  text_ = text;
}

const std::string& TextSurface::getText() const
{
  return text_;
}
  
void TextSurface::update()
{
  const SDL_Color darkmagenta_color = { 139, 0, 139, SDL_ALPHA_OPAQUE };
  SDL_Surface *temp_surf = NULL;
  
  // Print the text, if changed.
  if (text_ != previous_text_)
    {
      boxRGBA(surf_, 0, 0, getSize().x, getSize().y, 127, 255, 212, 150);
      rectangleRGBA(surf_, 1, 1, getSize().x - 2 , getSize().y - 2, 127, 255, 212, 150);

      temp_surf = TTF_RenderText_Solid(font_, text_.c_str(), darkmagenta_color);
      if (temp_surf == NULL)
        PRINT_AND_THROW(TTFError, "RenderText: ");
      SDL_Rect dst = { 5, 5, 0, 0};
      SDL_BlitSurface(temp_surf, NULL, surf_, &dst);
      SDL_FreeSurface(temp_surf);
      previous_text_ = text_;
      redraw_all_ = true;
    }

  Surface::update();
}
