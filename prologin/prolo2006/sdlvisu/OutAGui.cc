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



OutAGui::OutAGui(Api* api, ClientCx* ccx)
  : api_(api),
    ccx_(ccx),
    map_x_(1),
    map_y_(1),
    vscreen_(NULL),
    info_box_(NULL),
    screen_(NULL)
{
  map_x_ = api_->taille_ville_x();
  map_y_ = api_->taille_ville_y();
}

OutAGui::~OutAGui()
{
  delete vscreen_;
  delete info_box_;
}

bool OutAGui::initSDL()
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
      ERR("Error in initialisation of the SDL : " << SDL_GetError());
      return false;
    }
  if (TTF_Init() < 0)
    {
      ERR("Error in initialisation of TTF : " << TTF_GetError());
      return false;
    }
  screen_ = SDL_SetVideoMode(800, 640, 0, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_ANYFORMAT);
  if (screen_ == NULL)
    {
      ERR("Unable to activate graphic mode : " << SDL_GetError());
      return false;
    }

  SDL_WM_SetCaption("OutAGui", NULL);
  SDL_WM_GrabInput(SDL_GRAB_OFF);

  vscreen_ = new VirtualScreen(screen_, 128 * map_x_, 128 * map_y_);
  info_box_ = new InfoBox(api_);
  
  LOG4("SDL initialization successful.");
  return true;
}

void OutAGui::init()
{
  for (int x = 0; x < map_x_; x++)
    for (int y = 0; y < map_y_; y++)
      {
        map_[x][y].setScreen(vscreen_->getScreen());
        map_[x][y].setGameInfo(api_, Point(x, y));
        initMapSquare(x, y);
      }

  // init objects
  delorean_.setScreen(vscreen_->getScreen());
  delorean_.setSurf(resource_.getImage(delorean));
  almanach_.setScreen(vscreen_->getScreen());
  almanach_.setSurf(resource_.getImage(almanach));
  for (int team_id = 0; team_id < api_->equipes(); team_id++)
    for (int unit_id = 0; unit_id < MAX_GOODMEN; unit_id++)
      {
        SDLObject& o = unit_[team_id][unit_id];
        o.setScreen(vscreen_->getScreen());
        o.setSurf(resource_.getImage(players[team_id % nb_player]));
      }
  
  setZoom(0);
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
        sq.setSurf(resource_.getImage(streets[street_around]));
      }
      break;

    case BUILDING:
      sq.setSurf(resource_.getImage(buildings[rand() % nb_building]));
      break;

    case BET_PLACE:
      sq.setSurf(resource_.getImage(building_bet));
      break;

    case GRASSY_AREA:
    default:
      sq.setSurf(resource_.getImage(squares[rand() % nb_square]));
      break;
    }
}

void OutAGui::setZoom(int dec)
{
  int zoom = vscreen_->getZoom();

  if (dec > 0 && zoom < 7)
    zoom++;
  else if (dec < 0 && zoom > 1)
    zoom--;
  else if (dec != 0)
    return;
  
  vscreen_->setZoom(zoom);
  for (int x = 0; x < map_x_; x++)
    for (int y = 0; y < map_y_; y++)
      {
        Point pt(x, y);
        map_[x][y].setPos(pt * (128 / zoom));
        map_[x][y].setZoom(zoom);
      }

  drawBG();

  Point pos_delorean(api_->position_delorean_x(), api_->position_delorean_y());
  Point pos_almanach(api_->position_almanach_x(), api_->position_almanach_y());

  delorean_.setZoom(zoom);
  delorean_.setPos(pos_delorean * (128 / zoom));
  almanach_.setZoom(zoom);
  almanach_.setPos(pos_almanach * (128 / zoom));

  for (int team_id = 0; team_id < api_->equipes(); team_id++)
    {
      for (int unit_id = 0; unit_id < MAX_GOODMEN; unit_id++)
        {
          SDLObject& o = unit_[team_id][unit_id];
          Point pos_unit(api_->position_joueur_x(team_id * 3 + unit_id),
                         api_->position_joueur_y(team_id * 3 + unit_id));
          o.setZoom(zoom);
          LOG2("place unit " << unit_id << " at " << pos_unit);
          o.setPos(pos_unit * (128 / zoom));
        }
    }

  draw();
}

