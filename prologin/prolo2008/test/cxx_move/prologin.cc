
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
// Fonction appele a chaque tour
//

pair<int,int>pos[3];

void get_pos() {
    for (int i=0 ; i < 3 ; i++)
      pos[i] = make_pair(pos_x(i), pos_y(i));
}

void aff() {
  for (int i=0 ; i < taille_carte_y() ; i++) {
    for (int j=0 ; j < taille_carte_x() ; j++) {
      bool rob=false;
      for (int id=0 ; id < 6 ; id++)	
	if (pos_x(id)==j && pos_y(id) == i) {
	  rob = true;
	  cout<<id;
	}
      if (rob) continue;
      if (pomme(j,i)) {
	cout<<"B";
	continue;
      }
      switch (type_case(j,i)) {
      case OBSTACLE :
	cout<<"X";
	break;
      case TRANCHEE :
	cout<<"O";
	break;
      case NORMAL :
	cout<<".";
	break;
      }
    }
    cout<<endl;
  }
  cout<<endl;
  /*
  for (int i=0 ; i < taille_carte_y() ; i++) {
    for (int j=0 ; j < taille_carte_x() ; j++) {
      int d = distance(0,0,j,i,false);
      if (d != INFINI)
	cout<< d <<" ";
      else
	cout<<"X ";
    }
    cout<<endl;
  }
  */
}

void check_pos(pair<int,int> *pos) {
  for (int i=0 ; i < 3 ; i++) {	   
    cout<<pos_x(i)<<" "<<pos[i].first<<" "<<pos_y(i)<<" "<<pos[i].second<<endl;
    assert(pos_x(i) == pos[i].first && pos_y(i) == pos[i].second);
  }
}

