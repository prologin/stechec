//
// event.hpp for  in /home/lapie_t/prologin_gui
// 
// Made by Hazgar
// Login   <lapie_t@epitech.net>
// 
// Started on  Wed Apr 22 14:21:10 2009 Hazgar
// Last update Wed Apr 22 14:29:41 2009 Hazgar
//

#ifndef EVENT_HPP_
# define EVENT_HPP_

typedef	enum
{
  EV_CASETYPE,
  EV_CASEPRICE,
  EV_CASEOWNER,
  EV_NEWBID,
  EV_NEWTURN
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

#endif /* !EVENT_HPP_ */
