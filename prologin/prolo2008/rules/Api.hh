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

#ifndef API_HH_
# define API_HH_

# include <Contest.hh>
# include "GameData.hh"

/*!
** Method of this call are called by the candidat, throught 'interface.cc'
**
** You can:
**  - access to the GameData class: _g
**  - send message to the server: void SendToServer(struct s_com&)
*/
class Api: public StechecApi          
{

public:

  Api(GameData* gameData, Client* c);
  virtual ~Api() { }

public:

   int numero_tour();
   int nombre_tours();
   bool commence();
   int pos_x(int id);
   int pos_y(int id);
   bool porte_balle(int id);
   int distance(int x1, int y1, int x2, int y2);
   int type_case(int x, int y);
   bool balle(int x, int y);
   int deplacer(int id, int direction);
   int turbo(int id);
   int lacher_balle(int id);
   int prendre_balle(int id);
   int grapin(int id, int direction);
   int attendre(int id);

protected:
  virtual void teamSwitched();
  

};


#endif /* !API_HH_ */
