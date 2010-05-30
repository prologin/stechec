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

  int last_order_id;

   bool mon_tour();

///
// Renvoie le numero de votre equipe.
//
   int mon_equipe();
///
// Renvoie le score d'une equipe.
//
   int score(int team);
///
// Renvoie le nombre d'equipes sur la map
//
   int nombre_equipes();
///
// Renvoie le numéro du tour actuel.
//
   int tour_actuel();
///
// Renvoie les pieces qui sont sur la map
//
   std::vector<piece> pieces_en_jeu();
///
// Renvoie le 10 prochaines pieces a vennir
//
   std::vector<piece> pieces_a_vennir();
///
// Retourne la liste des unités actuellement en jeu.
//
   std::vector<unite> unites();
///
// Déplace une unité vers une position à portée.
//
   erreur deplacer(position cible, position pos);
///
// Achete un objet
//
   erreur acheter_objet(position cible, type_objet objet);
///
// utilise un objet
//
   erreur utiliser(position attaquant, position cible);
///
// coupe la partie, cette action coute 100 d'or et met fin a la partie.
//
   erreur achever_la_partie();
///
// Affiche le contenu d'une valeur de type erreur
//
   void afficher_erreur(erreur v);
///
// Affiche le contenu d'une valeur de type type_objet
//
   void afficher_type_objet(type_objet v);
///
// Affiche le contenu d'une valeur de type position
//
   void afficher_position(position v);
///
// Affiche le contenu d'une valeur de type caracteristiques_objet
//
   void afficher_caracteristiques_objet(caracteristiques_objet v);
///
// Affiche le contenu d'une valeur de type unite
//
   void afficher_unite(unite v);
///
// Affiche le contenu d'une valeur de type piece
//
   void afficher_piece(piece v);
};


#endif /* !API_HH_ */
