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
  "/graphics/player_t1.png",
  "/graphics/player_t2.png",
  "/graphics/player_t3.png",
  "/graphics/player_t4.png"
};
static const int nb_player = sizeof(players) / sizeof(char*);

static const char* virus[] = {		// OK
  "/graphics/virus_0.png",
  "/graphics/virus_1.png",
  "/graphics/virus_2.png",
  "/graphics/virus_3.png",
  "/graphics/virus_4.png"
};
static const int nb_virus = sizeof(virus) / sizeof(char*);

static const char* bacteria[] = {	// OK
  "/graphics/bacteria_1.png",
  "/graphics/bacteria_2.png",
  "/graphics/bacteria_3.png"
};
static const int nb_bacteria = sizeof(bacteria) / sizeof(char*);

static const char* cell[] = {		// OK
  "/graphics/cell.png",
  "/graphics/cell_infected.png",
  "/graphics/cell2.png",
  "/graphics/cell2_infected.png"
};
static const int nb_cell = sizeof(cell) / sizeof(char*);

static const char* flesh[] = {		// OK
  "/graphics/full_blood2.png",
  "/graphics/1left_top_corner.png",
  "/graphics/2top_center.png",
  "/graphics/3right_top_corner.png",
  "/graphics/4left_center.png",
  "/graphics/5center.png",
  "/graphics/6right_center.png",
  "/graphics/7left_bottom_corner.png",
  "/graphics/8bottom_center.png",
  "/graphics/9right_bottom_corner.png",
  "/graphics/left_vertical_line.png",
  "/graphics/right_vertical_line.png",
  "/graphics/line_bottom.png",
  "/graphics/line_top.png",
  "/graphics/double_line.png",
  "/graphics/double_vertical_line.png"
};
static const int nb_flesh = sizeof(flesh) / sizeof(char*);

static const char* vessel[] = {		// OK
  "/graphics/full_blood.png"
};
static const int nb_vessel = sizeof(vessel) / sizeof(char*);

static const char* food[] = {		// OK
  "/graphics/nutriment.png",
  "/graphics/nutriment2.png",
  "/graphics/nutriment3.png"
};
static const int nb_food = sizeof(food) / sizeof(char*);

static const char* antibody[] = {		// OK
  "/graphics/anticorps.png",
  "/graphics/anticorps2.png",
  "/graphics/anticorps3.png"
};
static const int nb_antibody = sizeof(antibody) / sizeof(char*);

//
// End of resource listing.
//



