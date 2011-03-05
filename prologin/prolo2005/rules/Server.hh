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

#ifndef SERVER_HH_
# define SERVER_HH_

# include "Contest.hh"
# include "GameData.hh"

class Server : public StechecServer
{
public:
  Server(GameData *g) : StechecServer(g) {}
  virtual ~Server() {}

  void ChangeState(Coleoptere* col, int state);
  void UpdatePlasma(Coleoptere* col, int qte);
  void UpdatePlasma(Factory* build, int qte);
  void UpdateOre(Coleoptere* col, int qte);
  void UpdateOre(Factory* build, int qte);
};

#endif /* !SERVER_HH_ */

