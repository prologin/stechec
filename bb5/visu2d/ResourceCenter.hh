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

#ifndef RESOURCECENTER_HH_
# define RESOURCECENTER_HH_

# include <SDL.h>
# include "tools.hh"
# include "Surface.hh"

/*!
** @brief Exception thrown by SDL functions.
**
*/
class SDLError : public Exception
{
public:
  SDLError(const std::string& msg);
};

/*!
** @brief Exception thrown by TTF functions.
**
*/
class TTFError : public Exception
{
public:
  TTFError(const std::string& msg);
};

struct _TTF_Font;
typedef struct _TTF_Font TTF_Font;

/*!
** @brief Provide cache for SDL resources.
**
** This class is a singleton. You can access it from anywhere.
*/
class ResourceCenter
{
public:
  ResourceCenter();
  ~ResourceCenter();

  //! @brief Get the instance of this class.
  static ResourceCenter* getInst();

  //! @brief Load an image, with cache enabled.
  //!
  //! If the image, with specified angle and zoom level, can be found
  //! in the cache, it is not reloaded. Otherwise, a copy will be kept
  //! for further call.
  //!
  //! @param filename Image file to load.
  //! @param zoom Zoom level to apply, wrt the original image.
  //! @param angle Rotate image, wrt the original image (0-360 degre).
  //! @throw SDLError Thrown if the file could not be opened.
  Surface getImage(const std::string& filename, double zoom, double angle);

  //! @brief Load a font file.
  //! @note Caller is reponsible of calling releaseFont when it has finished.
  TTF_Font* getFont(const std::string font_name, int font_size);

  //! @brief Release a font.
  void releaseFont(TTF_Font* font);
  
  //! @brief Print cache info.
  void printStatistics();
  
private:
  //! @brief Load an SDL_surface from a picture (png) file.
  //! @note No caching is done.
  //! @param filename Image file to load.
  //! @throw SDLError Thrown if the file could not be opened.
  SDL_Surface* loadImage(const std::string& filename);

  //! @brief Resize/Rotate an image.
  //! @return A copy of @c from, with modification.
  SDL_Surface* transformImage(SDL_Surface* from, double zoom, double angle);

  static ResourceCenter* inst_;

  typedef std::set<Surface, Surface::ImgSort> ImageList;
  ImageList             image_list_;

  struct LoadedFont {
    TTF_Font* font_;    ///< TTF_Font ptr.
    std::string name_;  ///< Font filename.
    int size_;          ///< Font size.
    int ref_count_;     ////< Number of instance currently using this font.
    friend bool operator< (const LoadedFont& lhs, const LoadedFont& rhs)
    {
      if (lhs.name_ < rhs.name_)
        return true;
      return lhs.size_ < rhs.size_;
    }
  };
  typedef std::set<LoadedFont> FontList;
  FontList font_list_;
};

#endif /* !RESOURCECENTER_HH_ */
