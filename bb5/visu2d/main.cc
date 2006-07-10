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

const string icon = ADD_IMG_PATH ("general/tbt.ico");
const string file = ADD_IMG_PATH ("screens/title_bg.jpg");

vector <DialogBox*> list_box;
Menu* menu;

//Replace by real player info
struct Player
{
  string playername;
  string ip;
} player;

// Parse xml configuration file.
static bool parse_config (const char *opt_file, xml::XMLConfig & cfg)
{
  try
  {
    if (opt_file != NULL)
      cfg.parse (opt_file);
    else
      cfg.parse ("");
  }
  catch (const xml::XMLError &)
  {
    ERR ("Sorry, I can't go further without a working configuration file...");
    return false;
  }
  return true;
}

    // Set some basic settings based on XML config file.
static void set_opt(const char *section, xml::XMLConfig & cfg, Log & log)
{
  cfg.switchSection (section);
  log.setVerboseLevel (cfg.getAttr <int>("debug", "verbose"));
  log.setPrintLoc (cfg.getAttr <bool> ("debug", "printloc"));
}

// To clean and exit
void Exit(void)
{
      // clean 
  SDL_FreeSurface (background);
  SDL_FreeSurface (screen);
      //clean list
  list_box.clear ();
      //quit SDL
  SDL_Quit ();
  LOG1 ("Bye bye");
}

    // Init SDL
bool initSDL (void)
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    ERR("Error in initialisation of the SDL : " << SDL_GetError ());
    return false;
  }

  SDL_WM_SetIcon(IMG_Load (icon.c_str ()), NULL);
  screen = SDL_SetVideoMode (SCREEN_WIDTH_MIN, SCREEN_HEIGHT_MIN, SCREEN_BPP, SDL_SWSURFACE);

  if(screen == NULL)
  {
    ERR("Impossible to activate graphic mode : " << SDL_GetError ());
    return false;
  }

  SDL_WM_SetCaption("TBT", NULL);
      // Mouse an keyboard are confined to the application window
  SDL_WM_GrabInput(SDL_GRAB_OFF);
      //Enables Unicode keyboard translation.
  SDL_EnableUNICODE(1);
  return true;
}

void addDialog(DialogBox * box)
{
  list_box.push_back(box);
}

void MainLoop(void)
{
  SDL_Event event;
  bool quit = false;
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
          if(keysym->unicode<127 and keysym->unicode>=32)
          {
                // ASCII character
            char ch = (keysym->unicode & 0x7F);
            ushort index=focus->get_index();
            focus->add_char(ch);
            focus->set_index(++index);
            focus->refresh();   
          }
          else
          {
            switch(keysym->sym)
            {
              case SDLK_DELETE:
              {
                focus->delete_char();
                focus->refresh();
                break;
              }
              case SDLK_BACKSPACE:
              {
                ushort index = focus->get_index();
                if(index > 0)
                {
                  focus->set_index(--index);
                  focus->delete_char();
                  focus->refresh();
                }
                break;
              }
              case SDLK_LEFT:
              {
                ushort index=focus->get_index();
                if(index > 0)
                {
                  focus->set_index(--index);
                  focus->refresh();
                }
                break;
              }
              case SDLK_RIGHT:
              {
                ushort index=focus->get_index();
                focus->set_index(++index);
                focus->refresh(); 
                break;
              }
              case SDLK_RETURN:
              {
                    // hmm seems KMOD_MODE is Alt Gr key !
                    // we need to ignore 0x1000 (KMOD_NUM) and 0x2000 (KMOD_CAPS)
                ushort ModState = (SDL_GetModState() & 0x4FFF);
                if (ModState == KMOD_LALT || ModState == KMOD_MODE)
                {
                  SDL_WM_ToggleFullScreen(screen);
                }
                break;
              }
            }
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
                  ERR ("Error in creation of Menu : " << error.getMessage ());
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
                  ERR ("Error in creation of Menu : " << error.getMessage ());
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
                  ERR("Error in creation of the menu : " << error.getMessage ());
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

bool initBackground (void)
{
  background = LoadImage (file, 0);
  if (background == NULL)
  {
    ERR("Error in initialisation of the background : " << SDL_GetError());
    return false;
  }
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
    ERR("Error in initialisation of the background : " << SDL_GetError());
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
    ERR("Error in intialisation of the SDL_TTF : " << SDL_GetError());
    return false;
  }
  return true;
}

int main (int argc, char *argv[])
{
  Log log_client(5);
  xml::XMLConfig cfg;

  if (initSDL() == false)
    return -1;
  LOG1("Init SDL .. ok");
  if (initTTF() == false)
    return -1;
  LOG1("Init SDL_ttf .. ok");
  if (initBackground() == false)
    return -1;
  LOG1("Init background .. ok");

      //display a menu to test it
  try
  {
    menu = new Menu (5, 5, screen, MENU_WIDTH, "MENU");
  }
  catch (GUIError & error)
  {
    ERR ("Error in creation of Menu : " << error.getMessage ());
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
    ERR("Error in creation of a DialogBox : " << error.getMessage());
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
