/*
** game.h for  in prologin_gui
** 
** Made by Hazgar
** Login   <lapie_t@epitech.net>
** 
** Started on  Fri Feb 27 09:55:25 2009 Hazgar
** Last update Sat Apr 25 16:54:19 2009 Hazgar
*/

#ifndef GAME_H_
# define GAME_H_

# define MAP_SIZE	10100
# define MAP_WIDTH	100
# define MAP_HEIGHT	100
# define START_POSX	MAP_WIDTH / 2
# define START_POSY	MAP_HEIGHT / 2

/* Landscape types */
typedef	enum
{
  LD_EMPTY,
  LD_HOUSE,
  LD_RESERVED,
  LD_MONUMENT,
  LD_ROAD,
  LD_MONUMENTS
}	t_landscape;

/* owners */
typedef	enum
{
  OWN_TOWNHALL
}	t_owner;

/* errors */
typedef	enum
{
  BAD_ACTION,
  BAD_PLAYER,
  NO_HOUSE,
  NO_MONUMENT,
  NO_MONEY,
  NOT_CONNEX,
  NOT_EMPTY,
  OUT_OF_MAP,
  STUCK,
  SUCCESS,
  TOO_FAR,
  TOO_MUCH_HOUSES
}	t_error;
  

#endif /* !GAME_H_ */
