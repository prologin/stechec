///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_cxx.rb
//

#include "prologin.hh"
#include <iostream>
#include <algorithm>
using namespace std;
///
// Fonction appele au debut de la partie
//

pair<int,int>goal[3];
bool state[3];

bool go(int id, int goal_x, int goal_y) {
  if (pos_x(id) == goal_x && pos_y(id) == goal_y)
    return false;
  static const int directions[4][2] = {{0,-1},{-1,0},{0,1},{1,0}};
  int dmin=INFINI;
  int goal_dir;
  for (int k=0 ; k < 4 ; k++) {
    int nx = pos_x(id) + directions[k][0];
    int ny = pos_y(id) + directions[k][1];
    if (nx >= 0 && ny >= 0 && nx < taille_carte_x() && ny < taille_carte_y()) {
      int d = distance(nx, ny, goal_x, goal_y,false);
      if (d < dmin) {
	dmin = d;
	goal_dir = k;
      }
    }
  }
  deplacer(id, goal_dir);
  return true;
}


pair<int,int> select_goal_get_apple(int id) {
  pair<int,int>g;
  int nextdmin = INFINI;
  for (int i=2 ; i < taille_carte_y() ; i++)
    for (int j=0 ; j < taille_carte_x(); j++)
      if (pomme(j,i)) {
	bool unassigned = true;
	for (int k=0 ; k < 3 ; k++)
	  if (goal[k] == make_pair(j,i)) unassigned=false;
	if (unassigned) {
	  int d = distance(pos_x(id),pos_y(id), j,i,false);
	  if (d < nextdmin) {
	    nextdmin = d;
	    g = make_pair(j,i);
	  }
	}
      }
  return g;
}

void init_game()
{
  std::fill(state, state+3, true);
  std::fill(goal, goal+3, pair<int,int>(-1,-1));
  // fonction a completer
}

///
// Fonction appele a chaque tour
//

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
      int d = distance(0,0,j,i);
      if (d != INFINI)
	cout<< d <<" ";
      else
	cout<<"X ";
    }
    cout<<endl;
  }
  */
}


void play_turn()
{
  aff();
  for (int id=0 ; id < 3 ; id++) {

    if (porte_pomme(id) )
      state[id] = true;
    if (state[id] == true ){
      cout<<"robot "<<id<<" is going home"<<endl;
      //revenir a la maison et lacher la pomme
      int dmin=INFINI;
      int x_min,y_min;
      for (int i=0 ; i < taille_carte_x() ; i++) {
	for (int j=0 ; j < 2 ; j++) {
	  if (pomme(i,j)) continue;
	  int d = distance(pos_x(id), pos_y(id), i, j,false);
	  if (d < dmin) {
	    dmin = d;
	    x_min = i;
	    y_min = j;
	  }
	}
      }
      cout<<"dmin for robot "<<id<<" : "<<dmin<<endl;
      bool ok = go(id, x_min, y_min);
      if (!ok) {
	lacher_pomme(id,ICI);
	state[id] = false;
	goal[id] = select_goal_get_apple(id);
      }
      
    } else {
      cout<<"goal for robot "<<id<<" is "<<goal[id].first<<" "<<goal[id].second<<endl;

      if (!go(id, goal[id].first, goal[id].second)) {
	ramasser_pomme(id);
	cout<<"robot "<<id<<" is at its goal"<<endl;
	state[id]=true;
      } else { 
	cout<<"robot "<<id<<" is moving"<<endl;
      }
    }
  }
}

