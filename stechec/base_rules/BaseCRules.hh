/*
** TowBowlTactics, an adaptation of the tabletop game Blood Bowl.
**
** Copyright (C) 2006, 2007 The TBT Team.
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
  
  //! @brief Get coach id.
  //! @return -1 when server, otherwise [|0..nb_coach-1|]
  int           getCoachId() const;

  //! @brief Get team id.
  //! @return -1 when server, otherwise [|0..nb_team-1|]
  int           getTeamId() const;

  //! @brief Check if rules is processing an order, ie when an
  //!   order has been issued to the server, and waiting that the
  //!   server send it last answer. When sending a sequence of commands,
  //!   it is generally advised to wait that a command is finished before
  //!   sending another one. It is not mandatory.
  //! @return true if rules
  bool		isBusy() const;

  //! @brief Send a packet to the other side.
  //! @param p A packet to send.
  //! @note Override method in BaseRule, to be able to increment busy_count_.
  void          sendPacket(const Packet& p) const;
  
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
  void		msgCatchSync(const MsgSync* m);
  void          msgCatchUid(const ClientUid* m);
  void          msgCatchGameFinished(const GameFinished* m);

  int           coach_id_;
  int		team_id_;
  EventProcess  evp_;
  mutable int	busy_count_;
};

# include "BaseCRules.hxx"

#endif /* !BASECRULES_HH_ */
