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

static const char* players[] = {
  "/prolo2007/graphics/player_t1.png",
  "/prolo2007/graphics/player_t2.png",
  "/prolo2007/graphics/player_t3.png",
  "/prolo2007/graphics/player_t4.png",
  "/prolo2007/graphics/player_t5.png",
  "/prolo2007/graphics/player_t6.png"
};
static const int nb_player = sizeof(players) / sizeof(char*);

static const char* virus[] = {
  "/prolo2007/graphics/virus_1.png",
#if 0
  "/prolo2007/graphics/virus_2.png",
  "/prolo2007/graphics/virus_3.png",
  "/prolo2007/graphics/virus_4.png",
  "/prolo2007/graphics/virus_5.png",
  "/prolo2007/graphics/virus_6.png",
  "/prolo2007/graphics/virus_7.png",
  "/prolo2007/graphics/virus_8.png",
  "/prolo2007/graphics/virus_9.png"
#endif
};
static const int nb_virus = sizeof(virus) / sizeof(char*);

static const char* bacteria[] = {
  "/prolo2007/graphics/bacteria_1.png",
  "/prolo2007/graphics/bacteria_2.png",
  "/prolo2007/graphics/bacteria_3.png"
};
static const int nb_bacteria = sizeof(bacteria) / sizeof(char*);

#define CELL_SANE	0
#define CELL_INFECTED	1

static const char* cell[] = {
  "/prolo2007/graphics/cell.png",
  "/prolo2007/graphics/cell_infected.png",
};
static const int nb_cell = sizeof(cell) / sizeof(char*);

static const char* flesh[] = {
  "/prolo2007/graphics/flesh_1.png",
  "/prolo2007/graphics/flesh_2.png",
};
static const int nb_flesh = sizeof(flesh) / sizeof(char*);

static const char* vessel[] = {
  "/prolo2007/graphics/vessel_1.png",
  "/prolo2007/graphics/vessel_2.png",
};
static const int nb_vessel = sizeof(vessel) / sizeof(char*);

static const char* food[] = {
  "/prolo2007/graphics/food_1.png",
  "/prolo2007/graphics/food_2.png",
  "/prolo2007/graphics/food_3.png",
  "/prolo2007/graphics/food_4.png",
  "/prolo2007/graphics/food_5.png",
  "/prolo2007/graphics/food_6.png",
  "/prolo2007/graphics/food_7.png",
  "/prolo2007/graphics/food_8.png",
  "/prolo2007/graphics/food_9.png"
};
static const int nb_food = sizeof(food) / sizeof(char*);

static const char* antibody[] = {
  "/prolo2007/graphics/antybody_1.png",
  "/prolo2007/graphics/antybody_2.png",
  "/prolo2007/graphics/antybody_3.png",
  "/prolo2007/graphics/antybody_4.png",
  "/prolo2007/graphics/antybody_5.png",
  "/prolo2007/graphics/antybody_6.png",
  "/prolo2007/graphics/antybody_7.png",
  "/prolo2007/graphics/antybody_8.png",
  "/prolo2007/graphics/antybody_9.png",
};
static const int nb_antibody = sizeof(antibody) / sizeof(char*);

//
// End of resource listing.
//



GlobulusGui::GlobulusGui(xml::XMLConfig* xml, Api* api, ClientCx* ccx)
  : api_(api),
    ccx_(ccx),
    case_size_(128) // .png are 128x128
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
      }

  for (int team_id = 0; team_id < api_->equipes(); team_id++)
    for (int unit_id = 0; unit_id < MAX_WHITE_CELL; unit_id++)
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
  for (int i = 0; i < MAX_BACTERIA; i++)
    {
        Sprite& o = bacterias_[i];

        o.setZ(4);
        o.hide();
        vscreen_->addChild(&o);
    }
}

