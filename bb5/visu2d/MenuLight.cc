/*
** TowBowlTactics, an adaptation of the tabletop game Blood Bowl.
**
** Copyright (C) 2006 The TBT Team.
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** The TBT Team consists of people listed in the `AUTHORS' file.
*/

#include "tools.hh"
#include "SDLWindow.hh"
#include "VirtualSurface.hh"
#include "InputTextSurface.hh"
#include "Sprite.hh"
#include "Colors.hh"
#include "Game.hh"
#include "ClientApp.hh"

/*!
** @brief Implementation of a 'very light' menu.
**
** Inherit of 'ClientApp', a crappy class that does all dirty
** things a stechec client need: handle command line options,
** configuration file, opens dynamic library, connect to the server, ...
*/
class MenuLight : public ClientApp
{
public:
  MenuLight(int argc, char** argv);
  ~MenuLight();

  int init();
  void run();

private:
  void loadResources();
  void unloadResources();

  virtual void showVersion();
  virtual int showMenu();
  virtual int onPlay(bool is_replay);

  SDLWindow win_;
  VirtualSurface* screen_;
  Surface bg_;
  Sprite bg_hi_;
  TextSurface create_;
  TextSurface join_;
  TextSurface join_host_;
  TextSurface join_port_;
  InputTextSurface host_;
  InputTextSurface port_;
};



/********************************************************************************/

/* Constructor. Only pass argc and argv to ClientApp, it need them to locate xml file. */
MenuLight::MenuLight(int argc, char** argv)
  : ClientApp(argc, argv, ".tbt/tbtrc", PKGDATADIR)
{
}

MenuLight::~MenuLight()
{
}

/*
** First thing to do: create and open the SDL window.
** SDL Window is a sensible thing, it's best to have it as soon as possible.
*/
int MenuLight::init()
{
  try {
    win_.init(&cfg_);
  } catch (...) {
    LOG1("Sorry, I'm unable to open up a window... You're out of luck :/");
    return 1;
  }
  return 0;
}

/*
** Create and attach all required resources to the window
*/
void MenuLight::loadResources()
{
  // Initialize resources.
  screen_ = &win_.getScreen();
  bg_.load("image/screens/lightmenu.jpg");
  screen_->addChild(&bg_);

  bg_hi_.load("image/screens/end_btn.jpg");
  bg_hi_.splitNbFrame(2, 1);
  bg_hi_.setFrame(2);
  bg_hi_.hide();
  bg_hi_.setPos(666, 509);
  screen_->addChild(&bg_hi_);

  create_ = TextSurface("Vera.ttf", 20, 155, 35);
  create_.setBgColor(aquamarine_color);
  create_.setRenderMethod(eTextShaded);
  create_.setText("Create game");
  create_.setPos(100, 180);
  screen_->addChild(&create_);

  join_ = TextSurface("Vera.ttf", 20, 115, 35);
  join_.setBgColor(aquamarine_color);
  join_.setRenderMethod(eTextShaded);
  join_.setText("Join game");
  join_.setPos(100, 300);
  screen_->addChild(&join_);

  join_host_ = TextSurface("Vera.ttf", 16, 150, 30);
  join_host_.setRenderMethod(eTextShaded);
  join_host_.setText("host:");
  join_host_.setPos(270, 300);
  screen_->addChild(&join_host_);

  join_port_ = TextSurface("Vera.ttf", 16, 150, 30);
  join_port_.setRenderMethod(eTextShaded);
  join_port_.setText("port:");
  join_port_.setPos(270, 330);
  screen_->addChild(&join_port_);

  host_ = InputTextSurface("Vera.ttf", 120);
  host_.setText(cfg_.getAttr<std::string>("client", "connect", "host"));
  host_.setPos(330, 300);
  screen_->addChild(&host_);

  port_ = InputTextSurface("Vera.ttf", 70);
  port_.setText(cfg_.getAttr<std::string>("client", "connect", "port"));
  port_.setPos(330, 330);
  screen_->addChild(&port_);
}

