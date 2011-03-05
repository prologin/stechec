// Gui.hpp -- The Prologin 2008 Map Editor

#ifndef GUI_HPP_
# define GUI_HPP_

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <map>

typedef void	(*GuiCallback)(SDL_Event *);

class	Sprite;

class	Gui
{
  public:
    Gui(void);
    Gui(const char *wnd_name, unsigned int xsize, unsigned int ysize);
    ~Gui(void);

    void		PutPixel(unsigned int x, unsigned int y, unsigned int color);
    void		PutSprite(unsigned int x, unsigned int y, Sprite *s);
    void		DrawRect(unsigned int x, unsigned int y, unsigned int xsize, unsigned int ysize, unsigned int color);
    void		SetCallback(Uint8 event, GuiCallback f);
    void		Clear(void);
    void		Refresh(void);
    void		Loop(void);

    unsigned int	GetXSize(void) { return (_xsize); }
    unsigned int	GetYSize(void) { return (_ysize); }

  private:
    SDL_Surface		*_wnd;
    unsigned int	_xsize;
    unsigned int	_ysize;
    unsigned int	_bpp;
    std::map<Uint8, GuiCallback>	_callbacks;
};

#endif
