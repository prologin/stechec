/*
** sprite.h<2> for  in /home/lapie_t/prologin_gui
** 
** Made by Hazgar
** Login   <lapie_t@epitech.net>
** 
** Started on  Fri Feb 27 11:21:32 2009 Hazgar
** Last update Sat Apr 25 10:26:56 2009 Hazgar
*/

#ifndef SPRITE_H_
# define SPRITE_H_

# include "surface.h"

enum			SpriteID
{
  SP_HOUSE,
  SP_ROAD1,
  SP_ROAD2,
  SP_ROAD3,
  SP_ROAD4,
  SP_ROAD5,
  SP_ROAD6,
  SP_ROAD7,
  SP_ROAD8,
  SP_ROAD9,
  SP_ROAD10,
  SP_ROAD11,
  SP_RESERVED,
  SP_MONUMENT1,
  SP_MONUMENT2,
  SP_MONUMENT3,
  SP_MONUMENT4,
  SP_MONUMENT5,
  SP_MONUMENT6,
  SP_MONUMENT7,
  SP_MONUMENT8,
  SP_MONUMENT9,
  SP_MONUMENT10,
  SP_MONUMENT11,
  SP_MONUMENT12,
  SP_MONUMENT13,
  SP_MONUMENT14,
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
};

#endif /* !SPRITE_H_ */