void OutAGui::moveGoodman(int player_id, int unit_id, const Position& pos)
{
  LOG3("++++++++ goodman: move team: " << player_id << " uid " << unit_id << " to " << pos);
  unit_[player_id][unit_id].move(Point(pos) * (128 / vscreen_->getZoom()));
}


// Redraw all the screen.
bool OutAGui::drawBG()
{
  bool have_to_blit = false;

  // 1. Print background changes on the virtual screen.
  for (int x = 0; x < map_x_; x++)
    for (int y = 0; y < map_y_; y++)
      have_to_blit = map_[x][y].update() || have_to_blit;
  return have_to_blit;
}

void OutAGui::draw()
{ 
  bool have_to_blit = drawBG();

  // 2. Pre-Update objects position. (restore background if they were moving)
  delorean_.preUpdate();
  almanach_.preUpdate();

  for (int team_id = 0; team_id < api_->equipes(); team_id++)
    {
      for (int unit_id = 0; unit_id < MAX_GOODMEN; unit_id++)
        {
          SDLObject& o = unit_[team_id][unit_id];
          Point pos_unit(api_->position_joueur_x(team_id * 3 + unit_id),
                         api_->position_joueur_y(team_id * 3 + unit_id));
          o.preUpdate();
        }
    }

  // 3. Update objects position.
  for (int team_id = 0; team_id < api_->equipes(); team_id++)
    {
      for (int unit_id = 0; unit_id < MAX_GOODMEN; unit_id++)
        {
          SDLObject& o = unit_[team_id][unit_id];
          Point pos_unit(api_->position_joueur_x(team_id * 3 + unit_id),
                         api_->position_joueur_y(team_id * 3 + unit_id));
          have_to_blit = o.update(have_to_blit);
        }
    }

  have_to_blit = delorean_.update(have_to_blit);
  have_to_blit = almanach_.update(have_to_blit);

  // 4. Update the virtual screen (scroll)
  have_to_blit = vscreen_->update(have_to_blit);

  // 5. Update info box, print directly on the screen.
  have_to_blit = info_box_->update(have_to_blit);

  // Last. Flip.
  if (have_to_blit)
    SDL_Flip(screen_);
}

int OutAGui::run()
{
  // This class will handle events.
  api_->setEventHandler(this);

  if (!initSDL())
    return 1;
  init();

  while (api_->getState() != GS_END)
    {
      // Process events
      SDL_Event event;
      while (SDL_PollEvent(&event))
        {
          switch (event.type)
            {
            case SDL_QUIT:
              return 0;
               
            case SDL_KEYDOWN:
              // Bye dude.
              if (event.key.keysym.sym == 'q')
                return 0;
 
              // Say we are ready for the next turn.
              if (event.key.keysym.sym == SDLK_RETURN
                  || event.key.keysym.sym == SDLK_SPACE)
                ccx_->setReady();

              // Zoom in-out
              if (event.key.keysym.sym == '-')
                setZoom(1);
              
              if (event.key.keysym.sym == '+' || event.key.keysym.sym == 'p')
                setZoom(-1);

              // Test
              if (event.key.keysym.sym == 'b')
                delorean_.move(Point(800, 300));
              
              // See a specific player view.
              if (event.key.keysym.sym >= '1' && event.key.keysym.sym <= '9')
                {
                  int team_id = event.key.keysym.sym - '1';
                  LOG2("Switch to team id `" << team_id << "' view.");
                  if (!api_->switchTeam(team_id))
                    LOG2("Failed. No such team ?");
                }

              // See all the map
              if (event.key.keysym.sym == '0')
                {
                  LOG2("Switch to super-user view");
                  api_->switchTeam(-1);
                }
              break;

            case SDL_MOUSEMOTION:
              vscreen_->mouseMotion(event.button.x, event.button.y);
              break;

            default:
              break;
            }
        }

      // Process any incoming messages. Block at most 50 ms.
      while (ccx_->process(true))
        ;

      draw();
    }
  return 0;
}

// Library entry point.
extern "C" int run(xml::XMLConfig*, Api* api, ClientCx* ccx)
{
  OutAGui visu(api, ccx);
  return visu.run();
}
