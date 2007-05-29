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

#ifndef INTERFACE_HH_
# define INTERFACE_HH_

# include "ClientCx.hh"
# include "Api.hh"
# include "Event.hh"
# include "input.hh"

/*!
** @brief A stupid command line based client.
*/
class CmdLineInterface : public Event
{
public:
  CmdLineInterface(xml::XMLConfig* cfg, Api* api, ClientCx* client_cx, bool use_readline);

  void                hello();
  void                init();
  void                run();

  void                printGlobal();
  void                printPlayerList();
  void                printPlayer(int player_id, int team_id);
  void                printField();

private:
  bool                checkInput();

  // All callbacks.
  virtual void          evIllegal(int team_id, int was_token);
  virtual void          evInitGame();
  virtual void          evDrawKicker(int team_id, bool is_a_question);
  virtual void          evNewTurn(int player_id, int cur_half, int cur_turn);
  virtual void          evKickOff(int team_id, bool place_team);
  virtual void          evGiveBall(int team_id);
  virtual void          evMoveTurnMarker();
  virtual void          evTurnOver(enum eTurnOverMotive motive);
  virtual void          evTouchdooown(int team_id, int player_id);
  virtual void          evChat(const std::string& msg);
  virtual void          evPlayerStatus(int team_id, int player_id, enum eStatus status);
  virtual void          evPlayerPos(int team_id, int player_id, const Point& pos);
  virtual void          evPlayerMove(int team_id, int player_id, const Point& pos);
  virtual void          evPlayerKnocked(int team_id, int player_id);
  virtual void          evResult(int team_id, int player_id, enum eRoll action_type, int result, 
                                 int modifier, int required, bool reroll, enum eSkill skill);
  virtual void          evBlock(int team_id, int player_id, int opponent_id);
  virtual void          evBlockResult(int team_id, int player_id, int opponent_id, 
                                      int nb_dice, enum eBlockDiceFace result[3],
                                      int choose, bool reroll);
  virtual void          evFollow();
  virtual void          evBlockPush(const Position& pos, int nb_choice, const Position choices[]);
  virtual void          evSkill(int team_id, int player_id, enum eSkill skill, int choice);

  xml::XMLConfig*       cfg_;
  Api*                  api_;
  ClientCx*             ccx_;
  
  bool                  standalone_;
  Input                 input_;
  bool                  our_turn_;
  bool                  paused_;
};

#endif /* !INTERFACE_HH_ */
