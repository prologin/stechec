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

#include "server.hh"
#include "game_hosting.hh"

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
    started_(false),
    game_finished_(false),
    thread_finished_(false)
{
  cfg.switchSection("game");
  nb_waited_coach_ = cfg.getData<int>("nb_team");
  rules->setTeamNumber(nb_waited_coach_);
  cfg.switchSection("server");
  nb_waited_viewer_ = cfg.getData<int>("nb_spectator");

  LOG1("Creating a new game, uid '" << game_uid << "'. Wait for `"
       << nb_waited_coach_ << "' teams and `" << nb_waited_viewer_
       << "' spectators.");
  
  if (cfg.getAttr<bool>("log", "enabled"))
    {
      std::string filename = cfg.getAttr<std::string>("log", "file");
      log_.open(filename, CX_WO);

      ClientUid pkt_id(CLIENT_UID);
      pkt_id.client_id = UID_COACH_BASE;
      pkt_id.nb_team = nb_waited_coach_;
      log_.send(&pkt_id);
      LOG4("Log game into file `" << filename << "'");
    }
  
}

GameHosting::~GameHosting()
{
  delete rules_;

  // All clients _should_ have been disconnected.
  assert(client_list_.empty());
}

bool GameHosting::isFinished() const
{
  return thread_finished_;
}

pthread_t GameHosting::getThreadId() const
{
  return self_;
}

void    GameHosting::sendPacket(const Packet& p)
{
  LOG5("Send packet `" << rules_->getPacketStr(p.token)
       << "' to client_id `" << p.client_id << "'");
  log_.send(&p);
  for_all(client_list_, Client::Send(p));
}

void    GameHosting::outputStatistics()
{
  ClientIter it;
  for (it = coach_list_.begin(); it != coach_list_.end(); ++it)
    rules_->outputStat((*it)->getId(), (*it)->getClientStatistic());
}

// FIXME: some race condition remains !
void    GameHosting::addClient(Cx* cx, int client_extid, bool wanna_be_coach)
{
  int        uid = -1;

  if (game_finished_)
    {
      LOG3("Deny access for game '" << game_uid_ << "': it is already finished !");
      Packet pkt_game_finished(CX_DENY);
      cx->send(&pkt_game_finished);
      delete cx;
      return;
    }

  // FIXME: may allow viewer acces even if game is started.
  //   for now, it doesn't work.
  if (started_)
    {
      LOG3("Deny coach access for game '" << game_uid_
           << "': it is already started !");
      Packet pkt_full(CX_DENY);
      cx->send(&pkt_full);
      delete cx;
      return;
    }

  if (wanna_be_coach)
    {
      uid = nb_coach_++ + UID_COACH_BASE;
      LOG4("Grant access for coach `" << uid << "' (league id: "
           << client_extid << "), game '" << game_uid_ << "'.");
      if (nb_coach_ == nb_waited_coach_ && nb_viewer_ >= nb_waited_viewer_)
        started_ = true;
    }
  else
    {
      uid = viewer_base_uid_++ + UID_VIEWER_BASE;
      nb_viewer_++;
      rules_->setViewerState(rules_->getViewerState() | VS_HAVEVIEWER);
      LOG4("Grant access for spectator uid '" << uid << "', game '"
           << game_uid_ << "'.");
      if (nb_coach_ == nb_waited_coach_ && nb_viewer_ >= nb_waited_viewer_)
        started_ = true;
    }

  // Accepted.
  client_list_.push_back(new Client(cx, uid, client_extid, nb_waited_coach_));
  if (wanna_be_coach)
    coach_list_.push_back(client_list_.back());
}

