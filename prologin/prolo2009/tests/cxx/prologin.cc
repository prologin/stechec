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
void init_game()
{
  // fonction a completer
}

///
// Fonction appele a chaque tour 1
//
void jouer()
{
  const int M = TAILLE_CARTE / 2;
  char output[100] = {0};
  output[ROUTE] = '|';
  output[MONUMENT] = 'M';
  output[RESERVATION] = 'R';
  output[VIDE] = '.';

  cout << "jouer :: mon_tour() : " << mon_tour() << " num " << numero_tour() << endl;
  for (int i = 40 ; i < 60 ; i++) {
    for (int j = 40 ; j < 60 ; j++) {
      if (i == M && j == M) {
	cout << "*";
	continue;
      }
	
      int k = type_case(j, i);
      if (k != MAISON)
	cout << output[k];
      else {
	int a = appartenance(j, i);
	cout << a;
      }
    }
    cout << endl;
  }

  for (int i = 40 ; i < 60 ; i++) {
    for (int j = 40 ; j < 60 ; j++) {
      int d = ::distance(M+1, M+1, j, i);
      if (d < 10) cout << 0;
      cout << d << " ";
    }
    cout << endl;
  }

  // play:
  if (!mon_tour())
    return;
  switch(numero_tour()) {
  case 1:
    assert(monument_en_cours() == PAS_DE_MONUMENT);
    break;
  case 3:
    assert(montant_encheres(1) == 1 || montant_encheres(2) == 1);
    assert(montant_encheres(0) == 0);
    assert(construire_route(M+2,M-1) == SUCCES);
    assert(construire_route(M+2,M-2) == SUCCES);
    assert(construire_route(M+1,M-2) == SUCCES);
    assert(score(0) == 0);
    assert(construire_maison(M,M-2) == SUCCES);
    assert(score(0) == 5);
    assert(finances(0) == 9);
    break;
  case 4:
    assert(montant_encheres(2) == 0);
    assert(montant_encheres(1) == 0);
    assert(montant_encheres(0) == 0);
    // vérification de la transmission des ordres de l'autre joueur.
    assert(score(1) == 15 || score(2) == 15);
    assert(finances(1) == 7 || finances(2) == 7);
    int a;
    a = appartenance(M-1,M);
    assert(a==1 || a==2);
    assert(type_case(M,M-1) == MONUMENT);
    assert(type_case(M-1,M-1) == MAISON);
    a = appartenance(M-1,M-1);
    assert(a==1 || a==2);
    break;
 case 6:
   assert(type_case(M-1,M+2) == ROUTE);   
   break;
  case 8:
    assert(montant_encheres(1) == 3 || montant_encheres(2) == 3);
    assert(montant_encheres(0) == 0);
    assert(construire_route(M-1,M+6) == CASE_OCCUPEE);
    assert(vendre_maison(M-1, M+6) != SUCCES);
    assert(finances(1) == 3 || finances(2) == 3);
    assert(type_case(M-1,M+5) == MAISON);
    assert(type_case(M-1,M+4) == RESERVATION);
    assert(vendre_maison(M-1,M+5) != SUCCES);
    assert(vendre_maison(M-1,M+1) != SUCCES);
    assert(appartenance(M-1,M+1) == MAIRIE);
    break;
  case 11:
    assert(montant_encheres(1) == 1 || montant_encheres(2) == 1);
    assert(montant_encheres(0) == 0);
    assert(score(0) == 6);
    assert(score(1) == 14 || score(2) == 14);
    break;
  case 12: // vérification des scores lors des destructions
    assert(finances(0) == 9);
    // this house is within reach the monument at x=M,y=M-1 (5,1) and x=M-1,y=M+1 (1,3)
    // the house has thus value 5 + 1.
    detruire_maison(M, M - 2);
    assert(score(0) == 0);
    assert(finances(0) == 4);
    break;
  case 13:
    construire_maison(M, M - 2);
    assert(score(0) == 6);
    assert(finances(0) == 4);
    {
      int d1 = ::distance(M + 1, M + 4, M + 1, M + 6);
      assert(d1 == ::distance(M + 1, M + 6, M + 1, M + 4));
      assert(d1 == 2);
    }
    assert(construire_maison(M+1, M+5) == SUCCES);
    {
      int d1 = ::distance(M + 1, M + 4, M + 1, M + 6);
      assert(d1 == ::distance(M + 1, M + 6, M + 1, M + 4));
      assert(d1 == 4);
    }
    assert(detruire_maison(M+1,M+5) == SUCCES);
    {
      int d1 = ::distance(M + 1, M + 4, M + 1, M + 6);
      assert(d1 == ::distance(M + 1, M + 6, M + 1, M + 4));
      assert(d1 == 2);
    }
    // The cells we booked can be passed through.
    assert(reserver_case(M+1, M+5) == SUCCES);
    {
#undef TROP_LOIN // todo, synchronize this with the server.
#define TROP_LOIN 50
      int d1 = ::distance(M + 1, M + 4, M + 1, M + 6);
      assert(d1 == ::distance(M + 1, M + 6, M + 1, M + 4));
      assert(d1 == 2);
      int d2 = ::distance(M + 1, M + 4, M + 1, M + 5);
      assert(d2 == 1);
      int d3 = ::distance(0,0,25,25);
      assert(d3 == TROP_LOIN);
      int d4 = ::distance(M,M,M,M);
      assert(d4 == 0); // we should be able to pass threw roads
      int d7 = ::distance(M-1,M,M-1,M);
      assert(d7 == TROP_LOIN);
      int d5 = ::distance(M+1,M+1,M+3,M);
      assert(d5 == 3);
      int d8 = ::distance(M, M-1, M, M-4);
      assert(d8 == TROP_LOIN);
      int d6 = ::distance(M+1,M+1,M+7,M+1);
      assert(d6 == 6);
    }
    break;
  case 14:
    assert(score(0) + finances(0) == 7);
    break;
  }
}

///
// Fonction appele a chaque tour 2
//
void enchere()
{
  assert(mon_tour());
  cout << "enchere :: mon_tour() : " << mon_tour() << endl;
  switch(numero_tour()) {
  case 7:
    assert(finances(1) == 1 || finances(2) == 1);
    break;
  }
}

///
// Fonction appele a chaque tour 3
//
void placement()
{
  cout << "placement :: mon_tour() : " << mon_tour() << endl;
  if (mon_tour())
    cout << "Won auction " << endl;
  // fonction a completer
}

