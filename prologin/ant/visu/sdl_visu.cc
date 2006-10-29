
#include "sdl_visu.hh"

SDLVisu::SDLVisu(Api* api, ClientCx* ccx)
  : api_(api),
    ccx_(ccx),
    screen(NULL)
{
}

SDL_Surface *SDLVisu::load_image(const std::string& filename) 
{ 
  SDL_Surface* loadedImage = NULL;
  SDL_Surface* optimizedImage = NULL;

  LOG(3, "Load " << PKGDATADIR "texture_2D/" << filename);
  loadedImage = IMG_Load((std::string(PKGDATADIR "texture_2D/") + filename).c_str());
  if (loadedImage == NULL)
    throw filename;

  optimizedImage = SDL_DisplayFormatAlpha(loadedImage);
  SDL_FreeSurface(loadedImage);
  if (optimizedImage == NULL)
    throw filename;
  return optimizedImage;
}


bool SDLVisu::initSDL(void)
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
      ERR("Error in initialisation of the SDL: %1", SDL_GetError());
      return false;
    }

  screen = SDL_SetVideoMode(800, 640, 0, SDL_SWSURFACE);
  if (screen == NULL)
    {
      ERR("Unable to activate graphic mode: ", SDL_GetError());
      return false;
    }
  LOG4("SDL initialization successful.");

  SDL_WM_SetCaption("Prologin Ant visu", NULL);
  // Mouse an keyboard are confined to the application window
  SDL_WM_GrabInput(SDL_GRAB_OFF);

  anthill = load_image("fourmiliere.png");
  food = load_image("bouffe.png");
  rock = load_image("rocher.png");
  ground = load_image("sol.png");
  ant[0] = load_image("0000.png");
  ant[1] = load_image("0001.png");

  return true;
}

bool SDLVisu::drawSquare(int x, int y)
{
  int blit_res = 0;
  // FIXME: size of the screen !
  SDL_Rect srcrect = {0, 0, 32, 32};
  SDL_Rect dstrect;

  dstrect.x = x * srcrect.w;
  dstrect.y = y * srcrect.h;

  switch (api_->get_land_type(x, y))
    {
    case FOOD:
    case NOTHING:
      blit_res = SDL_BlitSurface(ground, &srcrect, screen, &dstrect);
      if (blit_res == 0 && api_->get_land_type(x, y) == FOOD)
        blit_res = SDL_BlitSurface(food, &srcrect, screen, &dstrect);
      break;

    case OBSTACLE:
      blit_res = SDL_BlitSurface(rock, &srcrect, screen, &dstrect);
      break;

    case ANTHILL:
      blit_res = SDL_BlitSurface(anthill, &srcrect, screen, &dstrect);
      break;
    }

  // if there is an ant to draw.
  int ant_team = api_->get_land_ant(x, y);
  if (ant_team != NO_ANT && blit_res == 0)
    blit_res = SDL_BlitSurface(ant[0], &srcrect, screen, &dstrect);

  if (blit_res != 0)
    {
      ERR("Error while blitting the surface: %1", SDL_GetError());
      return false;
    }
  return true;
}

// Redraw all the screen
bool SDLVisu::drawAll()
{
  // clear the screen.
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

  const int w = api_->get_landscape_width();
  const int h = api_->get_landscape_height();

  for (int x = 0; x < w; x++)
    for (int y = 0; y < h; y++)
      drawSquare(x, y);

  SDL_Flip(screen);
  return true;
}

void SDLVisu::mapContent(int x, int y, int)
{
  redraw_ = true;
}

void SDLVisu::newUnit()
{
}

void SDLVisu::antMove(int id, int x, int y)
{
  redraw_ = true;
}

void SDLVisu::antGetFood(int id)
{
}

void SDLVisu::antLetFood(int id)
{
}

int SDLVisu::run()
{
  // This class will handle events.
  api_->setEventHandler(this);

  if (!initSDL())
    return 1;
  if (!drawAll())
    return 1;

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

              // See a specific player view.
              if (event.key.keysym.sym >= '1' && event.key.keysym.sym <= '9')
                {
                  int team_id = event.key.keysym.sym - '1';
                  LOG(2, "Switch to team id `" << team_id << "' view.");
                  if (!api_->switchTeam(team_id))
                    LOG(2, "Failed. No such team ?");
                  drawAll();
                }

              // See all the map
              if (event.key.keysym.sym == '0')
                {
                  LOG(2, "Switch to super-user view");
                  api_->switchTeam(-1);
                  drawAll();
                }
                
              break;

            default:
              break;
            }
        }

      // Process any incoming messages. Block at most 50 ms.
      // Return if finished.
      while (ccx_->process(true))
        ;

      // If something happened, redraw.
      if (redraw_)
        {
          if (!drawAll())
            return 1;
          redraw_ = false;
        }
    }
  return 0;
}

// Library entry point.
extern "C" int run(xml::XMLConfig*, Api* api, ClientCx* ccx)
{
  SDLVisu visu(api, ccx);
  return visu.run();
}
