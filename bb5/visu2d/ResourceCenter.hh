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
  typedef std::set<Surface> ImageList;
  ImageList             image_list_;
};

#endif /* !RESOURCECENTER_HH_ */
