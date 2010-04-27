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
# include "Actions.hh"

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
  // Internal API
  bool need_retirer_ko();
  bool retirer_ko(unite u);
  bool mon_tour();
  void sendActions();
public:

  int nombre_pc();
  int nombre_unites_spawnees(bool e);

///
// Renvoie le num�ro du tour actuel.
//
   int tour_actuel();
///
// Renvoie la position du spawn (ennemi ou non).
//
   position pos_spawn(bool ennemi);
///
// Renvoie les caract�ristiques d'un type d'unit�.
//
   caracs caracteristiques(type_unite tu);
///
// Retourne une structure \texttt{cartes} contenant les informations sur les cartes que vous avez en main.
//
   cartes mes_cartes();
///
// Retourne la liste des unit�s actuellement en jeu.
//
   std::vector<unite> unites();
///
// Retourne la taille actuelle du terrain et les coordonn�es min/max dans une structure \texttt{taille_terrain}.
//
   taille_terrain taille_terrain_actuelle();
///
// Utilise une carte � Quoi d'neuf docteur ? � que vous avez dans votre main.
//
   erreur soin(position cible);
///
// Utilise une carte � D�guisement � que vous avez dans votre main.
//
   erreur deguisement(position cible, type_unite nouveau_type);
///
// Utilise une carte � Banza� � que vous avez dans votre main.
//
   erreur banzai(position cible);
///
// Utilise une carte � Pacifisme � que vous avez dans votre main.
//
   erreur pacifisme();
///
// D�place une unit� vers une position � port�e.
//
   erreur deplacer(position cible, position pos);
///
// Rel�ve une unit� n'ayant plus de tours KO.
//
   erreur relever(position cible);
///
// Attaque une autre unit�.
//
   erreur attaquer(position attaquant, position cible);
///
// Fait apparaitre une unit� sur la case de spawn.
//
   erreur spawn(type_unite quoi);
///
// Annule l'effet de la derni�re action et remet le jeu dans l'�tat pr�c�dent.
//
   bool annuler();

};

#define ASSERT(c, err) \
  if (!(c)) return (err)


#endif /* !API_HH_ */
