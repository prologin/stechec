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
** @author Maxime Audouin <maxime.audouin@mlstate.com>
*/

#include <prologin.hh>
#include "view.hh"
#include <string>
#include <cstdio>
#include <vector>
#include <cstring>
#include <assert.h>

///
// Fonction appell�e au d�but de la partie.
//
void init_game()
{
}

static const char * string_of_type(type_unite t){
  switch(t){
  case PERROQUET: return "perroquet";
  case SINGE: return "singe";
  case CHAT: return "chat";
  case KANGOUROU: return "kangourou";
  }
}

static type_unite unit_of_int(int i){
  switch(i){
  case 1: return PERROQUET;
  case 2: return SINGE;
  case 3: return CHAT;
  case 4: return KANGOUROU;
  default: assert(false);
  }
}

position retirer_ko()
{
  view();
  printf("retirer ko :\n");
  std::vector<unite> u = unites();
  for (int i = 0 ; i < u.size(); i++){
    if (u[i].ennemi){
      printf("indice=%d, type=%s, pa=%d, ko=%d, pos=(%d, %d)\n",
	     i, string_of_type(u[i].type_unite_actuel), u[i].pa,
	     u[i].ko, u[i].pos.x, u[i].pos.y);
    }
  }
  int n;
  bool bad;
  do{
    char buffer[80];
    fgets(buffer, 80, stdin);
    sscanf(buffer, "%d\n", &n);
    if (n < 0 || n >= u.size() || (! u[n].ennemi) ){
      printf("veuillez enlever un point ko a une unite ennemie\n");
      bad = true;
    }else{
      bad = false;
    }
  } while(bad);
  printf("fin retirer_ko;\n");
  return u[n].pos;
}

unite choisir_unite(position p){
  std::vector<unite> u = unites();
  for (int  i = 0; i < u.size(); i++){
    if (u[i].pos.x == p.x && u[i].pos.y == p.y)
      return u[i];
  }
  assert(false);
}

bool contient_unite(position p){
  std::vector<unite> u = unites();
  for (int  i = 0; i < u.size(); i++){
    if (u[i].pos.x == p.x && u[i].pos.y == p.y)
      return true;
  }
  return false;
}

void jouer()
{
  view();
  printf("jouer :\n");
  std::vector<unite> u = unites();
  for (int i = 0 ; i < u.size(); i++){
    if (!u[i].ennemi){
      printf("type=%s, pa=%d, ko=%d, pos=(%d, %d)\n",
	     string_of_type(u[i].type_unite_actuel), u[i].pa,
	     u[i].ko, u[i].pos.x, u[i].pos.y);
    }
  }
  int n;
  bool continue_;
  position from, to;
  do{
    char buffer[80];
    fgets(buffer, 80, stdin);
    if (sscanf(buffer, "from (%d, %d) to (%d, %d)\n", &from.x, &from.y, &to.x, &to.y) == 4){
      if (contient_unite(from)){
	afficher_erreur(deplacer(from, to));
      }else{
	printf("aucune unite ici !");
      }
    }else if (strcmp(buffer, "spawn\n") == 0){
	do{
	  printf("1 pour perroquet\n2 pour singe\n3 pour chat\n4 pour kangourou\n");
	  fgets(buffer, 80, stdin);
	  if (sscanf(buffer, "%d", &n) != 1) n = 0; // bad value
	} while (n < 1 || n > 4);
        afficher_erreur(renfort(unit_of_int(n)));
    }else if (strcmp(buffer, "end\n") == 0){
      break;
    }
  } while(true);
  printf("fin jouer;\n");
}

///
// Fonction appell�e � la fin de la partie.
//
void end_game()
{
  view();
}

