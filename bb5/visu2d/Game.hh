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

#ifndef GAME_HH_
# define GAME_HH_

# include "Event.hh"
# include "Sprite.hh"
# include "tools.hh"

# include <SDL.h>

// All forward decl. Avoid recompilation of all sdlvisu when a header
// is modified.
namespace xml { class XMLConfig; }
class Api;
class ClientCx;
class SDLWindow;
class Input;
class TextSurface;
class VirtualSurface;

BEGIN_NS(sdlvisu);

class Map;
class Panel;
class VisuPlayer;
class ActionPopup;
class ActionDlg;

/*!
** @brief Visu global status.
** @ingroup tbt_2d_gui
** Element with lower value (top of the list) will have higher priority.
*/
enum eVisuGameState {
  VGS_PAUSE = 0,            ///< Unused for now.
  VGS_SHOWDLGBOX = 10,      ///< A dialog box is displayed.
  VGS_WAITINPUT,            ///< Unused for now.
  VGS_WAITINIT = 20,        ///< Wait for game startup.
  VGS_DOKICKOFF = 30,       ///< Wait for coach to kick-off the ball.
  VGS_DOTOUCHBACK,          ///< Wait for coach to give the ball to one of his players.
  VGS_DOPLACETEAM,          ///< It's our time to enter into the field.
  VGS_WAITKICKOFF,          ///< Wait for opponent to kick-off the ball.
  VGS_WAITPLACETEAM,        ///< Wait for opponent team to enter into the field.
  VGS_DOBLOCKPUSHCHOICE,    ///< A square must be chosen to push a player in.
  VGS_SHOWBLOCKPUSH,        ///< Push squares are shown but can not be chosen by us.
  VGS_SHOWACTIONPOPUP = 40, ///< Actions popup menu is displayed.
  VGS_DOACTION = 50,        ///< A Player will perform a real action, woow...
  VGS_DOPLAY = 60,          ///< It's our turn of game.
  VGS_WAITPLAY,             ///< It's opponent turn of game.
  VGS_NOTHING = 70          ///< Lowest priority - matched if there is nothing else.
};

/*!
** @brief Main class to create a new game.
** @ingroup tbt_2d_gui
** To destroy when a game is finished. Control all the game.
*/
class Game : public Event
{
public:
  Game(SDLWindow& win, xml::XMLConfig* xml, Api* api, ClientCx* ccx);
  ~Game();

  Api*                  getApi();
  Input&                getInput();
  VirtualSurface&       getScreen();
  Panel&                getPanel();
  Map&                  getField();

  //! @brief Unselect all selected players on the screen.  
  void unselectAllPlayer();

  //! @brief Check if the specified state is set (ie, the one which have the
  //!  higher priority);
  bool isStateSet(enum eVisuGameState s) const;

  //! @brief Set a status in the list.
  void setState(enum eVisuGameState s);

  //! @brief Remove a status from the list of currently set status.
  void unsetState(enum eVisuGameState s);

  static const char* stringify(enum eVisuGameState s);

  int run();

private:

  /*
  ** All Events.
  */

  virtual void evIllegal(int team_id, int was_token);
  virtual void evChat(const std::string& msg);

  virtual void evInitGame();
  virtual void evEndGame();

  virtual void evHalf(int half);
  virtual void evDrawKicker(int team_id, bool is_a_question);
  virtual void evPlaceTeam(int team_id);
  virtual void evKickOff(int team_id);
  virtual void evGiveBall(int team_id, int player_id);
  virtual void evNewTurn(int team_id, int cur_half, int cur_turn);
  virtual void evMoveTurnMarker();
  virtual void evTurnOver(enum eTurnOverMotive motive);
  virtual void evTouchdooown(int team_id, int player_id);

  virtual void evBallPos(const Point& pos);

  virtual void evPlayerCreate(int team_id, int player_id);
  virtual void evPlayerStatus(int team_id, int player_id, enum eStatus status);
  virtual void evPlayerPos(int team_id, int player_id, const Point& pos);
  virtual void evPlayerMove(int team_id, int player_id, const Point& pos);
  virtual void evPlayerKnocked(int team_id, int player_id);
  virtual void evPlayerKO(int team_id, int player_id, int dice);

  virtual void evDeclare(int team_id, int player_id, enum eDeclaredAction action);
  virtual void evResult(int team_id, int player_id, enum eRoll action_type, int result, 
                        int modifier, int required, bool reroll, enum eSkill skill);
  virtual void evBlockResult(int team_id, int player_id, int opponent_player_id, 
                             int nb_dice, enum eBlockDiceFace result[3],
                             int strongest_team_id, bool reroll);
  virtual void evBlockPush(const Position& pos, int nb_choice, const Position choices[]);
  virtual void evFollow();

  virtual void evReroll(int team_id, bool reroll);
  virtual void evSkill(int team_id, int player_id, enum eSkill skill, int choice);

  SDLWindow&            win_;    ///< The SDL window.
  xml::XMLConfig*       xml_;    ///< Configuration file.
  Api*                  api_;    ///< Interface with client's rules.
  ClientCx*             ccx_;    ///< Connection with the server.

  Panel*                panel_;  ///< Game panel.
  Map*                  field_;  ///< Game field.
  ActionPopup*          action_popup_;  ///< Action for player, on left click.

  VisuPlayer*           player_[2][16]; ///< Players...

  std::set<enum eVisuGameState> state_list_;
  ActionDlg*            game_dlg_; ///< List of all displayed action.

  Sprite                block_push_[3];
};

END_NS(sdlvisu);

#endif /* !GAME_HH_ */
