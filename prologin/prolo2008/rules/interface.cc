
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

#include "Api.hh"

// from Api.cc
extern Api* api;

extern "C" {

  int taille_carte_x()
  {
    return api->taille_carte_x();
  }

  int taille_carte_y()
  {
    return api->taille_carte_y();
  }

  int numero_tour()
  {
    return api->numero_tour();
  }

  int nombre_tours()
  {
    return api->nombre_tours();
  }

  bool commence()
  {
    return api->commence();
  }

  int pos_x(int id)
  {
    return api->pos_x(id);
  }

  int pos_y(int id)
  {
    return api->pos_y(id);
  }

  int porte_pomme(int id)
  {
    return api->porte_pomme(id);
  }

  int distance(int x1, int y1, int x2, int y2, bool sort_tranchee)
  {
    return api->distance(x1, y1, x2, y2, sort_tranchee);
  }
  
  int trognons_restants(int id)
  {
    return api->trognons_restants(id);
  }

  int type_case(int x, int y)
  {
    return api->type_case(x, y);
  }

  int pomme(int x, int y)
  {
    return api->pomme(x, y);
  }

  int deplacer(int id, int direction)
  {
    return api->deplacer(id, direction);
  }

  int turbo(int id)
  {
    return api->turbo(id);
  }

  int lacher_pomme(int id, int direction)
  {
    return api->lacher_pomme(id, direction);
  }

  int ramasser_pomme(int id)
  {
    return api->ramasser_pomme(id);
  }

  int grappin(int id, int direction)
  {
    return api->grappin(id, direction);
  }

  int trognon(int id, int direction)
  {
    return api->trognon(id, direction);
  }

  int attendre(int id)
  {
    return api->attendre(id);
  }

  // to keep after a ./generator
  int actions_faites(int numero)
  {
    return api->actions_faites(numero);
  }

}