/*
** Destroy all graphical resources used by the menu, called
** before entering in the game. This has 2 effects:
**  - remove objects from the screen Virtualsurface, don't want
**    some artifacts on the game field.
**  - free some memory before loading game data... hum, not sure
**    it is memory leak free :)
** This function is needed because I use the same SDL engine for menu
** and game, otherwise, you don't need to do that.
*/
void MenuLight::unloadResources()
{
  bg_.free();
  bg_hi_.free();
  create_.free();
  join_.free();
  join_host_.free();
  join_port_.free();
  host_.free();
  port_.free();
}

/*
** This function is called right after CLientApp::runApp(),
**
** In this function you need to kindly display to the user a way
** to choose configuration (in other word, a menu), and modify xml
** configuration object 'cfg_' accordingly.
**
** To start a game, simply return 0.
** To exit the program without throwing error, return < 0.
** To exit the program with error, return > 0.
*/
int MenuLight::showMenu()
{
  static bool first = true;
  if (!first)
    return -1;
  first = false;
  
  // As a special case, if the option 'connect_on_startup' is on in the configuration
  // file, don't even jump in menu loop, 
  if (cfg_.getAttr<bool>("client", "connect", "connect_on_startup"))
   return 0;

  loadResources();
  
  Input& inp = win_.getInput();

  // Main loop, for each menu frame.
  while (true)
    {
      if (win_.processOneFrame())
        return 1;

      // Quit. Return negative value.
      if (bg_hi_.getRect().inside(inp.mouse_))
        bg_hi_.show();
      else
        bg_hi_.hide();
      if (win_.getInput().key_pressed_[SDLK_q] ||
          bg_hi_.getRect().inside(inp.mouse_) && inp.button_pressed_[1])
        return -1;

      // Highlights
      if (join_.getRect().inside(inp.mouse_))
        join_.setBgColor(darkcyan_color);
      else
        join_.setBgColor(aquamarine_color);
      if (create_.getRect().inside(inp.mouse_))
        create_.setBgColor(darkcyan_color);
      else
        create_.setBgColor(aquamarine_color);
      
      // Create game
      if (create_.getRect().inside(inp.mouse_) && inp.button_pressed_[1])
        {
          LOG1("Create game...");
          cfg_.setAttr<std::string>("client", "connect", "host", "localhost");

          // FIXME: fork a server
          unloadResources();
          return 0;
        }

      // Join game
      if (join_.getRect().inside(inp.mouse_) && inp.button_pressed_[1])
        {
          LOG1("Join game...");
          unloadResources();
          return 0;
        }

      // Inputboxes
      if (host_.getRect().inside(inp.mouse_) && inp.button_pressed_[1])
        host_.acquireInput("host");
      if (host_.isAcquireFinished())
        {
          cfg_.setAttr<std::string>("client", "connect", "host", host_.getText());
          host_.resetAcquire();
        }
      if (port_.getRect().inside(inp.mouse_) && inp.button_pressed_[1])
        port_.acquireInput("port");
      if (port_.isAcquireFinished())
        {
          cfg_.setAttr<std::string>("client", "connect", "port", port_.getText());
          port_.resetAcquire();
        }
    }

  // Should never go here.
  return 21;
}

/*
** This function is called when a game is ready to begin.
** Just load the game gui with the required parameters, and
** run it.
** It will return from the 'run' function whenever the user want
** to return to the menu, then 'showMenu' will be called again, ...
*/
int MenuLight::onPlay(bool)
{
  sdlvisu::Game padaboum(win_, &cfg_, rules_->getApi(), &ccx_);
  return padaboum.run();
}

/* Show binary version, if --version is given to the command line. */
void MenuLight::showVersion()
{
  std::cout << "TowBowlTactics client v" PACKAGE_VERSION << "\n"
            << "Copyright (C) 2006 TBT Team.\n";
}

/********************************************************************/

/* And here is our shiny main() function ! */
int main(int argc, char** argv)
{
  MenuLight m(argc, argv);

  if (m.init())
    return 1;
  return m.runApp();
}
