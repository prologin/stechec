/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2005, 2006 Prologin
*/

#ifndef STECHECSERVERENTRY_HH_
# define STECHECSERVERENTRY_HH_

# include "StechecGameData.hh"
# include "StechecEntry.hh"

# ifndef LIB_NAME
#  define LIB_NAME ""
# endif
# ifndef PKG_DATA_DIR
#  define PKG_DATA_DIR ""
# endif

/*!
** This class provides hooks, either for real players or viewers.
**
** You can:
**  - overload any virual method you wants (see StechecEntry)
**  - send message to clients: void SendToAll(const StechecPkt&)
**  - access to the GameData class: g_
**
** Note:
**  - afterGame() is called only if beforeGame() was called.
**  - getScore() is called _after_ afterGame(). So don't free all memory
**     in afterGame() !
*/
class StechecServerEntry: protected StechecServer, public StechecEntry
{

public:

  StechecServerEntry(GameData* game, Server* server, const ConfSection* cfg)
    : StechecServer(game, server),
      cfg_(cfg)
  { }
  virtual ~StechecServerEntry() {}

  /**
  ** Check if match is finish and update winner's list of gameData
  */
  virtual bool isMatchFinished() = 0;

  /**
  ** Fetch the score for each player, after prolo_is_finished
  ** has returned true.
  */
  virtual int getScore(int uid) = 0;

protected:

  /*!
  ** Open a map file, searching on different possible path.
  ** Return NULL if not found.
  */
  FILE*        OpenMap(const char* filename);

  //! @brief Keep rules parameters, somebody will need it.
  const ConfSection* cfg_;
};


// This one must remain inlined. Macro comes from contest Makefiles.
inline FILE*  StechecServerEntry::OpenMap(const char* filename)
{
  FILE* f;

  // search on current directory
  f = fopen(filename, "r");
  LOG5("Check if mapfile `%1' exists.", filename);
  if (f != NULL)
    return f;

  // search based on environment variable "<contest-name>_SHARED_MAP"
  char file[256];
  snprintf(file, 255, LIB_NAME "_SHARED_MAP");
  char* envDir = getenv(file);
  if (envDir != NULL)
    {
      snprintf(file, 255, "%s/%s", envDir, filename);
      LOG5("Check if mapfile `%1' exists.", file);
      if ((f = fopen(file, "r")) != NULL)
        return f;
    }

  // search on package installation shared directory
  snprintf(file, 255, "%s/%s", PKG_DATA_DIR, filename);
  LOG5("Check if mapfile `%1' exists.", file);
  if ((f = fopen(file, "r")) != NULL)
    return f;

  // not found
  return NULL;
}

//! @}

#endif // !STECHECSERVERENTRY_HH_