// Process one packet for one client.
// Return true if the client should be killed.
bool GameHosting::processOne(Client* cl, std::string& remove_reason)
{
  Packet* pkt;

  // Get the packet, close the connection on failure.
  pkt = cl->getPacket();

  // This packet is accepted whatever.
  if (pkt->token == CX_READY)
    {
      cl->setReady(true);
      ClientIter it;
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
      return false;
    }

  // Client wants to quit. Let him go.
  if (pkt->token == CX_ABORT)
    {
      remove_reason = "You aborted the connection.";
      return true;
    }

  if (cl->isCoach())
    {
      if (started_)
        rules_->handlePacket(pkt);
      else
        {
          LOG2("A coach is trying to send a packet before game start, kill it.");
	  remove_reason = "Tried to send packet before game start.";
          return true;
        }
    }
  else
    {
      LOG2("A viewer is trying to send illegal message, kill it.");
      remove_reason = "Viewers are not allowed to send messages";
      return true;
    }

  return false;
}

bool GameHosting::process()
{
  // Receive and manage incoming packets.
  int nb_ready;
  try {
    nb_ready = client_poll_.poll();
  } catch (const NetError& e) {
    LOG2("Fatal network error: " << e);
    return false;
  }

  for (int i = 0; i < nb_ready; i++)
    {
      std::string remove_reason = "A mysterious reason.";
      Client* cl = client_list_[i];

      // Dead client, or game finished. Do nothing with them except
      // waiting to trash them.
      if (cl->isDead() || game_finished_)
	{
	  if (cl->discardInput())
	    {
	      if (!cl->isCoach())
		delete cl;
	      const int cl_size = client_list_.size();
	      client_list_[i] = client_list_[cl_size - 1];
	      client_list_.erase(client_list_.end() - 1);
	      if (nb_ready >= cl_size)
		nb_ready--;
	    }
	  continue;
	}

      try {
        if (processOne(cl, remove_reason))
	  cl->setDead(remove_reason);
      } catch (const NetError& e) {
        LOG2("Network error: " << e);
        remove_reason = std::string("Network error: ") + e.what();
	cl->setDead(remove_reason, true);
      }

      if (cl->isDead() && cl->isCoach())
	{
	  nb_coach_--;
	  rules_->coachKilled(cl->getId(), cl->getClientStatistic().custom_);
	}
      if (cl->isDead() && !cl->isCoach() && !--nb_viewer_)
	{
	  rules_->setViewerState(rules_->getViewerState() & ~VS_HAVEVIEWER);
	}
      assert(nb_viewer_ >= 0 && nb_coach_ >= 0);
    }

  return true;
}

void GameHosting::run()
{
  self_ = pthread_self();

  // Set logger options.
  Log log(4);
  log.setVerboseLevel(cfg_.getAttr<int>("debug", "verbose"));
  log.setPrintLoc(cfg_.getAttr<bool>("debug", "printloc"));
  std::ostringstream is;
  is << " " << game_uid_;
  log.setModuleSuffix(is.str().c_str());
  int game_start_timeout = cfg_.getAttr<int>("options", "start_game_timeout");
  Timer start_timeout(game_start_timeout);
  start_timeout.start();

  // Wait for the coaches.
  while (!started_ && !start_timeout.isTimeElapsed())
    process();

  if (start_timeout.isTimeElapsed())
    LOG3("Allowed time for starting game elapsed. Canceling game.");

  // Really play the game.
  if (started_)
    try {
      LOG5("Game starting...");
      rules_->setSendPacketObject(this);
      rules_->serverStartup();
      while (rules_->getState() != GS_END)
        {
          // Maybe the server has something to do
          // (called at least every 500ms).
          rules_->serverProcess();
          process();
          if (nb_coach_ != nb_waited_coach_)
            {
              // FIXME: it should be optional.
              LOG3("A coach has disconnected. Canceling game.");
              break;
            }
        }
      outputStatistics();
    } catch (const NetError& e) {
      LOG2("Network error: " << e << ". Aborting game.");
    }
  
  sendPacket(GameFinished());
  game_finished_ = true;

  LOG4("Wait that `" << client_list_.size() << "' remaining clients quit.");
  while (!client_list_.empty())
    process();
  for_all(coach_list_, Deleter());
  LOG5("Ok, all remaining clients have left.");

  thread_finished_ = true;
}

void*        GameHosting::startThread(void* gh_inst)
{
  GameHosting* inst = static_cast<GameHosting*>(gh_inst);
  inst->run();
  return NULL;
}
