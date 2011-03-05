/*
** display.h for Prolo_gui09 in prologin_gui
** 
** Made by Hazgar
** Login   <lapie_t@epitech.net>
** 
** Started on  Thu Feb 26 10:27:25 2009 Hazgar
** Last update Fri May  1 09:24:11 2009 user
*/

#ifndef DISPLAY_H_
# define DISPLAY_H_

# include <string>
# include <SDL.h>
# include "thread.h"
# include "surface.h"
# include "sprite.h"
# include "font.h"
# include "displaymap.h"

/* Default window size (in pixel) */
# define WIN_WIDTH	800
# define WIN_HEIGHT	800

/* Motions */
# define MOTION_RESET	0x00
# define MOTION_LEFT	0x01
# define MOTION_RIGHT	0x02
# define MOTION_UP	0x04
# define MOTION_DOWN	0x08
# define MOTION_LOCK	0x10

# define MOUSE_MOTION_MARGIN 40 /* window margin */ 

/* surfaces */
struct			SurfacesList
{
  SurfaceID		id;
  const char		*path;
  Surface		*sfc;
};

/* sprites */
struct			SpritesList
{
  SpriteCfg		cfg;
  Sprite		*sprite;
};

/* font */
struct			FontsList
{
  FontID		id;
  int			size;
  const char		*path;
  Font			*font;
};

/*
** Display control class (singleton and command pattern)
*/
class			Display : public Thread
{
 private:
  Display(unsigned int winWidth = WIN_WIDTH, unsigned int winHeight = WIN_HEIGHT);
  Display(const Display &right);
  Display		&operator=(const Display &right);
  static Display	*_instance;
  Surface		*_screen;
  Surface		*_layout;
  DisplayMap		*_map;
  unsigned char		_motions;
  unsigned int		_winWidth;
  unsigned int		_winHeight;
  std::string		_winCaption;
  int			_display_motion[2];
  void			loadSurfaces(void);
  void			loadSprites(void);
  void			loadFonts(void);
  void			keyboardEvent(unsigned int keysym);
  void			mouseMotionEvent(unsigned int x, unsigned int y);
  void			mouseClickEvent(unsigned int x, unsigned int y);
  void			userEvent(unsigned int code, void *data);
  void			displayMotion(void);
  void			Core(void);

 public:
  ~Display();
  static Display	*GetInstance(void);
  Surface		*GetSurface(SurfaceID id);
  Sprite		*GetSprite(SpriteID id);
  Font			*GetFont(FontID id);
  void			setWinSize(unsigned int winWidth, unsigned int winHeight);
  void			setWinCaption(std::string caption);
  void			setDisplayMotion(int x, int y);
};

#endif /* !DISPLAY_H_ */
