/*
** gamecase.h for  in /home/lapie_t/prologin_gui
** 
** Made by Hazgar
** Login   <lapie_t@epitech.net>
** 
** Started on  Wed Apr 22 14:41:57 2009 Hazgar
** Last update Wed Apr 22 14:54:38 2009 Hazgar
*/

#ifndef GAMECASE_H_
# define GAMECASE_H_

# include "game.h"

/* */
class			GameCase
{
 private:
  t_landscape		_type;
  int			_price;
  t_owner		_owner;

 public:
  GameCase(void);
  GameCase(t_landscape type, int price, t_owner owner);
  ~GameCase(void);
  GameCase(const GameCase &right);
  GameCase		&operator=(const GameCase &right);
  void			setType(t_landscape type);
  void			setPrice(int price);
  void			setOwner(t_owner owner);
  t_landscape		getType(void) const;
  int			getPrice(void) const;
  t_owner		getOwner(void) const;
};

#endif /* !GAMECASE_H_ */
