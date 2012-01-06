/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2007 Prologin
*/

#ifndef CLIENTENTRY_HH_
# define CLIENTENTRY_HH_

# include "Contest.hh"
# include "ClientDiffer.hh"
# include "Client.hh"

class		ClientEntry : public StechecClientEntry
{
   public:

      ClientEntry(GameData* game, ClientDiffer* diff, Client* client);

      virtual int	beforeGame();
      virtual int	initGame();
      virtual int	beforeNewTurn();
      virtual int	afterNewTurn();
      virtual int	afterGame();
   private:
      std::pair<char, short>	astar(Position *start, Position *end);
      // FIXME: To precalculate path
      std::pair<char, short>	calculate_paths(int row, int col, int row1, int col1);
      int	visible_test(Position ref, Position pos);
      int	visible_from(int from_x, int from_y);
};

#endif // !CLIENTENTRY_HH_
