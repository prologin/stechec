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

#include <stdlib.h>

#include "Api.hh"

// global used in interface.cc
Api* api;


// Internal API
bool Api::retirer_ko(unite u)
{
  return u.ko >= 0;
}

bool Api::mon_tour()
{
  return g_->mon_tour();
}

Api::Api(GameData* gameData, Client* c) : StechecApi(gameData, c)
{
  api = this;
}

///
// Retourne une structure \texttt{cartes} contenant les informations sur les cartes que vous avez en main.
//
cartes Api::mes_cartes()
{
  static cartes c = { 1, 2, 3, 4 };
  return c;
}

///
// Retourne la liste des unit�s actuellement en jeu.
//
std::vector<unite> Api::unites()
{
  static unite u1 = {
    { 1, 2 },
    false,
    CHAT,
    SINGE,
    2,
    4,
    6
  };

  static unite u2 = {
    { 5, 6 },
    true,
    PERROQUET,
    PERROQUET,
    4,
    2,
    10
  };

  std::vector<unite> ret;
  ret.push_back(u1);
  ret.push_back(u2);
  return ret;
}

///
// Retourne la taille actuelle du terrain et les coordonn�es min/max dans une structure \texttt{taille_terrain}.
//
taille_terrain Api::taille_terrain_actuelle()
{
  static taille_terrain tt = {
    25,
    0,
    25
  };

  return tt;
}

///
// Utilise une carte � Quoi d'neuf docteur ? � que vous avez dans votre main.
//
erreur Api::soin(unite cible)
{
  return OK;
}

///
// Utilise une carte � D�guisement � que vous avez dans votre main.
//
erreur Api::deguisement(unite cible, type_unite nouveau_type)
{
  return OK;
}

///
// Utilise une carte � Banza� � que vous avez dans votre main.
//
erreur Api::banzai(unite cible)
{
  return OK;
}

///
// Utilise une carte � Pacifisme � que vous avez dans votre main.
//
erreur Api::pacifisme()
{
  return OK;
}

///
// D�place une unit� vers une position � port�e.
//
erreur Api::deplacer(unite cible, position pos)
{
  return OK;
}

///
// Attaque une autre unit�.
//
erreur Api::attaquer(unite attaquant, unite cible)
{
  return OK;
}

///
// Fait apparaitre une unit� sur la case de spawn.
//
erreur Api::spawn(type_unite quoi)
{
  return OK;
}

///
// Annule l'effet de la derni�re action et remet le jeu dans l'�tat pr�c�dent.
//
bool Api::annuler()
{
  return false;
}



void Api::teamSwitched()
{
}
