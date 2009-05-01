/*
** infopanel2.h for  in /home/user/prolo_stechec/prologin/prolo2009/gui
** 
** Made by user
** Login   <user@epitech.net>
** 
** Started on  Thu Apr 30 13:09:42 2009 user
** Last update Fri May  1 02:48:26 2009 user
*/

#ifndef INFOPANEL2_H_
# define INFOPANEL2_H_

# include "surface.h"
# include "player.h"

class			InfoPanel2
{
 private:
  Surface		*_dst;
  Surface		*_sfc;
  SfcField		_pos;
  int			_player_id;
  Player		_player;
  bool			_visible;

 public:
  InfoPanel2(Surface *src, Surface *dst);
  ~InfoPanel2(void);
  void			UpdatePlayer(int id, int score, int money, int bid);
  void			Refresh(void);
  void			setDst(Surface *dst);
  void			setPos(unsigned int x, unsigned int y);
  void			setVisible(bool v);
  int			getWidth(void) const;
  int			getHeight(void) const;
};

#endif /* !INFOPANEL2_H_ */
