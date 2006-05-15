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

#include "Gui2D.hh"

static const char *centrales[] = {
  "centrale_t1",
  "centrale_t2",
  "centrale_t3",
  "centrale_t4",
  "centrale_t5",
  "centrale_t6"
};

static const char *hotels[] = {
  "hotel_t1",
  "hotel_t2",
  "hotel_t3",
  "hotel_t4",
  "hotel_t5",
  "hotel_t6"
};

static const char *mines[] = {
  "mine_t1",
  "mine_t2",
  "mine_t3",
  "mine_t4",
  "mine_t5",
  "mine_t6"
};	

static const char *usines[] = {
  "usine_t1",
  "usine_t2",
  "usine_t3",
  "usine_t4",
  "usine_t5",
  "usine_t6"
};

static const char *vaisseaux[] = {
  "vaisseau_t1",
  "vaisseau_t2",
  "vaisseau_t3",
  "vaisseau_t4",
  "vaisseau_t5",
  "vaisseau_t6"
};

static const char *fires_v[] = {
  "laser_vert_t1",
  "laser_vert_t2",
  "laser_vert_t3",
  "laser_vert_t4",
  "laser_vert_t5",
  "laser_vert_t6"
};

static const char *fires_h[] = {
  "laser_horz_t1",
  "laser_horz_t2",
  "laser_horz_t3",
  "laser_horz_t4",
  "laser_horz_t5",
  "laser_horz_t6"
};


Gui2D::Gui2D(ClientCx* ccx, Api* api)
  : Gui(ccx),
    api_(api)
{
  // Set some variables.
  width_ = api->world_size_x();
  height_ = api->world_size_y();
  case_length_ = 80;
  path_ = PKGDATADIR "/texture_2D";

  // Initialize the gui.
  api_->setEventHandler(this);
  init();

  // Load the map.
  memset(map_bg_, 0, sizeof(map_bg_));
  memset(map_fg_, 0, sizeof(map_fg_));
  memset(map_building_, 0, sizeof(map_building_));
  memset(unit_, 0, sizeof(unit_));
  loadMap();
}

Gui2D::~Gui2D()
{
}


void Gui2D::loadMap()
{
  const char* background_pixmap[4];
  Object2D* tile;

  background_pixmap[TERRAIN] = "floor";
  background_pixmap[WATER] = "water3";
  background_pixmap[PLASMA] = "energie_tneutre";
  background_pixmap[ORE] = "pierre";

  for (int i = 0; i < width_; i++) 
    for (int j = 0; j < height_; j++)
      {
        int land = api_->land_type(i, j);
        int land_back = land == WATER ? WATER : TERRAIN;
        int land_front = -1;
        if (land == ORE || land == PLASMA)
          land_front = land;

        // set background
        tile = world_->createObject();
        tile->setAnim(background_pixmap[land_back], true);
        tile->jump(i, j);
        tile->setZ(-100);
        map_bg_[i][j] = tile;

        // set foreground
        if (land_front != -1)
          {
	    tile = world_->createObject();
	    tile->setAnim(background_pixmap[land_front], true);
	    tile->jump(i, j);
            tile->setZ(-50);
            map_fg_[i][j] = tile;
	  }

        // search existing units.
        int team_id = api_->coleoptera_team(i, j);
        int unit_id = api_->coleoptera_id(i, j);
        if (team_id >= 0)
          {
            tile = world_->createObject();
            tile->setAnim(vaisseaux[team_id % 6], true);
            tile->jump(i, j);
            tile->setZ(-1);
              
            delete unit_[team_id][unit_id];
            unit_[team_id][unit_id] = tile;
          }
      }
}


void Gui2D::newUnit(int player_id, int unit_id, const Position& pos)
{
  Object2D* tile = world_->createObject();
  tile->setAnim(vaisseaux[player_id % 6], true);
  tile->jump(pos.row, pos.col);
  tile->setZ(-1);
    
  delete unit_[player_id][unit_id];
  unit_[player_id][unit_id] = tile;
}

void Gui2D::moveColeoptera(int player_id, int unit_id, const Position& pos)
{
  unit_[player_id][unit_id]->move(pos.row, pos.col);
}

void Gui2D::buildingFinished(int player_id, const Position& pos)
{
  uint8_t type = api_->building_type(pos.row, pos.col);

  Object2D* tile = world_->createObject();
  tile->setAnim(type == MINE ?    mines[player_id % 6] :
                type == FACTORY ? usines[player_id % 6] :
                type == CENTRAL ? centrales[player_id % 6] :
                                  hotels[player_id % 6], true);
  tile->jump(pos.row, pos.col);
  tile->setZ(-40);

  delete map_building_[pos.row][pos.col];
  map_building_[pos.row][pos.col] = tile;
}



#if 0
void Gui2D::applyDiff(StechecPkt* pkt)
{  
  switch (pkt->type)
    {

    case THROW_PLASMA:
      {
	int		pid = pkt->client_id;
	Player*		p = &player_[pid];
  	int           	id = pkt->arg[0];
  	Object2D*	col = &p->coleopteres[id];
  	int		dirx = pkt->arg[2];
  	int		diry = pkt->arg[3];
  	int		tmpx = 10 + dirx; // FIXME: 
  	int		tmpy = 10 + diry; // FIXME: 

 	// lance le tir
        int cpt = 10; // FIXME: 
  	while (cpt--)
          {
            Object2D *fire = new Object2D(gui_, "");

            fire->move(tmpx, tmpy);
            fire->anim(dirx == 0 ? fires_h[pid % 6] : fires_v[pid % 6], 150);
            fire->kill(dirx == 0 ? fires_h[pid % 6] : fires_v[pid % 6], 150);
            fire->setZ(20);
	  
            tmpx += dirx;
            tmpy += diry;
          }
	break;
      }

    case COLEOPTERA_CHANGE_STATE:
      {
	int id_player = pkt->client_id;
	int id = pkt->arg[0];
	int state = pkt->arg[1];

	if (state == STATE_DEAD)
	  player_[id_player].coleopteres[id].kill("vaisseau_explode");
	break;
      }
    }
}
#endif

void Gui2D::slotPeriodic()
{
  Gui::slotPeriodic();
}
