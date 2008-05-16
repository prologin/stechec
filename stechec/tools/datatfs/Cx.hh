/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2007 Prologin
*/

#ifndef CX_HH_
# define CX_HH_

# include "datatfs/Packet.hh"
# include "misc/Exception.hh"

//! @ingroup tools_net
const unsigned CX_TOKEN_START = 0x8000;

/*!
** @brief Network tokens.
** @ingroup tools_net
** Handled by Stechec in base_rules.
*/
enum eCxToken {
  CX_INIT = CX_TOKEN_START,
  CX_ACCEPT,
  CX_DENY,
  CX_QUERYLIST,
  CX_LIST,
  CX_JOIN,
  CX_READY,
  CX_ABORT,
  CX_ERROR,
  CX_TOKEN_LAST,
};

/*!
** @brief Exception class for network errors.
** @ingroup tools_net
*/
class NetError : public Exception
{
public:
  //! @brief Standard constructor.
  //! @param msg The failing message.
  NetError(const std::string& msg);
};

/*!
** @brief Exception class for network errors.
** @ingroup tools_net
*/
class NetSysError : public NetError
{
public:
  //! @brief Standard constructor.
  //! @param msg The failing message.
  NetSysError(const std::string& msg);
};

//! @ingroup tools_net
//! @{

const unsigned PACKET_SIZE_MAX  = 2048;
const unsigned BUF_SEND_SIZE    = PACKET_SIZE_MAX * 4;
const unsigned BUF_RECV_SIZE    = PACKET_SIZE_MAX * 2;

const int CX_CLOSED     = 0x00;
const int CX_READ       = 0x01;
const int CX_WRITE      = 0x02;
const int CX_RW         = 0x03;
const int CX_CONNECTING = 0x04;
const int CX_LISTENING  = 0x08;

//! @}

/*!
** @brief Base class for network/file data transfert.
** @ingroup tools_net
**
** This class contain only methods related to data transfert (send,
** receive). Use a derived class to be able to open a connection, like
** FileCx or TcpCx. You can send/receive arbitrary data or file, or
** you can use the Packet class that already contains useful
** attributes.
**
** Take care that all read/write functions throw exception on error,
** instead of using a return value indicator. Be sure to add the necessary
** try/catch blocks.
**
** @see TcpCx
** @see Packet
*/
class Cx
{
  template<typename> friend class CxPool;

public:
  Cx();
  virtual ~Cx();

  //! @brief Check if there is some data to fetch.
  //! @note Be sure to call this function before trying to retrieve data.
  //! @return true if there is some data to fetch.
  //! @exception NetError Thrown on network error, such as timeout or broken pipe.
  bool poll(int timeout = 0);

  //! @brief Begin a send-in-batch-mode packet.
  void begin();
  //! @brief End a send-in-batch-mode packet. It flush the write buffer.
  void commit();
  //! @brief Send a packet, or add it to the write buffer if in batch mode.
  //! @param pkt Packet to send.
  //! @exception NetError Thrown on network error, such as timeout or broken
  //! pipe.
  void send(const Packet& pkt);

  //! @brief Receive a Packet class.
  //! @return A Packet.
  //! @note packet is allocated, and never deleted.
  Packet* receive();

  //! @brief Get last error
  const std::string getLastError() const;

  //! @brief Print some debug info into the stream.
  friend std::ostream& operator<< (std::ostream& os, const Cx& cx);

protected:
  virtual bool pollInternal(int* timeout) = 0;
  virtual int recvData(bool use_exception) = 0;
  virtual void sendData(unsigned char* data, unsigned size) = 0;

  //! @brief Print some debug info.
  virtual void print(std::ostream& os) const;

  int		fd_;                          ///< The file descriptor.
  int		state_;                       ///< CX_WRITE / CX_READ.
  char          last_error_[64];              ///< Last cx error, if not using exception.

  // Send buffer
  unsigned char	buff_send_[BUF_SEND_SIZE];    ///< Internal buffer for outcoming packets.
  bool		batch_send_;		      ///< True if we must pack messages before sending.
  unsigned	buff_send_tail_;	      ///< Index to the end of used buff_send_.

  // Receive buffer
  unsigned char buff_recv_[BUF_RECV_SIZE];    ///< Internal buffer for incoming packets.
  unsigned	buff_recv_head_;
  unsigned	buff_recv_tail_;

private:
  bool arrangeReadBuffer();
};


inline void  Cx::print(std::ostream&) const
{
}

inline std::ostream& operator<< (std::ostream& os, const Cx& cx)
{
  cx.print(os);
  return os;
}

//
// Cx packets, managed by clients and server, not by rules.
//

//! @ingroup tools_net
struct CxInit : public Packet
{
  CxInit()
   : Packet(CX_INIT, -1) { data_size = sizeof(*this); }
  int binary_version;
};

//! @ingroup tools_net
struct CxDeny : public Packet
{
  CxDeny()
   : Packet(CX_DENY, -1) { data_size = sizeof(*this); }
  int reason[16];
};

//! @ingroup tools_net
struct CxList : public Packet
{
  CxList()
   : Packet(CX_LIST, -1) { data_size = sizeof(*this); }
  int server_name[8];
  int game_uid;
  int nb_coach;
  int nb_viewer;
};

//! @ingroup tools_net
struct CxJoin : public Packet
{
  CxJoin(int player_id = -1)
    : Packet(CX_JOIN, player_id) { data_size = sizeof(*this); }
  int rules_name[8];
  int server_lib[16];
  int rules_major;
  int rules_minor;
  int game_uid;
  int is_coach;
  int client_extid;
};

//! @ingroup tools_net
struct CxError : public Packet
{
  CxError(int player_id = -1)
    : Packet(CX_ERROR, player_id) { data_size = sizeof(*this); }
  int msg[16];
};

#endif /* !CX_HH_ */
