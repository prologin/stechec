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

#include "Api.hh"

// from Api.cc
extern Api* api;

extern "C" {

int equipes()
{
  return api->equipes();
}

int taille_equipe()
{
  return api->taille_equipe();
}

int mon_equipe()
{
  return api->mon_equipe();
}

int taille_ville_x()
{
  return api->taille_ville_x();
}

int taille_ville_y()
{
  return api->taille_ville_y();
}

int distance(int x_d, int y_d, int x_a, int y_a)
{
  return api->distance(x_d, y_d, x_a, y_a);
}

int meilleure_direction(int x_d, int y_d, int x_a, int y_a)
{
  return api->meilleure_direction(x_d, y_d, x_a, y_a);
}

int regarde(int x, int y)
{
  return api->regarde(x, y);
}

int visible(int x, int y)
{
  return api->visible(x, y);
}

int visible_depuis(int from_x, int from_y, int to_x, int to_y)
{
  return api->visible_depuis(from_x, from_y, to_x, to_y);
}

int score(int id)
{
  return api->score(id);
}

int current_date()
{
  return api->current_date();
}

int remaining_turn()
{
  return api->remaining_turn();
}

int time_remaining()
{
  return api->getTimeRemaining();
}

int extra_time_remaining()
{
  return api->getExtraTimeRemaining();
}

int almanach_visible ()
{
  return api->almanach_visible ();
}

int position_almanach_x()
{
  return api->position_almanach_x();
}

int position_almanach_y()
{
  return api->position_almanach_y();
}

int qui_possede_almanach()
{
  return api->qui_possede_almanach();
}

int joueur_possede_almanach()
{
  return api->joueur_possede_almanach();
}

int delorean_visible ()
{
  return api->delorean_visible ();
}

int position_delorean_x()
{
  return api->position_delorean_x();
}

int position_delorean_y()
{
  return api->position_delorean_y();
}

int autonomie_delorean()
{
  return api->autonomie_delorean();
}

int qui_possede_delorean()
{
  return api->qui_possede_delorean();
}

int joueur_possede_delorean()
{
  return api->joueur_possede_delorean();
}

int joueur_visible (int id)
{
  return api->joueur_visible (id);
}

int equipe(int id)
{
  return api->equipe(id);
}

int position_joueur_x(int id)
{
  return api->position_joueur_x(id);
}

int position_joueur_y(int id)
{
  return api->position_joueur_y(id);
}

int etat_joueur(int id)
{
  return api->etat_joueur(id);
}

int casino(int x, int y)
{
  return api->casino(x, y);
}

int casino_ouvert(int id)
{
  return api->casino_ouvert(id);
}

int jeu_du_casino(int id)
{
  return api->jeu_du_casino(id);
}

int jeux()
{
  return api->jeux();
}

int cagnotte(int id)
{
  return api->cagnotte(id);
}

int casino_du_jeu(int id)
{
  return api->casino_du_jeu(id);
}

int date_jeu(int id)
{
  return api->date_jeu(id);
}

int memorise_jeu(int id)
{
  return api->memorise_jeu(id);
}

int convecteur_temporel(int id, int date)
{
  return api->convecteur_temporel(id, date);
}

int deplace_joueur(int id, int x, int y)
{
  return api->deplace_joueur(id, x, y);
}


}
