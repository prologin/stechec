/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2007 Prologin
*/

#include "GlobulusGui.hh"


//
// Resource listing.
//

const SDL_Color white_color = { 255, 255, 255, SDL_ALPHA_OPAQUE };

static const char* players[] = {		// OK
  "/prolo2007/graphics/player_t1.png",
  "/prolo2007/graphics/player_t2.png",
  "/prolo2007/graphics/player_t3.png",
  "/prolo2007/graphics/player_t4.png",
};
static const int nb_player = sizeof(players) / sizeof(char*);

static const char* virus[] = {		// OK
  "/prolo2007/graphics/virus_0.png",
  "/prolo2007/graphics/virus_1.png",
  "/prolo2007/graphics/virus_2.png",
  "/prolo2007/graphics/virus_3.png",
  "/prolo2007/graphics/virus_4.png",
};
static const int nb_virus = sizeof(virus) / sizeof(char*);

static const char* bacteria[] = {	// OK
  "/prolo2007/graphics/bacteria_1.png",
  "/prolo2007/graphics/bacteria_2.png",
  "/prolo2007/graphics/bacteria_3.png"
};
static const int nb_bacteria = sizeof(bacteria) / sizeof(char*);

static const char* cell[] = {		// OK
  "/prolo2007/graphics/cell.png",
  "/prolo2007/graphics/cell_infected.png",
  "/prolo2007/graphics/cell2.png",
  "/prolo2007/graphics/cell2_infected.png",
};
static const int nb_cell = sizeof(cell) / sizeof(char*);

static const char* flesh[] = {		// OK
  "/prolo2007/graphics/full_blood2.png",
  "/prolo2007/graphics/1left_top_corner.png",
  "/prolo2007/graphics/2top_center.png",
  "/prolo2007/graphics/3right_top_corner.png",
  "/prolo2007/graphics/4left_center.png",
  "/prolo2007/graphics/5center.png",
  "/prolo2007/graphics/6right_center.png",
  "/prolo2007/graphics/7left_bottom_corner.png",
  "/prolo2007/graphics/8bottom_center.png",
  "/prolo2007/graphics/9right_bottom_corner.png",
  "/prolo2007/graphics/left_vertical_line.png",
  "/prolo2007/graphics/right_vertical_line.png",
  "/prolo2007/graphics/line_bottom.png",
  "/prolo2007/graphics/line_top.png",
  "/prolo2007/graphics/double_line.png",
  "/prolo2007/graphics/double_vertical_line.png"
};
static const int nb_flesh = sizeof(flesh) / sizeof(char*);

static const char* vessel[] = {		// OK
  "/prolo2007/graphics/full_blood.png",
};
static const int nb_vessel = sizeof(vessel) / sizeof(char*);

static const char* food[] = {

};
static const int nb_food = sizeof(food) / sizeof(char*);

static const char* antibody[] = {

};
static const int nb_antibody = sizeof(antibody) / sizeof(char*);

//
// End of resource listing.
//



GlobulusGui::GlobulusGui(xml::XMLConfig* xml, Api* api, ClientCx* ccx)
  : api_(api),
    ccx_(ccx),
    case_size_(64) // .png are 128x128
{
  map_x_ = api_->taille_corps_x();
  map_y_ = api_->taille_corps_y();

  LOG2("Map size %1 %2\n", map_x_, map_y_);

  // Initialize SDL window and the main scrollable surface.
  win_.init(xml);
  vscreen_ = new VirtualScrollableSurface("Screen",
                                          win_.getInput(),
                                          win_.getScreen().getSize(),
                                          Point(map_x_ * case_size_, map_y_ * case_size_));
  win_.getScreen().addChild(vscreen_);

  // Initialize labels on the right-top.
  txt_beat_ = TextSurface("Vera.ttf", 12, 200, 22);
  txt_beat_.setPos(590, 10);
  txt_beat_.setZ(8);
  txt_beat_.setTextColor(white_color);
  win_.getScreen().addChild(&txt_beat_);

  for (int i = 0; i < api_->equipes(); i++)
    {
      txt_score_[i] = TextSurface("Vera.ttf", 12, 200, 22);
      txt_score_[i].setPos(590, 54 + i * 22);
      txt_score_[i].setZ(8);
      txt_score_[i].setTextColor(white_color);
      win_.getScreen().addChild(&txt_score_[i]);
    }
}

GlobulusGui::~GlobulusGui()
{
  delete vscreen_;
}

