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

#ifndef ACTIONDLG_HH_
# define ACTIONDLG_HH_

# include "tools.hh"
# include "DlgBox.hh"

BEGIN_NS(sdlvisu);

class Game;

enum eDlgAction {
  eDlgActInfo,
  eDlgActReroll,
  eDlgActFollow,
  eDlgActTouchback,
  eDlgActBlockDice,
  eDlgActKickOrReceive
};

/*!
** Handles all game dialog boxes
*/
class ActionDlg
{
public:
  ActionDlg(Game& g);
  ~ActionDlg();

  void push(enum eDlgAction id);
  void pop();
  DialogBox* front();

  //! @brief Shortcut for dlgs.front()->setText(...)
  void setText(const std::string& txt);

  // Special for block
  void addRerollLabel();
  void setBlockChoice();
  
private:
  Game& g_;

  DialogBox* create(enum eDlgAction style);
  
  class Callback : public DialogBoxCb
  {
  public:
    Callback(ActionDlg *p, Game* g, enum eDlgAction id)
      : parent_(p), g_(g), id_(id) {}
  private:
    virtual void clicked(int btn_index);
    ActionDlg* parent_;
    Game* g_;
    enum eDlgAction id_;
  };

  std::deque<DialogBox*> list_;

  int reroll_index_;
  bool block_choice_;
};

END_NS(sdlvisu);

#endif /* !ACTIONDLG_HH_ */
