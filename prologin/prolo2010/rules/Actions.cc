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

#include "Actions.hh"


Actions::Actions(){

}

void Action::soin(int i){
  type = e_soin;
  i1 = i;
}

void Action::deguisement(int indice_unite, type_unite t){
  type = e_deguisement;
  i1 = indice_unite;
  tu = t;  
}
void Action::banzaii(int indice_unite){
  type = e_banzaii;
  i1 = indice_unite;
}
void Action::pacifisme(){
  type = e_pacifisme;
}
void Action::deplacer(int indice_unite, position pos, int d){
  type = e_deplacer;
  i1 = indice_unite;
  p = pos;
  d = d;
}
void Action::attaquer(int indice_attaquant, int indice_cible){
  type = e_attaquer;
  i1 = indice_attaquant;
  i2 = indice_cible;
}
void Action::spawn(type_unite quoi){
  type = e_spawn;
  tu = quoi;
}