void GlobulusGui::init()
{
  // init background
  for (int x = 0; x < map_x_; x++)
    for (int y = 0; y < map_y_; y++)
      {
	initMapSquare(x, y);

        food_[x][y].setPos(Point(x * case_size_, y * case_size_));
        food_[x][y].setZ(3);
	food_[x][y].hide();
	vscreen_->addChild(&food_[x][y]);

        antibody_[x][y].setPos(Point(x * case_size_, y * case_size_));
        antibody_[x][y].setZ(3);
	antibody_[x][y].hide();
	vscreen_->addChild(&antibody_[x][y]);

	bacterias_[x][y].setPos(Point(x * case_size_, y * case_size_));
        bacterias_[x][y].setZ(3);
	bacterias_[x][y].hide();
	vscreen_->addChild(&bacterias_[x][y]);
      }

  /* FIXME: a revoir, mettre MAX_PLAYER ici est une mauvaise idee */
  for (int team_id = 0; team_id < api_->equipes(); team_id++)
    for (int unit_id = 0; unit_id < MAX_PLAYER; unit_id++)
      {
        Sprite& o = units_[team_id][unit_id];

        o.load(players[team_id % nb_player]);
        o.setZ(6);
        o.hide();
        vscreen_->addChild(&o);
      }

  for (int i = 0; i < MAX_CELL; i++)
    {
        Sprite& o = cells_[i];

        o.setZ(4);
        o.hide();
        vscreen_->addChild(&o);
    }
  for (int i = 0; i < MAX_VIRUS; i++)
    {
        Sprite& o = virus_[i];

        o.setZ(5);
        o.hide();
        vscreen_->addChild(&o);
    }
}

// Set the surface for all map square.
void GlobulusGui::initMapSquare(int x, int y)
{
  Square& sq = map_[x][y];
  int id = 0;
  int around[3][3];
  memset(around, 1, sizeof(around));

  switch (api_->terrainAt(x, y))
    {
      case FLESH:


	if (x > 0 && y > 0 && api_->terrainAt(x - 1, y - 1) != FLESH)
	  around[0][0] = 0;
	if (x > 0 && api_->terrainAt(x - 1, y) != FLESH)
	  around[0][1] = 0;
	if (x > 0 && y < map_y_ - 1 && api_->terrainAt(x - 1, y + 1) != FLESH)
	  around[0][2] = 0;
	if (x < map_x_ - 1  && y > 0 && api_->terrainAt(x + 1, y - 1) != FLESH)
	  around[2][0] = 0;
	if (x < map_x_ - 1 && api_->terrainAt(x + 1, y) != FLESH)
	  around[2][1] = 0;
	if (x < map_x_ - 1 && y < map_y_ - 1 && api_->terrainAt(x + 1, y + 1) != FLESH)
	  around[2][2] = 0;
	if (y > 0 && api_->terrainAt(x, y - 1) != FLESH)
	  around[1][0] = 0;
	if (y < map_y_ - 1 && api_->terrainAt(x, y + 1) != FLESH)
	  around[1][2] = 0;

	if (around[0][1] == 0)
	  id = 10;
	if (around[2][1] == 0)
	  id = 11;
	if (around[1][2] == 0)
	  id = 12;
	if (around[1][0] == 0)
	  id = 13;

	if (around[1][0] == 0 && around[1][2] == 0)
	  id = 14;
	if (around[0][1] == 0 && around[2][1] == 0)
	  id = 15;

	if (around[1][2] == 0 && around[2][1] == 0)
	  id = 1;
	if (around[1][2] == 0 && around[0][1] == 0)
	  id = 3;
	if (around[1][0] == 0 && around[2][1] == 0)
	  id = 7;
	if (around[0][1] == 0 && around[1][0] == 0)
	  id = 9;

	if (around[0][1] == 0 && around[1][2] == 0&& around[2][1] == 0)
	  id = 2;
	if (around[1][0] == 0 && around[1][2] == 0&& around[2][1] == 0)
	  id = 4;
	if (around[1][2] == 0 && around[0][1] == 0 && around[1][0] == 0)
	  id = 6;
	if (around[0][1] == 0 && around[1][0] == 0&& around[2][1] == 0)
	  id = 8;

	if (around[0][1] == 0 && around[1][0] == 0&& around[2][1] == 0 && around[1][2] == 0)
	  id = 5;

	sq.load(flesh[id]);
	break;
      case VESSEL:
	sq.load(vessel[0]);
	break;
      default:
	assert(0);
	break;
    }

  vscreen_->addChild(&sq);
  sq.setGameInfo(api_, Point(x, y));
  sq.setPos(Point(x * case_size_, y * case_size_));
  sq.setZ(1);
}

void GlobulusGui::setZoom(int dec)
{
  int zoom_val = (int)vscreen_->getZoom();
  int zoom = 0;

  while ((zoom_val >>= 1))
    zoom++;

  if (dec > 0 && zoom < 3)
    zoom++;
  else if (dec < 0 && zoom > 0)
    zoom--;
  else
    return;
  case_size_ = (int)(128 / (1 << zoom));
  vscreen_->setZoom(1 << zoom);
  win_.clearScreen();
}

