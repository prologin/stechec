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

  Api(GameData* gameData, Client* c);

  virtual ~Api() { }

protected:
  virtual void teamSwitched();

public:

///
// Retourne le numéro de votre équipe
//
   int mon_equipe();
///
// Retourne les scores de chaque équipe
//
   std::vector<int> scores();
///
// Retourne le nombre d'équipes sur le terrain
//
   int nombre_equipes();
///
// Retourne le numéro du tour actuel
//
   int tour_actuel();
///
// Retourne la liste des sources d'énergie
//
   std::vector<source_energie> sources_energie();
///
// Retourne la liste des traînées de moto
//
   std::vector<trainee_moto> trainees_moto();
///
// Retourne le type d'une case
//
   type_case regarder_type_case(position pos);
///
// Retourne le type de bonus d'une case
//
   bonus regarder_type_bonus(position pos);
///
// Retourne la liste des bonus d'une équipe
//
   std::vector<bonus> regarder_bonus(int equipe);
///
// Déplace une moto
//
   erreur deplacer(int id, position de, position vers);
///
// Coupe une traînée de moto en deux nouvelles traînées. « entre » et « et » doivent être deux positions adjacentes occupées par une même traînée de moto.
//
   erreur couper_trainee_moto(int id, position entre, position et);
///
// Annuler l'action précédente
//
   erreur cancel();
///
// Enrouler la traînée de moto en un point
//
   erreur enrouler(int id, position point);
///
// Régénère une source d'énergie à son maximal
//
   erreur regenerer_source_energie(int id);
///
// Allonge le tour en rajoutant des points d'action
//
   erreur allonger_pa();
///
// Allonge une traînée de moto. L'allongement se fera aux prochains déplacements. La longueur du prolongement doit être comprise entre 0 et MAX_ALLONGEMENT (inclus).
//
   erreur agrandir_trainee_moto(int id, int longueur);
///
// Pose un point de croisement sur une case du terrain. La case doit ne pas déjà être un point de croisement.
//
   erreur poser_point_croisement(position point);
///
// Fusionner deux traînées de moto. Les deux doivent appartenir à la même équipe, mais doivent être deux traînées distinctes. « pos1 » et « pos2 » doivent être adjacentes et occupées respectivement par « id1 » et « id2 ».
//
   erreur fusionner(int id1, position pos1, int id2, position pos2);
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

};


#endif /* !API_HH_ */
