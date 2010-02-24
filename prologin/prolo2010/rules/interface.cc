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

#include "Api.hh"

// from Api.cc
extern Api* api;

///
// Retourne une structure \texttt{cartes} contenant les informations sur les cartes que vous avez en main.
//
cartes mes_cartes()
{
  return api->mes_cartes();
}

///
// Retourne la liste des unit�s actuellement en jeu.
//
std::vector<unite> unites()
{
  return api->unites();
}

///
// Retourne la taille actuelle du terrain et les coordonn�es min/max dans une structure \texttt{taille_terrain}.
//
taille_terrain taille_terrain_actuelle()
{
  return api->taille_terrain_actuelle();
}

///
// Utilise une carte � Quoi d'neuf docteur ? � que vous avez dans votre main.
//
erreur soin(unite cible)
{
  return api->soin(cible);
}

///
// Utilise une carte � D�guisement � que vous avez dans votre main.
//
erreur deguisement(unite cible, type_unite nouveau_type)
{
  return api->deguisement(cible, nouveau_type);
}

///
// Utilise une carte � Banza� � que vous avez dans votre main.
//
erreur banzai(unite cible)
{
  return api->banzai(cible);
}

///
// Utilise une carte � Pacifisme � que vous avez dans votre main.
//
erreur pacifisme()
{
  return api->pacifisme();
}

///
// D�place une unit� vers une position � port�e.
//
erreur deplacer(unite cible, position pos)
{
  return api->deplacer(cible, pos);
}

///
// Attaque une autre unit�.
//
erreur attaquer(unite attaquant, unite cible)
{
  return api->attaquer(attaquant, cible);
}

///
// Fait apparaitre une unit� sur la case de spawn.
//
erreur spawn(type_unite quoi)
{
  return api->spawn(quoi);
}

///
// Annule l'effet de la derni�re action et remet le jeu dans l'�tat pr�c�dent.
//
bool annuler()
{
  return api->annuler();
}

