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

#ifndef STECHECGAMEDATA_HH_
# define STECHECGAMEDATA_HH_

# include "Rules.hh"

// Forward declaration of contest dependant class GameData.
class Caller;
class Client;
class ClientDiffer;
class Server;
class GameData;

typedef std::vector<StechecPkt>       CommandList;
typedef std::vector<StechecPkt*>      CommandListRef;

/*!
** Class containing all client/server common data and methods.
*/
class StechecGameData
{
   public:
      StechecGameData();
      virtual ~StechecGameData();

      //! @brief Get the number of team in play.
      int   getNbPlayer() const;
      //! @brief Get the current turn number.
      int   getCurrentTurn() const;
      //! @brief Get the team identifier.
      int   getUid() const;

      virtual void  AddPlayer(int uid, int team) {};

   private:
      friend class SRules;
      friend class CRules;
      friend class StechecApi;

      int   nb_player_;
      int   current_turn_;
      int   uid_;
};

inline StechecGameData::StechecGameData()
   : nb_player_(0),
     current_turn_(0),
     uid_(-1)
{
}

inline StechecGameData::~StechecGameData()
{
}


inline int StechecGameData::getNbPlayer() const
{
   return nb_player_;
}

inline int StechecGameData::getCurrentTurn() const
{
   return current_turn_;
}

inline int StechecGameData::getUid() const
{
   return uid_;
}

#endif // !STECHECGAMEDATA_HH_
