/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2006 Prologin
*/

#ifndef QT_VISU_HH_
# define QT_VISU_HH_

# include "tools.hh"
# include "client_cx.hh"
# include "Api.hh"
# include "Event.hh"
# include "Gui.hh"

class QTVisu : public Gui, public AntEvent
{
public:
  QTVisu(Api* api, ClientCx* ccx);
  virtual ~QTVisu();
  
private:
  /*!
  ** Events
  */
  virtual void  mapContent(const Position& pos, int c);
  virtual void  antMove(int player_id, int ant_id, const Position& pos);

  /*!
  ** Helper functions.
  */
  void          printSquare(int i, int j);
  void          resetMap();
  void          printMap();

  Api*          api_;

  Object2D*     map_[50][50]; // max_map_size
  Object2D*     map_food_[50][50]; // max_map_size
  Object2D*     ant_[16][MAX_UNIT]; // max_player
};

#endif /* !QT_VISU_HH_ */
