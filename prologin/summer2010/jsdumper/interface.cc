/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2005, 2006 Prologin
** author : Maxime AUDOUIN : <coucou747@gmail.com>
*/

#include <cstdlib>
#include <iostream>
#include <string>

extern "C" int api_state_is_end(void*);
extern "C" int api_state_is_playturn(void*);

extern "C" int client_cx_process(void*);
extern "C" int client_cx_set_ready(void*);

extern std::string dump_infos();

extern "C" int run(void* foo, void* api, void* client_cx)
{
  while (!api_state_is_end(api)){
    std::cout << dump_infos() << std::endl;
    while (api_state_is_playturn(api)){
      if (api_state_is_end(api)) return 0;
    }
    client_cx_set_ready(client_cx);
    while(client_cx_process(client_cx)) ;
  }
  return 0;
}
