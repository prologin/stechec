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

int type_case(int x, int y)
{
  return api->type_case(x, y);
}

int valeur_case(int x, int y)
{
  return api->valeur_case(x, y);
}

int appartenance(int x, int y)
{
  return api->appartenance(x, y);
}

int type_monument(int x, int y)
{
  return api->type_monument(x, y);
}

int portee_monument(int num_monument)
{
  return api->portee_monument(num_monument);
}

int prestige_monument(int num_monument)
{
  return api->prestige_monument(num_monument);
}

int numero_tour()
{
  return api->numero_tour();
}

int commence()
{
  return api->commence();
}

int montant_encheres(int num_joueur)
{
  return api->montant_encheres(num_joueur);
}

int vainqueur_encheres()
{
  return api->vainqueur_encheres();
}

int score(int num_joueur)
{
  return api->score(num_joueur);
}

int finances(int num_joueur)
{
  return api->finances(num_joueur);
}

int monument_en_cours()
{
  return api->monument_en_cours();
}

int distance(int x1, int y1, int x2, int y2)
{
  return api->distance(x1, y1, x2, y2);
}

int route_possible(int x, int y)
{
  return api->route_possible(x, y);
}

int construction_possible(int x, int y)
{
  return api->construction_possible(x, y);
}

int cout_achat_maison()
{
  return api->cout_achat_maison();
}

int cout_achat_route()
{
  return api->cout_achat_route();
}

bool mon_tour()
{
  return api->mon_tour();
}

int construire_route(int x, int y)
{
  return api->construire_route(x, y);
}

int construire_maison(int x, int y)
{
  return api->construire_maison(x, y);
}

int reserver_case(int x, int y)
{
  return api->reserver_case(x, y);
}

int detruire_maison(int x, int y)
{
  return api->detruire_maison(x, y);
}

int vendre_maison(int x, int y)
{
  return api->vendre_maison(x, y);
}

int encherir(int montant)
{
  return api->encherir(montant);
}

int construire_monument(int x, int y)
{
  return api->construire_monument(x, y);
}


}
