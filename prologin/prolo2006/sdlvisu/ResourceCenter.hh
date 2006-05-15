/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2006 Prologin
*/

#ifndef RESOURCECENTER_HH_
# define RESOURCECENTER_HH_


/*
** Cache for resources.
*/
class ResourceCenter
{
public:
  ResourceCenter() {}
  ~ResourceCenter()
  {
    ImageList::iterator it;
    for (it = image_list_.begin(); it != image_list_.end(); ++it)
      SDL_FreeSurface(it->second);
  }

  //! @brief Get an SDL_Surface from an image.
  SDL_Surface* getImage(const std::string& filename)
  {
    ImageList::iterator it = image_list_.find(filename);
    if (it != image_list_.end())
      return it->second;

    SDL_Surface* loaded_image = NULL;
    SDL_Surface* surf = NULL;

    LOG3("Load " << PKGDATADIR "texture_2D/" << filename << ".png");
    loaded_image = IMG_Load((std::string(PKGDATADIR "texture_2D/")
                             + filename + ".png").c_str());
    if (loaded_image == NULL)
      throw filename;
    surf = SDL_DisplayFormatAlpha(loaded_image);
    SDL_FreeSurface(loaded_image);
    if (surf == NULL)
      throw filename;
    image_list_[filename] = surf;
    return surf;
  }

private:
  typedef std::map<std::string, SDL_Surface*> ImageList;
  ImageList image_list_;
};

#endif /* !RESOURCECENTER_HH_ */
