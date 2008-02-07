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

#include <stdlib.h>

#include "Api.hh"

// global used in interface.cc
Api* api;

Api::Api(GameData* gameData, Client* c)
  : StechecApi(gameData, c)
{
  api = this;
}

int Api::numero_tour()
{
  return 0;
}

int Api::nombre_tours()
{
  return 0;
}

bool Api::commence()
{
  return 0;
}

int Api::pos_x(int id)
{
  return 0;
}

int Api::pos_y(int id)
{
  return 0;
}

bool Api::porte_balle(int id)
{
  return 0;
}

int Api::distance(int x1, int y1, int x2, int y2)
{
  return 0;
}

int Api::type_case(int x, int y)
{
  return 0;
}

bool Api::balle(int x, int y)
{
  return 0;
}

int Api::deplacer(int id, int direction)
{
  return 0;
}

int Api::turbo(int id)
{
  return 0;
}

int Api::lacher_balle(int id)
{
  return 0;
}

int Api::prendre_balle(int id)
{
  return 0;
}

int Api::grapin(int id, int direction)
{
  return 0;
}

int Api::attendre(int id)
{
  return 0;
}


void Api::teamSwitched()
{
  // GUI stuff. don't care about it
}