// Set the surface for all map square.
void GlobulusGui::initMapSquare(int x, int y)
{
  Square& sq = map_[x][y];

  switch (api_->terrainAt(x, y))
    {
      case FLESH:
	sq.load(flesh[0]);
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

  int date = api_->current_beat();
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

// EVENT-BASE refresh

void GlobulusGui::moveLeucocyte(int team_id, int unit_id, const Position& to)
{
  LOG2("moveLeucocyte %1 %2 %3 %4\n", team_id, unit_id, to.row, to.col);
  units_[team_id][unit_id].move(Point(to) * case_size_, 20.);
}

void GlobulusGui::newLeucocyte(int team_id, int unit_id, const Position& at)
{
  LOG2("newLeucocyte %1 %2 %3 %4\n", team_id, unit_id, at.row, at.col);
  units_[team_id][unit_id].setPos(Point(at) * case_size_);
  units_[team_id][unit_id].show();
}

void GlobulusGui::moveVirus(int virus_id, const Position& to)
{
  LOG2("moveVirus %1 %2 %3\n", virus_id, to.row, to.col);
  virus_[virus_id].move(Point(to) * case_size_, 20.);
}

void GlobulusGui::newVirus(int virus_id, const Position& at, int type)
{
  LOG2("newVirus %1 %2 %3 %4\n", virus_id, at.row, at.col, type);
  virus_[virus_id].load(virus[type]);
  virus_[virus_id].setPos(Point(at) * case_size_);
  virus_[virus_id].show();
}

void GlobulusGui::dieVirus(int virus_id)
{
  LOG2("dieVirus %1\n", virus_id);
  virus_[virus_id].hide();
}

void GlobulusGui::newBacteria(int bacteria_id, const Position& at)
{
  LOG2("newBacteria %1 %2 %3\n", bacteria_id, at.row, at.col);
  bacterias_[bacteria_id].load(bacteria[rand() % nb_bacteria]);
  bacterias_[bacteria_id].setPos(Point(at) * case_size_);
  bacterias_[bacteria_id].show();
}

void GlobulusGui::dieBacteria(int bacteria_id)
{
  LOG2("dieBacteria %1\n", bacteria_id);
  bacterias_[bacteria_id].hide();
}

void GlobulusGui::newCell(int cell_id, const Position& at)
{
  LOG2("newCell %1 %2 %3\n", cell_id, at.row, at.col);
  cells_[cell_id].load(cell[CELL_SANE]);
  cells_[cell_id].setPos(Point(at) * case_size_);
  cells_[cell_id].show();
}

void GlobulusGui::caseUpdate(int x, int y, int food, int antibody)
{
  LOG2("caseUpdate %1 %2 %3 %4\n", x, y, food, antibody);
  // FIXME
}

void GlobulusGui::cellUpdate(int cell_id, int type)
{
  LOG2("cellUpdate %1 %2\n", cell_id, type);
  switch (type)
    {
      case STATE_DEAD:
	cells_[cell_id].hide();
	break;
      case CELL_STATE_PRODUCTING_VIRII:
	cells_[cell_id].load(cell[CELL_INFECTED]);
	break;
      case STATE_NORMAL:
	cells_[cell_id].load(cell[CELL_SANE]);
	break;
      default:
	break;
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
#if 1
      for (int team_id = 0; team_id < api_->equipes(); team_id++)
	for (int unit_id = 0; unit_id < MAX_WHITE_CELL; unit_id++)
	  {
	    Sprite& o = units_[team_id][unit_id];
	    int x, y;

	    if (api_->getLeucocyte(team_id, unit_id, &x, &y))
	      o.hide();
	    else
	      {
		LOG2("Found leucocyte %1 %2 at %3 %4\n", team_id, unit_id,
		     x, y);
		o.move(Point(x, y) * case_size_, 100.);
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
	      o.load(virus[type % nb_virus]);
	      o.move(Point(x, y) * case_size_, 100.);
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
	      o.load(cell[sane ? CELL_SANE : CELL_INFECTED]);
	      o.move(Point(x, y) * case_size_, 100.);
	      o.show();
	    }
	}
      for (int i = 0; i < MAX_BACTERIA; i++)
	{
	  Sprite& o = bacterias_[i];
	  int x, y;

	  if (api_->getBacteria(i, &x, &y))
	    o.hide();
	  else
	    {
	      o.load(bacteria[rand() % nb_bacteria]);
	      o.move(Point(x, y) * case_size_, 100.);
	      o.show();
	    }
	}
#if 0
      for (int x = 0; x < map_x_; x++) // FIXME: segfault
	for (int y = 0; y < map_y_; y++)
	  {
	    LOG2("Map size %1/%2 %3/%4\n", x,map_x_, y,map_y_);
	    int amount = (api_->getFood(x, y) * 100) / NUTRIMENT_MODULO;

	    if (amount < 11)
	      food_[x][y].hide();
	    else
	      {
	       	food_[x][y].load(food[/*(amount - 1) / 10*/ 0]); // FIXME
		food_[x][y].show();
	      }
	  }
      for (int x = 0; x < map_x_; x++)
	for (int y = 0; y < map_y_; y++)
	  {
	    int amount = (api_->getAntibody(x, y) * 100) / 1; // FIXME

	    if (amount < 11)
	      antibody_[x][y].hide();
	    else
	      {
	       	antibody_[x][y].load(antibody[(amount - 1) / 10]);
		andibody_[x][y].show();
	      }
	  }
#endif
#endif

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
