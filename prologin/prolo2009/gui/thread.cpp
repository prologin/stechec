//
// thread.cpp for  in prolo_gui_09
// 
// Made by stephane2 lapie
// Login   <lapie_t@epitech.net>
// 
// Started on  Fri Mar  6 15:52:42 2009 stephane2 lapie
// Last update Thu Apr 30 10:55:18 2009 user
//

#include <unistd.h>
#include "thread.h"

/* */
int		LaunchThread(void *data)
{
  Thread	*thread;

  thread = static_cast<Thread*>(data);
  thread->Core();
  return (0);
}

Thread::Thread()
{
  if (pipe(this->_pipe) == -1)
    throw "Thread creation error";
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

int		Thread::Read(void *buf, int size)
{
  int		return_value;

  return_value = read(this->_pipe[0], buf, size);
  return (return_value);
}

int		Thread::Write(void *buf, int size)
{
  int		return_value;

  return_value = write(this->_pipe[1], buf, size);
  return (return_value);
}
