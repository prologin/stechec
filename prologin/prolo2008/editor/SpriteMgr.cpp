// SpriteMgr.cpp -- The Prologin 2008 Map Editor

#include "SpriteMgr.hpp"
#include "Gui.hpp"

void	SpriteMgr::AddSprite(char key, const char *path)
{
  _sprites[key] = new Sprite(path);
}

Sprite	*SpriteMgr::GetSprite(char key)
{
  std::map<char, Sprite *>::iterator iter;

  iter = _sprites.find(key);
  if (iter != _sprites.end())
    return (iter->second);
  return (NULL);
}
