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

#include <SDL_ttf.h>
#include <SDL_gfxPrimitives.h>

#include "TextSurface.hh"

// FIXME: use Colors.hh
static SDL_Color black_color = { 0, 0, 0, SDL_ALPHA_OPAQUE };
static SDL_Color white_color = { 255, 255, 255, SDL_ALPHA_OPAQUE };

TextSurface::TextSurface()
  : font_(NULL),
    surf_font_ref_(NULL),
    fg_(black_color),
    bg_(white_color),
    font_size_(-1),
    line_skip_(-1),
    method_(eTextBlended),
    auto_wrap_(true),
    content_changed_(true)
{
}

TextSurface::TextSurface(const std::string& font_name, int font_size,
			 int surf_width, int surf_height)
  : fg_(black_color),
    bg_(white_color),
    font_name_(font_name),
    font_size_(font_size),
    method_(eTextBlended),
    auto_wrap_(true),
    content_changed_(true)
{
  font_ = ResourceCenter::getInst()->getFont(font_name, font_size);
  line_skip_ = TTF_FontLineSkip(font_);
  setSize(Point(surf_width, surf_height));

  // Create a ref font surface, to create this Surface with the same attributes.
  SDL_Color white_color = { 255, 255, 255, SDL_ALPHA_TRANSPARENT };
  SDL_Surface* temp_surf = TTF_RenderText_Blended(font_, "a", white_color);
  surf_font_ref_ = Surface(temp_surf);
  create(surf_width, surf_height, surf_font_ref_.getSDLSurface());
  SDL_FreeSurface(temp_surf);
}

TextSurface::~TextSurface()
{
  ResourceCenter::getInst()->releaseFont(font_);
}

TextSurface::TextSurface(const TextSurface& ts)
  : Surface(ts)
{
  font_name_ = ts.font_name_;
  font_size_ = ts.font_size_;
  font_ = ResourceCenter::getInst()->getFont(font_name_, font_size_);
  assert(font_ == ts.font_);
  surf_font_ref_ = ts.surf_font_ref_;
  fg_ = ts.fg_;
  bg_ = ts.bg_;
  line_skip_ = ts.line_skip_;
  method_ = ts.method_;
  auto_wrap_ = ts.auto_wrap_;
  content_changed_ = !ts.lines_.empty();
  lines_ = ts.lines_;
  text_ = ts.text_;
}

TextSurface& TextSurface::operator= (const TextSurface& rhs)
{
  Surface::operator=(rhs);

  font_name_ = rhs.font_name_;
  font_size_ = rhs.font_size_;
  font_ = ResourceCenter::getInst()->getFont(font_name_, font_size_);
  assert(font_ == rhs.font_);
  surf_font_ref_ = rhs.surf_font_ref_;
  fg_ = rhs.fg_;
  bg_ = rhs.bg_;
  line_skip_ = rhs.line_skip_;
  method_ = rhs.method_;
  auto_wrap_ = rhs.auto_wrap_;
  content_changed_ = !rhs.lines_.empty();
  lines_ = rhs.lines_;
  text_ = rhs.text_;
  return *this;
}

void TextSurface::clearText()
{
  lines_.clear();
  text_ = "";
  content_changed_ = true;
}

void TextSurface::setText(const std::string& text)
{
  if (text_ == text)
    return;
  
  lines_.clear();
  text_ = "";
  addText(text);
}

void TextSurface::addText(const std::string& text_to_add)
{
  using namespace std;

  const char* search_string = auto_wrap_ ? " \n" : "\n";
  string text(text_to_add + "\n");
  string::size_type index = 0;
  string::size_type prev_index = 0;

  while ((index = text.find_first_of(search_string, index + 1)) != string::npos)
    {
      // Get string width.
      string chunck = text.substr(0, index);
      int w;
      if (TTF_SizeText(font_, chunck.c_str(), &w, NULL) != 0)
        PRINT_AND_THROW(TTFError, "TTF_SizeText");
          
      if (w > getRect().w - 8)
        {
          // Sanity check... if a word overrun the box, print it whatever.
          if (prev_index == 0)
            prev_index = index;
 
          // Out of box... Add text up to previous word, then add a newline.
          lines_.push_back(text.substr(0, prev_index));
          text = text.substr(prev_index + 1, 0xFFFF);
          index = 0;
        }
      else if (text[index] == '\n')
        {
          // Line break. Add a new line.
          lines_.push_back(text.substr(0, index));
          text = text.substr(index + 1, 0xFFFF);
          index = 0;
        }

      prev_index = index;
    }

  // Remove extra lines.
  const unsigned max_line = (getRect().h - 8) / line_skip_;
  if (lines_.size() > max_line)
    {
      LineList::iterator it = lines_.begin() + (lines_.size() - max_line);
      lines_.erase(lines_.begin(), it);
    }

  text_ = text_ + text_to_add;
  content_changed_ = true;
}

std::string TextSurface::getText() const
{
  return text_;
}

void TextSurface::setAutoWrap(bool enabled)
{
  auto_wrap_ = enabled;
}

bool TextSurface::getAutoWrap() const
{
  return auto_wrap_;
}

void TextSurface::setTextColor(const SDL_Color& fg)
{
  fg_ = fg;
}

void TextSurface::setBgColor(const SDL_Color& bg)
{
  bg_ = bg;
}

void TextSurface::setRenderMethod(enum eTextRenderMethod m)
{
  method_ = m;
}


void TextSurface::update()
{
  SDL_Surface *temp_surf = NULL;
  
  // Print the text, if changed.
  if (content_changed_)
    {
      SDL_FillRect(surf_, NULL, 0);

      // Print each line in our Surface.
      int index = 0;
      LineList::const_iterator it;
      for (it = lines_.begin(); it != lines_.end(); ++it)
        {
	  switch (method_)
	    {
	    case eTextSolid:
	      temp_surf = TTF_RenderText_Solid(font_, it->c_str(), fg_);
	      break;
	    case eTextShaded:
	      temp_surf = TTF_RenderText_Shaded(font_, it->c_str(), fg_, bg_);
	      break;
	    case eTextBlended:
	      temp_surf = TTF_RenderText_Blended(font_, it->c_str(), fg_);
	      break;
	    }
	  if (temp_surf == NULL)
	    {
	      // FIXME: why, oh WHY this $^*#$ system don't allow me to print spaces...
	      char hackme[256] = {0};
	      strncpy(hackme, it->c_str(), 255);
	      for (int i = 0; hackme[i] != 0; i++)
		if (hackme[i] == ' ')
		  hackme[i] = '_';
	      temp_surf = TTF_RenderText_Solid(font_, hackme, fg_);
	    }
          if (temp_surf == NULL)
	    PRINT_AND_THROW(TTFError, "RenderText '" << *it << "'");

	  Surface temp(temp_surf);
	  temp.setInheritAlpha(true);
	  temp.setPos(4, 4 + (index++ * line_skip_));
	  temp.blit(*this);

	  SDL_FreeSurface(temp_surf);
	  temp_surf = NULL;
	}

      redraw_all_ = true;
      content_changed_ = false;
    }

  Surface::update();
}
