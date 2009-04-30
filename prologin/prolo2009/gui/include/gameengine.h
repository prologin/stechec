/*
** gameengine.h for  in /goinfre/lapie_t/prolo_gui_09
** 
** Made by stephane2 lapie
** Login   <lapie_t@epitech.net>
** 
** Started on  Fri Mar  6 16:04:01 2009 stephane2 lapie
** Last update Thu Apr 30 13:07:14 2009 user
*/

#ifndef GAMEENGINE_H_
# define GAMEENGINE_H_

# include <map>
# include "player.h"
# include "gamecase.h"
# include "game.h"

/* GameEngine (singleton pattern) */
class			GameEngine
{
 private:
  GameEngine(void);
  GameEngine(const GameEngine &right);
  GameEngine		&operator=(const GameEngine &right);
  void			RetrieveData(void);
  static GameEngine	*_instance;
  GameCase		_map[MAP_SIZE];
  std::map<int, Player>	_player;

 public:
  ~GameEngine(void);
  static GameEngine	*GetInstance(void);
  void			Run(void);
};

#endif /* !GAMEENGINE_H_ */
