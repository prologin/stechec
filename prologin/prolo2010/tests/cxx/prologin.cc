///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_cxx.rb
//

#include "prologin.hh"
#include <vector>

///
// Fonction appell�e au d�but de la partie.
//
void init_game()
{
  // fonction a completer
}

///
// Fonction appell�e pour la phase de retrait de KO.
//
position retirer_ko()
{
  std::vector<unite> u = unites();
  for (int i = 0, size = u.size(); i < size; i ++){
    if (u[i].ennemi && u[i].ko > 0){
      return u[i].pos;
    }
  }
}

///
// Fonction appell�e pour la phase de jeu.
//
void jouer()
{
  afficher_unite(unites()[0]);
}

///
// Fonction appell�e � la fin de la partie.
//
void end_game()
{
  // fonction a completer
}

