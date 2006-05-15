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

#ifndef CLIENTCX_HH_
# define CLIENTCX_HH_

# include "datatfs/cx.hh"
# include "PacketSender.hh"
# include "BaseCRules.hh"

/*! @brief Handle connection with the server.
**
** Takes care of network connection with the server, provides methods
** to connect, disconnect, ...
*/
class ClientCx : public PacketSender
{
public:
  ClientCx(BaseCRules* rules, int client_gid);
  virtual ~ClientCx();

  //! @brief Connect to the server.
  //! @param cfg The client configuration.
  //! @return true if the connection was successful, false otherwise.
  bool          connect(xml::XMLConfig& cfg);
  //! @brief Open a log file (for game replay).
  //! @note Mutually exclusive with connect().
  bool          openLog(const std::string& filename);

  //! @brief Disconnect from the server, or close the log file.
  void          disconnect();
  //! @brief Is there currently a connection with the server.
  bool          isConnected() const;
  
  //! @brief Main method to fetch and handle incoming packets.
  //! @return true if a packet was processed.
  bool          process(bool block = false);

  //! @brief Set readyness. See how specific Rules handle this state, mostly
  //!    useful for viewers.
  void          setReady();


private:
  //! @brief Last call to send a packet to the arbiter.
  virtual void  sendPacket(const Packet& p);

  virtual void  gameFinished();
  
  //! @brief Connect to the server.
  bool          connect(const std::string& host, int port, const std::string& rules);
  bool          join(int game_uid, bool wanna_be_viewer);

  //! @brief Syncing with the arbiter, in standalone mode.
  void          createDirectCx(int uid);
  bool          syncArbiter();

  Cx*           cx_;
  BaseCRules*   rules_;
  int           client_gid_;
};

#endif /* !CLIENTCX_HH_ */
