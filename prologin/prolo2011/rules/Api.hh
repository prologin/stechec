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

  void sendActions();

///
// Retourne le num�ro de votre �quipe
//
   int mon_equipe();
///
// Retourne les scores de chaque �quipe
//
   std::vector<int> scores();
///
// Retourne le nombre d'�quipes sur le terrain
//
   int nombre_equipes();
///
// Retourne le num�ro du tour actuel
//
   int tour_actuel();
///
// Retourne la liste des sources d'�nergie
//
   std::vector<source_energie> sources_energie();
///
// Retourne la liste des tra�n�es de moto
//
   std::vector<trainee_moto> trainees_moto();
///
// Retourne le type d'une case
//
   type_case regarder_type_case(position pos);
///
// Retourne le type de bonus d'une case
//
   type_bonus regarder_type_bonus(position pos);
///
// Retourne la liste des bonus d'une �quipe
//
   std::vector<type_bonus> regarder_bonus(int equipe);
///
// D�place une moto
//
   erreur deplacer(int id, position de, position vers);
///
// Coupe une tra�n�e de moto en deux nouvelles tra�n�es. � entre � et � et � doivent �tre deux positions adjacentes occup�es par une m�me tra�n�e de moto.
//
   erreur couper_trainee_moto(int id, position entre, position et);
///
// Annuler l'action pr�c�dente
//
   erreur cancel();
///
// Enrouler la tra�n�e de moto en un point
//
   erreur enrouler(int id, position point);
///
// R�g�n�re une source d'�nergie � son maximal
//
   erreur regenerer_source_energie(int id);
///
// Allonge le tour en rajoutant des points d'action
//
   erreur allonger_pa();
///
// Allonge une tra�n�e de moto. L'allongement se fera aux prochains d�placements. La longueur du prolongement doit �tre comprise entre 0 et MAX_ALLONGEMENT (inclus).
//
   erreur agrandir_trainee_moto(int id, int longueur);
///
// Pose un point de croisement sur une case du terrain. La case doit ne pas d�j� �tre un point de croisement.
//
   erreur poser_point_croisement(position point);
///
// Fusionner deux tra�n�es de moto. Les deux doivent appartenir � la m�me �quipe, mais doivent �tre deux tra�n�es distinctes. � pos1 � et � pos2 � doivent �tre adjacentes et occup�es respectivement par � id1 � et � id2 �.
//
   erreur fusionner(int id1, position pos1, int id2, position pos2);

///
// GUI specific
// Renvoie la liste des actions effectuees
  std::vector<std::vector<int> > actions_effectuees();

  bool mon_tour();

};


#endif /* !API_HH_ */
