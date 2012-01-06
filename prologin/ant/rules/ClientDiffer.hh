/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2004, 2005, 2006 Prologin
*/

#ifndef DIFFER_HH_
# define DIFFER_HH_

# include "Contest.hh"
# include "GameData.hh"

/*!
** @ingroup stechec_ant
** @{
*/

/*!
** @class _ant__ClientDiffer
** @brief Ant client differ.
*/
class ClientDiffer: public StechecClientDiffer          
{
public:

  ClientDiffer(GameData* game, Client* client):
    StechecClientDiffer(game, client)
  { }
  
  // Dispatcheur de s_com
  virtual void ApplyDiff(const StechecPkt* com); 
};

#endif // !DIFFER_HH_ 