void play_turn()
{

  /*
   * Champion de test pour move.map
   */

  aff();
#if 0
  cout<<distance(0,0,19,19,false)<<endl;
  assert( distance(0,0, 19,19,false) == INFINI );
  assert( distance(0,0, 19,20,false) == BAD_ARGUMENT );
  assert( distance(0,0, 0, 3,false) == 19);
#endif

  switch(numero_tour()) {
  case 1:
    deplacer(0,DROITE);
    deplacer(0,DROITE);
    deplacer(0,DROITE);
    grappin(1,GAUCHE);
    deplacer(1,DROITE);
    deplacer(1,DROITE);
    break;
  case 2:
    aff();
    assert( pos_x(0)== 5);
    assert( pos_x(1)== 6);
    assert(pos_x(2) == 7);
    deplacer(0,DROITE);
    deplacer(0,DROITE);
    deplacer(0,DROITE);
    break;	
  case 3:
    aff();
    assert(pos_x(0) == 8);
    assert(pos_x(1) == 9);
    assert(pos_x(2) == 10);
    assert(pos_y(0) == 0);
    grappin(1, GAUCHE);
    deplacer(1,BAS);
    deplacer(1,BAS);
    attendre(2);
    attendre(2);
    deplacer(2,GAUCHE);
    break;
  case 4:
    aff();
    assert(pos_x(0) == 9);
    assert(pos_x(1) == 9);
    assert(pos_x(2) == 9);
    assert(pos_y(0)==1 && pos_y(1) == 2 && pos_y(2) == 0);
    
    grappin(1,HAUT); //1 -> 0
    deplacer(2,BAS);
    attendre(1);
    deplacer(2,BAS);
    deplacer(1,BAS);
    break;
    
  case 5:
    aff();
    for (int i=0 ; i < 3 ; i++)
      assert(pos_x(i) == 9);
    assert(pos_y(2) == 2);
    assert(pos_y(0) == 4);
    assert(pos_y(1) == 5);
    deplacer(2,BAS);
    deplacer(2,GAUCHE);
    deplacer(2,BAS);
    break;

  case 6:
    assert(pos_x(2) == 8);
    assert(pos_y(2) == 4);
    deplacer(2,BAS);
    grappin(2,DROITE); //2->1
    attendre(0);
    deplacer(0,BAS);
    deplacer(2,BAS);
    break;

  case 7:
    assert(pos_y(0) == 5);
    assert(pos_y(1) == 6);
    assert(pos_y(2) == 6);
    grappin(1,GAUCHE);
    deplacer(0,BAS);
    deplacer(0,BAS);
    attendre(2);
    deplacer(2,DROITE);
    break;
  case 8:
    assert(pos_y(1) == 8 && pos_y(0) == 7 && pos_y(2) == 6);
    deplacer(2,BAS);
    deplacer(2,BAS);
    break;
  case 9:
    grappin(1, HAUT);
    grappin(0,HAUT);
    grappin(2,BAS);
    deplacer(1,BAS);
    deplacer(1,BAS);
    deplacer(2,GAUCHE);
    break;

  case 10 :
    assert(pos_y(1) == 10);
    assert(pos_y(2) == 8 && pos_y(0) == 8);
    assert(pos_x(0) == 9);

    grappin(0, GAUCHE);
    deplacer(0,BAS);
    attendre(2);
    deplacer(2,BAS);
    deplacer(2,BAS);
    break;

  case 11 :
    assert(pos_y(0) > 10 && pos_y(1) > 10);
    assert(pos_y(2) == 10);

    deplacer(2, BAS);
    grappin(0,HAUT);
    deplacer(0, BAS);
    
    //    grappin(
    break;
  case 12 :
    assert(pos_y(2) == 11);
    assert(pos_y(0) == 12);

    grappin(0,HAUT); 
    grappin(1,HAUT);
    deplacer(1,BAS);
    deplacer(1,BAS);
    break;

  case 13 :
    assert(pos_y(2) == 11);
    assert(pos_y(0) == 14);
    assert(pos_y(1) == 15);
    for (int i=0 ; i < 3 ; i++)
      pos[i] = make_pair(pos_x(i), pos_y(i));
    /*
     * Maintenant, test du trognon 
     * configuration :
     * 2
     * .
     * .
     * 0
     * 1
     */
    grappin(0,BAS);    
    trognon(2,BAS);
    deplacer(0,DROITE);
    pos[0].first++;
    pos[0].second++;
    break;
  case 14 :
    check_pos(pos);
    //test turbo :
    turbo(2);	 
    deplacer(2,BAS);
    deplacer(2,BAS);
    pos[2].second += 4;
    pos[1].second ++;
    break;
  case 15 :	
    check_pos(pos);
    turbo(0);
    turbo(2);
    deplacer(0,BAS);
    deplacer(0,BAS);
    deplacer(2,BAS);
    deplacer(2,BAS);
    grappin(1,HAUT);
    attendre(1);
    deplacer(1,DROITE);  
  break;
  case 16 :
    for (int i=0 ; i < 3 ; i++)
      assert(pos_y(i) == 19);
    for (int i=0 ; i < 3 ; i++)
      pos[i] = make_pair(pos_x(i), pos_y(i));
    turbo(2);
    deplacer(2,DROITE);
    deplacer(2,DROITE);
    grappin(1,GAUCHE);
    deplacer(1,DROITE);
    deplacer(1,DROITE);
    break;
  case 17:
    ramasser_pomme(0);
    lacher_pomme(0,GAUCHE);
    attendre(1);
    lacher_pomme(1,GAUCHE);
    deplacer(2,DROITE);
    deplacer(2,DROITE);
    break;
  case 18 :
    assert(porte_pomme(0) == 0 && porte_pomme(1)==false && porte_pomme(2)==true);
    ramasser_pomme(1);
    ramasser_pomme(0);
    lacher_pomme(0,GAUCHE); 
    break;
  case 19:
    for (int i=0 ; i<3;i++)
      assert(porte_pomme(i));
    turbo(0);
    deplacer(0,GAUCHE);
    deplacer(0,GAUCHE);
    break;
  case 20:
    turbo(0);
    deplacer(0,GAUCHE);
    deplacer(0,GAUCHE);
    break; 
  case 21:
    for (int i=0 ; i<3;i++) {
      deplacer(i,HAUT);
      deplacer(i,HAUT);
    }
    lacher_pomme(2,GAUCHE);
    lacher_pomme(1,HAUT);
    lacher_pomme(0,ICI);
    break;
  case 22:
    for (int i=0 ; i<3;i++)
      assert(!porte_pomme(i));
    for (int i=0 ; i<3;i++) {
      deplacer(i,HAUT);
      deplacer(i,HAUT);
    }
    break;
  case 23 :
    deplacer(1,BAS);
    ramasser_pomme(1);
    deplacer(1,HAUT);
    deplacer(0,DROITE);
    deplacer(0,DROITE);
    grappin(2,DROITE);
    grappin(1,GAUCHE);
    trognon(0,GAUCHE); //test du lacher de pomme lorsque l'on se prend un trognon
    break;
  case 24 :
    for (int i=0 ; i < 3 ; i++)
      pos[i] = make_pair(pos_x(i), pos_y(i));
    assert( pomme(10, 15) );
    deplacer(2,HAUT);
    deplacer(2,HAUT);
    deplacer(2,HAUT);
    deplacer(1,GAUCHE);
    deplacer(1,HAUT);
    break;
  case 25 :
    assert(pos_y(2) == pos[2].second-3);
    get_pos();
    deplacer(2,HAUT);     
    deplacer(2,HAUT);
    break;
  case 26:
    get_pos();
    attendre(1);
    assert(trognons_restants(1) == 2);
    trognon(1,HAUT);
    trognon(1,HAUT);
    break;
  case 27: 
    assert(pos_y(2) == pos[2].second-1); //un trognon ne fait pas sortir d'un trou
    get_pos();
    // et un hamster dans un trou peut-etre pousse par un trognon si ceci ne le fait pas sortir du trou
    assert(trognons_restants(1) == 0);
    deplacer(2,BAS);
    trognon(1,HAUT);
    break;
  case 28: 
    assert(pos_y(2) == pos[2].second+1); // si plus de trognons, pas de tir..
    get_pos();
    break;
  }

}
