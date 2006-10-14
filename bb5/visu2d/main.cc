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
#include "xml/xml_config.hh"
#include "SDLWindow.hh"
#include "ResourceCenter.hh"

#include "Global.hh"
#include "DialogBox.hh"
#include "GuiError.hh"
#include "Label.hh"
#include "Entry.hh"
#include "LabelEntry.hh"
#include "Menu.hh"

using namespace std;

//The attributes of the window
const unsigned int SCREEN_WIDTH_MIN = 800;
const unsigned int SCREEN_HEIGHT_MIN = 600;
const unsigned int SCREEN_BPP = 32;

//The surfaces that will be used
SDL_Surface *background = NULL;
SDL_Surface *screen = NULL;

vector <DialogBox*> list_box;
Menu* menu;

//Replace by real player info
struct Player
{
  string playername;
  string ip;
} player;


struct CmdLineOption
{
  CmdLineOption()
    : config_file(""),
      client_gid(1)
  {}

  char* config_file;  ///< Optionnal configuration file to load.
  int   client_gid;   ///< Client game id, as stored in meta-data.
};

// Very basic command line manager. We don't need anything more powerful.
static void parse_option(int argc, char** argv, CmdLineOption& opt)
{
  if (argc >= 2 && (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")))
    {
      cout << "usage: " << argv[0] << " [client_id] [config-file]\n";
      exit(0);
    }
  if (argc >= 2 && (!strcmp(argv[1], "--version") || !strcmp(argv[1], "-v")))
    {
      cout << "TowBowlTactics visu2D v" PACKAGE_VERSION << "\n";
      cout << "Copyright (C) 2006 TBT Team.\n";
      exit(0);
    }

  if (argc >= 2)
    {
      char* endptr;
      int client_gid = strtol(argv[1], &endptr, 10);
      if (*endptr == 0)
        {
          opt.client_gid = client_gid;
          if (argc >= 3)
            opt.config_file = argv[2];
        }
      else
        opt.config_file = argv[1];
    }
}

// Parse xml configuration file.
static void parse_config(const CmdLineOption& opt, xml::XMLConfig& cfg)
{
  try {
    cfg.parse(opt.config_file);
  } catch (const xml::XMLError& e) {
    ERR("Sorry, I can't go further without a working configuration file...");
    exit(3);
  }
}

// Set some basic settings based on XML config file.
static void set_opt(const CmdLineOption& opt, xml::XMLConfig& cfg, Log& log)
{
  cfg.switchClientSection(opt.client_gid);
  log.setVerboseLevel(cfg.getAttr<int>("client", "debug", "verbose"));
  log.setPrintLoc(cfg.getAttr<bool>("client", "debug", "printloc"));
}



// To clean and exit
void Exit(void)
{
  // clean 
  SDL_FreeSurface (background);
  list_box.clear ();
  LOG1 ("Bye bye");
}

void addDialog(DialogBox * box)
{
  list_box.push_back(box);
}

void MainLoop(void)
{
  SDL_Event event;
  bool quit = false;
  SDL_EnableUNICODE(1);
  while (SDL_WaitEvent(&event) && quit == false)
  {				/* Loop until there are no events left on the queue */
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
            SDL_WM_ToggleFullScreen(screen);
          }
        }
        break;
      }
      case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT)
        {
              // All DialogBox
          for (vector <DialogBox*>::iterator box = list_box.begin ();
               box != list_box.end (); ++box)
          {
            int res = (*box)->mousebuttondown(event.button.x, event.button.y);
            if (res > 0)
            {
              switch (res)
              {
                case CLOSE:
                  delete(*box);
                  list_box.erase(box);
                  break;
              }
              break;
            }
          }
              // Menu
          int res = menu->mousebuttondown(event.button.x, event.button.y);
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
                menu->erase();
                delete menu;
                try
                {
                  menu = new Menu(5, 5, screen, MENU_WIDTH, "MENU");
                }
                catch(GUIError & error)
                {
                  ERR ("Error in creation of Menu : %1", error.getMessage());
                  quit = true;
                  break;
                }
                menu->draw ();
                menu->addWidget(new TextButton(0, 0, MENU_WIDGET_WIDTH, MENU_WIDGET_HEIGHT, screen, menu, "NETWORK GAME", NETWORK));
                menu->addWidget(new TextButton(0, 0, MENU_WIDGET_WIDTH, MENU_WIDGET_HEIGHT, screen, menu, "LOCAL GAME", STANDALONE));
                menu->addWidget(new TextButton(0, 0, MENU_WIDGET_WIDTH, MENU_WIDGET_HEIGHT, screen, menu, "SERVER", SERVER));
                menu->addWidget(new TextButton(0, 0, MENU_WIDGET_WIDTH, MENU_WIDGET_HEIGHT, screen, menu, "OPTIONS", OPTIONS));
                menu->addWidget(new TextButton(0, 0, MENU_WIDGET_WIDTH, MENU_WIDGET_HEIGHT, screen, menu, "EXIT", EXIT));
                menu->refresh ();
                break;
              }
              case NETWORK:
              {
                menu->erase();
                delete menu;
                try
                {
                  menu = new Menu(5, 5, screen, (2 * MENU_WIDGET_WIDTH) + 15, MENU_WIDGET_HEIGHT + 10,"NETWORK GAME");
                }
                catch (GUIError & error)
                {
                  ERR ("Error in creation of Menu : %1", error.getMessage());
                  quit = true;
                  break;
                }
                menu->draw();
                menu->addWidget(new TextButton(MENU_WIDGET_WIDTH, MENU_WIDGET_HEIGHT, screen, menu, "Back", BACK),
                                (menu->getwidth() - ((2 * MENU_WIDGET_WIDTH) + menu->getpadx())) / 2, 
                                (menu->getheight() - MENU_WIDGET_HEIGHT - menu->getpady()));
                menu->addWidget(new TextButton(MENU_WIDGET_WIDTH, MENU_WIDGET_HEIGHT, screen, menu, "Ok", OK),
                                (menu->getwidth() + menu->getpadx()) / 2, 
                                (menu->getheight() - MENU_WIDGET_HEIGHT - menu->getpady()));
                menu->refresh();
                break;
              }
              case OPTIONS:
              {
                menu->erase();
                delete menu;
                try
                {
                  menu = new Menu(5, 5, screen, MENU_WIDTH, "OPTIONS");
                }
                catch(GUIError & error)
                {
                  ERR("Error in creation of the menu : %1", error.getMessage());
                  quit = true;
                  break;
                }
                menu->draw();
                menu->addWidget(new LabelEntry(0, 0, 2 * MENU_WIDGET_WIDTH, MENU_WIDGET_HEIGHT, screen, menu, "Nom du joueur", &player.playername));
                menu->addWidget(new TextButton(MENU_WIDGET_WIDTH, MENU_WIDGET_HEIGHT, screen, menu, "Back", BACK));
                menu->refresh();
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
          for (vector <DialogBox*>::iterator box = list_box.begin();
               box != list_box.end(); ++box)
          {
            if ((*box)->mousebuttonup() == 1)
              break;
          }
        }
        break;
      case SDL_MOUSEMOTION:
            // WARNING CLIPING BUG HERE !!
        menu->mousemotion(event.button.x, event.button.y);
        for (vector<DialogBox*>::iterator box = list_box.begin();
             box != list_box.end(); ++box)
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

