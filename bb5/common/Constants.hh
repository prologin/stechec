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

#ifndef CONSTANTS_HH_
# define CONSTANTS_HH_

/*! @file Constants.hh
**
** Put here some meaningful constants here.
*/

# include "PacketHandler.hh"

// Constants describing the current status of the game.
const int GS_INITGAME      = 0x0001;
const int GS_INITHALF      = 0x0002;
const int GS_COACH1        = 0x0004; ///< Coach 1 turn.
const int GS_COACH2        = 0x0008; ///< Coach 2 turn.
const int GS_PAUSE         = 0x0010;

const int GS_COACHBOTH     = 0x000C; ///< Any coach turn.

// Field limits.
const int COLS             = 15; ///< [| 0, 14 |]
const int ROWS             = 26; ///< [| 0, 25 |]

//! @brief Maximum number of players
const int MAX_PLAYER	   = 16;

//! @brief Constant defining turnover action result.
const int ACT_RES_TO       = 0xDEADBEEF; 


//! @brief Constants that can be returned by the API.
const int BAD_TEAM         = -3;
const int BAD_PLAYER       = -2;
const int BAD_ARGUMENT     = -1;
const int SUCCESS          = 0;

//! @brief Tokens used for the game.
//! @see Packet.
//! @note Keep this list below 250, otherwise you'll run into trouble.
enum {
  MSG_CHAT = 10,
  MSG_ILLEGAL,
  MSG_INITGAME,
  MSG_INITHALF,
  MSG_NEWTURN,
  MSG_ENDTURN,
  MSG_ENDGAME,
  MSG_BALLPOS,
  MSG_WEATHER,
  MSG_TIMEEXCEEDED,
  MSG_TEAMINFO,
  MSG_PLAYERINFO,
  MSG_PLAYERPOS,
  MSG_PLAYERKNOCKED,
  MSG_PLAYERSTATUS,
  MSG_ROOLINFO, // FIXME: complete usage
  ACT_MOVETURNMARKER,
  ACT_ILLEGALPROC,
  ACT_MOVE,
  ACT_BLOCK,
  ACT_BLOCKPUSH,
  ACT_MULTIBLOCK,
  ACT_PASS
};

enum eStatus {
  STA_UNASSIGNED,

  // Common status
  STA_RESERVE,
  STA_STANDING,
  STA_PRONE,
  STA_STUNNED,
  STA_KO,
  STA_INJURIED,
  STA_SEVERE_INJURIED,
  STA_DEAD,
  STA_SENTOFF,

  // skill/trait related... for futur usage
  STA_ROOTED,
  STA_BONEHEAD,
  STA_STUPID,
  STA_GAZE
};


/*!
** Messages (Packet) used to control game status.
*/
DECLARE_EMPTY_PACKET(MSG_INITGAME, MsgInitGame);
DECLARE_PACKET(MSG_INITHALF, MsgInitHalf)
  int cur_half;
END_PACKET
DECLARE_PACKET(MSG_NEWTURN, MsgNewTurn)
  int cur_turn;
END_PACKET
DECLARE_EMPTY_PACKET(MSG_ENDTURN, MsgEndTurn);
DECLARE_EMPTY_PACKET(MSG_ENDGAME, MsgEndGame);
DECLARE_EMPTY_PACKET(MSG_TIMEEXCEEDED, MsgTimeExceeded);

DECLARE_EMPTY_PACKET(ACT_MOVETURNMARKER, ActMoveTurnMarker);
DECLARE_EMPTY_PACKET(ACT_ILLEGALPROC, ActIllegalProc);

DECLARE_PACKET(MSG_CHAT, MsgChat)
  int msg[32];
END_PACKET

// A coach tried something forbidden.
DECLARE_PACKET(MSG_ILLEGAL, MsgIllegal)
  int was_token;
END_PACKET


#endif /* !CONSTANTS_HH_ */
