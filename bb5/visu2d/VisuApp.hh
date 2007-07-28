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

#ifndef VISUAPP_HH_
# define VISUAPP_HH_

# include "tools.hh"
# include "xml/xml_config.hh"

# include <SDL.h>

class DialogBox;
class Menu;

class VisuApp
{
public:
  static VisuApp* getInstance();
  ~VisuApp();

  int run(int argc, char** argv);

private:
  VisuApp(); ///< Private constructor.
  static VisuApp* instance_; ///< Singleton.

  void parseOptions(int argc, char** argv);
  void showUsage(char* program_name);
  void showVersion();
  void cleanAndExit();

  void MainLoop();
  bool initBackground();
  bool initTTF();
  void addDialog(DialogBox* box);
  bool drawMainMenu();

  Log logger_;
  char* config_file_; ///< Custom config file name.
  xml::XMLConfig cfg_;

  // menu related
  SDL_Surface* background_;
  SDL_Surface* screen_;
  std::vector<DialogBox*> list_box_;
  Menu* menu_;
  struct CoachInfo
  {
    std::string name;
    std::string ip;
  } coach_info_;

};

#endif /* !VISAPP_HH_ */
