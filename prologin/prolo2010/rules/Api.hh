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

# include <vector>

# include <Contest.hh>
# include "Constant.hh"
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

  Api(GameData* gameData, Client* c) :
    StechecApi(gameData, c)
  { }

  virtual ~Api() { }

public:

///
// Retourne un booléen représentant si la case existe. On considére qu'une case existe si ses coordonnées sont dans le terrain (on exclut dont les coordonnées négatives, inférieures à la coordonnée minimale et supérieures à la coordonnée maximale donnés par la fonction infos_carte.
//
   bool case_existe(position pos);
///
// Prend un tableau en param
//
   void test_tableau(std::vector<int> test, std::vector<position> test2, std::vector<teststruct> test3);
};


#endif /* !API_HH_ */
