#include "prologin.hh"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

static std::ofstream fs;

extern "C" std::vector<std::vector<int> > api_actions_effectuees();

void init_game()
{
    std::string path = "dump.json";
    if (getenv("DUMP_PATH"))
        path = getenv("DUMP_PATH");

    fs.open(path.c_str());
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    os << "[";
    typename std::vector<T>::const_iterator it;
    for (it = v.begin(); it != v.end(); ++it)
    {
        if (it != v.begin())
            os << ", ";
        os << *it;
    }
    os << "]";
    return os;
}

static std::ostream& operator<<(std::ostream& os, position p)
{
    return os << "{ \"x\": " << p.x << ", \"y\": " << p.y << "}";
}

static std::ostream& operator<<(std::ostream& os, unite_energie s)
{
    return os << "{ \"id\": " << s.id << ", \"pos\": " << s.pos <<
                 ", \"valeur\": " << s.valeur << ", \"valeur_max\": " <<
                 s.valeur_max << "}";
}

static std::ostream& operator<<(std::ostream& os, trainee_moto t)
{
    return os << "{ \"id\": " << t.id << ", \"emplacement\": " << t.emplacement <<
                 ", \"team\": " << t.team << ", \"intensite\": " << t.intensite <<
                 "}";
}

void jouer()
{
    fs << "{";

    fs << "\"mon_equipe\": " << mon_equipe() << ", ";
    fs << "\"scores\": " << scores() << ", ";
    fs << "\"nombre_equipes\": " << nombre_equipes() << ", ";
    fs << "\"tour_actuel\": " << tour_actuel() << ", ";
    fs << "\"unites_energie\": " << unites_energie() << ", ";
    fs << "\"trainees_moto\": " << trainees_moto () << ", ";
    fs << "\"actions_effectuees\": " << api_actions_effectuees () << ", ";

    fs << "\"type_cases\": [";
    for (int y = 0; y < TAILLE_TERRAIN; ++y)
    {
        for (int x = 0; x < TAILLE_TERRAIN; ++x)
        {
            if (x != 0 || y != 0)
                fs << ", ";
            position p = { x, y };
            fs << regarder_type_case(p);
        }
    }
    fs << "], ";

    fs << "\"type_bonus\": [";
    for (int y = 0; y < TAILLE_TERRAIN; ++y)
    {
        for (int x = 0; x < TAILLE_TERRAIN; ++x)
        {
            if (x != 0 || y != 0)
                fs << ", ";
            position p = { x, y };
            fs << regarder_type_bonus(p);
        }
    }
    fs << "], ";

    fs << "\"bonus_team\": [";
    fs << api_regarder_bonus(0) << ", ";
    fs << api_regarder_bonus(1) << "]";

    fs << "}" << std::endl;
}

void end_game()
{
    fs.close();
}
