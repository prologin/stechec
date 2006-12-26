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

# include <SDL.h>

# include "tools.hh"
# include "Event.hh"
# include "DlgBox.hh"

// All forward decl. Avoid recompilation of all sdlvisu when a header
// is modified.
namespace xml { class XMLConfig; }
class Api;
class ClientCx;
class SDLWindow;
class Input;
class Sprite;
class TextSurface;
class VirtualSurface;

BEGIN_NS(sdlvisu);

class Panel;
class VisuField;
class VisuPlayer;
class ActionPopup;

/*!
** @brief Visu global status.
** Element with lower value (top of the list) will have higher priority.
*/
enum eState {
  stPause = 0,
  stShowDlgBox = 10,
  stWaitInput,
  stWait = 20,
  stDoKickoff = 30,
  stBlockPushChoice,
  stPopupShow = 40,
  stDoAction = 50,
  stWaitPlay = 60,
  stWaitOther,
  stNothing = 70    ///< last prio - matched if there is nothing else
};

/*!
** @brief Main class to create on new game, to destroy when a game is
** finished. Control all the game.
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
  VisuField&            getField();

  //! @brief Unselect all selected players on the screen.  
  void unselectAllPlayer();

  //! @brief Check if the specified state is set (ie, the one which have the
  //!  higher priority);
  bool isStateSet(enum eState s) const;

  //! @brief Set a status in the list.
  void setState(enum eState s);

  //! @brief Remove a status from the list of currently set status.
  void unsetState(enum eState s);

  //! @brief Show dialog box one at a time. If a box is already displayed,
  //!  delay drawing of it.
  void pushDialogBox(DialogBox* dlgbox);
  
  int run();

private:

  void initDialogBoxes();
  
  /*
  ** All Events.
  */
  virtual void evIllegal(int was_token);
  virtual void evNewTurn(int player_id, int cur_half, int cur_turn);
  virtual void evEndGame();
  virtual void evMoveTurnMarker();
  virtual void evTimeExceeded();
  virtual void evPlayerKnocked(int team_id, int player_id);
  virtual void evKickOff();
  virtual void evChat(const std::string& msg);
  virtual void evPlayerCreate(int team_id, int player_id);
  virtual void evPlayerPos(int team_id, int player_id, const Point& pos);
  virtual void evPlayerMove(int team_id, int player_id, const Point& pos);
  virtual void evBallPos(const Point& pos);
  virtual void evGiveBall();
  virtual void evResult(int team_id, int player_id, enum eRoll action_type, int result, 
			int modifier, int required, bool reroll);
  virtual void evBlockResult(int team_id, int player_id, int opponent_id, 
			     int nb_dice, enum eBlockDiceFace result[3],
			     int choose, bool reroll);
  virtual void evFollow();
  virtual void evBlockPush(Position pos, int nb_choice, Position choices[]);

  SDLWindow&            win_;    ///< The SDL window.
  xml::XMLConfig*       xml_;    ///< Configuration file.
  Api*                  api_;    ///< Interface with client's rules.
  ClientCx*             ccx_;    ///< Connection with the server.

  Panel*                panel_;  ///< Game panel.
  VisuField*            field_;  ///< Game field.
  ActionPopup*          action_popup_;  ///< Action for player, on left click.

  VisuPlayer*           player_[2][16]; ///< Players...

  TextSurface           txt_status_;
  std::set<enum eState> state_list_;

  std::deque<DialogBox*> dlg_box_list_;
  DialogBox*		dlg_play_;
  DialogBox*		dlg_touchback_;
  DialogBox*		dlg_follow_;
  DialogBox*		dlg_reroll_;

  Sprite                block_push_[3];
  
  class DlgFollowAnser : public DialogBoxCb
  {
  public:
    DlgFollowAnser(Api* api) : api_(api) {}
  private:
    virtual void clicked(int btn_index);
    Api* api_;
  } dlg_follow_answer_;

  class DlgRerollAnser : public DialogBoxCb
  {
  public:
    DlgRerollAnser(Api* api) : api_(api) {}
  private:
    virtual void clicked(int btn_index);
    Api* api_;
  } dlg_reroll_answer_;

};

END_NS(sdlvisu);

#endif /* !GAME_HH_ */
