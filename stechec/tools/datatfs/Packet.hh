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
** `COPYING' file in the root directory.
**
** The TBT Team consists of people listed in the `AUTHORS' file.
*/

#ifndef PACKET_HH_
# define PACKET_HH_

# include <sstream>

/*!
** @brief Container for data passed through network.
** @ingroup tools_net
** @author victor
** @date 28/12/2005
*/
struct Packet
{
  //! @brief Type of packet carried. Rules specific.
  unsigned      token;
  //! @brief Source or destination client.
  //!   - server -> client: targeted client, or -1 if all.
  //!   - client -> server: source client.
  int           client_id;
  //! @brief Packet size, this header is included.
  unsigned      data_size;

  Packet(int token = 0, int client_id = -1);
  friend std::ostream& operator<< (std::ostream& os, const Packet& pkt);
};


inline Packet::Packet(int token, int client_id)
  : token(token), client_id(client_id)
{
  data_size = sizeof(*this);
}

inline std::ostream& operator<< (std::ostream& os, const Packet& pkt)
{
  os << "*** Packet dump ***" << std::endl;
  os << "  - token : " << pkt.token << std::endl;
  os << "  - player_id : " << pkt.client_id << std::endl;
  os << "  - data_size : " << pkt.data_size << std::endl;
  return os;
}

//! @ingroup tools_net
//! @{

//! @brief Encapsulate a string into a packet.
//! @param dst Packet attribute that will receive the string.
//! @param src Source string.
//! @param max_size Size of dst, in bytes.
void          stringToPacket(int* dst, const std::string& src, unsigned max_size);

//! @brief Decapsulate a string from a packet.
std::string   packetToString(const int* src);

//! @}

#endif /* !PACKET_HH_ */
