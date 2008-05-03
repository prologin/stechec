// SpriteMgr.hpp -- The Prologin 2008 Map Editor

#ifndef SPRITEMGR_HPP_
# define SPRITEMGR_HPP_

#include "Gui.hpp"

class	Sprite
{
  public:
    Sprite(void) { _sprite = NULL; }
    Sprite(const char *path) { _sprite = IMG_Load(path); }
    ~Sprite(void) { }

    SDL_Surface		*GetSprite(void) { return (_sprite); }
  private:
    SDL_Surface		*_sprite;
};

class	SpriteMgr
{
  public:
    SpriteMgr(void) { }
    ~SpriteMgr(void) { }

    void	AddSprite(char key, const char *path);
    Sprite	*GetSprite(char key);

  private:
    std::map<char, Sprite *>	_sprites;
};

#endif
