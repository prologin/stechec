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

#include "Server.hh"
#include "GameClient.hh"
#include "GameHosting.hh"

BEGIN_NS(server);

GameHosting::GameHosting(int game_uid,
                         const xml::XMLConfig& cfg,
                         BaseSRules* rules)
  : cfg_(cfg),
    game_uid_(game_uid),
    rules_(rules),
    client_poll_(client_list_, 500),
    nb_coach_(0),
    viewer_base_uid_(0),
    nb_viewer_(0),
    state_(eStarting)
{
  pthread_mutex_init(&lock_, NULL);
  pthread_mutex_lock(&lock_);
  client_poll_.setLock(&lock_);

  nb_team_ = cfg.getData<int>("game", "nb_team");
  int nb_per_team = cfg.getAttr<int>("game", "nb_team", "player_per_team");

  nb_waited_coach_ = nb_team_ * nb_per_team;
  rules->setTeamNumber(nb_waited_coach_, nb_team_);
  nb_waited_viewer_ = cfg.getData<int>("server", "nb_spectator");

  LOG1("Creating a new game, uid '%1`. Wait for `%2' coachs and `%3' spectators.",
       game_uid, nb_waited_coach_, nb_waited_viewer_);

  if (cfg.getAttr<bool>("server", "log", "enabled"))
    {
      std::string filename = cfg.getAttr<std::string>("server", "log", "file");
      log_.open(filename, CX_WO);

      ClientUid pkt_id(CLIENT_UID);
      pkt_id.client_id = UID_COACH_BASE;
      pkt_id.nb_team = nb_team_;
      pkt_id.nb_coach = nb_waited_coach_;
      log_.send(&pkt_id);
      LOG4("Log game into file `%1`", filename);
    }
}

GameHosting::~GameHosting()
{
  delete rules_;
  pthread_mutex_destroy(&lock_);

  // All clients _should_ have been disconnected.
  assert(client_list_.empty());
}

enum eGameState GameHosting::getState() const
{
  return state_;
}

pthread_t GameHosting::getThreadId() const
{
  return self_;
}

void    GameHosting::sendPacket(const Packet& p)
{
  LOG5("Send packet `%1' (to client_id `%2')", rules_->stringifyToken(p.token), p.client_id);
  log_.send(&p);
  for_all(client_list_, GameClient::Send(p));
}

void    GameHosting::outputStatistics()
{
  ClientStatisticList::iterator it;
  for (it = stats_list_.begin(); it != stats_list_.end(); ++it)
    rules_->outputStat((*it)->id_, **it);
}

// Called by Server, when a client wants to join a game.
void    GameHosting::addClient(Cx* cx, int client_extid, bool wanna_be_coach)
{
  int uid = -1;

  pthread_mutex_lock(&lock_);
  if (state_ == eFinishing || state_ == eFinished || state_ == eCrashed)
    {
      pthread_mutex_unlock(&lock_);
      LOG3("Deny access for game '%1': it is already finished!", game_uid_);
      CxDeny denial;
      stringToPacket(denial.reason, "Game already finished", 64);
      cx->send(&denial);
      delete cx;
      return;
    }

  // FIXME: may allow viewer acces even if game is started.
  //   for now, it doesn't work.
  if (state_ == ePlaying)
    {
      pthread_mutex_unlock(&lock_);
      LOG3("Deny coach access for game '%1': it is already started!", game_uid_);
      CxDeny denial;
      stringToPacket(denial.reason, "Game already started", 64);
      cx->send(&denial);
      delete cx;
      return;
    }

  if (wanna_be_coach)
    {
      if (nb_coach_ >= nb_waited_coach_)
	{
	  LOG3("Deny access for game '%1': too many coaches!", game_uid_);
	  CxDeny denial;
	  stringToPacket(denial.reason, "Too many coaches", 64);
	  cx->send(&denial);
	  delete cx;
	  return;
	}
      uid = nb_coach_++ + UID_COACH_BASE;
      LOG4("Grant access for coach `%1' (league id: %2), game '%3'.", uid,
      		client_extid, game_uid_);
      if (nb_coach_ == nb_waited_coach_ && nb_viewer_ >= nb_waited_viewer_)
        state_ = ePlaying;
    }
  else
    {
      uid = viewer_base_uid_++ + UID_VIEWER_BASE;
      nb_viewer_++;
      rules_->setViewerState(rules_->getViewerState() | VS_HAVEVIEWER);
      LOG4("Grant access for spectator uid '%1', game '%2'.", uid, game_uid_);
      if (nb_coach_ == nb_waited_coach_ && nb_viewer_ >= nb_waited_viewer_)
        state_ = ePlaying;
    }

  // Accepted.
  GameClient* cl = new GameClient(this, cx, uid, client_extid, nb_team_, nb_waited_coach_);
  client_list_.push_back(cl);

  pthread_mutex_unlock(&lock_);
}


