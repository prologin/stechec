/*
** thread.h for  in prolo_gui_09
** 
** Made by stephane2 lapie
** Login   <lapie_t@epitech.net>
** 
** Started on  Fri Mar  6 15:46:04 2009 stephane2 lapie
** Last update Thu Apr 30 10:55:24 2009 user
*/

#ifndef THREAD_H_
# define THREAD_H_

# include <SDL/SDL.h>
# include <SDL/SDL_thread.h>

class			Thread
{
 private:
  SDL_Thread		*_thread;
  int			_pipe[2];

 protected:
  virtual void		Core(void) = 0;

 public:
  Thread(void);
  virtual ~Thread(void);
  void			Run(void);
  void			Stop(void);
  int			Read(void *buf, int size);
  int			Write(void *buf, int size);
  friend int		LaunchThread(void *data);
};

#endif /* !THREAD_H_ */