bool initBackground(void)
{
  Surface s = ResourceCenter::getInst()->getImage("image/screens/title_bg.jpg");
  background = s.getSDLSurface();
  // At the end of the scope, 's' will be freed (not really, it is in cache,
  //    but _may_ be freed).
  // Explicitly give ownership to 'background'.
  background->refcount++;

  SDL_Rect *srect = NULL;
  uint bgw = background->w;
  uint bgh = background->h;
  uint sw = screen->w;
  uint sh = screen->h;
  if (bgw < sw || bgh < sh)
  {
    srect = new SDL_Rect;
    srect->w = bgw;
    srect->h = bgh;
    srect->x = (sw - bgw) / 2;
    srect->y = (sh - bgh) / 2;
  }
      //Blit the surface
  if (SDL_BlitSurface (background, NULL, screen, srect) != 0)
  {
    ERR("Error in initialisation of the background : %1", SDL_GetError());
    return false;
  }
  SDL_Flip (screen);
  delete (srect);
  return true;
}

bool initTTF (void)
{
  if (TTF_Init () < 0)
  {
    ERR("Error in intialisation of the SDL_TTF : %1", SDL_GetError());
    return false;
  }
  return true;
}

int main (int argc, char *argv[])
{
  Log log_client(5);
  xml::XMLConfig cfg;
  CmdLineOption opt;
  int ret_value = 1;
 
  // FIXME: for now, we don't need 'cfg'. But it will change.
  //parse_option(argc, argv, opt);
  //parse_config(opt, cfg);
  //set_opt(opt, cfg, log_client);

  SDLWindow win(&cfg);
  win.init();

  screen = win.getScreen().getSDLSurface();
  LOG1("Init SDL Window ok.");
  
  if (initBackground() == false)
    return -1;
  LOG1("Init background .. ok.");

  //display a menu to test it
  try
  {
    menu = new Menu (5, 5, screen, MENU_WIDTH, "MENU");
  }
  catch (GUIError & error)
  {
    ERR ("Error in creation of Menu : %1", error.getMessage());
    return -1;
  }
  menu->draw ();
  menu->addWidget(new TextButton(MENU_WIDGET_WIDTH, MENU_WIDGET_HEIGHT, screen, menu, "NETWORK GAME", NETWORK));
  menu->addWidget(new TextButton(MENU_WIDGET_WIDTH, MENU_WIDGET_HEIGHT, screen, menu, "LOCAL GAME", STANDALONE));
  menu->addWidget(new TextButton(MENU_WIDGET_WIDTH, MENU_WIDGET_HEIGHT, screen, menu, "SERVER", SERVER));
  menu->addWidget(new TextButton(MENU_WIDGET_WIDTH, MENU_WIDGET_HEIGHT, screen, menu, "OPTIONS", OPTIONS));
  menu->addWidget(new TextButton(MENU_WIDGET_WIDTH, MENU_WIDGET_HEIGHT, screen, menu, "EXIT", EXIT));
  menu->refresh ();

      // display a DialogBox to test it
  DInfo *infobox;
  try
  {
    infobox = new DInfo(screen,
                        "Bienvenue dans TBT\n\nAttention !!\nce programe est\nen développement\n");
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
  Exit();
  return EXIT_SUCCESS;
}
