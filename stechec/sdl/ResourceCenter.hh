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
** @file sdl/ResourceCenter.hh
** @ingroup sdl_base
*/

/*!
** @brief Exception thrown by SDL functions.
** @ingroup sdl_base
*/
class SDLError : public Exception
{
public:
  SDLError(const std::string& msg);
};

/*!
** @brief Exception thrown by TTF functions.
** @ingroup sdl_base
*/
class TTFError : public Exception
{
public:
  TTFError(const std::string& msg);
};

// Forward decl for TTF_Font.
struct _TTF_Font;
typedef struct _TTF_Font TTF_Font;

/*!
** @ingroup sdl_base
** @brief Provide loader (with cache) for SDL resources.
**
** This class is a singleton. You can access it from anywhere. It
** provides image and font loader, with a transparent caching
** system. It is usually loaded within the SDLWindow class.
**
*/
class ResourceCenter
{
public:
  ResourceCenter();
  ~ResourceCenter();

  //! @brief Get the instance of this class.
  static ResourceCenter* getInst();

  //! @brief Get the ressource prefix path. By default, it points to the
  //!  installed shared directory (@c pkgdatadir) of the program.
  //! @return Prefix path.
  const std::string& getResourcePrefix() const;
  
  //! @brief Change the ressource prefix path.
  //! @param prefix Path to set. If it is an empty string, the
  //!  default location is set.
  void setResourcePrefix(const std::string& prefix = "");
  
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
  Surface getImage(const std::string& filename, double zoom = 1., double angle = 0.);

  //! @brief Load a font file.
  //! @param font_name Font filename to load, relative to current prefix +
  //!   "font/" directory.
  //! @param font_size The size of the font (typically between 10-20).
  //! @return A valid font. Can't be NULL.
  //! @throw TTFError Thrown if font couldn't be opened.
  //! @note Caller is reponsible of calling releaseFont when it has finished.
  TTF_Font* getFont(const std::string font_name, int font_size);

  //! @brief Release a font.
  //! @param font The font to release.
  void releaseFont(TTF_Font* font);

  //! @brief Print cache info.
  void printStatistics();

private:
  //! @brief Load an SDL_surface from a picture (png) file.
  //! @note No caching is done, this is getImage job.
  //! @param filename Image file to load.
  //! @throw SDLError Thrown if the file could not be opened.
  SDL_Surface* loadImage(const std::string& filename);

  //! @brief Resize/Rotate an image.
  //! @return A copy of @c from, with modification.
  SDL_Surface* transformImage(SDL_Surface* from, double zoom, double angle);

  //! @brief Yes, ResourceCenter is a singleton.
  static ResourceCenter* inst_;

  std::string prefix_;
  
  typedef std::set<Surface, Surface::ImgSort> ImageList;
  ImageList             image_list_;

  /*!
  ** @brief Storage for already loaded fonts, with their carateristics.
  */
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
