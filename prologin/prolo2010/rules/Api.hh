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

  Api(GameData* gameData, Client* c) :
    StechecApi(gameData, c)
  { }

  virtual ~Api() { }

protected:
  virtual void teamSwitched();

public:
  // Internal API
  bool retirer_ko(unite u);

public:

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
   erreur soin(unite cible);
///
// Utilise une carte « Déguisement » que vous avez dans votre main.
//
   erreur deguisement(unite cible, type_unite nouveau_type);
///
// Utilise une carte « Banzaï » que vous avez dans votre main.
//
   erreur banzai(unite cible);
///
// Utilise une carte « Pacifisme » que vous avez dans votre main.
//
   erreur pacifisme();
///
// Déplace une unité vers une position à portée.
//
   erreur deplacer(unite cible, position pos);
///
// Attaque une autre unité.
//
   erreur attaquer(unite attaquant, unite cible);
///
// Fait apparaitre une unité sur la case de spawn.
//
   erreur spawn(type_unite quoi);
///
// Annule l'effet de la dernière action et remet le jeu dans l'état précédent.
//
   bool annuler();
};


#endif /* !API_HH_ */
