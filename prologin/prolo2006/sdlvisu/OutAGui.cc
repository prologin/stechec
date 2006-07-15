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
  "/prolo2006/texture_2D/street_alone.png",
  "/prolo2006/texture_2D/street_up.png",
  "/prolo2006/texture_2D/street_left.png",
  "/prolo2006/texture_2D/street_up_left.png",
  "/prolo2006/texture_2D/street_down.png",
  "/prolo2006/texture_2D/street_up_down.png",
  "/prolo2006/texture_2D/street_down_left.png",
  "/prolo2006/texture_2D/street_up_down_left.png",
  "/prolo2006/texture_2D/street_right.png", // 8
  "/prolo2006/texture_2D/street_up_right.png",
  "/prolo2006/texture_2D/street_left_right.png",
  "/prolo2006/texture_2D/street_up_left_right.png",
  "/prolo2006/texture_2D/street_down_right.png",
  "/prolo2006/texture_2D/street_up_down_right.png",
  "/prolo2006/texture_2D/street_down_left_right.png",
  "/prolo2006/texture_2D/street_all.png" // 15
};

static const char* squares[] = {
  "/prolo2006/texture_2D/square_1.png",
  "/prolo2006/texture_2D/square_2.png",
  "/prolo2006/texture_2D/square_3.png"
};
static const int nb_square = sizeof(squares) / sizeof(char*);

static const char* buildings[] = {
  "/prolo2006/texture_2D/building_1.png",
  "/prolo2006/texture_2D/building_2.png",
  "/prolo2006/texture_2D/building_3.png"
};
static const int nb_building = sizeof(buildings) / sizeof(char*);

static const char* players[] = {
  "/prolo2006/texture_2D/player_t1.png",
  "/prolo2006/texture_2D/player_t2.png",
  "/prolo2006/texture_2D/player_t3.png",
  "/prolo2006/texture_2D/player_t4.png",
  "/prolo2006/texture_2D/player_t5.png",
  "p/prolo2006/texture_2D/layer_t6.png"
};
static const int nb_player = sizeof(players) / sizeof(char*);

static const char* building_bet = "/prolo2006/texture_2D/building_bet.png";

//
// End of resources.
//



OutAGui::OutAGui(Api* api, ClientCx* ccx)
  : api_(api),
    ccx_(ccx),
    win_(NULL),
    case_size_(128) // .png are 128x128
{
  map_x_ = api_->taille_ville_x();
  map_y_ = api_->taille_ville_y();

  // Initialize SDL window and the main scrollable surface.
  win_.init();
  vscreen_ = new VirtualScrollableSurface("Screen",
                                          win_.getInput(),
                                          win_.getScreen().getSize(),
                                          Point(map_x_ * case_size_, map_y_ * case_size_));
  win_.getScreen().addChild(vscreen_);

  // Initialize labels on the right-top.
  txt_date_ = TextSurface("Vera.ttf", 200, 22);
  txt_date_.setPos(590, 10);
  txt_date_.setZ(8);
  win_.getScreen().addChild(&txt_date_);

  txt_fuel_ = TextSurface("Vera.ttf", 200, 22);
  txt_fuel_.setPos(590, 32);
  txt_fuel_.setZ(8);
  win_.getScreen().addChild(&txt_fuel_);
  
  for (int i = 0; i < api_->equipes(); i++)
    {
      txt_score_[i] = TextSurface("Vera.ttf", 200, 22);
      txt_score_[i].setPos(590, 54 + i * 22);
      txt_score_[i].setZ(8);
      win_.getScreen().addChild(&txt_score_[i]);
    }
}

OutAGui::~OutAGui()
{
  delete vscreen_;
}

void OutAGui::init()
{
  // init background
  for (int x = 0; x < map_x_; x++)
    for (int y = 0; y < map_y_; y++)
      initMapSquare(x, y);

  // init objects
  delorean_.load("/prolo2006/texture_2D/delorean.png");
  delorean_.setZ(5);
  delorean_.setPos(Point(api_->position_delorean_x(), api_->position_delorean_y()));
  vscreen_->addChild(&delorean_);
  
  almanach_.load("/prolo2006/texture_2D/almanach.png");
  almanach_.setZ(6);
  almanach_.setPos(Point(api_->position_almanach_x(), api_->position_almanach_y()));
  vscreen_->addChild(&almanach_);
  
  for (int team_id = 0; team_id < api_->equipes(); team_id++)
    for (int unit_id = 0; unit_id < MAX_GOODMEN; unit_id++)
      {
        Sprite& o = unit_[team_id][unit_id];
        o.load(players[team_id % nb_player]);
        o.setZ(4);
        o.hide();
        vscreen_->addChild(&o);
      }
}

