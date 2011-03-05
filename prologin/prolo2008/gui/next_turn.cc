/*
** next_turn.cc for Project in /home/deather/work/prologin/stechec2/prologin/prolo2008/gui
**
** Made by deather
** Login   <deather@epitech.net>
**
** Started on  Wed 07 May 2008 11:17:53 AM CEST deather
** Last update Fri May  9 23:30:28 2008 deather
*/

#include "ClientCx.hh"
#include "Api.hh"
#include "Event.hh"
#include <iostream>


// Handles event from the rules
class MyHamsterEvent : public Event
{
  public:
    MyHamsterEvent(void (*f)(void)) { _handler = f; }
    ~MyHamsterEvent() { };

  private:
    void (*_handler)(void);
    virtual void endOfTurn(void)
    {
      _handler();
    };
    virtual void evStechecPkt(const StechecPkt &pkt)
    {
    }
};

// Defines a C event handler
extern "C" void set_event_handler(void *a, void (*fn)(void))
{
  static MyHamsterEvent *hev = NULL;
  Api *api = reinterpret_cast<Api *>(a);

  if (!hev)
    hev = new MyHamsterEvent(fn);
  api->setEventHandler(hev);
}

extern "C" int get_state(Api *api)
{
  return (api->getState());
}

// Processes every incoming messages.
extern "C" int process_messages(ClientCx *ccx)
{
  while (ccx->process(true))
    ;
  return (0);
}

// Say to the server we are ready.
extern "C" void next_turn(void *ccx)
{
  ClientCx *cx = reinterpret_cast<ClientCx*>(ccx);

  cx->setReady();
}
