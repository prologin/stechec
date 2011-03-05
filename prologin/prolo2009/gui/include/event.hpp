//
// event.hpp for  in /home/lapie_t/prologin_gui
// 
// Made by Hazgar
// Login   <lapie_t@epitech.net>
// 
// Started on  Wed Apr 22 14:21:10 2009 Hazgar
// Last update Fri May  1 02:43:54 2009 user
//

#ifndef EVENT_HPP_
# define EVENT_HPP_

typedef	enum
{
  EV_CASETYPE,
  EV_CASEPRICE,
  EV_CASEOWNER,
  EV_NEWTURN,
  EV_PLAYER,
  EV_WINNER,
  EV_ENDGAME,
  EV_DISPLAY_NEXTTURN,
  EV_DISPLAY_END
}	t_userEvent;

struct	EventCase
{
  int	x;
  int	y;
  int	data;
  EventCase(int _x, int _y, int _data)
  {
    this->x = _x;
    this->y = _y;
    this->data = _data;
  };
  ~EventCase(void) {};
  EventCase(const EventCase &right)
  {
    EventCase::operator=(right);
  };
  EventCase	&operator=(const EventCase &right)
  {
    this->x = right.x;
    this->y = right.y;
    this->data = right.data;
    return (*this);
  };
};

struct	EventPlayer
{
  int	id;
  int	score;
  int	money;
  int	bid;
  EventPlayer(int _id, int _score, int _money, int _bid)
  {
    this->id = _id;
    this->score = _score;
    this->money = _money;
    this->bid = _bid;
  };
  ~EventPlayer(void) {};
  EventPlayer(const EventPlayer &right)
  {
    EventPlayer::operator=(right);
  };
  EventPlayer	&operator=(const EventPlayer &right)
  {
    this->id = right.id;
    this->score = right.score;
    this->money = right.money;
    this->bid = right.bid;
    return (*this);
  };
};

#endif /* !EVENT_HPP_ */