// Set the surface for all map square.
void OutAGui::initMapSquare(int x, int y)
{
  Square& sq = map_[x][y];
  
  switch (api_->terrain_graphique(x, y))
    {
    case STREET:
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
        sq.load(streets[street_around]);
      }
      break;

    case BUILDING:
      sq.load(buildings[rand() % nb_building]);
      break;

    case BET_PLACE:
      sq.load(building_bet);
      break;

    case GRASSY_AREA:
    default:
      sq.load(squares[rand() % nb_square]);
      break;
    }

  vscreen_->addChild(&sq);
  sq.setGameInfo(api_, Point(x, y));
  sq.setPos(Point(x * case_size_, y * case_size_));
  sq.setZ(1);
}

void OutAGui::setZoom(int dec)
{
  double zoom = vscreen_->getZoom();

  if (dec > 0 && zoom < 7)
    zoom++;
  else if (dec < 0 && zoom > 1)
    zoom--;
  else
    return;
  case_size_ = (int)(128 / zoom);
  vscreen_->setZoom(zoom);
  win_.clearScreen();
}

void OutAGui::refreshInfoBox()
{
  std::ostringstream os;

  int date = api_->current_date();
  os << "Date:_" << date;
  txt_date_.setText(os.str());
  os.rdbuf()->str("");

  int delorean_power = api_->autonomie_delorean();
  os << "Autonomie_delorean:_" << delorean_power;
  txt_fuel_.setText(os.str());
  os.rdbuf()->str("");

  for (int i = 0; i < api_->equipes(); i++)
    {
      int score = api_->score(i);
      os << "Score_team_" << i + 1 << ":_" << score;
      txt_score_[i].setText(os.str());
      os.rdbuf()->str("");
    }
}

void OutAGui::goodmanMove(int player_id, int unit_id, const Position& pos)
{
  LOG3("+++ goodmanMove: team " << player_id << " uid "
       << unit_id << " to " << pos);
  unit_[player_id][unit_id].move(Point(pos) * case_size_, 20.);
}

void OutAGui::goodmanChangeState(int team_id, int unit_id, int new_state)
{
  LOG3("+++ goodmanChangeState: team " << team_id << " uid "
       << unit_id << " to state " << new_state);
}

void OutAGui::goodmanLostDelorean(int team_id, int unit_id)
{
  LOG3("+++ goodmanLostDelorean: team " << team_id << " uid " << unit_id);
}

void OutAGui::goodmanGetDelorean(int team_id, int unit_id)
{
  LOG3("+++ goodmanGetDelorean: team " << team_id << " uid " << unit_id);
}

void OutAGui::goodmanLostAlmanach(int team_id, int unit_id)
{
  LOG3("+++ goodmanLostAlmanach: team " << team_id << " uid " << unit_id);
}

void OutAGui::goodmanGetAlmanach(int team_id, int unit_id)
{
  LOG3("+++ goodmanGetAlmanach: team " << team_id << " uid " << unit_id);
}

void OutAGui::deloreanMove(const Position& pos)
{
  LOG3("+++ deloreanMove: to " << pos);
  delorean_.move(Point(pos) * case_size_, 20.);
}

void OutAGui::almanachMove(const Position& pos)
{
  LOG3("+++ almanachMove: to " << pos);
  almanach_.move(Point(pos) * case_size_, 20.);
}



int OutAGui::run()
{
  Input& input = win_.getInput();

  // This class will handle events.
  api_->setEventHandler(this);

  // Init background, objects, ...
  init();

  while (api_->getState() != GS_END)
    {
      // Bye bye dude.
      if (input.key_pressed_[(unsigned char)'q'])
        return 0;
 
      // Say we are ready for the next turn.
      if (input.key_pressed_[SDLK_RETURN] || input.key_pressed_[SDLK_SPACE])
        ccx_->setReady();

      // Zoom in-out
      if (input.key_pressed_[(unsigned char)'-'])
        setZoom(1);
              
      if (input.key_pressed_[(unsigned char)'+'] || input.key_pressed_[(unsigned char)'p'])
        setZoom(-1);

      // See a specific player view.
      for (int i = 1; i < 10; i++)
        if (input.key_pressed_['0' + i])
          {
            LOG2("Switch to team id `" << i - 1 << "' view.");
            if (!api_->switchTeam(i - 1))
              LOG2("Failed. No such team ?");
          }

      // See all the map
      if (input.key_pressed_[(unsigned char)'0'])
        {
          LOG2("Switch to super-user view");
          api_->switchTeam(-1);
        }

      refreshInfoBox();

      // Show or hide delorean/almanach
      if (api_->delorean_visible())
        delorean_.show();
      else
        delorean_.hide();
      if (api_->almanach_visible())
        almanach_.show();
      else
        almanach_.hide();
        
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
extern "C" int run(xml::XMLConfig*, Api* api, ClientCx* ccx)
{
  OutAGui visu(api, ccx);
  return visu.run();
}
