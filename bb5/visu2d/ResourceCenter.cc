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

#include <SDL_rotozoom.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "ResourceCenter.hh"


SDLError::SDLError(const std::string& msg)
  : Exception(msg + ": " + SDL_GetError())
{
}

TTFError::TTFError(const std::string& msg)
  : Exception(msg + ": " + TTF_GetError())
{
}

ResourceCenter* ResourceCenter::inst_ = NULL;

ResourceCenter::ResourceCenter()
{
  // Only one instance is allowed.
  assert(inst_ == NULL);
  inst_ = this;
}

ResourceCenter::~ResourceCenter()
{
  inst_ = NULL;
}

ResourceCenter* ResourceCenter::getInst()
{
  assert(inst_ != NULL);
  return inst_;
}

Surface ResourceCenter::getImage(const std::string& filename, double zoom, double angle)
{
  ImageList::iterator it;
  SDL_Surface* orig_surf = NULL;

  // Search if exact image is already in the cache.
  const Surface ref_surf(NULL, zoom, angle, filename);
  it = image_list_.find(ref_surf);
  if (it != image_list_.end())
    return *it;

  // Search if reference image (without zoom and angle) was already loaded.
  // Don't reload it from file, but apply transformation.
  if (zoom != 1. && angle != 0.)
    {
      const Surface ref_surf(NULL, 1., 0., filename);
      it = image_list_.find(ref_surf);
      if (it != image_list_.end())
        orig_surf = const_cast<Surface&>(*it).getSDLSurface();
    }

  // If original image is not found, load it and insert it into the cache.
  if (orig_surf == NULL)
    {
      orig_surf = loadImage(filename);  
      image_list_.insert(Surface(orig_surf, 1., 0., filename));
    }

  SDL_Surface* wanted_surf = transformImage(orig_surf, zoom, angle);
  Surface res_surf(wanted_surf, zoom, angle, filename);
  image_list_.insert(res_surf);
  return res_surf;
}

SDL_Surface* ResourceCenter::loadImage(const std::string& filename)
{
  SDL_Surface* loaded_image = NULL;
  SDL_Surface* surf = NULL;
  std::string fn_base = std::string(PKGDATADIR) + "/" + filename;
  std::string fn_loaded;

  LOG3("Load `" << fn_base << ".png'");
  loaded_image = IMG_Load((fn_base + ".png").c_str());
  if (loaded_image != NULL)
    fn_loaded = fn_base + ".png";
  else
    {
      LOG3("Load `" << fn_base << ".jpg'");
      loaded_image = IMG_Load((fn_base + ".jpg").c_str());
      if (loaded_image == NULL)
        PRINT_AND_THROW(SDLError, "Can't load image " << fn_base << ".{png,jpg}");
      fn_loaded = fn_base + ".jpg";
    }
  surf = SDL_DisplayFormatAlpha(loaded_image);
  SDL_FreeSurface(loaded_image);
  if (surf == NULL)
    PRINT_AND_THROW(SDLError, "Can't convert " << fn_loaded << "to standard display");
  return surf;
}

SDL_Surface* ResourceCenter::transformImage(SDL_Surface* from, double zoom, double angle)
{
  return rotozoomSurface(from, angle, 1. / zoom, SMOOTHING_OFF);
}
