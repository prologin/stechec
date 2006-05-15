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

#include "OutAGui.hh"

//
// Resources
//

static const char* streets[] = {
  "street_alone",
  "street_up",
  "street_left",
  "street_up_left",
  "street_down",
  "street_up_down",
  "street_down_left",
  "street_up_down_left",
  "street_right", // 8
  "street_up_right",
  "street_left_right",
  "street_up_left_right",
  "street_down_right",
  "street_up_down_right",
  "street_down_left_right",
  "street_all" // 15
};

static const char* squares[] = {
  "square_1",
  "square_2",
  "square_3"
};
static const int nb_square = sizeof(squares) / sizeof(char*);

static const char* buildings[] = {
  "building_1",
  "building_2",
  "building_3"
};
static const int nb_building = sizeof(buildings) / sizeof(char*);

static const char* players[] = {
  "player_t1",
  "player_t2",
  "player_t3",
  "player_t4",
  "player_t5",
  "player_t6"
};
static const int nb_player = sizeof(players) / sizeof(char*);

static const char* building_bet = "building_bet";

static const char* delorean = "delorean";

static const char* almanach = "almanach";

//
// End of resources.
//



OutAGui::OutAGui(ClientCx* ccx, Api* api)
  : Gui(ccx),
    api_(api)
{
  // Set some variables.
  width_ = api->taille_ville_x();
  height_ = api->taille_ville_y();
  case_length_ = 80;
  path_ = PKGDATADIR "/texture_2D";

  // Initialize the gui.
  api_->setEventHandler(this);
  init();

  // Load the map.
  memset(map_, 0, sizeof(map_));
  memset(unit_, 0, sizeof(unit_));
  loadMap();
}

OutAGui::~OutAGui()
{
  // FIXME: free data
}

const char* OutAGui::getBackground(int land, int x, int y)
{
  if (land == STREET)
    {
      int street_around = 0;
      if (api_->terrain_graphique(x, y - 1) == STREET)
        street_around += 1;
      if (api_->terrain_graphique(x - 1, y) == STREET)
        street_around += 2;
      if (api_->terrain_graphique(x, y + 1) == STREET)
        street_around += 4;
      if (api_->terrain_graphique(x + 1, y) == STREET)
        street_around += 8;
      return streets[street_around];
    }
  if (land == GRASSY_AREA)
    return squares[rand() % nb_square];
  if (land == BUILDING)
    return buildings[rand() % nb_building];
  if (land == BET_PLACE)
    return building_bet;
}

void OutAGui::loadMap()
{
  const char* background_pixmap[3];
  Object2D* tile;

  for (int x = 0; x < width_; x++) 
    for (int y = 0; y < height_; y++)
      {
        int land = api_->terrain_graphique(x, y);
        if (land != UNKNOWN || 1)
          {
            // set background, even under goodman.
            if (land == GOODMAN)
              land = STANDARD;
            tile = world_->createObject();
            tile->setAnim(getBackground(land, x, y), true);
            tile->jump(x, y);
            tile->setZ(-100);
            map_[x][y] = tile;
          }
      }

  // update men informations
  int old_team_id = api_->mon_equipe();
  for (int team_id = 0; team_id < api_->equipes(); team_id++)
    {
      api_->switchTeam(team_id);
      for (int unit_id = 0; unit_id < api_->taille_equipe(); unit_id++)
        {
          // fetch man position
          int x = api_->position_joueur_x(unit_id);
          int y = api_->position_joueur_y(unit_id);

          // create an object for him
          tile = world_->createObject();
          tile->setAnim(players[team_id % nb_player], true);
          tile->jump(x, y);
    
          delete unit_[team_id][unit_id];
          unit_[team_id][unit_id] = tile;
        }
    }
  api_->switchTeam(old_team_id);
}


void OutAGui::moveGoodman(int player_id, int unit_id, const Position& pos)
{
  unit_[player_id][unit_id]->move(pos.row, pos.col);
}

void OutAGui::changeDate(int date)
{
  LOG2("change date...");
}

void OutAGui::slotPeriodic()
{
  Gui::slotPeriodic();
}
