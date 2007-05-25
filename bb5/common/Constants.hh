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
*/

/*!
** @defgroup rules Blood Bowl rules manager (according to LRB5).
*/
//@{

# include "BaseRules.hh"

//! @brief Constants describing the current status of the game.
enum {
  GS_INITGAME = 1,  ///< Wait for coaches to declare their team and players.
  GS_DRAWKICKER,    ///< Wait for coach to choose to either kick off or receive the ball.
  GS_INITKICKOFF,   ///< Wait for coach to set up team placement.
  GS_KICKOFF,       ///< Wait for coach to place the ball on the field.
  GS_TOUCHBACK,     ///< Wait for coach to give the ball to one of his players.
  GS_COACH1,        ///< Coach 1 turn.
  GS_COACH2,        ///< Coach 2 turn.
// Team status.
  GS_REROLL,        ///< Wait for coach to use or not a reroll.
  GS_BLOCK,         ///< Wait for coach to choose a block dice.
  GS_PUSH,          ///< Wait for coach to choose a square.
  GS_FOLLOW,        ///< Wait for coach to follow or not, after a block.
  GS_SKILL          ///< Wait for coach to use or not a skill.
};

// Field limits.
const int COLS             = 15;  ///< Field width [| 0, 14 |]
const int ROWS             = 26;  ///< Field length [| 0, 25 |]
const int SIDE             = 4;   ///< Wide zone width [| 0, 3 |] [| 11, 14 |]

//! @brief Maximum number of players in a team.
const int MAX_PLAYER       = 16;

// Constants that can be returned by the API.
const int INVALID_ACTION   = -4;  ///< Coach's request doesn't follow the rules. @see Api
const int BAD_TEAM         = -3;  ///< No team has been selected. @see Api
const int BAD_PLAYER       = -2;  ///< No player has been selected. @see Api
const int BAD_ARGUMENT     = -1;  ///< Your client program stinks. @see Api
const int SUCCESS          = 0;   ///< Success. @see Api

//! @brief Tokens used for the game.
//! @see Packet.
//! @note Keep this list below BASE_TOKEN_START (248), otherwise you'll run into trouble.
enum {
  MSG_CHAT = 0,       ///< Coach chat message.
  MSG_ILLEGAL,        ///< Client's message has been refused.
  MSG_INITGAME,       ///< Coaches must declare their team and players.
  MSG_INITHALF,       ///< Beginning of a half-time.
  MSG_DRAWKICKER,     ///< Coach's choice to kick-off/receive.
  MSG_INITKICKOFF,    ///< Players and ball placement before a kick-off.
  MSG_CHEATDICE,      ///< Load the next dice. @see Dice.hh
  MSG_RESULT,         ///< Common rolls result. @see bb5/common/Player.hh
  MSG_BLOCKRESULT,    ///< Block rolls result. @see bb5/common/Player.hh
  MSG_BLOCKDICE,      ///< Block dice's choice. @see bb5/common/Player.hh
  MSG_FOLLOW,         ///< Player follows or not after a block. @see bb5/common/Player.hh
  MSG_REROLL,         ///< Dice's reroll. @see bb5/common/Team.hh
  MSG_NEWTURN,        ///< Beginning of a turn.
  MSG_ENDTURN,        ///< End of a turn.
  MSG_ENDGAME,        ///< End of the game.
  MSG_BALLPOS,        ///< Ball position. @see Ball.hh
  MSG_GIVEBALL,       ///< Receiving team is awarded a touchback. @see Ball.hh
  MSG_WEATHER,        ///< Weather. @see Weather.hh
  MSG_TIMER,          ///< Timer synchronization.
  MSG_TURNOVER,       ///< A turnover happens.
  MSG_TOUCHDOOOWN,    ///< A player scores a touchdooown. @see bb5/common/Team.hh
  MSG_TEAMINFO,       ///< Team general info. @see bb5/common/Team.hh
  MSG_PLAYERCREATE,   ///< New player declaration. @see bb5/common/Player.hh
  MSG_PLAYERPOS,      ///< Player position on the field. @see bb5/common/Player.hh
  MSG_PLAYERKNOCKED,  ///< Player has been knocked down. @see bb5/common/Player.hh
  MSG_PLAYERSTATUS,   ///< Player status. @see bb5/common/Player.hh
  MSG_PLAYERKO,       ///< Try to wake up KO'd player. @see bb5/common/Player.hh
  MSG_SKILL,          ///< Skill use. @see bb5/common/Player.hh
  MSG_ROLLINFO, // FIXME: complete usage ... TODO: remember which usage ... if any ...
  MSG_MOVETURNMARKER, ///< Coach declares the beginning of his turn.
  MSG_ILLEGALPROC,    ///< Coach calls for an illegal procedure.
  MSG_DECLARE,        ///< Coach declares a player action. @see bb5/common/Player.hh
  MSG_MOVE,           ///< Player moves to a square in the field. @see bb5/common/Player.hh
  MSG_STANDUP,        ///< Player stands up. @see bb5/common/Player.hh
  MSG_BLOCK,          ///< Player blocks another player. @see bb5/common/Player.hh
  MSG_BLOCKPUSH,      ///< Player pushes a player after a block. @see bb5/common/Player.hh
  MSG_MULTIBLOCK,     ///< Player blocks two players in the same time. @see bb5/common/Player.hh
  MSG_PASS,           ///< Player throws the ball. @see bb5/common/Player.hh

