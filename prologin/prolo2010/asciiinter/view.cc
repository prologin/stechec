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
#include <string>
#include <iostream>
#include <stdio.h>

#include "view.hh"

#include <assert.h>

static void show_carte(int number, std::string txt)
{
  std::string pluriel;
  pluriel = (number == 1) ? "" : "s";
  if (number)
    {
      std::cout << "vous avez " << number
		<< " carte" << pluriel
		<< "de " << txt << "\n";
    }
}

static void show_cartes(cartes c)
{
  show_carte(c.soin, "soin");
  show_carte(c.deguisement, "deguisement");
  show_carte(c.pacifisme, "pacifisme");
  show_carte(c.banzai, "banzai");

}

static void show_toon(unite u){
  std::string color = (u.ennemi)?"\e[031m":"\e[034m";
  std::string name;
  std::string end_color = "\e[0m";
  switch (u.type_unite_actuel) {
  case PERROQUET: name = "p"; break;
  case SINGE: name = "s"; break;
  case CHAT: name = "c"; break;
  case KANGOUROU: name = "k"; break;
  default: assert(false);
  }
  if (u.ko != -1)
    std::cout << "|" << color << u.ko <<  name << u.pa << end_color;
  else
    std::cout << "|" << color << " " <<  name << u.pa << end_color;
}

static void flush(){
  std::cout << std::flush;;
  fflush(stdout);
}

static void show_map(std::vector<unite> units, taille_terrain size)
{
  int s = units.size();
  std::cout << "\e[2J";
  for (int i=0; i < s ; i ++){
    for (int j = i + 1; j < s; j ++){
      if (units[i].pos.y > units[j].pos.y ||
	  (units[i].pos.y == units[j].pos.y && units[i].pos.x > units[j].pos.x)){
	unite swap = units[i];
	units[i] = units[j];
	units[j] = swap;
      }
    }
  }
  flush();
  bool show = true;
  position p = { size.min_coord, size.min_coord };
  for (int i = size.min_coord; i <= size.max_coord; i ++){
    printf("|%3d", i);
  }
  flush();
  std::cout << "\n";
  if (s == 0)
    {
      std::cout << "no unit\n";
      return;
    }
  for (int i = 0; i < s ; i ++){
    unite u = units[i];
    while(p.x != u.pos.x || p.y != u.pos.y){
      if (show) std::cout << "|   ";
      flush();
      p.x ++;
      if (p.x == size.max_coord + 1){
	p.x = size.min_coord;
	printf("%3d\n", p.y);
	p.y ++;
      }
      flush();
      show = true;
    }
    show_toon(u);
    show = false;
    flush();
  }
  while(p.x <= size.min_coord || p.y <= size.max_coord){
    if (show) std::cout << "|   ";
    p.x ++;
    flush();
    if (p.x == size.max_coord + 1){
      p.x = size.min_coord;
      printf("%3d\n", p.y);
      p.y ++;
    }
    show = true;
  }
  flush();
  std::cout << "\n";
}

void view(void)
{
  std::cout << "tour actuel : " << tour_actuel() << "_n";
  cartes c = mes_cartes();
  show_cartes(c);
  taille_terrain size = taille_terrain_actuelle();
  std::vector<unite> u = unites();
  show_map(u, size);
}
