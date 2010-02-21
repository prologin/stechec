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

///
// Retourne un bool�en repr�sentant si la case existe. On consid�re qu'une case existe si ses coordonn�es sont dans le terrain (on exclut dont les coordonn�es n�gatives, inf�rieures � la coordonn�e minimale et sup�rieures � la coordonn�e maximale donn�s par la fonction infos_carte.
//
bool case_existe(position pos)
{
  return api->case_existe(pos);
}

///
// Prend un tableau en param
//
void test_tableau(std::vector<int> test, std::vector<position> test2, std::vector<teststruct> test3)
{
  api->test_tableau(test, test2, test3);
}

