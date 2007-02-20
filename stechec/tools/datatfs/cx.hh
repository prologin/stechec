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

#ifndef CX_HH_
# define CX_HH_

/*! @file cx.hh
**
** @defgroup tools_net Networking tools.
** @{
*/

# include "tools.hh"

//! @brief Network tokens.
//! Handled by Stechec, either client, server, or base_rules.
enum {
  CX_INIT,
  CX_ACCEPT,
  CX_DENY,
  CX_QUERYLIST,
  CX_LIST,
  CX_JOIN,
  CX_READY,
  CX_ABORT
};

// Please don't play with them.
# define BUFF_SIZE              8192
# define PACKET_MAX_SIZE        4096
# define HEADER_SIZE            10

//! @brief Default connection timeout (in seconds).
# define DEFAULT_NETCX_TO       8

/*!
** @brief Exception class for network errors.
*/
class NetError : public Exception
{
public:  
  //! @brief Standard constructor.
  //! @param msg The failing message.
  NetError(const std::string& msg);
};

    
/*!
** @brief Exception class for file.
*/
class FileIOError : public Exception
{
public:
  //! @brief Standard constructor.
  //! @param msg The failing message.
  FileIOError(const std::string& msg);
};


# define CX_RW        0
# define CX_RO        1
# define CX_WO        2

/*! \brief Base class for network/file data transfert.
**
** This class contain only methods related to data transfert (send,
** receive). Use a derived class to be able to open a connection, like
** FileCx or TcpCx. You can send/receive arbitrary data or file, or
** you can use the Packet class that already contains useful
** attribute.
**
** Take care that all read/write functions throw exception on error,
** instead of using a return value indicator. Be sure to add the necessary
** try/catch block around.
**
** \author victor
** \date 01/04/2005
** \see FileCx
** \see TcpCx
** \see Packet
*/
class Cx
{
public:
  Cx();
  virtual ~Cx();

  //! \brief Send arbitrary data.
  //! \param data Data to send.
  //! \param size Data size (in bytes).
  //! \exception NetError Thrown on network error, such as timeout or broken
  //! pipe.
  void send(const char* data, unsigned size);
  //! \brief Send a packet.
  //! \param com The packet to send.
  //! \exception NetError Thrown on network error, such as timeout or broken
  //! pipe.
  virtual void send(const Packet* com);
  //! \brief Send a file.
  //! \param filename File to open and read data from.
  //! \exception NetError Thrown on network error, such as timeout or broken
  //! pipe.
  //! \exception FileIOError IO error, such as non-existing file.
  void sendFile(const char* filename);

  
  //! \brief Receive arbitrary data.
  //! \param size Returned value, contains the size of the allocation (in bytes)
  //! \return Allocated memory zone, containing the received data.
  //! \exception NetError Thrown on network error, such as timeout or broken
  //! pipe.
  void* receive(unsigned* size);
  //! \brief Receive a Packet class.
  //! \exception NetError Thrown on network error, such as timeout or broken pipe.
  //! \return A Packet.
  //! \note packet is allocated, and never deleted.
  virtual Packet* receive();
  //! \brief Receive a file.
  //! \param filedest File to open and write data to.
  //! \exception NetError Thrown on network error, such as timeout or broken
  //! pipe.
  //! \exception FileIOError IO error, such as broken filesystem.
  void receiveFile(const char* filedest);

  //! @brief Get the file descriptor connected to the remote host.
  int getFd() const; 

  //! @brief Check if there is some data to fetch.
  //! @return true if there is some data to fetch.
  virtual bool poll(int timeout = 0) = 0;

  //! @brief Print some debug info into the stream.
  friend std::ostream& operator<< (std::ostream& os, const Cx& cx);
  
protected:
  //! @brief Print some debug info.
  virtual void print(std::ostream& os) const;

  int   fd;                           ///< The file descriptor.
  char  buff_recv_[PACKET_MAX_SIZE];  ///< Internal buffer for incoming packets.
  char  buff_send_[PACKET_MAX_SIZE];  ///< Internal buffer for outcoming packets.
  char  msgerr[200];                  ///< Error message buffer for internal use.
  int   method;                       ///< CX_RO, CX_WO or CX_RW.

private:
  int sendHeader(unsigned size);
  int recvHeader(char *method);
  int finalizeRcvTransfert(int cnt, int snd_bytes);
  int finalizeSndTransfert(int cntr, int cntw, int snd_bytes);
};


//
// Cx packets, managed by clients and server, not by rules.
//
struct CxInit : public Packet
{
  CxInit()
   : Packet(CX_INIT, -1) { data_size = sizeof(*this); }
  int binary_version;
};

struct CxDeny : public Packet
{
  CxDeny()
   : Packet(CX_DENY, -1) { data_size = sizeof(*this); }
  int reason[16];
};

struct CxList : public Packet
{
  CxList()
   : Packet(CX_LIST, -1) { data_size = sizeof(*this); }
  int server_name[8];
  int game_uid;
  int nb_coach;
  int nb_viewer;
};

struct CxJoin : public Packet
{
  CxJoin(int player_id = -1)
    : Packet(CX_JOIN, player_id) { data_size = sizeof(*this); }
  int rules_name[8];
  int rules_major;
  int rules_minor;
  int game_uid;
  int is_coach;
  int client_extid;
};

//! !defgroup
//! @}

# include "cx.hxx"

#endif /* !CX_HH_ */
