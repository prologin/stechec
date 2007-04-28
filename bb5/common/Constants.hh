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

#ifndef CONSTANTS_HH_
# define CONSTANTS_HH_

/*!
** @ingroup rules
** @file common/Constants.hh
**
** Put here some meaningful constants here.
*/

/*!
** @defgroup rules Blood Bowl rules manager (according to LRB5).
*/
//@{

# include "BaseRules.hh"

// Constants describing the current status of the game.
enum {
  GS_INITGAME = 1,  
  GS_DRAWKICKER,  ///< wait for coach to choose either to kick or to receive.
  GS_INITKICKOFF, ///< wait for coach to set up team placement.
  GS_KICKOFF,     ///< wait for coach to place the ball on the field.
  GS_COACH1,      ///< Coach 1 turn.
  GS_COACH2,      ///< Coach 2 turn.
  GS_TOUCHBACK,   ///< wait for coach to give the ball to one of his players.
  GS_REROLL,      ///< wait for coach to use or not a reroll
  GS_BLOCK,       ///< wait for coach to choose a block dice
  GS_PUSH,        ///< wait for coach to choose a square
  GS_FOLLOW,      ///< wait for coach to follow or not
  GS_SKILL,       ///< wait for coach to use or not a skill
};

// Field limits.
const int COLS             = 15; ///< [| 0, 14 |]
const int ROWS             = 26; ///< [| 0, 25 |]
const int SIDE             = 4;  ///< [| 0, 3 |] [| 11, 14 |]

//! @brief Maximum number of players on a team.
const int MAX_PLAYER       = 16;


//! @brief Constants that can be returned by the API.
const int INVALID_ACTION   = -4;
const int BAD_TEAM         = -3;
const int BAD_PLAYER       = -2;
const int BAD_ARGUMENT     = -1;
const int SUCCESS          = 0;

//! @brief Tokens used for the game.
//! @see Packet.
//! @note Keep this list below MAX_TOKEN (256), otherwise you'll run into trouble.
enum {
  MSG_CHAT,
  MSG_ILLEGAL,
  MSG_INITGAME,
  MSG_INITHALF,
  MSG_DRAWKICKER,
  MSG_INITKICKOFF,
  MSG_CHEATDICE,
  MSG_RESULT,
  MSG_BLOCKRESULT,
  MSG_BLOCKDICE,
  MSG_FOLLOW,
  MSG_REROLL,
  MSG_NEWTURN,
  MSG_ENDTURN,
  MSG_ENDGAME,
  MSG_BALLPOS,
  MSG_GIVEBALL,
  MSG_WEATHER,
  MSG_TURNOVER,
  MSG_TEAMINFO,
  MSG_PLAYERCREATE,
  MSG_PLAYERPOS,
  MSG_PLAYERKNOCKED,
  MSG_PLAYERSTATUS,
  MSG_PLAYERKO,
  MSG_SKILL,
  MSG_ROLLINFO,     // FIXME: complete usage
  MSG_MOVETURNMARKER,
  MSG_ILLEGALPROC,
  MSG_DECLARE,
  MSG_MOVE,
  MSG_STANDUP,
  MSG_BLOCK,
  MSG_BLOCKPUSH,
  MSG_MULTIBLOCK,
  MSG_PASS,
  RULES_TOKEN_LAST,
};

//! @brief Player status.
//! @note You can get a stringified version into the
//!   class Player.
enum eStatus {
  STA_UNASSIGNED = 0,

  // Common status
  STA_RESERVE,
  STA_STANDING,
  STA_PRONE,
  STA_STUNNED,
  STA_KO,
  STA_INJURED,
  STA_SENTOFF

  // skill/trait related... for futur usage
  //STA_ROOTED,
  //STA_BONEHEAD,
  //STA_STUPID,
  //STA_GAZE
};

//! @brief Player actions.
//! @note You can get a stringified version into the
//!   class Player.
enum eDeclaredAction {
  DCL_NONE = 0,
  DCL_MOVE,
  DCL_BLOCK,
  DCL_BLITZ,
  DCL_PASS
};

