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

#ifndef TEXTSURFACE_HH_
# define TEXTSURFACE_HH_

# include "ResourceCenter.hh"
# include "Surface.hh"

class TextSurface : public Surface
{
public:
  TextSurface(const std::string& font_name, int surf_width, int surf_height);
  TextSurface(const TextSurface& ts);
  TextSurface& operator= (const TextSurface& rhs);
  virtual ~TextSurface();

  void setText(const std::string& text);
  const std::string& getText() const;
  
  virtual void update();

private:
  TTF_Font*     font_;
  int           line_skip_;

  std::string   previous_text_;
  int           ref_count_;

protected:
  std::string   text_;
};

#endif /* !TEXTSURFACE_HH_ */