  RULES_TOKEN_LAST
};

//! @brief Player status.
//! @note You can get a stringified version into the
//!   class Player.
enum eStatus {
  STA_UNASSIGNED = 0,

  // Common status
  STA_RESERVE,    ///< Player is in the reserve.
  STA_STANDING,   ///< Player is standing in the field.
  STA_PRONE,      ///< Player is prone in the field.
  STA_STUNNED,    ///< Player is stunned in the field.
  STA_KO,         ///< Player is KO'd.
  STA_INJURED,    ///< Player is injured or dead.
  STA_SENTOFF,    ///< Player is ejected for a foul.

  STA_LAST = STA_SENTOFF

  // skill/trait related... for futur usage
  //STA_ROOTED,
  //STA_BONEHEAD,
  //STA_STUPID,
  //STA_GAZE
};

//! @brief Player declared actions.
//! @note You can get a stringified version into the
//!   class Player.
enum eDeclaredAction {
  DCL_UNASSIGNED = 0,

  DCL_MOVE,     ///< Move (stand up, move).
  DCL_BLOCK,    ///< Block (block only).
  DCL_BLITZ,    ///< Blitz (stand up, move, block), only one per turn.
  DCL_PASS,     ///< Pass (stand up, move, throw), only one per turn.

  DCL_LAST = DCL_PASS

  // extra rules actions:
  //DCL_HANDOFF,  ///< Hand-off, only one per turn.
  //DCL_FOUL,    ///< Foul, only one per turn.
};

//! @brief Player simple actions.
enum eRealAction {
  ACT_UNASSIGNED = 0,

  ACT_BLOCK,      ///< Block an adjacent player.
  ACT_MOVE,       ///< Move to a square.
  ACT_STANDUP,    ///< Stand up if prone.
  ACT_THROW,      ///< Throw the ball to a square.

  ACT_LAST = ACT_THROW

  // extra rules actions:
  //ACT_AGGRESS,
  //ACT_GATHER,
  //ACT_GOFORIT,
  //ACT_HANDOFF,
  //ACT_LEAP,
  //ACT_THROWTM,
  //ACT_TRANSMIT,
};

//! @brief Type of dice Roll
//! @note You can get a stringified version into the
//!   class Dice.
enum eRoll {
  R_UNASSIGNED = 0,

  R_ARMOUR,   ///< Two D6 rolls added and compared to armour value.
  R_BLOCK,    ///< One, two or three block dices rolls.
  R_CATCH,    ///< A D6 roll compared to agility with catching modifiers.
  R_DODGE,    ///< A D6 roll compared to agility with dodging modifiers.
  R_INJURY,   ///< Two D6 rolls added and compared to injury table.
  R_PICKUP,   ///< A D6 roll compared to agility with pick-up modifiers.
  R_STANDUP,  ///< A D6 roll which succeeds with results 4, 5 and 6.
  R_THROW,    ///< A D6 roll compared to agility with passing modifiers.

  R_LAST = R_THROW
};

//! @brief List of player skills.
//! @note You can get a stringified version into the
//!   class Player.
enum eSkill {
  SK_UNASSIGNED = 0,

  SK_BLOCK,         ///< Block, usable with "both down" block result.
  SK_CATCH,         ///< Catch, usable to reroll a catching roll.
  SK_DODGE,         ///< Dodge, usable to reroll a dodging roll,
                    ///< and usable with "defender stumbles" block result.
  SK_PASS,          ///< Pass, usable with throwing roll.
  SK_SUREHANDS,     ///< Sure Hands, usable to reroll a picking up roll.

