/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2006, 2007 Prologin
*/

#include "misc/Conf.hh"
#include "net/FileCx.hh"
#include "Server.hh"
#include "Client.hh"
#include "GameHosting.hh"

BEGIN_NS(server);

GameHosting::GameHosting(int game_uid, const ConfSection* cfg, BaseSRules* rules)
  : cfg_(cfg),
    log_(0),
    cl_pool_(500),
    rules_(rules),
    game_uid_(game_uid),
    nb_coach_(0),
    viewer_base_uid_(0),
    nb_viewer_(0),
    state_(eStarting)
{
  pthread_mutex_init(&lock_, NULL);
  pthread_mutex_lock(&lock_);
  cl_pool_.setLock(&lock_);

  nb_team_ = rules->getTeamNumber();
  nb_waited_coach_ = nb_team_;
  nb_waited_viewer_ = cfg->getValue<int>("nb_spectator");

  LOG1("Creating a new game, uid '%1`. Wait for `%2' coachs and `%3' spectators.",
       game_uid, nb_waited_coach_, nb_waited_viewer_);

  if (cfg->getValue<std::string>("log") != "")
    {
      std::string filename = cfg->getValue<std::string>("log");
      log_ = new FileCx();
      log_->open(filename, CX_WRITE);

      ClientUid pkt_id(CLIENT_UID);
      pkt_id.client_id = UID_COACH_BASE;
      pkt_id.nb_team = nb_team_;
      pkt_id.nb_coach = nb_waited_coach_;
      log_->send(pkt_id);
      LOG2("Log game into file `%1`", filename);
    }
}

GameHosting::~GameHosting()
{
  delete rules_;
  delete log_;
  pthread_mutex_destroy(&lock_);

  // All clients _should_ have been disconnected.
  assert(cl_map_.empty() && cl_.empty());
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
  LOG5("-> `%1' (id: %2)", rules_->stringifyToken(p.token), p.client_id);

  if (log_)
    log_->send(p);
  for_all(cl_, std::bind2nd(std::mem_fun(&Client::send), &p));
}

void    GameHosting::outputStatistics()
{
  ClientList::iterator it;
  for (it = coach_.begin(); it != coach_.end(); ++it)
    rules_->outputStat((*it)->getId(), (*it)->st_);
}

// Called by Server, when a client wants to join a game.
void    GameHosting::addClient(Client* cl)
{
  Cx* cx = cl->cx_;
  int uid = -1;

  pthread_mutex_lock(&lock_);

  if (state_ == eFinishing || state_ == eFinished || state_ == eCrashed)
    {
      pthread_mutex_unlock(&lock_);
      LOG3("Deny access for game '%1': it is already finished!", game_uid_);
      CxDeny denial;
      stringToPacket(denial.reason, "Game already finished", 64);
      cx->send(denial);
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
      cx->send(denial);
      delete cx;
      return;
    }

  if (cl->isCoach())
    {
      if (nb_coach_ >= nb_waited_coach_)
	{
          pthread_mutex_unlock(&lock_);
	  LOG3("Deny access for game '%1': too many coaches!", game_uid_);
	  CxDeny denial;
	  stringToPacket(denial.reason, "Too many coaches", 64);
	  cx->send(denial);
          delete cx;
          return;
	}
      uid = nb_coach_++ + UID_COACH_BASE;
      LOG4("Grant access for coach `%1' (league id: %2), game '%3'.", uid,
      		cl->league_id_, game_uid_);
    }
  else
    {
      uid = viewer_base_uid_++ + UID_VIEWER_BASE;
      nb_viewer_++;
      rules_->setViewerState(rules_->getViewerState() | VS_HAVEVIEWER);
      LOG4("Grant access for spectator uid '%1', game '%2'.", uid, game_uid_);
    }

  // Accepted
  cl->id_ = uid;
  cl->st_.ext_id_ = cl->league_id_;
  cl_.push_back(cl);
  cl_map_.insert(std::make_pair(cx, cl));
  cl_pool_.addElt(cx);

  pthread_mutex_unlock(&lock_);

  Packet acpt(CX_ACCEPT);
  cx->send(acpt);

  ClientUid pkt_id(CLIENT_UID);
  pkt_id.client_id = cl->id_;
  pkt_id.league_id = cl->league_id_;
  pkt_id.nb_team = nb_team_;
  pkt_id.nb_coach = nb_waited_coach_;
  cx->send(pkt_id);
}


