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

#ifndef ACTIONS_HH_
# define ACTIONS_HH_

# include "Contest.hh"
# include "Constant.hh"

typedef enum {
  e_deplacer = 0,
  e_spawn,
  e_attaquer,

  e_deguisement,
  e_banzai,
  e_pacifisme,
  e_soin
} action_e;

class Actions
{
public :
  Actions();
  void deguisement(int indice_unite, type_unite nouveau_type);
  void banzai(int indice_unite);
  void pacifisme();
  void soin(int indice_unite);

  void deplacer(int indice_unite, position pos, int d);
  void attaquer(int indice_attaquant, int indice_cible);
  void spawn(type_unite quoi);
  // private ?
  action_e type;
  int i1, i2, d;
  position p;
  type_unite tu;
};

#endif
