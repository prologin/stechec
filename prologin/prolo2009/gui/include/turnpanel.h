/*
** turnpanel.h for  in /home/user/prolo_stechec/prologin/prolo2009/gui
** 
** Made by user
** Login   <user@epitech.net>
** 
** Started on  Thu Apr 30 13:09:42 2009 user
** Last update Thu Apr 30 15:59:51 2009 user
*/

#ifndef TURNPANEL_H_
# define TURNPANEL_H_

# include <map>
# include "surface.h"
# include "player.h"

class			TurnPanel
{
 private:
  Surface		*_dst;
  Surface		*_sfc;
  SfcField		_pos;
  int			_turn;

 public:
  TurnPanel(Surface *src, Surface *dst);
  ~TurnPanel(void);
  void			UpdateTurn(int turn);
  void			Refresh(void);
  void			setDst(Surface *dst);
  void			setPos(unsigned int x, unsigned int y);
  int			getWidth(void) const;
  int			getHeight(void) const;
};

#endif /* !TURNPANEL_H_ */
