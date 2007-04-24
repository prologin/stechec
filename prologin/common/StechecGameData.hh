/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2005, 2006, 2007 Prologin
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

  //! @brief Get the number of players in play.
  int   getNbPlayer() const;

  //! @brief Get the number of team in play.
  int   getNbTeam() const;

  //! @brief Get the current turn number.
  int   getCurrentTurn() const;

  //! @brief Get the player unique identifier.
  //! @return An uid in the range [0 - MAX_PLAYER-1] for real player,
  //!   an uid greater that UID_VIEWER_BASE for visu.
  //! @note Don't make any sense to call it from the server side.
  int   getUid() const;

  //! @brief Get the player's team identifier.
  //! @return A team uid (not in a range) for a real player,
  //!   -1 for visu.
  //! @note Don't make any sense to call it from the server side.
  int	getTeamId() const;

  //! @brief Get a player's team identifier from it's unique identifier
  //! @param uid Player unique identifier, in the range [0 - MAX_PLAYER-1]
  //! @return An uid > 0 identifying a team (can be a high value), else -1.
  int	getTeamId(int player_id) const;

  //! @brief Get all player uid belonging to a team.
  //! @param team_id The wanted team identifiant.
  //! @param tab Returned uids, tab must at least be of size (MAX_PLAYER / MAX_TEAM).
  //! @param size Number of returned entry in 'tab'.
  //! @return true if this function is successful.
  bool	getAllIdFromTeamId(int team_id, int tab[], int* size) const;


private:
  friend class SRules;
  friend class CRules;
  friend class StechecApi;

  int   nb_player_;
  int   nb_team_;
  int   current_turn_;
  int   id_;
  int	team_id_;
  int	team_[MAX_PLAYER];

};

inline StechecGameData::StechecGameData()
  : nb_player_(0),
    nb_team_(0),
    current_turn_(0),
    id_(-1),
    team_id_(-1)
{
  for (int i = 0; i < MAX_PLAYER; i++)
    team_[i] = -1;
}

inline StechecGameData::~StechecGameData()
{
}


inline int StechecGameData::getNbPlayer() const
{
  return nb_player_;
}

inline int StechecGameData::getNbTeam() const
{
  return nb_team_;
}

inline int StechecGameData::getCurrentTurn() const
{
  return current_turn_;
}

inline int StechecGameData::getUid() const
{
  return id_;
}

inline int StechecGameData::getTeamId() const
{
  return team_id_;
}

inline int StechecGameData::getTeamId(int player_id) const
{
  if (player_id < 0 || player_id >= MAX_PLAYER)
    return -1;
  return team_[player_id];
}

inline bool StechecGameData::getAllIdFromTeamId(int team_id, int tab[], int* size) const
{
  int j = 0;

  if (team_id < 0 || team_id >= MAX_TEAM)
    return false;

  for (int i = 0; i < MAX_PLAYER; i++)
    {
      if (team_[i] == team_id)
	{
	  if (tab != NULL)
	    tab[j] = i;
	  j++;
	}
    }
  if (size != NULL)
    *size = j;
  return j > 0;
}


#endif // !STECHECGAMEDATA_HH_
