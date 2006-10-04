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

#ifndef PACKETHANDLER_HH_
# define PACKETHANDLER_HH_

//! @file PacketHandler.hh

# include "tools.hh"
# include "EventProcess.hh"

//! @brief Base packet handler.
//!
//! There is no way (that I know) to hold a collection
//! of template objects with different parameter. Thus,
//! hold them in a base classe and use virtual methods.
class BasePacketHandler
{
public:
  virtual ~BasePacketHandler() {}
  virtual bool handle(const Packet* p) = 0;
  virtual int getCstValue() const = 0;
  virtual const char* getCstStr() const = 0;
  virtual void dispatchUIEvent(const EventProcess* evp,
                               const Packet& pkt) = 0;
};

//! @brief Packet Handler. This empty class is meant to be
//! specialized, not instancied.
template <int N, typename T>
class PacketHandler : public BasePacketHandler
{
};

/*! @brief Declare a structure for a packet.
**
** @param Cst One message constant, as defined in Constants.hh.
** @param PClass The struct name, derived from Packet, to create.
**
** The following code snippet declare a new struct derived
** from Packet, and the required code for automatic message handling.
**
** @code
** DECLARE_PACKET(MSG_BALLPOS, MsgBallPos)
**   int row;
**   int col;
** END_PACKET
** @endcode
**
** @note Please take some care if you overwrite the default constructor.
** @author victor
** @date 10/01/2006
*/
# define DECLARE_PACKET(Cst, PClass)                            \
struct PClass;                                                  \
template <typename T>                                           \
class PacketHandler<Cst, T> : public BasePacketHandler          \
{                                                               \
public:                                                         \
  typedef void (T::*fct_t)(const PClass*);                      \
  typedef bool (T::*filter_fct_t)(const PClass*);               \
  PacketHandler<Cst, T>(T* obj, fct_t f, filter_fct_t ff)       \
    : obj_(obj), f_(f), filt_f_(ff) {}                          \
  virtual bool handle(const Packet* p)                          \
  {                                                             \
    if (filt_f_ &&						\
	!(obj_->*filt_f_)(reinterpret_cast<const PClass*>(p)))	\
      return false;						\
    LOG5("PacketHandler gets message '"                         \
         #Cst "' (client_id: " << p->client_id << ")");         \
    (obj_->*f_)(reinterpret_cast<const PClass*>(p));            \
    return true;                                                \
  }                                                             \
  virtual int getCstValue() const { return Cst; }               \
  virtual const char* getCstStr() const { return #Cst; }        \
  virtual void dispatchUIEvent(const EventProcess* evp,         \
                               const Packet& pkt)               \
  {                                                             \
    evp->dispatch(reinterpret_cast<const PClass&>(pkt));        \
  }                                                             \
private:                                                        \
  T* obj_;                                                      \
  fct_t f_;                                                     \
  filter_fct_t filt_f_;						\
};                                                              \
struct PClass : public Packet                                   \
{                                                               \
  PClass(int client_id = -1)                                    \
   : Packet(Cst, client_id) { data_size = sizeof(*this); }

// Ugly. To have balanced braces.
# define END_PACKET };
 
//! @brief Declare a Packet without additional attributes 
# define DECLARE_EMPTY_PACKET(Cst, PClass)      \
  DECLARE_PACKET(Cst, PClass)                   \
  END_PACKET 

#define HANDLE_WITH(Token, Class, Object, Method, When)                        \
  handleWith(new PacketHandler<Token, Class>(Object, &Class::Method, NULL), When)

#define HANDLE_F_WITH(Token, Class, Object, Method, Filter, When)              \
  handleWith(new PacketHandler<Token, Class>(Object, &Class::Method, &Class::Filter), When)


#endif /* !PACKETHANDLER_HH_ */
