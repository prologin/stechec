/*
** infopanel.h for  in /home/user/prolo_stechec/prologin/prolo2009/gui
** 
** Made by user
** Login   <user@epitech.net>
** 
** Started on  Thu Apr 30 13:09:42 2009 user
** Last update Fri May  1 06:05:51 2009 user
*/

#ifndef INFOPANEL_H_
# define INFOPANEL_H_

# include <map>
# include "surface.h"
# include "player.h"

class			InfoPanel
{
 private:
  Surface		*_dst;
  Surface		*_sfc;
  SfcField		_pos;
  std::map<int, Player>	_player_info;

 public:
  InfoPanel(Surface *src, Surface *dst);
  ~InfoPanel(void);
  void			UpdatePlayer(int id, int score, int money, int bid);
  void			Refresh(void);
  void			setDst(Surface *dst);
  void			setPos(unsigned int x, unsigned int y);
  int			getWidth(void) const;
  int			getHeight(void) const;
};

#endif /* !INFOPANEL_H_ */
