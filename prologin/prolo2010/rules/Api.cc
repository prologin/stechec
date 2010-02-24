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


///
// Retourne une structure \texttt{cartes} contenant les informations sur les cartes que vous avez en main.
//
cartes Api::mes_cartes()
{
  // TODO
  abort();
}

///
// Retourne la liste des unit�s actuellement en jeu.
//
std::vector<unite> Api::unites()
{
  // TODO
  abort();
}

///
// Retourne la taille actuelle du terrain et les coordonn�es min/max dans une structure \texttt{taille_terrain}.
//
taille_terrain Api::taille_terrain_actuelle()
{
  // TODO
  abort();
}

///
// Utilise une carte � Quoi d'neuf docteur ? � que vous avez dans votre main.
//
erreur Api::soin(unite cible)
{
  // TODO
  abort();
}

///
// Utilise une carte � D�guisement � que vous avez dans votre main.
//
erreur Api::deguisement(unite cible, type_unite nouveau_type)
{
  // TODO
  abort();
}

///
// Utilise une carte � Banza� � que vous avez dans votre main.
//
erreur Api::banzai(unite cible)
{
  // TODO
  abort();
}

///
// Utilise une carte � Pacifisme � que vous avez dans votre main.
//
erreur Api::pacifisme()
{
  // TODO
  abort();
}

///
// D�place une unit� vers une position � port�e.
//
erreur Api::deplacer(unite cible, position pos)
{
  // TODO
  abort();
}

///
// Attaque une autre unit�.
//
erreur Api::attaquer(unite attaquant, unite cible)
{
  // TODO
  abort();
}

///
// Fait apparaitre une unit� sur la case de spawn.
//
erreur Api::spawn(type_unite quoi)
{
  // TODO
  abort();
}

///
// Annule l'effet de la derni�re action et remet le jeu dans l'�tat pr�c�dent.
//
bool Api::annuler()
{
  // TODO
  abort();
}



void Api::teamSwitched()
{
}