// Handle CX_READY packet from spectators.
// It is used to say that spectators are ready for the next turn.
void GameHosting::spectatorReadiness(Client* cl)
{
  cl->is_ready_ = true;

  // Check if all spectators are ready.
  ClientList::iterator it;
  for (it = cl_.begin(); it != cl_.end(); ++it)
    if (!(*it)->isCoach() && !(*it)->is_ready_)
      break;
  if (it == cl_.end())
    {
      LOG3("All viewers are ready.");
      for (it = cl_.begin(); it != cl_.end(); ++it)
	(*it)->is_ready_ = false;
      rules_->setViewerState(rules_->getViewerState() | VS_READY);
    }
}

// Process one packet for one client, in state ePlaying.
void GameHosting::servePlaying(Client* cl)
{
  Packet* pkt = cl->cx_->receive();

  if (pkt->token == CX_READY && !cl->isCoach())
    {
      spectatorReadiness(cl);
      return;
    }

  if (cl->isCoach())
    {
      MsgSync sync;
      for_all(cl_, std::mem_fun(&Client::begin));
      rules_->handlePacket(pkt);
      for_all(cl_, std::bind2nd(std::mem_fun(&Client::send), &sync));
      for_all(cl_, std::mem_fun(&Client::commit));
    }
  else
    {
      LOG2("A viewer is trying to send illegal message, kill it.");
      killClient(cl, "Viewers are not allowed to send messages");
    }
}

// Called when a client is disconnected, or when we force a client
// to disconnect.
//
// Either the socket was closed, or we want to kill the client.
// In both case, remove the client from our lists (cl_pool_, cl_), and
// close its connection.
void GameHosting::killClient(Client* cl, const std::string& msg, int msg_prio)
{
  Cx* cx = cl->cx_;

  cl_pool_.removeElt(cx);
  cl->setFailReason(msg, msg_prio);
  cl_map_.erase(cx);
  cl_.erase(std::remove(cl_.begin(), cl_.end(), cl), cl_.end());

  if (cl->isCoach())
    {
      LOG5("Coach `%1' died: %2", cl->getId(), msg);
      nb_coach_--;
      if (state_ == ePlaying)
	if (!rules_->coachKilled(cl->getId(), cl->st_.custom_))
          {
            LOG4("Rules requested that game should stop (on killed coach)");
            state_ = eFinishing;
          }
    }
  else
    {
      if (!--nb_viewer_)
	rules_->setViewerState(rules_->getViewerState() & ~VS_HAVEVIEWER);
    }

  if ((state_ == ePlaying || state_ == eWaiting) && nb_coach_ == 0)
    {
      LOG4("Cancel game `%1', all clients has left!", game_uid_);
      state_ = eFinishing;
    }

  // Send error report to the client, if its socket is still alive.
  CxError err;
  stringToPacket(err.msg, msg, 64);
  cx->send(err);

  delete cx;
  cl->cx_ = NULL;
  if (!cl->isCoach() || state_ == eWaiting)
    delete cl;
}

void GameHosting::waitCoaches()
{
  int game_start_timeout = cfg_->getValue<int>("start_game_timeout");
  Timer start_timeout(game_start_timeout);
  start_timeout.start();

  while (state_ == eWaiting && !start_timeout.isTimeElapsed())
    {
      CxPool<Cx>::ConstEltIter it;
      const CxPool<Cx>::EltList& ready_list = cl_pool_.poll();

      for (it = ready_list.begin(); it != ready_list.end(); ++it)
        {
          Packet* pkt;
          Cx* cx = it->second;
          Client* cl = cl_map_[cx];

          switch (it->first)
            {
            case E_FD_CONNECTION_CLOSED:
              if (cx->getLastError() != "No error")
                killClient(cl, cx->getLastError(), 3);
              else
                killClient(cl, "I don't know what to do to help you", 0);
              break;

            case E_FD_READ_READY:
              pkt = cx->receive();
              if (pkt->token == CX_ABORT)
                killClient(cl, "GameClient asked to abort game.");
              else if (pkt->token == CX_READY && !cl->isCoach())
                spectatorReadiness(cl);
              else
                killClient(cl, "Tried to send illegal message before game start.");
              break;

            default:
              WARN("BUG()");
            }
        }
      if (nb_coach_ == nb_waited_coach_ && nb_viewer_ >= nb_waited_viewer_)
        state_ = ePlaying;
    }

  if (start_timeout.isTimeElapsed())
    {
      LOG3("Allowed time for starting game elapsed. Canceling game.");
      state_ = eFinishing;
      ClientList::iterator it;
      for (it = cl_.begin(); it != cl_.end(); ++it)
        delete *it;
      cl_.clear();
      cl_map_.clear();
    }
}

