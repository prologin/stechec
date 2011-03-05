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

# define MAP_SIZE	10000
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
  OWN_PLAYER0,
  OWN_PLAYER1,
  OWN_PLAYER2,
  OWN_TOWNHALL,
  OWN_NOBODY
}	t_owner;
# define NB_PLAYERS	OWN_TOWNHALL

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