GlobulusGui::GlobulusGui(ConfFile* cfg_file, Api* api, ClientCx* ccx)
  : api_(api),
    ccx_(ccx),
    case_size_(64) // .png are 128x128
{
  map_x_ = api_->taille_corps_x();
  map_y_ = api_->taille_corps_y();

  // Set ressources prefix to share/stechec/prolo2007
  ResourceCenter::getInst()->setResourcePrefix(PKGDATADIR);
  
  LOG4("Map size %1 %2\n", map_x_, map_y_);

  // Initialize SDL window and the main scrollable surface.
  win_.init(cfg_file);
  vscreen_ = new VirtualScrollableSurface("Screen",
                                          win_.getInput(),
                                          win_.getScreen().getSize(),
                                          Point(map_x_ * case_size_, map_y_ * case_size_));
  win_.getScreen().addChild(vscreen_);

  // Init surface behind text
  back_txt_ = Surface("/graphics/background_text.png");
  back_txt_.setPos(580, 5);
  back_txt_.setZ(-1);
  win_.getScreen().addChild(&back_txt_);

  // Initialize labels on the right-top.
  txt_beat_ = TextSurface("Vera.ttf", 12, 200, 22);
  txt_beat_.setPos(590, 10);
  txt_beat_.setZ(8);
  txt_beat_.setTextColor(white_color);
  win_.getScreen().addChild(&txt_beat_);

  for (int i = 0; i < api_->equipes(); i++)
    {
      txt_score_[i] = TextSurface("Vera.ttf", 12, 200, 22);
      txt_score_[i].setPos(590, 34 + i * 22);
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
  current_team_ = -1;

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
#if 0
      if (input.key_pressed_[SDLK_MINUS] || input.key_pressed_[SDLK_m])
        setZoom(1);
      if (input.key_pressed_[SDLK_PLUS] || input.key_pressed_[SDLK_p])
        setZoom(-1);
#endif

      // See a specific player view.
      for (int i = 1; i < 10; i++)
        if (input.key_pressed_['0' + i])
          {
            LOG2("Switch to team id `%1' view.", i -1);
            if (!api_->switchTeam(i - 1))
	      {
		LOG2("Failed. No such team ?");
	      }
	    else
	      current_team_ = i - 1;
          }

      // See all the map
      if (input.key_pressed_[SDLK_0])
        {
          LOG2("Switch to super-user view");
          api_->switchTeam(-1);
	  current_team_ = -1;
        }

      refreshInfoBox();

      // POLLING CODE
      // FIXME: try to transform it into event-based
      for (int team_id = 0; team_id < api_->equipes(); team_id++)
	for (int unit_id = 0; unit_id < MAX_PLAYER; unit_id++)
	  {
	    Sprite& o = units_[team_id][unit_id];
	    int x, y;

	    if (api_->getLeucocyte(team_id, unit_id, &x, &y) || (api_->visible(x, y) == 0 && team_id != current_team_ && current_team_ != -1))
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

	  if (api_->getVirus(i, &x, &y, &type) || (api_->visible(x, y) == 0 && current_team_ != -1))
	    o.hide();
	  else
	    {
	      if (!o.isShown ())
		{
		  o.load(virus[type % nb_virus]);
		  o.setPos(Point(x, y) * case_size_);
		}
	      else
		o.move(Point(x, y) * case_size_, 15.);
	      o.show();
	    }
	}
      for (int i = 0; i < MAX_CELL; i++)
	{
	  Sprite& o = cells_[i];
	  int x, y, sane;

	  if (api_->getCell(i, &x, &y, &sane) || (api_->visible(x, y) == 0 && current_team_ != -1))
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
	{
	for (int y = 0; y < map_y_; y++)
	  {
	    int nvis = (api_->visible(x, y) == 0 && current_team_ != -1);

	    int amount = (api_->getFood(x, y) * 100) / MAX_NUTRIENT;

	    if (amount < 50 || nvis)
	      food_[x][y].hide();
	    else
	      {
		if (amount < 130)
		  food_[x][y].load(food[0]);
		else
		  if (amount < 190)
		    food_[x][y].load(food[1]);
		  else
		    food_[x][y].load(food[2]);
		food_[x][y].show();
	      }

	    amount = api_->getAntibody(current_team_, x, y);

	    if (amount < 10 || nvis)
	      antibody_[x][y].hide();
	    else
	      {
		if (amount < 85)
		  antibody_[x][y].load(antibody[0]);
		else
		  if (amount < 180)
		    antibody_[x][y].load(antibody[1]);
		  else
		    antibody_[x][y].load(antibody[2]);
		antibody_[x][y].show();
	      }

	    int pop;

	    if ((pop = api_->getBacteria(x, y)) == -1 || nvis)
	      bacterias_[x][y].hide();
	    else
	      {
		if (pop < 25)
		  bacterias_[x][y].load(bacteria[0]);
		else
		  if (pop < 50)
		    bacterias_[x][y].load(bacteria[1]);
		  else
		    bacterias_[x][y].load(bacteria[2]);
		bacterias_[x][y].show();
	      }
	  }
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
extern "C" int run(ConfFile* cfg_file, Api* api, ClientCx* ccx)
{
  GlobulusGui visu(cfg_file, api, ccx);
  return visu.run();
}
