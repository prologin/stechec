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

#include "Api.hh"
#include "Game.hh"
#include "DlgBox.hh"
#include "ActionDlg.hh"

BEGIN_NS(sdlvisu);

ActionDlg::ActionDlg(Game& g)
  : g_(g)
{
}

ActionDlg::~ActionDlg()
{
  for_all(list_, Deleter());
}

// Called when a new dialog box is about to be opened.
// Set its property, and display it.
DialogBox* ActionDlg::create(enum eDlgAction style)
{
  DialogBox* dlg = NULL;
  switch (style)
    {
    case eDlgActInfo:
      dlg = new DialogBox(g_, eDlgBoxOk, eDlgInfo);
      break;
    case eDlgActReroll:
      dlg = new DialogBox(g_, eDlgBoxYesNo, eDlgInfo);
      dlg->setText("Do you want to reroll?");
      dlg->setActionHandler(new Callback(this, &g_, style));
      break;
    case eDlgActFollow:
      dlg = new DialogBox(g_, eDlgBoxYesNo, eDlgInfo);
      dlg->setText("Do you want to follow your opponent?");
      dlg->setActionHandler(new Callback(this, &g_, style));
      break;
    case eDlgActTouchback:
      dlg = new DialogBox(g_, eDlgBoxOk, eDlgInfo);
      dlg->setText("Touchback! you can give the ball to any of your player");
      dlg->setActionHandler(new Callback(this, &g_, style));
      break;
    case eDlgActBlockDice:
      dlg = new DialogBox(g_, eDlgBoxBlock, eDlgInfo);
      dlg->setActionHandler(new Callback(this, &g_, style));
      reroll_index_ = -1;
      block_choice_ = false;
      break;
    }
  dlg->setPos(150, 150);
  return dlg;
}

// Called when a button from the dialog box is clicked.
void ActionDlg::Callback::clicked(int btn_index)
{
  Api* api = g_->getApi();

  LOG6("dlg callback for btn: %1, id: %2", btn_index, id_);
  switch (id_)
  {
    case eDlgActInfo:
      break;
    case eDlgActReroll:
      api->doReroll(btn_index == 0);
      break;
    case eDlgActFollow:
      api->doFollow(btn_index == 0);
      break;
    case eDlgActTouchback:
      g_->setState(stDoKoffGiveBall);
      break;
    case eDlgActBlockDice:
      if (btn_index == parent_->reroll_index_)
        {
          LOG4("want reroll!!!!");
          api->doReroll(true);
        }
      else if (parent_->block_choice_)
        {
          LOG4("choose dice %1", btn_index + 1);
          api->doChooseBlockDice(btn_index + 1);
        }
      break;
  }
}

void ActionDlg::addRerollLabel()
{
  reroll_index_ = front()->addRerollLabel();
}

void ActionDlg::setBlockChoice()
{
  block_choice_ = true;
}

void ActionDlg::push(enum eDlgAction id)
{
  DialogBox* dlg = create(id);

  LOG6("create dlgbox %1.", id);
  assert(dlg != NULL);
  if (list_.empty())
    {
      dlg->enable();
      LOG6("display it immediately.");
    }
  list_.push_front(dlg);
}

void ActionDlg::pop()
{
  assert(!list_.empty());

  delete list_.back()->getActionHandler();
  list_.pop_back();
  LOG6("a dlgbox has been closed");

  if (!list_.empty())
    {
      LOG6("display the next one.");
      list_.back()->enable();
    }
}

DialogBox* ActionDlg::front()
{
  assert(!list_.empty());
  return list_.front();
}

void ActionDlg::setText(const std::string& txt)
{
  assert(!list_.empty());
  list_.front()->setText(txt);
}


END_NS(sdlvisu);