void GameHosting::playGame()
{
  LOG4("Game starting... %1 connected", nb_coach_);
  rules_->setSendPacketObject(this);

  // Copy active coaches into coach list;
  ClientList::iterator it;
  for (it = cl_.begin(); it != cl_.end(); ++it)
    if ((*it)->isCoach())
      coach_.push_back(*it);

  // Send to the rules the correspondance between league_id and id.
  for (it = coach_.begin(); it != coach_.end(); ++it)
    rules_->addPlayer((*it)->getId(), (*it)->getLeagueId());

  rules_->serverStartup();
  if (rules_->getState() == GS_END)
    return;

  while (rules_->getState() != GS_END && state_ == ePlaying)
    {
      // Maybe the server has something to do
      // (called at least every 500ms).
      rules_->serverProcess();
      if (rules_->getState() == GS_END)
        break;

      // Get data from clients
      CxPool<Cx>::ConstEltIter it;
      const CxPool<Cx>::EltList& ready_list = cl_pool_.poll();

      for (it = ready_list.begin(); it != ready_list.end(); ++it)
        {
          Cx* cx = it->second;
          Client* cl = cl_map_[cx];

          switch (it->first)
            {
            case E_FD_CONNECTION_CLOSED:
              if (cx->getLastError() != "No error")
                killClient(cl, cx->getLastError(), 3);
              else
                killClient(cl, "I don't know what to do to help you", 0);
              break;

            case E_FD_READ_READY:
              servePlaying(cl);
              break;

            default:
              WARN("BUG()");
            }
        }
#if 0
      if (nb_coach_ != nb_waited_coach_)
        {
          // FIXME: it should be optional.
          LOG3("A coach has disconnected. Canceling game.");
          break;
        }
#endif
    }
  outputStatistics();
}

void GameHosting::run(Log& log)
{
  self_ = pthread_self();

  // Set logger options.
  log.setVerboseLevel(cfg_->getValue<int>("verbose"));
  log.setPrintLoc(cfg_->getValue<bool>("verbose_location"));
  std::ostringstream is;
  is << " " << game_uid_;
  log.setModuleSuffix(is.str().c_str());

  // Wait for the coaches.
  state_ = eWaiting;
  waitCoaches();

  // Really play the game.
  if (state_ == ePlaying)
    playGame();

  // Send cx_abort to all alive clients
  Packet abort(CX_ABORT);
  for_all(cl_, std::bind2nd(std::mem_fun(&Client::send), &abort));

  // Clean game.
  state_ = eFinishing;
  cl_pool_.flush();
  for_all(cl_, Deleter());
  cl_map_.clear();
  cl_.clear();

  // Ready to be reaped by Server.
  state_ = eFinished;
  pthread_mutex_unlock(&lock_);
}

void* GameHosting::startThread(void* gh_inst)
{
  Log log(4);
  GameHosting* inst = static_cast<GameHosting*>(gh_inst);
  try {
    inst->run(log);
    return NULL;
  } catch (const ConfException& e) {
    ERR("%1", e);
  } catch (const Exception& e) {
    ERR("Unhandled error: %1", e);
  }

  // Clean game.
  inst->state_ = eFinishing;
  inst->cl_pool_.flush();
  for_all(inst->cl_, Deleter());
  inst->cl_.clear();
  inst->cl_map_.clear();

  inst->state_ = eCrashed;
  pthread_mutex_unlock(&inst->lock_);
  return NULL;
}

END_NS(server);
