/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2005, 2006 Prologin
** author : Maxime AUDOUIN : <coucou747@gmail.com>
*/

#include <cstdlib>
#include <iostream>
#include "show.h"

extern "C" int api_state_is_end(void*);
extern "C" int api_state_is_playturn(void*);

extern "C" int client_cx_process(void*);
extern "C" int client_cx_set_ready(void*);


extern "C" int run(void* foo, void* api, void* client_cx)
{
  init();
  while (!api_state_is_end(api)){
    event_turn();
    while (api_state_is_playturn(api)){
      if (api_state_is_end(api)) return end();
    }
    client_cx_set_ready(client_cx);
    while(client_cx_process(client_cx)) ;
  }
  end();
  return 0;
}
