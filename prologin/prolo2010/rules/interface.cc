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
extern "C" cartes api_mes_cartes()
{
  return api->mes_cartes();
}

///
// Retourne la liste des unités actuellement en jeu.
//
extern "C" std::vector<unite> api_unites()
{
  return api->unites();
}

///
// Retourne la taille actuelle du terrain et les coordonnées min/max dans une structure \texttt{taille_terrain}.
//
extern "C" taille_terrain api_taille_terrain_actuelle()
{
  return api->taille_terrain_actuelle();
}

///
// Utilise une carte « Quoi d'neuf docteur ? » que vous avez dans votre main.
//
extern "C" erreur api_soin(unite cible)
{
  return api->soin(cible);
}

///
// Utilise une carte « Déguisement » que vous avez dans votre main.
//
extern "C" erreur api_deguisement(unite cible, type_unite nouveau_type)
{
  return api->deguisement(cible, nouveau_type);
}

///
// Utilise une carte « Banzaï » que vous avez dans votre main.
//
extern "C" erreur api_banzai(unite cible)
{
  return api->banzai(cible);
}

///
// Utilise une carte « Pacifisme » que vous avez dans votre main.
//
extern "C" erreur api_pacifisme()
{
  return api->pacifisme();
}

///
// Déplace une unité vers une position à portée.
//
extern "C" erreur api_deplacer(unite cible, position pos)
{
  return api->deplacer(cible, pos);
}

///
// Attaque une autre unité.
//
extern "C" erreur api_attaquer(unite attaquant, unite cible)
{
  return api->attaquer(attaquant, cible);
}

///
// Fait apparaitre une unité sur la case de spawn.
//
extern "C" erreur api_spawn(type_unite quoi)
{
  return api->spawn(quoi);
}

///
// Annule l'effet de la dernière action et remet le jeu dans l'état précédent.
//
extern "C" bool api_annuler()
{
  return api->annuler();
}

