/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2011 Prologin
*/

#include <stdlib.h>

#include "Api.hh"

// global used in interface.cc
Api* api;


Api::Api(GameData* gameData, Client* c) : StechecApi(gameData, c)
{
  api = this;
}

///
// Retourne le num�ro de votre �quipe
//
int Api::mon_equipe()
{
  // TODO
  abort();
}

///
// Retourne les scores de chaque �quipe
//
std::vector<int> Api::scores()
{
  // TODO
  abort();
}

///
// Retourne le nombre d'�quipes sur le terrain
//
int Api::nombre_equipes()
{
  // TODO
  abort();
}

///
// Retourne le num�ro du tour actuel
//
int Api::tour_actuel()
{
  // TODO
  abort();
}

///
// Retourne la liste des sources d'�nergie
//
std::vector<source_energie> Api::sources_energie()
{
  // TODO
  abort();
}

///
// Retourne la liste des tra�n�es de moto
//
std::vector<trainee_moto> Api::trainees_moto()
{
  // TODO
  abort();
}

///
// Retourne le type d'une case
//
type_case Api::regarder_type_case(position pos)
{
  // TODO
  abort();
}

///
// Retourne le type de bonus d'une case
//
bonus Api::regarder_type_bonus(position pos)
{
  // TODO
  abort();
}

///
// Retourne la liste des bonus d'une �quipe
//
std::vector<bonus> Api::regarder_bonus(int equipe)
{
  // TODO
  abort();
}

///
// D�place une moto
//
erreur Api::deplacer(int id, position de, position vers)
{
  // TODO
  abort();
}

///
// Coupe une tra�n�e de moto en deux nouvelles tra�n�es. � entre � et � et � doivent �tre deux positions adjacentes occup�es par une m�me tra�n�e de moto.
//
erreur Api::couper_trainee_moto(int id, position entre, position et)
{
  // TODO
  abort();
}

///
// Annuler l'action pr�c�dente
//
erreur Api::cancel()
{
  // TODO
  abort();
}

///
// Enrouler la tra�n�e de moto en un point
//
erreur Api::enrouler(int id, position point)
{
  // TODO
  abort();
}

///
// R�g�n�re une source d'�nergie � son maximal
//
erreur Api::regenerer_source_energie(int id)
{
  // TODO
  abort();
}

///
// Allonge le tour en rajoutant des points d'action
//
erreur Api::allonger_pa()
{
  // TODO
  abort();
}

///
// Allonge une tra�n�e de moto. L'allongement se fera aux prochains d�placements. La longueur du prolongement doit �tre comprise entre 0 et MAX_ALLONGEMENT (inclus).
//
erreur Api::agrandir_trainee_moto(int id, int longueur)
{
  // TODO
  abort();
}

///
// Pose un point de croisement sur une case du terrain. La case doit ne pas d�j� �tre un point de croisement.
//
erreur Api::poser_point_croisement(position point)
{
  // TODO
  abort();
}

///
// Fusionner deux tra�n�es de moto. Les deux doivent appartenir � la m�me �quipe, mais doivent �tre deux tra�n�es distinctes. � pos1 � et � pos2 � doivent �tre adjacentes et occup�es respectivement par � id1 � et � id2 �.
//
erreur Api::fusionner(int id1, position pos1, int id2, position pos2)
{
  // TODO
  abort();
}

///
// Affiche le contenu d'une valeur de type erreur
//

///
// Affiche le contenu d'une valeur de type type_case
//

///
// Affiche le contenu d'une valeur de type bonus
//

///
// Affiche le contenu d'une valeur de type position
//

///
// Affiche le contenu d'une valeur de type source_energie
//

///
// Affiche le contenu d'une valeur de type trainee_moto
//



void Api::teamSwitched()
{
}
