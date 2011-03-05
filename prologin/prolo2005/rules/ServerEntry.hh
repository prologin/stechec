/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2005, 2006 Prologin
*/

#ifndef SERVERENTRYPOINT_HH_
# define SERVERENTRYPOINT_HH_

# include "Contest.hh"
# include "Server.hh"

/*!
** This class provides hooks for, either for real player or for visio.
**
** You can:
**  - overload any virual method you wants
**  - send message to clients: void SendToAll(const StechecPkt*)
**  - access to the GameData class: _g
**  - access to argc and argv, given on the command line for server
**
** Note:
**  - prolo_after_game() (where you usually free memory) is called only
**    if prolo_before_game() was called.
*/
class ServerEntry: public StechecServerEntry
{

public:

  ServerEntry(GameData* game, Server* server, const ConfSection* cfg);
  virtual ~ServerEntry();

  // Different entry point
  virtual int   beforeGame();
  virtual int   initGame();
  virtual int   beforeNewTurn();
  virtual int   afterNewTurn();
  virtual int   afterGame();

  // Indique la fin du jeu
  virtual bool  isMatchFinished();

  // Recupere le score
  virtual int   getScore(int uid);

private:
  int           ParseOptions();
  int           LoadMap(const std::string& map_file);
  void          coleoptere_end_turn(Coleoptere* col);
  void          factory_end_turn(Factory* f);
  std::string   mapFile;
};

#endif /* !SERVERENTRYPOINT_HH_ */
