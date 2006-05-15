#include "World.hh"

World::World(Gui* gui)
  : gui_(gui)
{
}

World::~World()
{
}

void World::setCaseLength(int lenght)
{
  case_length_ = lenght;
}
