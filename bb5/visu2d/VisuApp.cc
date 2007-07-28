/*
** TowBowlTactics, an adaptation of the tabletop game Blood Bowl.
** 
** Copyright (C) 2007 The TBT Team.
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

#include "VisuApp.hh"

#include "DialogBox.hh"
#include "Entry.hh"
#include "Global.hh"
#include "GuiError.hh"
#include "Label.hh"
#include "LabelEntry.hh"
#include "Menu.hh"

#include "ResourceCenter.hh"
#include "SDLWindow.hh"

//
// Singleton
//

VisuApp* VisuApp::instance_ = NULL;

VisuApp* VisuApp::getInstance()
{
  if (instance_ == NULL)
    instance_ = new VisuApp();
  return instance_;
}

//
// Constructor/destructor
//

VisuApp::VisuApp()
  : logger_(5),
    cfg_(".tbt/tbtrc", PKGDATADIR)
{
  config_file_ = "";
  background_ = NULL;
  screen_ = NULL;
}

VisuApp::~VisuApp()
{
}

//
//
//

int VisuApp::run(int argc, char** argv)
{
  parseOptions(argc, argv);
  cfg_.parse(config_file_);

  SDLWindow win;
  win.init(&cfg_);
  screen_ = win.getScreen().getSDLSurface();
  LOG1("Init SDL Window ok.");
  
  if (initBackground() == false)
    return -1;
  LOG1("Init background... ok.");

  if (drawMainMenu() == false)
    return -1;

  // display a DialogBox to test it
  DInfo *infobox;
  try
  {
    infobox = new DInfo(screen_,
        "Bienvenue dans TBT\n\nAttention !\nCe programe est\nen développement.\n");
  }
  catch(GUIError & error)
  {
    ERR("Error in creation of a DialogBox : %1", error.getMessage());
    return -1;
  }
  
  addDialog(infobox);
  infobox->draw();
  
      // event loop
  MainLoop();

      // .. and exit
  cleanAndExit();
  return 0;
}

//
// Options parser
//

void VisuApp::parseOptions(int argc, char** argv)
{
  int log_level;
  int i = 1;
  while (i < argc)
    {
      if (strcmp(argv[i], "--config") == 0
          || strcmp(argv[i], "-c") == 0)
        {
          i ++;
          if (i < argc)
            config_file_ = argv[i];
          else
            {
              cout
                << "Argument missing after `" << argv[i-1] << "' option" << endl
                << "More info with `" << argv[0] << " --help'" << endl;
              exit(EXIT_FAILURE);
            }
        }
      else if (strcmp(argv[i], "--help") == 0
          || strcmp(argv[i], "-h") == 0)
        {
          showUsage(argv[0]);
          exit(EXIT_SUCCESS);
        }
      else if (strcmp(argv[i], "--log-level") == 0
          || strcmp(argv[i], "-l") == 0)
        {
          i ++;
          if (i < argc)
            {
              log_level = atoi(argv[i]);
              log_level = max(0, log_level);
              log_level = min(31, log_level);
              logger_.setVerboseLevel(log_level);
            }
          else
            {
              cout
                << "Argument missing after `" << argv[i-1] << "' option" << endl
                << "More info with `" << argv[0] << " --help'" << endl;
              exit(EXIT_FAILURE);
            }
        }
      else if (strcmp(argv[i], "--version") == 0
          || strcmp(argv[i], "-v") == 0)
        {
          showVersion();
          exit(EXIT_SUCCESS);
        }
      else
        {
          cout
            << "Unknown option: `" << argv[i] << "'" << endl
            << "More info with `" << argv[0] << " --help'" << endl;
          exit(EXIT_FAILURE);
        }
      i ++;
    }
}

void VisuApp::showUsage(char* program_name)
{
  cout
    << "Usage: " << program_name << " [options]" << endl
    << endl
    << "Available options:" << endl
    << "  -c, --config <tbtrc>     Use <tbtrc> instead of any .tbtrc" << endl
    << "  -h, --help               Display this help message and exit" << endl
    << "  -l, --log-level <level>  Set the log verbosity to <level>" << endl
    << "  -v, --version            Display version information and exit" << endl;
}

void VisuApp::showVersion()
{
  cout
    << "TowBowlTactics client version " PACKAGE_VERSION << endl
    << "Copyright (C) 2007 TBT Team." << endl;
}

void VisuApp::cleanAndExit()
{
  // Clean 
  SDL_FreeSurface (background_);
  list_box_.clear ();

  // Exit
  LOG1 ("Bye bye");
  exit (EXIT_SUCCESS);
}

//
// Menu
//

void VisuApp::MainLoop()
{
  SDL_Event event;
  bool quit = false;
  SDL_EnableUNICODE(1);
  while (SDL_WaitEvent(&event) && quit == false)
  {
    /* Loop until there are no events left on the queue */
    switch(event.type)
    {
      case SDL_QUIT:
        quit = true;
        break;
      case SDL_KEYDOWN:
      {
        SDL_keysym* keysym = &event.key.keysym;
        
        Widget* focus = (Widget::get_focus()).get_focus();
        if(focus != NULL)
        {
          focus->keydown(keysym);
        }
        if(keysym->sym == SDLK_RETURN)
        {
              // hmm seems KMOD_MODE is Alt Gr key !
              // we need to ignore 0x1000 (KMOD_NUM) and 0x2000 (KMOD_CAPS)
          ushort ModState = (SDL_GetModState() & 0x4FFF);
          if (ModState == KMOD_LALT || ModState == KMOD_MODE)
          {
            SDL_WM_ToggleFullScreen(screen_);
          }
        }
        break;
      }
      case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT)
        {
              // All DialogBox
          for (vector <DialogBox*>::iterator box = list_box_.begin ();
               box != list_box_.end (); ++box)
          {
            int res = (*box)->mousebuttondown(event.button.x, event.button.y);
            if (res > 0)
            {
              switch (res)
              {
                case CLOSE:
                  delete(*box);
                  list_box_.erase(box);
                  break;
              }
              break;
            }
          }
              // Menu
          int res = menu_->mousebuttondown(event.button.x, event.button.y);
          if (res > 0)
          {
            switch(res)
            {
              case EXIT:
              {
                quit = true;
                break;
              }
              case BACK:
              {
                menu_->erase();
                delete menu_;
                quit = !drawMainMenu();
                break;
              }
              case NETWORK:
              {
                menu_->erase();
                delete menu_;
                try
                {
                  menu_ = new Menu(5, 5, screen_, (2 * MENU_WIDGET_WIDTH) + 15, MENU_WIDGET_HEIGHT + 10,"NETWORK GAME");
                }
                catch (GUIError & error)
                {
                  ERR ("Error in creation of Menu : %1", error.getMessage());
                  quit = true;
                  break;
                }
                menu_->draw();
                menu_->addWidget(
                    new TextButton(MENU_WIDGET_WIDTH, MENU_WIDGET_HEIGHT,
                      screen_, menu_, "Back", BACK),
                    (menu_->getwidth() - ((2 * MENU_WIDGET_WIDTH) + menu_->getpadx())) / 2, 
                    (menu_->getheight() - MENU_WIDGET_HEIGHT - menu_->getpady()));
                menu_->addWidget(
                    new TextButton(MENU_WIDGET_WIDTH, MENU_WIDGET_HEIGHT,
                      screen_, menu_, "Ok", OK),
                    (menu_->getwidth() + menu_->getpadx()) / 2, 
                    (menu_->getheight() - MENU_WIDGET_HEIGHT - menu_->getpady()));
                menu_->refresh();
                break;
              }
              case OPTIONS:
              {
                menu_->erase();
                delete menu_;
                try
                {
                  menu_ = new Menu(5, 5, screen_, MENU_WIDTH, "OPTIONS");
                }
                catch(GUIError & error)
                {
                  ERR("Error in creation of the menu : %1", error.getMessage());
                  quit = true;
                  break;
                }
                menu_->draw();
                menu_->addWidget(
                    new LabelEntry(0, 0, 2 * MENU_WIDGET_WIDTH, MENU_WIDGET_HEIGHT,
                      screen_, menu_, "Coach name", &coach_info_.name));
                menu_->addWidget(
                    new TextButton(MENU_WIDGET_WIDTH, MENU_WIDGET_HEIGHT,
                      screen_, menu_, "Back", BACK));
                menu_->refresh();
                break;
              }
              case SERVER:
              {
                break;
              }
              case STANDALONE:
              {
                break;
              }
            }
            break;
          }
        }
        break;
      case SDL_MOUSEBUTTONUP:
        if (event.button.button == SDL_BUTTON_LEFT)
        {
          for (vector <DialogBox*>::iterator box = list_box_.begin();
               box != list_box_.end(); ++box)
          {
            if ((*box)->mousebuttonup() == 1)
              break;
          }
        }
        break;
      case SDL_MOUSEMOTION:
            // WARNING CLIPING BUG HERE !!
        menu_->mousemotion(event.button.x, event.button.y);
        for (vector<DialogBox*>::iterator box = list_box_.begin();
             box != list_box_.end(); ++box)
        {
          if ((*box)->mousemotion(event.button.x, event.button.y,
                                  event.motion.xrel, event.motion.yrel) == 1)
            break;
        }
      default:
        break;
    }
  }
}

