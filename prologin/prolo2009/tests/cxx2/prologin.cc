///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_cxx.rb
//

#include "prologin.hh"
#include <iostream>
#include <cassert>
using namespace std;
///
// Fonction appele au debut de la partie
//

int cur_lig;

void init_game()
{
  cur_lig = 0;
  // fonction a completer
}

///
// Fonction appele a chaque tour 1
//

void jouer()
{
  const int M = TAILLE_CARTE / 2;
  if (!mon_tour()) {
    assert(construire_route(M-1,M-1) == ACTION_INTERDITE);
  }

  // Champion qui fait l'hypothèse qu'il n'y a personne d'autre qui joue avec une map
  // avec les monuments dans l'ordre.
  if (!mon_tour()) return;
  cout << endl << "*********** numero tour : " << numero_tour() << " ***********" << endl;

  cur_lig += mon_tour();  
  construire_route(M, M + cur_lig);

  switch(numero_tour()) {    
  case 1:
    assert(construire_route(M, M) == CASE_OCCUPEE);
    assert(construire_route(M - 2, M) == NON_CONNEXE);
    assert(finances(0) == 10);
    break;
  case 2:
    assert(construire_route(M+1, M) == SUCCES);
    assert(construire_route(M+2, M) == SUCCES);
    assert(construire_route(M+3, M) == SUCCES);
    assert(finances(0) == 8);
    assert(cout_achat_route() == 0);
    assert(cout_achat_maison() == 0);
    assert(construire_maison(M+1,M-1) == SUCCES);
    assert(finances(0) == 8);
    assert(cout_achat_maison() == 0);
    assert(score(0) == 1);
    assert(construire_maison(M-1,M-1) == SUCCES);
    assert(cout_achat_route() == 1);
    assert(construire_maison(0,0) == NON_CONNEXE);
    assert(score(0) == 2);
    assert(finances(0) == 8);
    break;
  case 3:
    assert(appartenance(M-1,M-1) == 0);
    assert(score(0) == 10);
    // building an house on a cell that has a positive value.
    assert(construire_maison(M-1,M) == SUCCES);
    assert(score(0) == 15);
    assert(finances(0) == 7);
    break;
    // during turn 4, the other player verifies he has the good data.
  case 5:
    // plusieurs actions sur la même case:
    assert(construire_maison(M-1, M+2) == SUCCES);
    assert(vendre_maison(M-1, M+2) == SUCCES);
    assert(finances(0) == 8);
    assert(appartenance(M-1, M+2) == MAIRIE);
    assert(type_case(M-1, M+2) == MAISON);
    assert(detruire_maison(M-1, M+2) == SUCCES);
    assert(finances(0) == 7);
    assert(construire_route(M-1, M+2) == SUCCES);
    assert(finances(0) == 6);
    assert(vendre_maison(M, M-2) != SUCCES);
    break;
    // turn 6: other player verify that he received only the last order (road)
  case 7:
    assert(finances(0) == 6);
    for (int i = 0 ; i < 5 ; i++)
      assert(reserver_case(M-1,M+3+i) == SUCCES);
    assert(reserver_case(M-1, M+3) == CASE_OCCUPEE);
    assert(reserver_case(M,M) == CASE_OCCUPEE);
    assert(finances(0) == 1);
    assert(construire_maison(M-1, M+5) == SUCCES);
    assert(finances(0) == 1);
    break;
  case 8:
    assert(construire_maison(M-1,M+7) == SUCCES);
    assert(finances(0) == 3);
    assert(vendre_maison(M-1,M+1) != SUCCES);
    assert(appartenance(M-1,M+1) == MAIRIE);
    break;
  case 12:
    {
      int d1 = ::distance(M + 1, M + 4, M + 1, M + 6);
      assert(d1 == ::distance(M + 1, M + 6, M + 1, M + 4));
      assert(d1 == 2);
    }
    break;
  case 14:
    {
      int d1 = ::distance(M + 1, M + 4, M + 1, M + 6);
      assert(d1 == ::distance(M + 1, M + 6, M + 1, M + 4));
      assert(d1 == 4); // cell M+1,M+5 is reserved by another player.
    }
    break;    
  }
}

///
// Fonction appele a chaque tour 2
//
void enchere()
{
  const int M = TAILLE_CARTE / 2;
  switch(numero_tour()) {
  case 1:
    assert(construire_maison(M-1,M) == ACTION_INTERDITE);
    break;
  case 2:
    assert(finances(0) == 8);
    assert(encherir(9) == FINANCES_DEPASSEES);
    encherir(1);
    assert(finances(0) == 8);
    break;
  case 7:
    cout << finances(0) << endl;
    assert(finances(0) == 1);
    assert(vendre_maison(M-1,M) == SUCCES);
    assert(finances(0) == 6);
    assert(encherir(3) == SUCCES);
    break;
  case 10:
    assert(encherir(1) == SUCCES);
    assert(encherir(1) == ACTION_INTERDITE);
    break;
  case 12:
    assert(score(1) == 0 || score(2) == 0);
    break;
  case MAX_MONUMENTS+1:
      assert(0); // check that there are less than MAX_MONUMENTS + 1 auction turns.
    break;
  }
}

///
// Fonction appele a chaque tour 3
//
void placement()
{
  const int M = TAILLE_CARTE / 2;
  switch(numero_tour()) {
  case 2:
    assert(finances(0) == 7);
    if (mon_tour()) {
      assert(construire_monument(M,M-1) == SUCCES);
      int po = portee_monument(monument_en_cours());
      int pr = prestige_monument(monument_en_cours());
      cout << "portee / prestige monument : " << po << "/"
	   << pr <<", id : " << monument_en_cours() << endl;
      assert(po == 1);
      assert(pr == 4);
      assert(score(0) == 10);
    }
    break;
  case 10:
    if (mon_tour()) {
      assert(score(0) == 12);
      assert(construire_monument(M-1,M+1) == SUCCES);
      assert(portee_monument(monument_en_cours()) == 3);
      assert(score(0) == 14);
    }
  }


  if (mon_tour()) {
    cout << "Won auction" << endl;
  }
}

