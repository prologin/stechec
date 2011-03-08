///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_cxx.rb
//

#include "prologin.hh"

#include <iostream>

///
// Fonction appellée au début de la partie
//
void init_game()
{
  std::cout << "equipe : " << mon_equipe() << std::endl;
}

///
// Fonction appellée pour la phase de jeu
//
void jouer()
{
  // position init = {99, 99};
  // position end = {99, 98};
  // deplacer(1, init, end);
  
std::cout << "tour : " << tour_actuel() << std::endl;
  std::vector<trainee_moto> t = trainees_moto();
  for (int i = 0; i < t.size(); i++){
    trainee_moto tr = t.at(i);
    std::cout << "trainee : " << i << std::endl;
    for (int j = 0; j < tr.emplacement.size(); j++){
      position p = tr.emplacement.at(j);
      std::cout << "  " << p.x << " " << p.y << std::endl;
    }
  }
}

///
// Fonction appellée à la fin de la partie
//
void end_game()
{
  // fonction a completer
}

///
// Affiche le contenu d'une valeur de type erreur
//

///
// Affiche le contenu d'une valeur de type type_case
//

///
// Affiche le contenu d'une valeur de type type_bonus
//

///
// Affiche le contenu d'une valeur de type position
//

///
// Affiche le contenu d'une valeur de type source_energie
//

///
// Affiche le contenu d'une valeur de type trainee_moto
//