// Call it when a client is disconnected.
// Call on state eWaiting or ePlaying
void GameHosting::clientDied(GameClient* cl)
{
  if (cl->isCoach())
    {
      LOG5("Coach `%1' died.", cl->getId());
      nb_coach_--;
      if (state_ == ePlaying)
	rules_->coachKilled(cl->getId(), cl->getClientStatistic().custom_);
    }
  else
    {
      if (!--nb_viewer_)
	rules_->setViewerState(rules_->getViewerState() & ~VS_HAVEVIEWER);
    }
  if (nb_coach_ == 0 && nb_viewer_ == 0)
    {
      LOG4("Cancel game `%1', all clients has left!", game_uid_);
      state_ = eFinishing;
    }
}

// Handle CX_READY packet from spectators.
// It is used to say that spectators are ready for the next turn.
void GameHosting::spectatorReadiness(GameClient* cl)
{
  cl->setReady(true);

  // Check if all spectators are ready.
  GameClientIter it;
  for (it = client_list_.begin(); it != client_list_.end(); ++it)
    if (!(*it)->isCoach() && !(*it)->isReady())
      break;
  if (it == client_list_.end())
    {
      LOG3("All viewers are ready.");
      for (it = client_list_.begin(); it != client_list_.end(); ++it)
	(*it)->setReady(false);
      rules_->setViewerState(rules_->getViewerState() | VS_READY);
    }
}

// Process one packet for one client, in state ePlaying.
void GameHosting::servePlaying(GameClient* cl, Packet* pkt)
{
  if (pkt->token == CX_READY && !cl->isCoach())
    {
      spectatorReadiness(cl);
      return;
    }

  if (cl->isCoach())
    {
      rules_->handlePacket(pkt);
      sendPacket(MsgSync(cl->getId()));
    }
  else
    {
      LOG2("A viewer is trying to send illegal message, kill it.");
      cl->kill("Viewers are not allowed to send messages");
    }
}

void GameHosting::run(Log& log)
{
  GameClientIter it;

  self_ = pthread_self();

  // Set logger options.
  log.setVerboseLevel(cfg_.getAttr<int>("server", "debug", "verbose"));
  log.setPrintLoc(cfg_.getAttr<bool>("server", "debug", "printloc"));
  std::ostringstream is;
  is << " " << game_uid_;
  log.setModuleSuffix(is.str().c_str());

  int game_start_timeout = cfg_.getAttr<int>("server", "options", "start_game_timeout");
  Timer start_timeout(game_start_timeout);
  start_timeout.start();
  state_ = eWaiting;

  // Wait for the coaches.
  while (state_ == eWaiting && !start_timeout.isTimeElapsed())
    client_poll_.poll();

  // Fill stats_list.
  if (state_ == ePlaying)
    {
      for (it = client_list_.begin(); it != client_list_.end(); ++it)
	if ((*it)->isCoach())
	  stats_list_.push_back(&(*it)->getClientStatistic());
    }

  if (start_timeout.isTimeElapsed())
    {
      LOG3("Allowed time for starting game elapsed. Canceling game.");
      for_all(client_list_, std::bind2nd(std::mem_fun(&GameClient::kill),
					 std::string("Not able to start game: timeout!")));
    }

  // Really play the game.
  if (state_ == ePlaying)
    {
      LOG5("Game starting... %1 connected", nb_coach_);
      rules_->setSendPacketObject(this);

      // Send to the rules the correspondance between league_id and id.
      for (it = client_list_.begin(); it != client_list_.end(); ++it)
	if ((*it)->isCoach())
	  rules_->addPlayer((*it)->getId(), (*it)->getLeagueId());

      rules_->serverStartup();
      if (rules_->getState() != GS_END)
	{
	  while (rules_->getState() != GS_END && state_ == ePlaying)
	    {
	      // Maybe the server has something to do
	      // (called at least every 500ms).
	      rules_->serverProcess();

	      client_poll_.poll();
	      if (nb_coach_ != nb_waited_coach_)
		{
		  // FIXME: it should be optional.
		  LOG3("A coach has disconnected. Canceling game.");
		  break;
		}
	    }
	  outputStatistics();
	}
    }

  if (state_ == ePlaying)
   sendPacket(GameFinished());
  state_ = eFinishing;

  // Wait 3 seconds that remaining clients quit, to let them time
  // to process their last commands before trashing them (for make check).
  if (!client_list_.empty())
    {
      Timer timeout(3);
      timeout.start();
      LOG4("Wait 3 seconds that `%1` clients still connected quit.", client_list_.size());
      while (!client_list_.empty() && !timeout.isTimeElapsed())
	client_poll_.poll();
    }

  for_all(client_list_, Deleter());
  client_list_.clear();
  for_all(stats_list_, Deleter());
  state_ = eFinished;
  pthread_mutex_unlock(&lock_);
}

void*        GameHosting::startThread(void* gh_inst)
{
  Log log(4);
  GameHosting* inst = static_cast<GameHosting*>(gh_inst);
  try {
    inst->run(log);
    return NULL;
  } catch (const xml::XMLError& e) {
    ERR("%1", e);
  } catch (const Exception& e) {
    ERR("Unhandled error: %1", e);
  }
  inst->state_ = eCrashed;
  for_all(inst->client_list_, Deleter());
  inst->client_list_.clear();
  for_all(inst->stats_list_, Deleter());
  pthread_mutex_unlock(&inst->lock_);
  return NULL;
}

END_NS(server);
