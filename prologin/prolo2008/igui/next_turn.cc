/*
** next_turn.cc for Project in /home/deather/work/prologin/stechec2/prologin/prolo2008/gui
**
** Made by deather
** Login   <deather@epitech.net>
**
** Started on  Wed 07 May 2008 11:17:53 AM CEST deather
** Last update Wed 07 May 2008 11:19:37 AM CEST deather
*/

#include "ClientCx.hh"

extern "C" void next_turn(void *ccx)
{
  ClientCx *cx = reinterpret_cast<ClientCx*>(ccx);

  cx->setReady();
}
