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

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <prologin.hh>

extern "C" int api_state_is_end(void*);
extern "C" int api_state_is_playturn(void*);

extern "C" int client_cx_process(void*);
extern "C" int client_cx_set_ready(void*);

void afficher(cartes c){
  std::cout
    << "{potion:" << c.potion << ", "
    << "banzai:" << c.banzai << ", "
    << "deguisement:" << c.deguisement << ", "
    << "pacifisme:" << c.pacifisme
    << "}";
}

void afficher(position p){
  std::cout
    << "{"
    << "x:" << p.x << ", "
    << "y:" << p.y
    <<"}";
}

void afficher(type_unite tu){
  std::cout << "{";
  switch (tu){
  case PERROQUET: std::cout << "perroquet"; break;
  case SINGE: std::cout << "singe" ; break;
  case CHAT: std::cout << "chat" ; break;
  case KANGOUROU: std::cout << "kangourou"; break;
  }
  std::cout << ":{}}";
}

void afficher(unite u){
  std::cout
    << "{"
    << "ennemi:" << u.ennemi << ", "
    << "ko:" << u.ko << ", "
    << "pa:" << u.pa << ", "
    << "attaques:" << u.attaques << ", "
    << "pos:" ;
  afficher(u.pos);
  std::cout << ", " << "type_unite_actuel:";
  afficher(u.type_unite_actuel);
  std::cout << ", " << "vrai_type_unite:";
  afficher(u.type_unite_actuel);
  std::cout
    << "}";
}

void afficher(std::vector<unite> u){
  std::cout << "[";
  afficher(u[0]);
  for (int i = 1; i < u.size(); i++){
    std::cout << ", ";
    afficher(u[i]);
  }
  std::cout << "]";
}

void afficher(taille_terrain tt){
  std::cout
    << "{"
    << "taille:" << tt.taille << ", "
    << "min_coord:" << tt.min_coord << ", "
    << "max_coord:" << tt.max_coord
    <<"}";
}

void dump_current_state()
{
  std::vector<unite> u = unites();
  int t = tour_actuel();
  bool ennemi = t % 2 == 0;
  if (ennemi)
    for (int i = 0; i < u.size(); i++)
      u[i].ennemi = ! u[i].ennemi;
  taille_terrain taille = taille_terrain_actuelle();
  cartes c = mes_cartes();
  cartes c2;
  c2.potion = MAX_CARTES - c.potion;
  c2.deguisement = MAX_CARTES - c.deguisement;
  c2.banzai = MAX_CARTES - c.banzai;
  c2.pacifisme = MAX_CARTES - c.pacifisme;
  if (ennemi){
    cartes c3 = c;
    c = c2;
    c2 = c3;
  }
  std::cout
    << "{"
    << "cards_white:";
  afficher(c);
  std::cout
    << ", " << "cards_black:";
  afficher(c2);
  std::cout
    << ", " << "taille_terrain:";
  afficher(taille);
  std::cout
    << ", " << "unites:";
  afficher(u);
  std::cout
    << "}"
    << std::endl;;
}

extern "C" int run(void* foo, void* api, void* client_cx)
{
    int turn = -1;
    struct timespec st = { 0, 1000 };
    printf("%u\n", time(NULL));

    while (!api_state_is_end(api))
    {
        nanosleep(&st, NULL);
        if (tour_actuel() != turn)
        {
            turn = tour_actuel();
            dump_current_state();
        }

        client_cx_set_ready(client_cx);
        while (client_cx_process(client_cx));
    }
    return 0;
}
