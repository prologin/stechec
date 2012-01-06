/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2011 Prologin
*/

#include "Utils.hh"

bool operator==(position p1, position p2){
  return p1.x == p2.x && p1.y == p2.y;
}

bool operator!=(position p1, position p2)
{
    return (p1.x != p2.x) || (p1.y != p2.y);
}

bool a_cote(position p1, position p2){
  return
    (p1.x == p2.x &&
     (p1.y == p2.y - 1 || p1.y == p2.y + 1)
     )
    ||
    (p1.y == p2.y &&
     (p1.x == p2.x - 1 || p1.x == p2.x + 1)
     )
    ;
}

bool position_invalide(int x, int y){
  return x < 0 ||
    y < 0 ||
    x >= TAILLE_TERRAIN ||
    y >= TAILLE_TERRAIN ;
}

bool position_invalide(const position& p)
{
  return p.x < 0 ||
    p.y < 0 ||
    p.x >= TAILLE_TERRAIN ||
    p.y >= TAILLE_TERRAIN ;
}

pair_position pos_to_pair(const position& p)
{
    pair_position p_;

    p_.first = p.x;
    p_.second = p.y;
    return p_;
}
