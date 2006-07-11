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

/*
** @brief Class that can draw text, similiar to a TextLabel.
**
** Support multi-line and autowrap.
*/
class TextSurface : public Surface
{
public:
  TextSurface();
  TextSurface(const std::string& font_name, int surf_width, int surf_height);
  TextSurface(const TextSurface& ts);
  TextSurface& operator= (const TextSurface& rhs);
  virtual ~TextSurface();

  //! @brief Erase all lines.
  void clearText();
  
  //! @brief Set text to be printed, clearing previous content.
  void setText(const std::string& text);

  //! @brief Add text to this object, on new line.
  void addText(const std::string& text);

  //! @brief Return the text currently hold.
  std::string getText() const;

  void setAutoWrap(bool enabled);
  bool getAutoWrap() const;
  
  virtual void update();

private:
  TTF_Font*     font_;
  std::string   font_name_;
  int           font_size_;
  int           line_skip_;

  bool          auto_wrap_;
  bool          content_changed_;

protected:
  typedef std::deque<std::string> LineList;
  LineList      lines_;
  std::string   text_;
};

#endif /* !TEXTSURFACE_HH_ */
