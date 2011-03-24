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
    std::cout << "Turn/Equipe : "
	      << tour_actuel() << "/"
	      << mon_equipe() << std::endl;
}

void my_print_trainees()
{
    // Affichage des trainees
    std::cout << "Trainees:" << std::endl;
    std::vector<trainee_moto> t = trainees_moto();
    for (unsigned int i = 0; i < t.size(); i++)
    {
        trainee_moto& tr = t[i];
        std::cout << "  [p" << tr.team << ":" << tr.id << "] :";
        for (unsigned int j = 0; j < tr.emplacement.size(); j++)
        {
            position p = tr.emplacement.at(j);
            std::cout << " (" << p.x << ", " << p.y << ")";
        }
	std::cout << std::endl;
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
    std::cout << "Scores:";
    for (unsigned int i = 0; i < s.size(); ++i)
        std::cout << " " << i << "->" << s[i];
    std::cout << std::endl;
}

void my_print_bonus()
{
    for (int j = 0; j < nombre_equipes(); ++j)
    {
	std::vector<type_bonus>	bonus = regarder_bonus(j);
	std::cout << "Bonus[" << j << "]:";
	if (bonus.size() == 0)
	    std::cout << " <vide>";
	for (unsigned int b = 0; b < bonus.size(); ++b)
	    std::cout << " " << str_type_bonus(bonus[b]) << "(" << b << ")";
	std::cout << std::endl;
    }
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

void get_team_motos(std::vector<trainee_moto>&	motos, int team)
{
    motos.clear();
    std::vector<trainee_moto> t = trainees_moto();
    for (unsigned int i = 0; i < t.size(); i++)
	if (t[i].team == team)
	    motos.push_back(t[i]);
}

void my_print_game()
{
    my_print_trainees();
    my_print_sources();
    my_print_scores();
    my_print_bonus();
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

    my_print_game();

    int		act_points = MAX_PA;
    static int	move_right = true;
    int		my_player = mon_equipe();
    int		turn = tour_actuel();
    int		incr = 1;
    position	cur_pos, next_pos;
    if (my_player)
	incr = -1;

    // Choose a moto to move
    std::vector<trainee_moto>	my_motos;
    get_team_motos(my_motos, my_player);
    cur_pos = my_motos[0].emplacement[0];
    next_pos = cur_pos;

    // 1st turn: error cases
    if (turn == 1)
	test_error_cases();
    // 2rd turn: split and merge
    else if (turn == 2)
    {
	position& pos1 = my_motos[0].emplacement[0];
	position& pos2 = my_motos[0].emplacement[1];
	std::cout << "Coupe: (" << pos1.x << ", " << pos1.y << ") <-> ("
		  << pos2.x << ", " << pos2.y << ")" << std::endl;
	if (couper_trainee_moto(my_motos[0].id,
				my_motos[0].emplacement[0],
				my_motos[0].emplacement[1]) != OK)
	    return;
	--act_points;
	get_team_motos(my_motos, my_player);
	std::cout << "Apres coupe:" << std::endl;
	my_print_trainees();


	if (fusionner(my_motos[0].id,
		      my_motos[0].emplacement[0],
		      my_motos[1].id,
		      my_motos[1].emplacement[0]) != OK)
	    return;
	--act_points;
	get_team_motos(my_motos, my_player);
	std::cout << "Apres fusion:" << std::endl;
	my_print_trainees();
    }
    // 3rd turn: enrouler
    else if (turn == 3)
    {
	trainee_moto&	moto = my_motos[0];
	position	point = my_motos[0].emplacement[my_motos[0].emplacement.size() - 1];

	std::cout << "Enrouler: (" << point.x << ", " << point.y << ")" << std::endl;
	enrouler(moto.id, point);
	--act_points;
	cur_pos = point;
	next_pos = point;
	get_team_motos(my_motos, my_player);
	std::cout << "Apres enroulage:" << std::endl;
	my_print_trainees();
    }

    // And the we move it
    for (; act_points > 0; --act_points)
    {
	if (move_right)
	    next_pos.x += incr;
	else
	    next_pos.y += incr;
	move_right = !move_right;
	std::cout << "Move: (" << cur_pos.x << ", " << cur_pos.y << ") -> ("
		  << next_pos.x << ", " << next_pos.y << ")" << std::endl;
	deplacer(my_motos[0].id, cur_pos, next_pos);
	cur_pos = next_pos;
    }

    if (turn == 3)
	my_print_game();
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

