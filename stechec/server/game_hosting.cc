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
    clients_poll_(clients_, 500),
    nb_coach_(0),
    viewer_base_uid_(0),
    nb_viewer_(0),
    started_(false),
    finished_(false)
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
  assert(clients_.empty());
}

bool GameHosting::isFinished() const
{
  return finished_;
}

pthread_t GameHosting::getThreadId() const
{
  return self_;
}

void    GameHosting::sendPacket(const Packet& p)
{
  LOG5("Send packet `" << rules_->getPacketStr(p.token)
       << "' to client_id `" << p.client_id << "'");
  std::for_each(clients_.begin(), clients_.end(), Client::Send(p));
  log_.send(&p);
}

void    GameHosting::outputStatistics()
{
  ClientIter it;
  for (it = clients_.begin(); it != clients_.end(); ++it)
    if ((*it)->isCoach())
      rules_->outputStat((*it)->getId(), (*it)->getClientStatistic());
  for (it = dead_clients_.begin(); it != dead_clients_.end(); ++it)
    rules_->outputStat((*it)->getId(), (*it)->getClientStatistic());
}

// FIXME: some race condition remains !
void    GameHosting::addClient(Cx* cx, int client_extid, bool wanna_be_coach)
{
  int        uid = -1;

  if (finished_)
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
  clients_.push_back(new Client(cx, uid, client_extid, nb_waited_coach_));
}

// Process one packet for one client.
// Return true if the client should be killed.
bool GameHosting::processOne(Client* cl)
{
  Packet* pkt;

  // Get the packet, close the connection on failure.
  pkt = cl->getPacket();

  // This packet is accepted whatever.
  if (pkt->token == CX_READY)
    {
      cl->setReady(true);
      ClientIter it;
      for (it = clients_.begin(); it != clients_.end(); ++it)
        if (!(*it)->isCoach() && !(*it)->isReady())
          break;
      if (it == clients_.end())
        {
          LOG3("All viewers are ready.");
          for (it = clients_.begin(); it != clients_.end(); ++it)
            (*it)->setReady(false);
          rules_->setViewerState(rules_->getViewerState() | VS_READY);
        }
      return false;
    }

  // Client wants to quit. Let him go.
  if (pkt->token == CX_ABORT)
    return true;

  if (cl->isCoach())
    {
      if (started_)
        rules_->handlePacket(pkt);
      else
        {
          LOG2("A coach is trying to send a packet before game start, kill it.");
          return true;
        }
    }
  else
    {
      LOG2("A viewer is trying to send illegal message, kill it.");
      return true;
    }

  return false;
}

bool GameHosting::process()
{
  // Receive and manage incoming packets.
  int nb_ready;
  try {
    nb_ready = clients_poll_.poll();
  } catch (const NetError& e) {
    LOG2("Fatal network error: " << e.Str());
    return false;
  }

  for (int i = 0; i < nb_ready; i++)
    {
      bool remove_from_cl = false;
      std::string remove_reason = "A mysterious reason.";
      Client* cl = clients_[i];
      
      try {
        remove_from_cl = processOne(cl);
      } catch (const NetError& e) {
        remove_reason = std::string("Network error: ") + e.Str();
        LOG2("Network error: " << e.Str());
        remove_from_cl = true;
      }
      if (remove_from_cl)
        {
          if (cl->isCoach())
            {
              nb_coach_--;
              cl->killConnection(remove_reason);
              rules_->coachKilled(cl->getId(), cl->getClientStatistic().custom_);
              dead_clients_.push_back(cl);
            }
          else if (!--nb_viewer_)
            {
              rules_->setViewerState(rules_->getViewerState() & ~VS_HAVEVIEWER);
              delete cl;
            }
          assert(nb_viewer_ >= 0);

          // Remove it from list as well.
          const int cl_size = clients_.size();
          clients_[i] = clients_[cl_size - 1];
          clients_.erase(clients_.end() - 1);
          if (nb_ready >= cl_size)
            nb_ready--;
        }
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
      sendPacket(GameFinished());
      outputStatistics();
    } catch (const NetError& e) {
      LOG2("Network error: " << e.Str() << ". Aborting game.");
    }
  
  // Force all remaining clients to disconnect, to ease thread cleaning.
  std::for_each(clients_.begin(), clients_.end(), Deleter());
  std::for_each(dead_clients_.begin(), dead_clients_.end(), Deleter());
  clients_.clear();
  dead_clients_.clear();

  finished_ = true;
}

void*        GameHosting::startThread(void* gh_inst)
{
  GameHosting* inst = static_cast<GameHosting*>(gh_inst);
  inst->run();
  return NULL;
}
