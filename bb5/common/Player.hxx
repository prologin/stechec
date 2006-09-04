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

# include "Field.hh"

inline Player::Player(int id, int team_id)
  : id_(id),
    team_id_(team_id),
    status_(STA_UNASSIGNED),
    ma_remain_(-1)
{
}

inline Player::~Player()
{
}

inline int Player::getId() const
{
  return id_;
}

inline int Player::getTeamId() const
{
  return team_id_;
}

inline const Position& Player::getPosition() const
{
  return pos_;
}

inline const std::string& Player::getName() const
{
  return name_;
}

inline int Player::getMa() const
{
  return ma_;
}

inline int Player::getMaRemain() const
{
  return ma_remain_;
}

inline int Player::getSt() const
{
  return st_;
}

inline int Player::getAg() const
{
  return ag_;
}

inline int Player::getAv() const
{
  return av_;
}

inline enum eStatus Player::getStatus() const
{
  return status_;
}


inline bool Player::hasDoneAction() const
{
  return has_done_action_;
}

inline bool Player::hasDoneMove() const
{
  return ma_remain_ != ma_;
}

inline bool Player::hasDoneBlock() const
{
  return has_done_block_;
}

inline void Player::setHasPlayed()
{
  has_done_action_ = true;
}

inline void Player::resetTurn()
{
  has_done_action_ = false;
  has_done_block_ = false;
  ma_remain_ = ma_;
  will_prone_ = (status_ == STA_STUNNED);
}

// Todo: status, status string

inline int Player::getPlayerPosition() const
{
  return player_position_;
}

inline const std::string& Player::getPlayerPicture() const
{
  return player_picture_;
}
  

inline std::ostream& operator<< (std::ostream& os, const Player& p)
{
  os << "Player '" << p.id_ << "' (team " << p.team_id_ << ") - " << p.name_ << "\n"
     << "  position      : " << p.pos_ << "\n"
     << "  carateristics :  ma: " << p.ma_ << " | "
     << "st: " << p.st_ << " | "
     << "ag: " << p.ag_ << " | "
     << "av: " << p.av_ << "\n"
     << "  ma_remain     : " << p.ma_remain_ << std::endl;
  return os;
}
