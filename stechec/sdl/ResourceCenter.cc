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
  prefix_ = PKGDATADIR;
  LOG4("Prefix for resources: `" << prefix_ << "'");
}

ResourceCenter::~ResourceCenter()
{
  inst_ = NULL;
  printStatistics();
}

const std::string& ResourceCenter::getResourcePrefix() const
{
  return prefix_;
}
  
void ResourceCenter::setResourcePrefix(const std::string& prefix)
{
  if (prefix == "")
    prefix_ = PKGDATADIR;
  else
    prefix_ = prefix;
}


void ResourceCenter::printStatistics()
{
  ImageList::iterator it;
  FontList::iterator fit;
  
  LOG3("*** ResourceCenter: " << image_list_.size() << " images in list.");
  for (it = image_list_.begin(); it != image_list_.end(); ++it)
    LOG4(" - " << it->filename_ << " (" << it->zoom_ << ", " << it->angle_
         << ") refcount: " << it->surf_->refcount << " ptr: " << (void*)it->surf_);
  if (!font_list_.empty())
    {
      LOG3("*** There is also " << font_list_.size() << " font(s) not released.");
      for (fit = font_list_.begin(); fit != font_list_.end(); ++fit)
        LOG4(" - " << fit->name_ << " (size " << fit->size_
             << ") refcount: " << fit->ref_count_);
    }
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
  if (zoom != 1. || angle != 0.)
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
      orig_surf->refcount = 0;
      image_list_.insert(Surface(orig_surf, 1., 0., filename));
      // If there is no transformation to do, return now.
      if (zoom == 1. && angle == 0.)    
        return Surface(orig_surf, 1., 0., filename);
    }

  SDL_Surface* wanted_surf = transformImage(orig_surf, zoom, angle);
  wanted_surf->refcount = 0;
  Surface res_surf(wanted_surf, zoom, angle, filename);
  image_list_.insert(res_surf);
  return res_surf;
}

SDL_Surface* ResourceCenter::loadImage(const std::string& filename)
{
  SDL_Surface* loaded_image = NULL;
  SDL_Surface* surf = NULL;
  std::string fn_loaded = prefix_ + "/" + filename;

  LOG3("Load `" << filename << "'");
  loaded_image = IMG_Load(fn_loaded.c_str());
  if (loaded_image == NULL)
    PRINT_AND_THROW(SDLError, "Can't load image `" << filename << "'");
  surf = SDL_DisplayFormatAlpha(loaded_image);
  SDL_FreeSurface(loaded_image);
  if (surf == NULL)
    PRINT_AND_THROW(SDLError, "Can't convert `" << filename << "' to standard display");
  return surf;
}

SDL_Surface* ResourceCenter::transformImage(SDL_Surface* from, double zoom, double angle)
{
  return rotozoomSurface(from, angle, 1. / zoom, SMOOTHING_OFF);
}


TTF_Font* ResourceCenter::getFont(const std::string font_name, int font_size)
{
  LoadedFont lf;

  lf.name_ = font_name;
  lf.size_ = font_size;

  // Search if this font was already loaded.
  FontList::iterator it = font_list_.find(lf);
  if (it != font_list_.end())
    {
      const_cast<LoadedFont&>(*it).ref_count_++;
      return it->font_;
    }

  // Load this font, and add it to the cache.
  LOG3("Load font `font/" << font_name << "' (size: " << font_size << ")");
  lf.ref_count_ = 1;
  lf.font_ = TTF_OpenFont((prefix_ + "/font/" + font_name).c_str(),
                          font_size);
  if (lf.font_ == NULL)
    PRINT_AND_THROW(TTFError, "OpenFont: ");
  
  font_list_.insert(lf);
  return lf.font_;
}

void ResourceCenter::releaseFont(TTF_Font* font)
{
  FontList::iterator it;
  for (it = font_list_.begin(); it != font_list_.end(); ++it)
    if (it->font_ == font)
      {
        if (--(const_cast<LoadedFont&>(*it).ref_count_) == 0)
          {
            TTF_CloseFont(it->font_);
            font_list_.erase(it);
          }
        return;
      }
  WARN("Trying to release font " << (void*)font << ", but wasn't loaded here !");
}
