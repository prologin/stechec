/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2004, 2005, 2006 Prologin
*/

#ifndef RESOLVER_HH_
# define RESOLVER_HH_

# include "Contest.hh"
# include "GameData.hh"

/*!
** @ingroup stechec_ant
** @{
*/

/*!
** @class _ant__ServerResolver
** @brief Ant ServerResolver.
*/
class ServerResolver: public StechecServerResolver          
{
public:

  ServerResolver(GameData* game, Server* server):
    StechecServerResolver(game, server)
  {}

  virtual void ApplyResolver(CommandListRef cmdList[]);
private:

  // prend la bouffe
  void        resolv_take_food(const StechecPkt* elt);
  // pose la nourriture dans la fourmiliere
  void        resolv_poser_food(const StechecPkt* elt);
  // Deplace la fourmis
  void        resolv_move(const StechecPkt* elt);

  // misc
  void move_ant(int player_id, int from_x, int from_y, int to_x, int to_y);
  void map_do_modif(int x, int y);
};


#endif // !RESOLVER_HH_

