/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2006 Prologin
*/

#include "qt_visu.hh"

QTVisu::QTVisu(Api* api, ClientCx* ccx)
  : Gui(ccx),
    api_(api)
{
  // Set some required variables.  
  width_ = api_->get_landscape_width();
  height_ = api_->get_landscape_height();
  case_length_ = 32;
  path_ = PKGDATADIR "texture_2D/";

  // This class will handle events.
  api_->setEventHandler(this);

  // Initialize the gui.
  init();

  // Print what we see on the map at the beginning.
  memset(map_, 0, sizeof(map_));
  memset(map_food_, 0, sizeof(map_food_));
  memset(ant_, 0, sizeof(ant_));
  printMap();
}

QTVisu::~QTVisu()
{
}

void QTVisu::printSquare(int i, int j)
{
  if (map_[i][j])
    delete map_[i][j];
  if (map_food_[i][j])
    delete map_food_[i][j];
  
  int land = api_->get_land_type(i, j);
  int land_back = land == FOOD ? NOTHING : land;
  Object2D* tile;

  if (land == UNKNOWN)
    return;

  // set background (land)
  map_[i][j] = tile = world_->createObject();
  switch (land_back)
    {
    case NOTHING:
      tile->setAnim("sol", true);
      break;
    case OBSTACLE:
      tile->setAnim("rocher", true);
      break;
    case ANTHILL:
      tile->setAnim("fourmiliere", true);
      break;
    }
  tile->jump(i, j);
  tile->setZ(-100);
  
  // set foreground (food)
  if (api_->get_land_type(i, j) == FOOD)
    {
      LOG1("create food");
      map_food_[i][j] = tile = world_->createObject();
      tile->setAnim("bouffe", true);
      tile->jump(i, j);
      tile->setZ(-50);
    }
}


void QTVisu::resetMap()
{
  // Erase all objects composing the map
  for (int i = 0; i < width_; i++)
    for (int j = 0; j < height_; j++)
      {
        delete map_[i][j];
        delete map_food_[i][j];
        map_[i][j] = NULL;
        map_food_[i][j];
      }
}

void QTVisu::printMap()
{
  // Print all missing parts.
  for (int i = 0; i < width_; i++)
    for (int j = 0; j < height_; j++)
      {
        if (map_[i][j] == NULL)
          printSquare(i, j);
      }
}

// Event.
void QTVisu::mapContent(const Position& pos, int)
{
  printSquare(pos.row, pos.col);
}

// Event.
void QTVisu::antMove(int player_id, int ant_id, const Position& pos)
{
  LOG2("ANT Move player id " << player_id << " id " << ant_id << " " << pos);

  Object2D* tile = ant_[player_id][ant_id];
  if (tile == NULL)
    {
      tile = ant_[player_id][ant_id] = world_->createObject();
      tile->setAnim("0000", true);
      tile->jump(pos.row, pos.col);
    }
  else
    tile->move(pos.row, pos.col);
}

// Library entry point.
extern "C" int run(xml::XMLConfig*, Api* api, ClientCx* ccx)
{
  QTVisu visu(api, ccx);
  visu.run();
  return 0;
}
