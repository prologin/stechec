//
// thread.cpp for  in prolo_gui_09
// 
// Made by stephane2 lapie
// Login   <lapie_t@epitech.net>
// 
// Started on  Fri Mar  6 15:52:42 2009 stephane2 lapie
// Last update Tue Apr 28 13:28:18 2009 user
//

#include "thread.h"

/* */
int		LaunchThread(void *data)
{
  Thread	*thread;

  thread = static_cast<Thread*>(data);
  thread->Core();
  return (0);
}

Thread::~Thread()
{
  this->Stop();
}

void		Thread::Run(void)
{
  this->_thread = SDL_CreateThread(LaunchThread, this);
  if (this->_thread == NULL)
    throw "Failed to run the thread.";
}

void		Thread::Stop(void)
{
  if (this->_thread != NULL)
    SDL_KillThread(this->_thread);
}
