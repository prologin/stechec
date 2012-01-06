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

#ifndef STECHECENTRY_HH_
# define STECHECENTRY_HH_

#include "StechecGameData.hh"

/*!
** @ingroup stechec
** @{
*/

class StechecEntry
{
public:
  StechecEntry() { }
  virtual ~StechecEntry() {}
  
  /**
  ** Different entry point that a contest implementation can use
  */
  virtual int  beforeGame()       { return 0; }
  virtual int  initGame()         { return 0; }
  virtual int  beforeNewTurn()    { return 0; }
  virtual int  afterNewTurn()     { return 0; }
  virtual int  afterGame()        { return 0; }
};

//! @}

#endif // !STECHECENTRY_HH_
