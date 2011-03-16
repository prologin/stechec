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

void my_print_trainees()
{
    // Affichage des trainees
    std::vector<trainee_moto> t = trainees_moto();
    for (unsigned int i = 0; i < t.size(); i++)
    {
        trainee_moto tr = t.at(i);
        std::cout << "trainee : " << i << std::endl;
        for (unsigned int j = 0; j < tr.emplacement.size(); j++)
        {
            position p = tr.emplacement.at(j);
            std::cout << "  " << p.x << " " << p.y << std::endl;
        }
    }
}

void my_print_sources()
{
    std::cout << "Sources:" << std::endl;
    std::vector<source_energie> sources = sources_energie();
    for (unsigned int i = 0; i < sources.size(); ++i)
    {
        source_energie& s = sources[i];
        std::cout << "  source[" << s.id << "] @ ("
                  << s.pos.x << ", " << s.pos.y << ") : "
                  << s.coef << std::endl;
    }
}

void my_print_scores()
{
    // Affichage des scores
    std::vector<int>    s = scores();
    std::cout << "scores:";
    for (unsigned int i = 0; i < s.size(); ++i)
        std::cout << " " << i << "->" << s[i];
    std::cout << std::endl;
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

    my_print_trainees();
    my_print_sources();
    my_print_scores();
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