  SK_LAST = SK_SUREHANDS
};

//! @brief Motive for turnover.
//! @note FIXME: propose a stringified version.
enum eTurnOverMotive {
  TOM_UNASSIGNED = 0,

  TOM_KNOCKEDDOWN,    ///< A player of the active team is Knocked Down.
  TOM_LOSTBALL,       ///< @brief A ball passed, or hand-off, by a player of the active team
                      ///< is not caught by any member of the active team before the ball
                      ///< comes to rest and stops to bounce.
  TOM_FAILEDPICKUP,   ///< A player of the active team fails to pick up the ball.
  TOM_TOUCHDOOOWN,    ///< A touchdooown is scored.
  TOM_TIMEEXCEEDED,   ///< The time limit for turns runs out.
  TOM_FUMBLEDPASS,    ///< A player of the active team fumbles his pass attempt.
  TOM_THROWNTMFAILED, ///< @brief A player of the active team with the ball is thrown or is
                      ///< attempted to be thrown by a team mate and fails to land successfully.
  TOM_EJECTEDFORAFOUL,///< A player of the active team is ejected by the referee for a foul.

  TOM_LAST = TOM_EJECTEDFORAFOUL
};

//! @brief List of possible errors, for use in MsgIllegal.
//! @note FIXME: propose a stringified version.
enum eError {
  ERR_UNASSIGNED = 0,

  ERR_UNREADABLE,               ///< A parameter is wrong or out of range.
  ERR_WRONGCONTEXT,             ///< The request is not allowed in the current context.
  ERR_ALREADYEXISTS,            ///< The player already exists.
  ERR_NONEXISTENTPLAYER,        ///< The player doesn't exist.
  ERR_CANNOTENTERINPLAY,        ///< The player is KO'd, injured, dead or ejected for a foul.
  ERR_TOOMANYPLAYERSINWIDEZONE, ///< No more than two players in each wide zone per team.
  ERR_LINEOFSCRIMMAGENOTFILLED, ///< At least three players on the line of scrimmage.
  ERR_TOOMANYPLAYERSINRESERVE,  ///< If a team can't field 11 players, reserves are not allowed.
  ERR_INVALIDBALLPLACEMENT,     ///< Ball must be placed in the opponent's half of the field.
  ERR_CANNOTCARRYTHEBALL,       ///< The ball can't be given to the player concerned.
  ERR_HASPLAYED,                ///< The player has already finished his action for this turn.
  ERR_ISPLAYING,                ///< Another player is still doing his own action.
  ERR_SINGLEACTIONUSED,         ///< Blitz, pass, hand-off and foul can be done once a turn.
  ERR_DOESNTOWNTHEBALL,         ///< A player must hold the ball in order to throw it...
  ERR_CANTPASSTOTHECROWD,       ///< The target of a throw must be a square within the field.
  ERR_CANTPASSTHATFARAWAY,      ///< The length of a throw can't be larger than a Long Bomb.
  ERR_NOTENOUGHMOVEMENT,        ///< The player doesn't have enough movement remaining.
  ERR_NOTADJACENTSQUARE,        ///< The target is not in an adjacent square.
  ERR_NOTEMPTYSQUARE,           ///< The aimed square is not empty.

  ERR_LAST = ERR_NOTEMPTYSQUARE
};


//! @brief Token string constants for bb5 rules.
//! @note Keep it synchronized with all MSG_ enum below.
extern const char *bb5_token_str[RULES_TOKEN_LAST];

//@}

/*
** Messages (Packet) used to control game status.
*/
DECLARE_PACKET(MSG_INITGAME, MsgInitGame)
  int time_per_turn;
END_PACKET
DECLARE_PACKET(MSG_INITHALF, MsgInitHalf)
  int cur_half;
END_PACKET
DECLARE_PACKET(MSG_DRAWKICKER, MsgDrawKicker);
  bool kickoff;
END_PACKET
DECLARE_PACKET(MSG_INITKICKOFF, MsgInitKickoff);
  int place_team; ///< Place team or kick-off the ball.
END_PACKET
DECLARE_PACKET(MSG_NEWTURN, MsgNewTurn)
  int cur_half;
  int cur_turn;
END_PACKET
DECLARE_EMPTY_PACKET(MSG_ENDTURN, MsgEndTurn)
DECLARE_EMPTY_PACKET(MSG_ENDGAME, MsgEndGame);
DECLARE_PACKET(MSG_TIMER, MsgTimer)
  bool pause;
  int time_remaining;
END_PACKET
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
