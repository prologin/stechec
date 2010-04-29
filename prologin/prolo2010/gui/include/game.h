/*
** game.h for  in prologin_gui
** 
** Made by Hazgar
** Login   <lapie_t@epitech.net>
** 
** Started on  Fri Feb 27 09:55:25 2009 Hazgar
** Last update Sat May  2 12:49:18 2009 user
*/

#ifndef GAME_H_
# define GAME_H_

# define MAP_SIZE	625
# define MAP_WIDTH	25
# define MAP_HEIGHT	25
# define START_POSX	MAP_WIDTH / 2
# define START_POSY	MAP_HEIGHT / 2

/* Landscape types */
typedef	enum
{
  LD_PARROT,
  LD_MONKEY,
  LD_CAT,
  LD_KANGAROO,
  LD_KO0,
  LD_KO1,
  LD_KO2,
  LD_KO3,
  LD_MOVE0,
  LD_MOVE1,
  LD_MOVE2,
  LD_MOVE3,
  LD_MOVE4,
  LD_MOVE5,
  LD_MOVE6,
  LD_MOVE7,
  LD_EMPTY
}	t_landscape;

#endif /* !GAME_H_ */
