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
*/

/* These functions are defined in common stechec rules. */
extern int api_state_is_end(void*);
extern int api_state_is_playturn(void*);
extern void api_do_end_turn(void*);
extern int api_get_nb_team(void*);
extern int client_cx_process(void*);

/* Must be defined in champion library. */
extern void init_game();
//extern void play_turn(); // Valid until prolo2008
// Only used for prolo2009:
extern void enchere();
extern void jouer();
extern void placement();

extern void end_game();

int run(void* foo, void* api, void* client_cx)
{
  init_game();
  int tour = 0;
  while (!api_state_is_end(api))
    {
      // Wait until we can play.
      while (!api_state_is_playturn(api) && !api_state_is_end(api))
	client_cx_process(client_cx);
      if (api_state_is_end(api))
        break;

      // Play a turn.
      //      play_turn();  // Use that until prolo2008.

      // For prolo2009, we call different functions depending of the type of the turn:
      int t = api_get_nb_team(api);
      int nb_virtual_turns = 2 * t + 1;
      if (tour % nb_virtual_turns < t)
	jouer();
      if (tour % nb_virtual_turns == t)
	enchere();
      if (tour % nb_virtual_turns > t)
	placement();
      
      api_do_end_turn(api);
      tour++;
    }

  end_game();
  foo = foo;
  return 0;
}
