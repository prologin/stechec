/*
** sprite.h<2> for  in /home/lapie_t/prologin_gui
** 
** Made by Hazgar
** Login   <lapie_t@epitech.net>
** 
** Started on  Fri Feb 27 11:21:32 2009 Hazgar
** Last update Fri May  1 03:15:44 2009 user
*/

#ifndef SPRITE_H_
# define SPRITE_H_

# include "surface.h"

enum			SpriteID
{
  SP_PARROT0,
  SP_MONKEY0,
  SP_CAT0,
  SP_KANGAROO0,
  SP_KO0,
  SP_KO1,
  SP_KO2,
  SP_KO3,
  SP_NONE
};

struct			SpriteCfg
{
  SpriteID		id;		/* Sprite ID			*/
  const char		*path;		/* Sprite surface source image	*/
  int			frames;		/* Sprite frames count		*/
  int			frameRate;	/* Sprite frame rate		*/
  int			direction;	/* Sprite direction set		*/
  bool			repeat;		/* Repeat mode			*/
  int			width;		/* Sprite width			*/
  int			height;		/* Sprite height		*/
  int			pos[2];		/* Sprite pos into src image	*/
};

class			Sprite : public Surface
{
 private:
  int			_frames;		/* Sprite frames count		*/
  int			_currentFrame;		/* Current frame		*/
  int			_frameRate;		/* Sprite frame rate		*/
  int			_currentFrameDuration;	/* Current frame duration	*/
  int			_direction;		/* Sprite direction set		*/
  bool			_repeat;		/* Sprite repeat mode		*/
  SDL_Rect		_defField;		/* Default sprite frame field	*/

 public:
  Sprite(const SpriteCfg &cfg);
  ~Sprite(void);
  Sprite(const Sprite &right);
  Sprite		&operator=(const Sprite &right);
  void			action();
  void			setFrameRate(int frate);
  void			setCurrentFrameDuration(int fduration);
  void			setCurrentFrame(int frame);
  void			setDirection(int dir);
  void			setRepeat(bool repeat);
  int			getFieldWidth(void) const;
  int			getFieldHeight(void) const;
};

#endif /* !SPRITE_H_ */
