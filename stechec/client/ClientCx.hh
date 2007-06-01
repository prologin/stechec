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

#ifndef CLIENTCX_HH_
# define CLIENTCX_HH_

# include "tools.hh"
# include "xml/xml_config.hh"
# include "PacketSender.hh"
# include "BaseCRules.hh"

/*! @file
**
** @defgroup client Generic client application.
** @{
*/

class Cx;

/*!
** @brief Handle connection with the server.
**
** Takes care of network connection with the server, provides methods
** to connect, disconnect, ...
*/
class ClientCx : public PacketSender
{
public:
  ClientCx();
  virtual ~ClientCx();

  //! @brief Connect to the server.
  //! @param cfg The client configuration.
  //! @return true if the connection was successful, false otherwise.
  bool          connect(xml::XMLConfig& cfg);

  //! @brief Join a game on the server.
  //! @param cfg The client configuration.
  //! @param desc The rules description (usually loaded from dynamic library).
  //! @return true if the join was successful, false otherwise.
  bool		join(xml::XMLConfig& cfg, const struct RuleDescription& desc);
    
  //! @brief Open a log file (for game replay).
  //! @note Mutually exclusive with connect()/join().
  bool          openLog(const std::string& filename);

  //! @brief Disconnect from the server, or close the log file.
  void          disconnect(bool send_abort = false);

  //! @brief Is there currently a connection with the server.
  bool          isConnected() const;
  
  //! @brief Main method to fetch and handle incoming packets.
  //! @return true if a packet was processed.
  bool          process(bool block = false);

  //! @brief Set Client rules.
  void		setRules(BaseCRules* rules);

  //! @brief Set client game ID.
  void		setClientGameId(int client_gid);
  
  //! @brief Set readyness. See how specific Rules handle this state, mostly
  //!    useful for viewers.
  void          setReady();


private:
  //! @brief Last call to send a packet to the arbiter.
  virtual void  sendPacket(const Packet& p);

  virtual void  gameFinished();

  //! @brief Connect to the server.
  bool          connect(const std::string& host, int port);
  bool          join(int game_uid, bool wanna_be_viewer, const struct RuleDescription& desc);

  //! @brief Syncing with the arbiter, in standalone mode.
  void          createDirectCx(int uid);
  bool          syncArbiter();

  Cx*           cx_;
  BaseCRules*   rules_;
  int           client_gid_;
};

//! @}

#endif /* !CLIENTCX_HH_ */
