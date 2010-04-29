/*
** font.h for  in /home/user/prolo_stechec/prologin/prolo2009/gui
** 
** Made by user
** Login   <user@epitech.net>
** 
** Started on  Tue Apr 28 14:15:47 2009 user
** Last update Thu Apr 30 14:24:48 2009 user
*/

#ifndef FONT_H_
# define FONT_H_

# include <SDL.h>
# include <SDL_ttf.h>
# include <sstream>
# include "surface.h"

enum			FontID
  {
    FT_PRICES,
    FT_INFOS,
    FT_INFOS2,
    FT_NONE
  };

/*
** SDL_Font overload
*/
class			Font
{
 private:
  Surface		*_sfc;
  TTF_Font		*_font;
  SDL_Color		_color;

 public:
  std::stringstream	Text;

  Font(const char *path, int size);
  ~Font(void);
  Font(const Font &right);
  Font			&operator=(const Font &right);
  void			setText(std::string str);
  void			setColor(unsigned int color);
  std::string		getText(void) const;
  unsigned int		getColor(void) const;
  void			*getSurface(void) const;
  void			Blit(Surface &dst, SfcField &pos);
  int			getWidth(void) const;
  int			getHeight(void) const;
};

#endif /* !FONT_H_ */
