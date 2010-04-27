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
// Renvoie le numéro du tour actuel.
//
   int tour_actuel();
///
// Renvoie la position du spawn (ennemi ou non).
//
   position pos_spawn(bool ennemi);
///
// Renvoie les caractéristiques d'un type d'unité.
//
   caracs caracteristiques(type_unite tu);
///
// Retourne une structure \texttt{cartes} contenant les informations sur les cartes que vous avez en main.
//
   cartes mes_cartes();
///
// Retourne la liste des unités actuellement en jeu.
//
   std::vector<unite> unites();
///
// Retourne la taille actuelle du terrain et les coordonnées min/max dans une structure \texttt{taille_terrain}.
//
   taille_terrain taille_terrain_actuelle();
///
// Utilise une carte « Quoi d'neuf docteur ? » que vous avez dans votre main.
//
   erreur soin(position cible);
///
// Utilise une carte « Déguisement » que vous avez dans votre main.
//
   erreur deguisement(position cible, type_unite nouveau_type);
///
// Utilise une carte « Banzaï » que vous avez dans votre main.
//
   erreur banzai(position cible);
///
// Utilise une carte « Pacifisme » que vous avez dans votre main.
//
   erreur pacifisme();
///
// Déplace une unité vers une position à portée.
//
   erreur deplacer(position cible, position pos);
///
// Relève une unité n'ayant plus de tours KO.
//
   erreur relever(position cible);
///
// Attaque une autre unité.
//
   erreur attaquer(position attaquant, position cible);
///
// Fait apparaitre une unité sur la case de spawn.
//
   erreur spawn(type_unite quoi);
///
// Annule l'effet de la dernière action et remet le jeu dans l'état précédent.
//
   bool annuler();

};

#define ASSERT(c, err) \
  if (!(c)) return (err)


#endif /* !API_HH_ */
