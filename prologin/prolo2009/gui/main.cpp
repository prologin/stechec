#include <time.h>
#include <csignal>
#include <cstdlib>
#include "display.h"
#include "gameengine.h"

void		*g_client_cx;
void		*g_api;

static void	exit_handler(int sig)
{
  (void)sig;
  exit(0);
}

extern "C" {
int		run(void* foo, void* api, void* client_cx)
{
  Display	*dsp;
  GameEngine	*ge;

  (void)foo;
  srandom(time(NULL));
  g_client_cx = client_cx;
  g_api = api;
  signal(SIGINT, exit_handler);
  dsp = Display::GetInstance();
  dsp->setWinCaption("Prologin 2009");
  ge = GameEngine::GetInstance();
  dsp->Run();
  ge->Run();
  return (0);
}
}
