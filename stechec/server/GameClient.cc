/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2006 Prologin
*/

#include "BaseSRules.hh"
#include "GameHosting.hh"
#include "GameClient.hh"

BEGIN_NS(server);

GameClient::GameClient(GameHosting* gh, Cx* cx, int id, int league_id,
		       int nb_team, int nb_coach)
  : Client(cx),
    gh_(gh),
    id_(id),
    league_id_(league_id),
    is_ready_(false),
    client_stat_(NULL)
{
  if (id_ < UID_VIEWER_BASE)
    client_stat_ = new ClientStatistic(id, league_id);

  Packet acpt(CX_ACCEPT);
  cx->send(&acpt);

  ClientUid pkt_id(CLIENT_UID);
  pkt_id.client_id = id;
  pkt_id.team_id = league_id;
  pkt_id.nb_team = nb_team;
  pkt_id.nb_coach = nb_coach;
  cx->send(&pkt_id);
}

GameClient::~GameClient()
{
  if (cx_ != NULL)
    delete cx_;
  if (gh_->getState() == eWaiting)
    delete client_stat_;
}

ClientStatistic& GameClient::getClientStatistic()
{
  assert(id_ < UID_VIEWER_BASE);
  assert(client_stat_ != NULL);
  return *client_stat_;
}
  
int GameClient::getId() const
{
  return id_;
}

int GameClient::getLeagueId() const
{
  return league_id_;
}

bool GameClient::isCoach() const
{
  return id_ < UID_VIEWER_BASE;
}

bool GameClient::isReady() const
{
  return is_ready_;
}

void GameClient::setReady(bool value)
{
  is_ready_ = value;
}

void GameClient::kill(std::string fail_msg)
{
  if (client_stat_)
    client_stat_->fail_reason_ = fail_msg;

  gh_->clientDied(this);

  if (!close_pending_)
    {
      delete cx_;
      cx_ = NULL;
    }
}


// Called when this client can fetch a packet.
bool GameClient::recvReady()
{
  Packet* pkt;
  enum eGameState state = gh_->getState();

  assert(state != eFinished && state != eCrashed && state != eStarting);

  // Connection recieved errors, but we have not closed the connection to
  // keep the receive buffer.
  if (close_pending_)
    {
      delete cx_;
      cx_ = NULL;
      return true;
    }
  if (cx_ == NULL)
    return true;

  try {
    pkt = cx_->receive();
  } catch (const NetError& e) {
    LOG2("Network error on client (from %1): %2", *cx_, e.what());
    kill(e.what());
    return true;
  }

  // GameClient wants to quit. Let him go.
  if (pkt->token == CX_ABORT)
    {
      kill("GameClient asked to abort game.");
      return true;
    }

  // We only wait that this client quits.
  if (state == eFinishing)
    return false;
  
  switch (state)
    {
    case eWaiting:
      if (pkt->token == CX_READY && !isCoach())
	{
	  gh_->spectatorReadiness(this);
	  break;
	}
      kill("Tried to send illegal message before game start.");
      return true;

    case ePlaying:
      gh_->servePlaying(this, pkt);
      break;

    default:
      WARN("BUG - should not be here.");
      break;
    }
  return false;
}


/*
** Packet sending.
*/

GameClient::Send::Send(const Packet& p)
  : p_(p)
{
}

void GameClient::Send::operator() (GameClient* cl)
{
  // Refuse sending packet if we know that client is already dead.
  if (cl->close_pending_ || cl->cx_ == NULL)
    return;
  
  try {
    cl->cx_->send(&p_);
  } catch (const NetError& e) {
    cl->close_pending_ = true;
    cl->kill(e.what());
  }
}

END_NS(server);
