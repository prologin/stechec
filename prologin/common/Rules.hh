/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2005, 2006 Prologin
*/

#ifndef RULES_HH_
# define RULES_HH_

/*! @file
**
** @defgroup stechec Stechec. Retain compatibility with Stechec V2 and Prologin contests.
**
** This file provides a general compatibility layer between
** the model used in Stechec V2 and the new one used by TBT.
**
** The old model imposed too much restrictions on the way it can be
** used. It worked in synchronous mode by default. A typical turn was:
**  - server sends a play turn request to clients,
**  - clients play their turns, then send their commands list to the server,
**  - server wait that \a all clients have ended their turns,
**  - server resolves all command lists from client,
**  - server sends reals game modification to clients.
**
** Beside this working flow, Rules writer (an human :p) was helped by
** a somewhat rigid class hierarchy, including fixed entry point,
** server resolver and client differ... More specialized, thus easier
** to implement, that's the tradeoff. When a new rule set is about to
** be implemented, if it fits the old model, it is often wise to use
** this compatibility layer. The new model offer more liberties, but
** is harder to take in hand.
**
** The compatibility is done by defining a new set of Rules/{C,S}Rules
** classes, which turn the asynchronous aspect of the new model by the
** synchronous aspect of the old one. All others specificities (entry
** points, resolver, differ) are \c emulated in these classes, and
** work the same way they used to work.
**
*/

# include "BaseRules.hh"

// Maximum number of coach (real players) that these rulesets can handle.
const int MAX_COACH             = 16;

// Constants describing the current state of the game.
// GS_CLIENT_END is like GS_END, but client must wait for GAME_FINISHED
// from BaseCRules before GS_END could be really set.
enum {
  GS_BEFOREGAME = 1,
  GS_INITGAME,
  GS_BEFORETURN,
  GS_PLAYTURN,
  GS_ENDTURN,
  GS_AFTERTURN,
  GS_AFTERGAME,
  GS_CLIENT_END,
};

// Kind of tokens used by the base rules of stechec.
enum {
  STECHEC_PKT = RULES_TOKEN_START,
  MSG_BEFORE_GAME,
  MSG_INIT_GAME,
  MSG_BEFORE_TURN,
  MSG_AFTER_TURN,
  MSG_AFTER_GAME,
  MSG_CHAMPION_ERROR
};

// Packets.
DECLARE_EMPTY_PACKET(MSG_BEFORE_GAME, MsgBeforeGame);
DECLARE_EMPTY_PACKET(MSG_INIT_GAME, MsgInitGame);
DECLARE_EMPTY_PACKET(MSG_BEFORE_TURN, MsgBeforeTurn);
DECLARE_EMPTY_PACKET(MSG_AFTER_TURN, MsgAfterTurn);
DECLARE_EMPTY_PACKET(MSG_AFTER_GAME, MsgAfterGame);

DECLARE_PACKET(MSG_CHAMPION_ERROR, MsgChampionError)
  int reason[16];
END_PACKET

//
// STECHEC_PKT represents any other kind of packet used by contests.
//
const unsigned MAX_ARG = 16;

// Maximum number of STECHEC_PKT type.
const int MAX_STECHEC_PKT_TYPE = 64;

DECLARE_PACKET(STECHEC_PKT, StechecPkt)
  int type;
private:
  int nb_args;
public:
  int arg[MAX_ARG];

  StechecPkt(int type, int player_id);
  void Push(unsigned nbarg, ...);
  int getNbArg() const;
  void setNbArg(unsigned nb);
  friend std::ostream& operator<< (std::ostream& os, const StechecPkt& pkt);
END_PACKET

inline StechecPkt::StechecPkt(int type, int player_id)
  : Packet(STECHEC_PKT, player_id),
    type(type),
    nb_args(0)
{
  data_size = sizeof(*this) - (MAX_ARG * sizeof(int));
}

inline int StechecPkt::getNbArg() const
{
  return nb_args;
}

inline void StechecPkt::setNbArg(unsigned nb)
{
  assert(nb < MAX_ARG);
  nb_args = nb;
  data_size = sizeof(*this) - ((MAX_ARG - nb_args) * sizeof(int));
}

inline void StechecPkt::Push(unsigned nbarg, ...)
{
  assert(nbarg + nb_args < MAX_ARG);

  va_list l;
  va_start(l, nbarg);
  while (nbarg--)
    {
      arg[nb_args++] = va_arg(l, unsigned);
      data_size += sizeof(int);
    }
  va_end(l);
}

#endif // !RULES_HH_
