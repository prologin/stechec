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


template <typename T>
inline Team<T>::Team(int team_id)
  : team_id_(team_id),
    score_(0),
    reroll_(0)
{
  for (int i = 0; i < MAX_PLAYER; i++)
    player_[i] = NULL;
}

template <typename T>
inline Team<T>::~Team()
{
  for (int i = 0; i < MAX_PLAYER; i++)
    delete player_[i];
}

template <typename T>
inline void Team<T>::resetTurn()
{
  blitz_or_pass_done_ = false;
  reroll_used_ = false;
}

template <typename T>
inline int Team<T>::getTeamId() const
{
  return team_id_;
}

template <typename T>
inline int Team<T>::getNbPlayer() const
{
  int count = 0;

  for (int i = 0; i < MAX_PLAYER; i++)
    if (player_[i] != NULL)
      count += 1;
  return count;
}

template <typename T>
inline T* Team<T>::getPlayer(int id)
{
  if (id >= 0 && id < MAX_PLAYER)
    return player_[id];

  LOG2("Wrong player_id: " << id << " (team: " << team_id_ << ")");
  return NULL;
}

template <typename T>
inline const std::string& Team<T>::getTeamName() const
{
  return team_name_;
}

template <typename T>
inline const std::string& Team<T>::getCoachName() const
{
  return coach_name_;
}
