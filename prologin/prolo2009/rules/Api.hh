/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2005, 2006 Prologin
*/

#ifndef API_HH_
# define API_HH_

# include <Contest.hh>
# include "GameData.hh"

/*!
** Method of this call are called by the candidat, throught 'interface.cc'
**
** You can:
**  - access to the GameData class: _g
**  - send message to the server: void SendToServer(struct s_com&)
*/

#define CHECK_TEAM(t) \
  if (t < 0 || t >= g_->getNbTeam()) \
    return JOUEUR_INCORRECT;

#define CHECK_POS(x,y) \
  if (x < 0 || y < 0 || x >= TAILLE_CARTE || y >= TAILLE_CARTE) \
    return HORS_TERRAIN;

#define ASSERT_POS(x,y) \
  assert(x >= 0 && y >= 0 && x < TAILLE_CARTE && y < TAILLE_CARTE);

#define CHECK_MONUMENT(m) \
  if (m < 0 || m >= MAX_MONUMENTS) \
    return PAS_DE_MONUMENT;

class Api: public StechecApi          
{

public:

  Api(GameData* gameData, Client* c);

  virtual ~Api() { }

public:

   int type_case(int x, int y);
  // peut être négatif !
   int valeur_case(int x, int y);
  // retourne HORS_TERRAIN, MAIRIE (si route, vide..) ou le joueur si c'est une réservation ou une maison
   int appartenance(int x, int y);
   int type_monument(int x, int y);
   int portee_monument(int num_monument);
   int prestige_monument(int num_monument);
   int numero_tour();
   int commence();
  // Renvoie 0 si pas d'enchères déjà faites (premier tour..)
   int montant_encheres(int num_joueur);
   int score(int num_joueur);
   int finances(int num_joueur);
   int monument_en_cours();
   int distance(int x1, int y1, int x2, int y2);
   int route_possible(int x, int y);
   int construction_possible(int x, int y);
   int cout_achat_maison();
   int cout_achat_route();
   bool mon_tour();
   int construire_route(int x, int y);
   int construire_maison(int x, int y);
   int reserver_case(int x, int y);
   int detruire_maison(int x, int y);
   int vendre_maison(int x, int y);
   int encherir(int montant);
   int construire_monument(int x, int y);

protected:
  virtual void teamSwitched();

private:
  // translates the team id's from server side to champion side.
  // (i.e. the champion will always believe he has team id 0).
  // Does nothing if the team is >= NbTeam() (for instance, if team
  // represents the town council).
  int TeamFromServer(int team);

  // The same, from champion to server.
  // Asserts that team >= 0 and team < NbTeam.
  int TeamFromChampion(int team);

  bool check_pos(int x, int y);
  bool CanBuild(int x, int y);
};


#endif /* !API_HH_ */
