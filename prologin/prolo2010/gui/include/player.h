/*
** player.h for  in /home/user/prolo_stechec/prologin/prolo2009/gui
** 
** Made by user
** Login   <user@epitech.net>
** 
** Started on  Thu Apr 30 13:07:18 2009 user
** Last update Fri May  1 02:42:58 2009 user
*/

#ifndef PLAYER_H_
# define PLAYER_H_

# include <vector>
# include "Constant.hh"

struct			Player
{
  cartes		cards;
  std::vector<unite>	units;
};

#endif /* !PLAYER_H_ */
