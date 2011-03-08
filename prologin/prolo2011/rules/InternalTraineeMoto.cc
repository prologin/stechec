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

#include "interface.hh"

InternalTraineeMoto::InternalTraineeMoto(int player, position init, int max_len){
  len_ = 1;
  player_ = player;
  max_len_ = max_len;
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
// ne check pas si la case est occupee par un autre cable ou par un obstacle
// ne check pas PLUS_DE_PA ni PAS_A_TOI ni ID_INVALIDE
void InternalTraineeMoto::reject_bad_move(position from, position to){
  LOG4("trainee_moto reject bad move");
  if (!a_cote(from, to)) throw POSITION_INVALIDE;
  if (begin(from)){
    return ;
  }else if (end(to)){
    return ;
  }
  throw POSITION_INVALIDE;
}

// todo dessier le cable dans gamedata pour pouvoir checker sa presence
erreur InternalTraineeMoto::move(position from, position to){
  LOG4("trainee_moto move");
  if (begin(from)){
    if (len_ == max_len_) content_.pop_back();
    else len_ ++;
    content_.push_front(to);
    return OK;
  }else if (end(to)){
    return OK;
  }
  LOG2("BAD MOVE : %1 -> %2", from, to);
  return POSITION_INVALIDE;
}

int InternalTraineeMoto::length(){
  return len_;
}

position InternalTraineeMoto::queue(position head__){
  LOG4("trainee_moto queue");
  if (begin(head__)){
    return head();
  }else if (end(head__)){
    return queue();
  }else{
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