void GlobulusGui::refreshInfoBox()
{
  std::ostringstream os;

  int date = api_->battement_courant();
  os << "Beat: " << date;
  txt_beat_.setText(os.str());
  os.rdbuf()->str("");

  for (int i = 0; i < api_->equipes(); i++)
    {
      int score = api_->score(i);
      os << "Score team " << i + 1 << ": " << score;
      txt_score_[i].setText(os.str());
      os.rdbuf()->str("");
    }
}

int GlobulusGui::run()
{
  Input& input = win_.getInput();

  // This class will handle events.
  api_->setEventHandler(this);

  // Init background, objects, ...
  init();

  while (api_->getState() != GS_END)
    {
      // Bye bye dude.
      if (input.key_pressed_[SDLK_q])
        return 0;

      // Say we are ready for the next turn.
      if (input.key_pressed_[SDLK_RETURN] || input.key_pressed_[SDLK_SPACE])
        ccx_->setReady();

      // Zoom in-out
      if (input.key_pressed_[SDLK_MINUS] || input.key_pressed_[SDLK_m])
        setZoom(1);
      if (input.key_pressed_[SDLK_PLUS] || input.key_pressed_[SDLK_p])
        setZoom(-1);

      // See a specific player view.
      for (int i = 1; i < 10; i++)
        if (input.key_pressed_['0' + i])
          {
            LOG2("Switch to team id `%1' view.", i -1);
            if (!api_->switchTeam(i - 1))
              LOG2("Failed. No such team ?");
          }

      // See all the map
      if (input.key_pressed_[SDLK_0])
        {
          LOG2("Switch to super-user view");
          api_->switchTeam(-1);
        }

      refreshInfoBox();

      // POLLING CODE
      // FIXME: try to transform it into event-based
      for (int team_id = 0; team_id < api_->equipes(); team_id++)
	for (int unit_id = 0; unit_id < MAX_PLAYER; unit_id++)
	  {
	    Sprite& o = units_[team_id][unit_id];
	    int x, y;

	    if (api_->getLeucocyte(team_id, unit_id, &x, &y))
	      o.hide();
	    else
	      {
		o.move(Point(x, y) * case_size_, 15.);
		o.show();
	      }
	  }
      for (int i = 0; i < MAX_VIRUS; i++)
	{
	  Sprite& o = virus_[i];
	  int x, y, type;

	  if (api_->getVirus(i, &x, &y, &type))
	    o.hide();
	  else
	    {
	      if (!o.isShown ())
		o.load(virus[type % nb_virus]);
	      o.move(Point(x, y) * case_size_, 15.);
	      o.show();
	    }
	}
      for (int i = 0; i < MAX_CELL; i++)
	{
	  Sprite& o = cells_[i];
	  int x, y, sane;

	  if (api_->getCell(i, &x, &y, &sane))
	    o.hide();
	  else
	    {
	      if (!o.isShown ())
		o.load(cell[sane ? 0 : 1]); // FIXME: rand to cell2
	      o.setPos(Point(x, y) * case_size_);
	      o.show();
	    }
	}
      for (int x = 0; x < map_x_; x++)
	for (int y = 0; y < map_y_; y++)
	  {
	    int amount = (api_->getFood(x, y) * 100) / MAX_NUTRIENT;


#if 0 // FIXME
	    if (amount < 11)
	      food_[x][y].hide();
	    else
	      {
	       	food_[x][y].load(food[(amount - 1) / 11]);
		food_[x][y].show();
	      }

	    amount = api_->getAntibody(x, y);
	    if (amount > 100)
	      amount = 100;

	    if (amount < 11)
	      antibody_[x][y].hide();
	    else
	      {
	       	antibody_[x][y].load(antibody[(amount - 1) / 11]);
		antibody_[x][y].show();
	      }
#endif
	    int pop;

	    if ((pop = api_->getBacteria(x, y)) == -1)
	      bacterias_[x][y].hide();
	    else
	      {

		bacterias_[x][y].load(bacteria[(pop * (nb_bacteria - 1)) / 30]);
		bacterias_[x][y].show();
	      }

	    map_[x][y].update();
	  }

      // Process any incoming messages. Block at most 50 ms.
      while (ccx_->process(true))
        ;

      // Update and render one frame.
      if (win_.processOneFrame())
        return 0;
    }
  return 0;
}


// Library entry point.
extern "C" int run(xml::XMLConfig* xml, Api* api, ClientCx* ccx)
{
  GlobulusGui visu(xml, api, ccx);
  return visu.run();
}
