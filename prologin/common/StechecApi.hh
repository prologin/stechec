/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2005, 2006 Prologin
*/

#ifndef STECHECAPI_HH_
# define STECHECAPI_HH_

# include "BaseApi.hh"
# include "CRules.hh"
# include "StechecGameData.hh"

/*!
** Do the binding between submited code (champions) and
** the current specialisation of Stechec (rules)
**
** Class' methods are called by the candidat through 'interface.cc',
** provinding C -> C++ glue.
**
** You can:
**  - access to the GameData class: g_
**  - send message to the server: void SendToServer(const StechecPkt pkt&)
*/
class StechecApi : public BaseApi<CRules>
{
public:

  StechecApi(GameData* gameData, Client* c);
  virtual ~StechecApi();

  //! @brief Called by the Api to ask something to the server.
  void SendToServer(const StechecPkt& com);

  //! @brief Called by the Api to end the turn.
  void doEndTurn();

  //! @brief Return true if this client is a viewer.
  bool isViewer() const;

  //! @brief Return the time remaining, in ms.
  //! @note Return a very high  value (99999) if the timer is not active.
  int getTimeRemaining() const;

  //! @brief Return the extra time remaining, in ms.
  //! @note Return a very high  value (99999) if the timer is not active.
  int getExtraTimeRemaining() const;
  
  //! @brief Switch to the selected team id.
  //!
  //! Further calls to the API will return the same thing that if the call
  //! has been made by this team.
  //!
  //! @note This method is only revelant for a viewer. For a non-viewer, it
  //!    will not have any effect.
  //! @param team_id The selected team_id, or -1 to switch to the super-user,
  //!    ie you can see through fog of war (depending on rules implementation).
  bool switchTeam(int team_id);

protected:
  GameData*     g_;
  Client*       c_;

  //! @brief Called whenever a viewer has changed his uid.
  //! @note Never called for a coach client.
  //! @see StechecApi::switchTeam
  virtual void teamSwitched() = 0;

private:

  // Rules can fetch the list of actions, and send it to the server.
  friend class CRules;

  void          setClientRules(CRules* rules);
  CommandList&  getCommitList();

  CommandList   commit_;
};




inline void StechecApi::SendToServer(const StechecPkt& com)
{
  // check if already exists in commit list.
  for (CommandList::const_iterator it = commit_.begin();
       it != commit_.end(); ++it)
    {
      if (it->getNbArg() != com.getNbArg() || it->type != com.type)
        continue;
      int i;
      for (i = 0; i < com.getNbArg(); i++)
        if (it->arg[i] != com.arg[i])
          break;
      if (i == com.getNbArg())
        return;
    }

  LOG3("Api::SendToServer: %1", com);
  commit_.push_back(com);
}

inline bool StechecApi::isViewer() const
{
  return rules_->getCoachId() >= UID_VIEWER_BASE;
}

inline int StechecApi::getTimeRemaining() const
{
  return rules_->getLimiter().getTimeRemaining();
}

inline int StechecApi::getExtraTimeRemaining() const
{
  return rules_->getLimiter().getExtraTimeRemaining();
}


#endif // !STECHECAPI_HH_
