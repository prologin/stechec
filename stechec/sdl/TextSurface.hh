/*
** TowBowlTactics, an adaptation of the tabletop game Blood Bowl.
** 
** Copyright (C) 2006, 2007 The TBT Team.
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

/*!
** @file sdl/TextSurface.hh
** @ingroup sdl_base
*/

# include "ResourceCenter.hh"
# include "Surface.hh"

/*!
** @brief Text render method.
*/
enum eTextRenderMethod {
  eTextSolid,   ///< Normal, not Anti-Alisased.
  eTextShaded,  ///< AA, with a defined background (don't keep transparency).
  eTextBlended, ///< AA, keep transparency. Slower.
};

/*!
** @ingroup sdl_base
** @brief Class that can draw text, similiar to a TextLabel.
**
** Support multi-line and autowrap.
*/
class TextSurface : public Surface
{
public:
  TextSurface();
  TextSurface(const std::string& font_name, int font_size, int surf_width, int surf_height);
  TextSurface(const TextSurface& ts);
  TextSurface& operator= (const TextSurface& rhs);
  virtual ~TextSurface();

  //! @brief Erase all lines.
  void clearText();
  
  //! @brief Set text to be printed, clearing previous content.
  //! @param text Text to set.
  void setText(const std::string& text);

  //! @brief Add text to this object, on new line. If the object is too little
  //!  to show all text, the oldest added lines are removed.
  //! @param text Text to add.
  void addText(const std::string& text);

  //! @brief Return the text (all lines) currently displayed.
  //! @return Displayed text.
  std::string getText() const;

  //! @brief Auto-wrap text, add new lines when text go beyond surface width.
  //! @param enable Enable or disable it (default: enabled).
  void setAutoWrap(bool enabled);

  //! @brief Get Auto-wrapping current setting.
  //! @return Auto-wrap current setting.
  bool getAutoWrap() const;

  //! @brief Set text color.
  //! @param fg Color to set (default: black).
  void setTextColor(const SDL_Color& fg);

  //! @brief Set baground color, only for method eTextShaded.
  //! @param bg Color to set (default: white).
  void setBgColor(const SDL_Color& bg);

  //! @brief Set rendering method.
  //! @param m Render method to use.
  void setRenderMethod(enum eTextRenderMethod m);
  
  virtual void update();

protected:

  //! @brief Called when a new text surface is generated.
  virtual void customTextRender(SDL_Surface* surf, int line);
  
  TTF_Font*     font_;

private:
  Surface       surf_font_ref_;
  SDL_Color	fg_;
  SDL_Color	bg_;

  std::string   font_name_;
  int           font_size_;
  int           line_skip_;
  enum eTextRenderMethod method_;

  bool          auto_wrap_;

protected:
  bool          content_changed_;
  typedef std::deque<std::string> LineList;
  LineList      lines_;
  std::string   text_;
};

#endif /* !TEXTSURFACE_HH_ */
