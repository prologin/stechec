/*
** thread.h for  in prolo_gui_09
** 
** Made by stephane2 lapie
** Login   <lapie_t@epitech.net>
** 
** Started on  Fri Mar  6 15:46:04 2009 stephane2 lapie
** Last update Fri Mar  6 16:29:38 2009 stephane2 lapie
*/

#ifndef THREAD_H_
# define THREAD_H_

# include <SDL/SDL.h>
# include <SDL/SDL_thread.h>

class		Thread
{
 private:
  SDL_Thread	*_thread;

 protected:
  virtual void	Core(void) = 0;

 public:
  virtual ~Thread(void);
  void		Run(void);
  void		Stop(void);
  friend int	LaunchThread(void *data);
};

#endif /* !THREAD_H_ */
