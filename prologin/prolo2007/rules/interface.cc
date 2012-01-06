/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
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

int nombre_leucocyte ()
{
  return api->nombre_leucocyte();
}

int taille_equipe(int team_number)
{
  return api->taille_equipe(team_number);
}

int mon_equipe()
{
  return api->mon_equipe();
}

int mon_identifiant()
{
  return api->mon_identifiant();
}


int taille_corps_x()
{
  return api->taille_corps_x();
}

int taille_corps_y()
{
  return api->taille_corps_y();
}

int bacteries_presentes(int x, int y)
{
  return api->bacteries_presentes(x, y);
}

int regarde(int x, int y)
{
  return api->regarde(x, y);
}

int visible(int x, int y)
{
  return api->visible(x, y);
}

int score(int id)
{
  return api->score(id);
}

int battement_courant()
{
  return api->battement_courant();
}

int leucocyte_visible(int id)
{
  return api->leucocyte_visible(id);
}

int identifiant_leucocyte(int x, int y)
{
  return api->identifiant_leucocyte(x, y);
}

int equipe(int id)
{
  return api->equipe(id);
}

int position_leucocyte_x(int id)
{
  return api->position_leucocyte_x(id);
}

int position_leucocyte_y(int id)
{
  return api->position_leucocyte_y(id);
}

int etat_leucocyte(int id)
{
  return api->etat_leucocyte(id);
}

int etat_cellule(int x, int y)
{
  return api->etat_cellule(x, y);
}

int connait_type(int x, int y)
{
  return api->connait_type(x, y);
}

int virus_type(int x, int y)
{
  return api->virus_type(x, y);
}

int definit_competence(int rapidite, int anticorps, int messages, int vision)
{
  return api->definit_competence(rapidite, anticorps, messages, vision);
}

int deplace_leucocyte(int x, int y)
{
  return api->deplace_leucocyte(x, y);
}

int phagocyte(int x, int y)
{
  return api->phagocyte(x, y);
}

int emet_anticorps()
{
  return api->emet_anticorps();
}

int transmettre_message(int message_arg1, int message_arg2, int message_arg3, int message_arg4)
{
  return api->transmettre_message(message_arg1, message_arg2, message_arg3, message_arg4);
}

int recevoir_prochain_message()
{
  return api->recevoir_prochain_message();
}

int corps_message(int message_id, int argument_id)
{
  return api->corps_message(message_id, argument_id);
}


}