//! @brief Type of dice Roll
//! @note You can get a stringified version into the
//!   class Dice.
enum eRoll {
  R_NONE = 0,
  R_ARMOUR,
  R_BLOCK,
  R_CATCH,
  R_DODGE,
  R_INJURY,
  R_PICKUP,
  R_STANDUP,
  R_THROW
};

//! @brief List of player skills.
//! @note You can get a stringified version into the
//!   class Player.
enum eSkill {
  SK_NONE = 0,
  SK_BLOCK,
  SK_CATCH,
  SK_DODGE,
  SK_PASS,
  SK_SUREHANDS
};

//! @brief Motive for turnover.
//! @note FIXME: propose a stringified version.
enum eTurnOverMotive {
  TOM_NONE = 0,
  TOM_KNOCKEDDOWN,
  TOM_LOSTBALL,
  TOM_FAILEDPICKUP,
  TOM_TOUCHDOOOWN,
  TOM_TIMEEXCEEDED,
  TOM_FUMBLEDPASS,
  TOM_THROWNTMFAILED,
  TOM_EJECTEDFORAFOUL
};

//! @brief List of possible errors, for use in MsgIllegal.
//! @note FIXME: propose a stringified version.
enum eError {
  ERR_NONE = 0,
  ERR_UNASSIGNED,
  ERR_UNREADABLE,
  ERR_WRONGCONTEXT,
  ERR_ALREADYEXISTS,
  ERR_NONEXISTENTPLAYER,
  ERR_CANNOTENTERINPLAY,
  ERR_TOOMANYPLAYERSINWIDEZONE,
  ERR_LINEOFSCRIMMAGENOTFILLED,
  ERR_TOOMANYPLAYERSINRESERVE,
  ERR_INVALIDBALLPLACEMENT,
  ERR_CANNOTCARRYTHEBALL,
  ERR_HASPLAYED,
  ERR_ISPLAYING,
  ERR_SINGLEACTIONUSED,
  ERR_DOESNTOWNTHEBALL,
  ERR_CANTPASSTOTHECROWD,
  ERR_CANTPASSTHATFARAWAY,
  ERR_NOTENOUGHMOVEMENT,
  ERR_NOTADJACENTSQUARE,
  ERR_NOTEMPTYSQUARE,
};


//! @brief Token string constants for bb5 rules.
//! @note Keep it synchronized with all MSG_ enum below.
extern const char *bb5_token_str[RULES_TOKEN_LAST];

//@}

/*
** Messages (Packet) used to control game status.
*/
DECLARE_EMPTY_PACKET(MSG_INITGAME, MsgInitGame);
DECLARE_PACKET(MSG_INITHALF, MsgInitHalf)
  int cur_half;
END_PACKET
DECLARE_PACKET(MSG_DRAWKICKER, MsgDrawKicker);
  bool kickoff;
END_PACKET
DECLARE_PACKET(MSG_INITKICKOFF, MsgInitKickoff);
  int place_team;
END_PACKET
DECLARE_PACKET(MSG_REROLL, MsgReroll)
  bool reroll;
END_PACKET
DECLARE_PACKET(MSG_NEWTURN, MsgNewTurn)
  int cur_half;
  int cur_turn;
END_PACKET
DECLARE_EMPTY_PACKET(MSG_ENDTURN, MsgEndTurn)
DECLARE_EMPTY_PACKET(MSG_ENDGAME, MsgEndGame);
DECLARE_PACKET(MSG_TURNOVER, MsgTurnOver)
  int motive; // enum eTurnOverMotive
END_PACKET
DECLARE_EMPTY_PACKET(MSG_MOVETURNMARKER, MsgMoveTurnMarker);
DECLARE_EMPTY_PACKET(MSG_ILLEGALPROC, MsgIllegalProc);

DECLARE_PACKET(MSG_CHAT, MsgChat)
  int msg[32];
END_PACKET

// A coach tried something forbidden.
DECLARE_PACKET(MSG_ILLEGAL, MsgIllegal)
  int was_token;
  int error;
END_PACKET

#endif /* !CONSTANTS_HH_ */
