///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_cxx.rb
//

#include "prologin.hh"

#include <iostream>

std::string	str_erreur(erreur	err)
{
    if (err == OK)
	return "OK";
    else if (err == ID_INVALIDE)
	return "ID_INVALIDE";
    else if (err == POSITION_INVALIDE)
	return "POSITION_INVALIDE";
    else if (err == PLUS_DE_PA)
	return "PLUS_DE_PA";
    else if (err == BONUS_INVALIDE)
	return "BONUS_INVALIDE";
    else if (err == PAS_A_TOI)
	return "PAS_A_TOI";
    else
	return "***ERREUR INVALIDE***";
}

std::string	str_type_case(type_case	c)
{
    if (c == VIDE)
	return "VIDE";
    else if (c == OBSTACLE)
	return "OBSTACLE";
    else if (c == BONUS)
	return "BONUS";
    else if (c == POINT_CROISEMENT)
	return "POINT_CROISEMENT";
    else
	return "***CASE INVALIDE***";
}

std::string	str_type_bonus(type_bonus b)
{
    if (b == PAS_BONUS)
	return "PAS_BONUS";
    else if (b == BONUS_CROISEMENT)
	return "BONUS_CROISEMENT";
    else if (b == PLUS_LONG)
	return "PLUS_LONG";
    else if (b == PLUS_PA)
	return "PLUS_PA";
    else if (b == BONUS_REGENERATION)
	return "BONUS_REGENERATION";
    else
	return "***BONUS INVALIDE***";
}

///
// Fonction appell�e au d�but de la partie
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

void test_error_cases()
{
    std::string	indent("   ");
    std::cout << "Error cases:" << std::endl;

    position	pos;
    pos.x = -1;
    pos.y = 0;
    int buffer = 0;

    buffer = regarder_type_case(pos);
    std::cout << indent
	      << "regarder_type_case(" << pos.x << ", " << pos.y << ") -> "
	      << str_type_case((type_case) buffer) << "(" << buffer << ")"
	      << std::endl;

    buffer = regarder_type_bonus(pos);
    std::cout << indent
	      << "regarder_type_bonus(" << pos.x << ", " << pos.y << ") -> "
	      << str_type_bonus((type_bonus) buffer) << "(" << buffer << ")"
	      << std::endl;
}

///
// Fonction appell�e pour la phase de jeu
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

    test_error_cases();
}

///
// Fonction appell�e � la fin de la partie
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

