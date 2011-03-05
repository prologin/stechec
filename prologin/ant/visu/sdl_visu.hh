
#ifndef SDLVISU_HH_
# define SDLVISU_HH_

# include <SDL.h>
# include <SDL_image.h>

# include "tools.hh"
# include "ClientCx.hh"
# include "Api.hh"
# include "Event.hh"

class SDLVisu : public AntEvent
{
public:
  SDLVisu(Api* api, ClientCx* ccx);

  int run();

private:

  // Callback events, from AntEvent
  virtual void  mapContent(const Position& pos, int map_content);
  virtual void  antMove(int coach_id, int id, const Position&);
  virtual void  antGetFood(int coach_id, int id);
  virtual void  antLetFood(int coach_id, int id);

  Api*          api_;
  ClientCx*     ccx_;
  bool          redraw_;

  // SDL specifics functions an variables.
  bool          initSDL();
  SDL_Surface*  load_image(const std::string& filename);
  bool          drawSquare(int x, int y);
  bool          drawAll();
    
  SDL_Surface*  screen;
  SDL_Surface*  anthill;
  SDL_Surface*  food;
  SDL_Surface*  ground;
  SDL_Surface*  rock;
  SDL_Surface*  ant[2];
};

#endif /* !SDLVISU_HH_ */