bool VisuApp::initBackground()
{
  Surface s = ResourceCenter::getInst()->getImage("image/screens/title_bg.jpg");
  background_ = s.getSDLSurface();
  // At the end of the scope, 's' will be freed (not really, it is in cache,
  //    but _may_ be freed).
  // Explicitly give ownership to 'background_'.
  background_->refcount++;

  SDL_Rect *srect = NULL;
  uint bgw = background_->w;
  uint bgh = background_->h;
  uint sw = screen_->w;
  uint sh = screen_->h;
  if (bgw < sw || bgh < sh)
  {
    srect = new SDL_Rect;
    srect->w = bgw;
    srect->h = bgh;
    srect->x = (sw - bgw) / 2;
    srect->y = (sh - bgh) / 2;
  }
      //Blit the surface
  if (SDL_BlitSurface (background_, NULL, screen_, srect) != 0)
  {
    ERR("Error in initialization of the background: %1", SDL_GetError());
    return false;
  }
  SDL_Flip (screen_);
  delete (srect);
  return true;
}

bool VisuApp::initTTF ()
{
  if (TTF_Init () < 0)
  {
    ERR("Error in initialization of the SDL_TTF: %1", SDL_GetError());
    return false;
  }
  return true;
}

void VisuApp::addDialog(DialogBox* box)
{
  list_box_.push_back(box);
}

