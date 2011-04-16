#include "prologin.hh"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

static std::ofstream fs;

void init_game()
{
    std::string path = "dump.json";
    if (getenv("DUMP_PATH"))
        path = getenv("DUMP_PATH");

    fs.open(path.c_str());
}

void jouer()
{
    // TODO
}

void end_game()
{
    fs.close();
}
