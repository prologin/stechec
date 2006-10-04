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


inline Client::Client(Cx* cx, int id, int ext_id, int nb_team)
  : cx_(cx),
    id_(id),
    is_ready_(false),
    is_dead_(false)
{
  client_stat_.ext_id_ = ext_id;
  client_stat_.custom_ = NULL;

  Packet acpt(CX_ACCEPT);
  cx->send(&acpt);

  ClientUid pkt_id(CLIENT_UID);
  pkt_id.client_id = id;
  pkt_id.nb_team = nb_team;
  cx->send(&pkt_id);
}

inline Client::~Client()
{
  assert(cx_ == NULL);
}

inline int Client::getFd() const
{
  return cx_ != NULL ? cx_->getFd() : -1;
}

inline int Client::getId() const
{
  return id_;
}

inline Packet* Client::getPacket()
{
  if (!is_dead_)
    return cx_->receive();
  return NULL;
}

inline bool Client::isCoach() const
{
  return id_ < UID_VIEWER_BASE;
}

inline bool Client::isReady() const
{
  return is_ready_;
}

inline void Client::setReady(bool value)
{
  is_ready_ = value;
}

inline bool Client::isDead() const
{
  return is_dead_;
}

inline void Client::setDead(std::string fail_msg, bool kill_now)
{
  client_stat_.fail_reason_ = fail_msg;
  is_dead_ = true;
  if (kill_now)
    {
      delete cx_;
      cx_ = NULL;
    }
  else
    cx_->shutdown();
}

inline bool Client::discardInput()
{
  if (cx_ == NULL)
    return true;
  if (!is_dead_)
    {
      cx_->shutdown();
      is_dead_ = true;
      return false;
    }
  if (cx_->waitClose())
    {
      delete cx_;
      cx_ = NULL;
      return true;
    }
  return false;
}

inline ClientStatistic& Client::getClientStatistic()
{
  return client_stat_;
}

inline Client::Send::Send(const Packet& p)
  : p_(p)
{
}

inline void Client::Send::operator() (Client* cl)
{
  if (cl->isDead() || cl->cx_ == NULL)
    return;
  try {
    cl->cx_->send(&p_);
  } catch (const NetError& e) {
    cl->setDead(e.what(), true);
  }
}