bool VisuApp::drawMainMenu()
{
  try
  {
    menu_ = new Menu (5, 5, screen_, MENU_WIDTH, "MENU");
  }
  catch (GUIError & error)
  {
    ERR ("Error in creation of Menu : %1", error.getMessage());
    return false;
  }
  menu_->draw ();
  menu_->addWidget(
      new TextButton(MENU_WIDGET_WIDTH, MENU_WIDGET_HEIGHT,
        screen_, menu_, "NETWORK GAME", NETWORK));
  menu_->addWidget(
      new TextButton(MENU_WIDGET_WIDTH, MENU_WIDGET_HEIGHT,
        screen_, menu_, "LOCAL GAME", STANDALONE));
  menu_->addWidget(
      new TextButton(MENU_WIDGET_WIDTH, MENU_WIDGET_HEIGHT,
        screen_, menu_, "SERVER", SERVER));
  menu_->addWidget(
      new TextButton(MENU_WIDGET_WIDTH, MENU_WIDGET_HEIGHT,
        screen_, menu_, "OPTIONS", OPTIONS));
  menu_->addWidget(
      new TextButton(MENU_WIDGET_WIDTH, MENU_WIDGET_HEIGHT,
        screen_, menu_, "EXIT", EXIT));
  menu_->refresh ();
  return true;
}
