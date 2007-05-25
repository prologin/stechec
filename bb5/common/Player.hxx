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

inline Player::Player(const MsgPlayerCreate* m)
  : id_(m->player_id),
    team_id_(m->client_id),
    status_(STA_RESERVE),
    ma_(m->ma),
    st_(m->st),
    ag_(m->ag),
    av_(m->av),
    ma_remain_(-1),
    has_blocked_(false),
    has_played_(false),
    action_(DCL_UNASSIGNED),
    will_prone_(false)
{
  name_ = packetToString(m->name);

  int nb = m->skill_nb > MAX_SKILL ? MAX_SKILL : m->skill_nb;
  for (int i = 0; i < nb; i++)
    skill_list_.push_back(static_cast<eSkill>(m->skill[i]));
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

inline const std::string& Player::getPositionName() const
{
  return position_name_;
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

inline bool Player::hasPlayed() const
{
  return has_played_;
}
 	
inline void Player::setHasPlayed()
{
  has_played_ = true;
}
 	
inline enum eDeclaredAction Player::getAction() const
{
  return action_;
}

inline void Player::setAction(enum eDeclaredAction action)
{
  action_ = action;
}

inline bool Player::hasSkill(enum eSkill skill) const
{
  return std::find(skill_list_.begin(), skill_list_.end(), skill) != skill_list_.end();
}

inline const Player::SkillList& Player::getSkillList() const
{
  return skill_list_;
}

inline bool Player::hasUsedSkill(enum eSkill skill) const
{
  return std::find(used_skill_list_.begin(), used_skill_list_.end(), skill) != used_skill_list_.end();
}

inline void Player::useSkill(enum eSkill skill)
{
  used_skill_list_.push_back(skill);
}

inline void Player::resetTurn()
{
  action_ = DCL_UNASSIGNED;
  has_played_ = false;
  ma_remain_ = ma_;
  used_skill_list_.clear();
  will_prone_ = (status_ == STA_STUNNED);
}

inline bool Player::isNearAnEmptySquare() const
{
  //FIXME: to do
  return true;
}

inline bool Player::isNearAnOpponent(bool standing) const
{
  //FIXME : to do
  return (standing == standing);
}

// Todo: status, status string

inline const char* Player::stringify(enum eStatus status)
{
  switch (status)
    {
    case STA_UNASSIGNED:
      return "No status";
    case STA_RESERVE:
      return "Reserve";
    case STA_STANDING:
      return "Standing";
    case STA_PRONE:
      return "Prone";
    case STA_STUNNED:
      return "Stunned";
    case STA_KO:
      return "KO";
    case STA_INJURED:
      return "Injured";
    case STA_SENTOFF:
      return "Sent Off";
    }
  return "Undefined status";
}

inline const char* Player::stringify(enum eDeclaredAction action)
{
  switch (action)
    {
    case DCL_UNASSIGNED:
      return "No declared action";
    case DCL_MOVE:
      return "Move";
    case DCL_BLOCK:
      return "Block";
    case DCL_BLITZ:
      return "Blitz";
    case DCL_PASS:
      return "Pass";
    }
  return "Undefined action";
}

inline const char* Player::stringify(enum eSkill skill)
{
  switch (skill)
    {
    case SK_UNASSIGNED:
      return "No skill";
    case SK_BLOCK:
      return "Block";
    case SK_CATCH:
      return "Catch";
    case SK_DODGE:
      return "Dodge";
    case SK_PASS:
      return "Pass";
    case SK_SUREHANDS:
      return "Sure Hands";
    }
  return "Undefined skill";
}


inline std::ostream& operator<< (std::ostream& os, const Player& p)
{
  os << "Player '" << p.id_ << "' (team " << p.team_id_ << ") - " << p.name_ << "\n";

  if (p.status_ != STA_STANDING && p.status_ != STA_PRONE && p.status_ != STA_STUNNED)
    os << "  position      : out of the field\n";
  else
    os << "  position      : " << p.pos_ << "\n";

  os << "  carateristics : ma: " << p.ma_ << " | "
     << "st: " << p.st_ << " | "
     << "ag: " << p.ag_ << " | "
     << "av: " << p.av_ << "\n";
    
  if (!p.skill_list_.empty())
    {
      os << "  skills        : ";
      Player::SkillList::const_iterator it = p.skill_list_.begin();
      os << p.stringify(*it);
      for (++it; it != p.skill_list_.end(); ++it)
	os << " | " << p.stringify(*it);
      os << std::endl;
    }

  os << "  ma_remain     : " << p.ma_remain_ << "\n"
     << "  has_played    : " << (p.has_played_ ? "yes" : "no") << "\n"
     << "  status        : " << p.stringify(p.status_);

  os << std::endl;
  return os;
}
