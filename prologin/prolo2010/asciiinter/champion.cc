///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_cxx.rb
//

#include <prologin.hh>
#include "view.hh"
#include <string>
#include <cstdio>
#include <vector>
#include <cstring>


///
// Fonction appellée au début de la partie.
//
void init_game()
{
}

char * string_of_type(type_unite t){
  switch(t){
  case PERROQUET: return "perroquet";
  case SINGE: return "singe";
  case CHAT: return "chat";
  case KANGOUROU: return "kangourou";
  }
}

unite retirer_ko()
{
  view();
  printf("retirer ko :\n");
  std::vector<unite> u = unites();
  for (int i = 0 ; i < u.size(); i++){
    if (u[i].ennemi){
      printf("indice=%d, type=%s, pa=%d, ko=%d, pos=(%d, %d)\n",
	     i, string_of_type(u[i].type_unite_actuel), u[i].pa,
	     u[i].ko, u[i].pos.x, u[i].pos.y);
    }
  }
  int n;
  bool bad;
  do{
    char buffer[80];
    fgets(buffer, 80, stdin);
    sscanf(buffer, "%d\n", &n);
    if (n < 0 || n >= u.size() || (! u[n].ennemi) ){
      printf("veuillez enlever un point ko a une unite ennemie\n");
      bad = true;
    }else{
      bad = false;
    }
  } while(bad);
  printf("fin retirer_ko;\n");
  return u[n];
}

///
// Fonction appellée pour la phase de jeu.
//
void jouer()
{
  view();
  printf("jouer :\n");
  std::vector<unite> u = unites();
  for (int i = 0 ; i < u.size(); i++){
    if (!u[i].ennemi){
      printf("indice=%d, type=%s, pa=%d, ko=%d, pos=(%d, %d)\n",
	     i, string_of_type(u[i].type_unite_actuel), u[i].pa,
	     u[i].ko, u[i].pos.x, u[i].pos.y);
    }
  }
  int n;
  bool continue_;
  position from, to;
  do{
    char buffer[80];
    fgets(buffer, 80, stdin);
    if (sscanf(buffer, "from (%d, %d) to (%d, %d)\n", &from.x, &from.y, &to.x, &to.y) == 4){
      printf("OK");
    }else if (strcmp(buffer, "end\n") == 0){
      break;
    }
  } while(true);
  printf("fin jouer;\n");
}

///
// Fonction appellée à la fin de la partie.
//
void end_game()
{
  view();
}

