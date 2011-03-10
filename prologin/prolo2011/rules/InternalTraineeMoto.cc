/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2011 Prologin
*/

#include "InternalTraineeMoto.hh"
#include "Utils.hh"
#include "Contest.hh"
#include "GameData.hh"

#include "interface.hh"

InternalTraineeMoto::InternalTraineeMoto(GameData* gd,
                                         int player,
                                         position init,
                                         int max_len)
{
  gd_ = gd;
  len_ = 1;
  player_ = player;
  max_len_ = max_len;
  last_end_moved_ = false;
  content_.push_front(init);
  LOG4("trainee_moto constructor");
}

bool InternalTraineeMoto::begin(position pos){
  return content_.front() == pos;
}

bool InternalTraineeMoto::end(position pos){
  return content_.back() == pos;
}

position InternalTraineeMoto::head(){
  return content_.front();
}

position InternalTraineeMoto::queue(){
  return content_.back();
}

// ne check pas PLUS_DE_PA ni PAS_A_TOI ni ID_INVALIDE
void InternalTraineeMoto::reject_bad_move(position from, position to){
  LOG4("trainee_moto reject bad move");
  if (!a_cote(from, to))
    throw POSITION_INVALIDE;
  if (!begin(from) && !end(from))
    throw POSITION_INVALIDE;
  Case& c = gd_->get_case(to);
  if ((c.type == OBSTACLE) ||
      (c.type != POINT_CROISEMENT && c.nb_trainees_moto == 1) ||
      (c.type == POINT_CROISEMENT && c.nb_trainees_moto == 4))
      throw POSITION_INVALIDE;
}

erreur InternalTraineeMoto::move(position from, position to)
{
  LOG4("trainee_moto move");
  if (begin(from))
  {
    if (len_ == max_len_)
    {
      const position& pos = content_.back();
      content_.pop_back();
      gd_->get_case(pos).nb_trainees_moto -= 1;
    }
    else
      len_ ++;
    content_.push_front(to);
    gd_->get_case(to).nb_trainees_moto += 1;
    last_end_moved_ = false;
    return OK;
  }
  else if (end(from))
  {
    if (len_ == max_len_)
    {
      const position& pos = content_.front();
      content_.pop_front();
      gd_->get_case(pos).nb_trainees_moto -= 1;
    }
    else
      len_ ++;
    content_.push_back(to);
    gd_->get_case(to).nb_trainees_moto += 1;
    last_end_moved_ = true;
    return OK;
  }
  LOG2("BAD MOVE : %1 -> %2", from, to);
  return POSITION_INVALIDE;
}

erreur InternalTraineeMoto::couper(position entre, position et)
{
    // First find the two adjectent nodes to split
    deque_type::iterator it;
    deque_type::iterator it2;
    int i = 1;

    for (it = content_.begin(); it != content_.end(); ++it, ++i)
        if (*it == entre)
        {
            it2 = it + 1;
            if (it2 == content_.end() || *it2 != et)
                throw POSITION_INVALIDE;
            break;
        }
        else if (*it == et)
        {
            it2 = it + 1;
            if (it2 == content_.end() || *it2 != entre)
                throw POSITION_INVALIDE;
            break;
        }

    // Then create the second TraineeMoto
    int new_max_len = 0;
    if (last_end_moved_)
        new_max_len = len_ - i;
    else
        new_max_len = max_len_ - i;
    InternalTraineeMoto& moto = gd_->creer_trainee_moto(player_,
                                                        *it2,
                                                        new_max_len);
    moto.last_end_moved_ = last_end_moved_;
    max_len_ -= new_max_len;
    len_ -= 1;
    while ((it2 = content_.erase(it2)) != content_.end())
    {
        moto.content_.push_back(*it2);
        moto.len_ += 1;
        len_ -= 1;
    }
}

/*
 * Just check that "entre" and "et" are two positions owned by the TraineeMoto
 * and that they are adjacent.
*/
void InternalTraineeMoto::reject_bad_coupe(position entre, position et)
{
    deque_type::const_iterator it;
    deque_type::const_iterator it2;

    for (it = content_.begin(); it != content_.end(); ++it)
        if (*it == entre)
        {
            it2 = it + 1;
            if (it2 == content_.end() || (*it2) != et)
                throw POSITION_INVALIDE;
        }
        else if (*it == et)
        {
            it2 = it + 1;
            if (it2 == content_.end() || *it2 != entre)
                throw POSITION_INVALIDE;
        }
}

int InternalTraineeMoto::length(){
  return len_;
}

position InternalTraineeMoto::queue(position head__){
  LOG4("trainee_moto queue");
  if (begin(head__))
    return head();
  else if (end(head__))
    return queue();
  else
  {
    position p = {0, 0};
    return p;
  }
}

trainee_moto InternalTraineeMoto::to_trainee_moto(int indice){
  trainee_moto out;
  out.id = indice;
  out.emplacement.reserve(len_);
  out.team = player_;
  for (int i = 0; i < len_; i++){
    out.emplacement.push_back(content_[i]); // TODO
  }
  return out;
}
