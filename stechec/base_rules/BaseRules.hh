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

#ifndef BASE_RULES_HH_
# define BASE_RULES_HH_

/*! @file BaseRules.hh
*/

# include "tools.hh"
# include "PacketHandler.hh"
# include "PacketSender.hh"

# define UID_COACH_BASE		0        ///< Base uid for coachs
# define UID_VIEWER_BASE	100      ///< Base uid for viewers

//! @brief Maximum number of different packets.
const unsigned MAX_TOKEN	= 256;
//! @brief Base number for specials packets.
const unsigned BASE_TOKEN_START = MAX_TOKEN - 8;

// Some very common game state.
const int GS_WAIT		= 0xFFFE; ///< Coach waiting for another coach.
const int GS_END		= 0xFFFF; ///< End of game, nothing to do anymore.

// Viewers State (VS). Only for the server. Set by the server.
const unsigned VS_HAVEVIEWER	= 0x0800; ///< If there is at least one viewer.
const unsigned VS_READY		= 0x2000; ///< All viewers are ready.

const unsigned MSG_SYNC		= BASE_TOKEN_START; ///< @see MsgSync.
/*!
** @struct MsgSync
** @brief Synchronization packet.
** Sent (if rules wish) when an incoming packet is done (processing finished).
** Server -> Client.
*/
DECLARE_EMPTY_PACKET(MSG_SYNC, MsgSync);

// Custom events.
const unsigned CUSTOM_EVENT	= BASE_TOKEN_START + 1;

const unsigned CLIENT_UID	= BASE_TOKEN_START + 2; ///< @see ClientUid.
/*!
** @struct ClientUid
** @brief Give an uid for the client.
** Server -> Client.
*/
DECLARE_PACKET(CLIENT_UID, ClientUid);
  int league_id;
  int nb_coach;
  int nb_team;
END_PACKET

const unsigned BASE_TOKEN_LAST	= BASE_TOKEN_START + 3;

/*!
** @brief Rule module description.
**
** Should be present in each rules module, to provide information
** and checking between client and server.
*/
struct RuleDescription
{
  const char* name;
  const char* description;
  const char* server_lib;
  int major;
  int minor;
};


/*! @brief Base rules class for all kind of game.
**
** This class takes care of:
**  - basic game state, derived classes are invited to add some more.
**  - client/team id on the client side.
**
** @author victor
** @date 30/12/2005
*/
class BaseRules
{
public:
  BaseRules(int team_number);
  virtual ~BaseRules();

  /*!
  ** @name Rules internal public methods.
  ** Methods that should only be used by the derived classes
  ** of BaseRules, and all classes that are related to rule
  ** management.
  ** @{
  */

  //! @brief Send a packet to the other side.
  //! @param p A packet to send.
  //! @see setSendPacketObject
  void          sendPacket(const Packet& p) const;

  //! @brief Register a packet handler.
  void          handleWith(BasePacketHandler* bph, int when = 0);

  //! @brief Get the number of teams.
  int           getCoachNumber() const;

  //! @brief Get the number of teams.
  int           getTeamNumber() const;

  //! @brief Get the current game state.
  int           getState() const;
  //! @brief Set the current game state.
  void          setState(int new_state);

  //! @brief Debug function. Return a stringified packet token.
  const char*	stringifyToken(unsigned token) const;


  /*!
  ** @}
  **
  ** @name Rules external public methods.
  ** This set of methods must _only_ be called by the owner of
  ** BaseRules, not by the derived classes.
  ** @{
  */

  //! @brief Set the object that will manage packet sending.
  void          setSendPacketObject(PacketSender* ps);

  //! @brief Called on each recieved packet.
  void          handlePacket(const Packet* p);

  // see later...
  virtual void  serialize(std::ostream& os) const;
  virtual void  unserialize(std::istream& is);

  /*!
  ** @}
  */

protected:
  virtual const char* tokenToString(unsigned token) const;

private:
  int           state_;

protected:
  int           team_number_;

  PacketSender* packet_sender_;
  typedef std::vector<std::pair<int, BasePacketHandler*> > PktHList;
  PktHList      pkt_hdl_[MAX_TOKEN];
};

#endif /* !BASE_RULES_HH_ */
