/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2010 Prologin
*/

#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <prologin.hh>

extern int api_state_is_end(void*);
extern int api_state_is_playturn(void*);

extern int client_cx_process(void*);
extern int client_cx_set_ready(void*);

void dump_current_state()
{
}

int run(void* foo, void* api, void* client_cx)
{
    int turn = -1;
    struct timespec st = { 0, 1000 };
    printf("%u\n", time(NULL));

    while (!api_state_is_end(api))
    {
        nanosleep(&st, NULL);
        if (tour_actuel() != turn)
        {
            turn = tour_actuel();
            dump_current_state();
        }

        client_cx_set_ready(client_cx);
        while (client_cx_process(client_cx))
            ;
    }
    return 0;
}
