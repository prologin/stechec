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

#ifndef BASECRULES_HH_
# define BASECRULES_HH_

# include "BaseRules.hh"

class Event;
class Api;

class BaseCRules : public BaseRules
{
public:
  BaseCRules();
  virtual ~BaseCRules();
  
  //! @brief Get the team id.
  //! @return -1 when server, otherwise [|0..nb_players-1|]
  int           getTeamId() const;

  //! @brief Register the class that will handle events.
  //! @param evp Class that will receive event. Contest-specific.
  void          setEventHandler(Event* evp);

  //! @brief Send an event to the UI.
  //! Warning: Ugly kludge spotted. On contest Event dispatch(),
  //! you will receive a Packet with token == CUSTOM_EVENT, and
  //! client_id containing this parameter. Dispatch it as you like.
  void		onEvent(int event);

  //! @brief Send an event to the UI.
  //! @param pkt Packet to send to the receiver.
  void          onEvent(const Packet* pkt);

  //! @brief Get the real API for the contest (client-side only).
  virtual Api*  getApi() = 0;

private:
  void          msgCatchUid(const ClientUid* m);
  void          msgCatchGameFinished(const GameFinished* m);
  int           team_id_;
  EventProcess  evp_;
};

# include "BaseCRules.hxx"

#endif /* !BASECRULES_HH_ */
