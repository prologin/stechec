/*
** surface.h for  in prologin_gui
** 
** Made by Hazgar
** Login   <lapie_t@epitech.net>
** 
** Started on  Fri Feb 27 10:31:15 2009 Hazgar
** Last update Wed Apr 29 18:52:52 2009 user
*/

#ifndef SURFACE_H_
# define SURFACE_H_

# include <SDL.h>

enum		SurfaceID
{
  SFC_FLOOR,
  SFC_PANEL,
  SFC_PRICE,
  SFC_NONE
};

/*
** Overload on SDL_Rect
*/
class		SfcField
{
 protected:
  SDL_Rect	_field;

 public:
  SfcField(void);
  ~SfcField(void);
  SfcField(const SfcField &right);
  SfcField	&operator=(const SfcField &right);
  void		setPos(unsigned int x, unsigned int y);
  void		setSize(unsigned int width, unsigned int height);
  void		*getField(void);
  int		getPosX(void) const;
  int		getPosY(void) const;
  unsigned int	getWidth(void) const;
  unsigned int	getHeight(void) const;
};

/*
** Overload on SDL_Surface
*/
class		Surface : public SfcField
{
 private:
  void		InitField(void);

 protected:
  SDL_Surface	*_sfc;

 public:
  Surface(void *sfc);
  Surface(const char *path);
  ~Surface(void);
  Surface(const Surface &right);
  Surface	&operator=(const Surface &right);
  void		*getSurface(void) const;
  void		SetAlphaFlags(void);
  void		UnsetAlphaFlags(void);
  void		Blit(Surface &dst, SfcField &pos);
};

#endif /* !SURFACE_H_ */
