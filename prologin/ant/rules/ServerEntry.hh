/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2004, 2005, 2006 Prologin
*/

#ifndef SERVERENTRYPOINT_HH_
# define SERVERENTRYPOINT_HH_

# include "Contest.hh"

/*!
** @ingroup stechec_ant
** @{
*/

/*!
** @brief Ant ServerEntry.
*/
class ServerEntry: public StechecServerEntry
{

public:

  ServerEntry(GameData* game, Server* server, const ConfSection* cfg);
  virtual ~ServerEntry();

  // Different entry point
  virtual int        beforeGame();
  virtual int        initGame();
  virtual int        beforeNewTurn();
  virtual int        afterNewTurn();
  virtual int        afterGame();

  // Indique la fin du jeu
  virtual bool       isMatchFinished();

  // Recupere le score
  virtual int        getScore(int uid);

private:
  int                LoadMap(const char *map_file);
  void               InitPlayer();
  int                ParseOptions();
  std::string        mapFile;
  int                maxTurn;
  int                max_map_player;
};

#endif /* !SERVERENTRYPOINT_HH_ */
