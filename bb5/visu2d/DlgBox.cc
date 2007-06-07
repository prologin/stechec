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

#include "Input.hh"
#include "Game.hh"
#include "DlgBox.hh"

BEGIN_NS(sdlvisu);

DialogBox::DialogBox(Game& game)
  : game_(game),
    handler_(NULL),
    text_("Vera.ttf", 16, 135, 90),
    dragging_(false),
    always_display_button_(false)
{
  // Set VS property. Disable by default.
  setZ(8);
  game_.getScreen().addChild(this);
  VirtualSurface::disable();

  // Attach children.
  addChild(&text_);
  addChild(&bg_);
  addChild(&icon_);
}

DialogBox::DialogBox(Game& game,
        enum eDlgBoxButton style,
        enum eDlgBoxIcon icon)
  : game_(game),
    handler_(NULL),
    text_("Vera.ttf", 16, 135, 90),
    dragging_(false),
    always_display_button_(false)
{
  // Set VS property. Disable it by default.
  setZ(8);
  game_.getScreen().addChild(this);
  VirtualSurface::disable();

  // Attach children.
  addChild(&text_);
  addChild(&bg_);
  addChild(&icon_);
  
  setStyle(style, icon);
}

DialogBox::~DialogBox()
{
  for_all(button_, Deleter());
  game_.getScreen().removeChild(this);
}

void DialogBox::setStyle(enum eDlgBoxButton style, enum eDlgBoxIcon icon)
{
  // Remove previously created button.
  ButtonList::iterator it;
  for (it = button_.begin(); it != button_.end(); ++it)
    {
      removeChild(*it);
      delete *it;
    }

  // Set text property.
  text_.setZ(5);
  text_.setPos(95, 35);

  // Set background property.
  bg_.setZ(2);
  bg_.setInheritAlpha(true);

  // Set icon property.
  icon_.load("image/dialog/icons.jpg");
  icon_.splitNbFrame(15, 1);
  icon_.setFrame(icon);
  icon_.setPos(25, 35);
  icon_.setZ(7);

  // Choose dialog-box style and create buttons
  Surface* btn;
  switch (style)
    {
    case eDlgBoxYesNo:
      {
        bg_.load("image/dialog/dialog_yesno.png");
        
        btn = new Surface("image/dialog/yes.jpg");
        btn->setZ(6);
        btn->setPos(60, 135);
        btn->hide();
        addChild(btn);
        button_.push_back(btn);

        btn = new Surface("image/dialog/no.jpg");
        btn->setZ(6);
        btn->setPos(130, 135);
        btn->hide();
        addChild(btn);
        button_.push_back(btn);
      }
      break;

    case eDlgBoxOk:
      {
        bg_.load("image/dialog/dialog_info.png");

        btn = new Surface("image/dialog/close.jpg");
        btn->setZ(6);
        btn->setPos(62, 136);
        btn->hide();
        addChild(btn);
        button_.push_back(btn);
      }
      break;

    case eDlgBoxBlock:
      {
        bg_.load("image/dialog/dialog_block.png");

        // Button will be added separately with addBlockButton();
        // We don't show icon here, and move text on the bottom.
        always_display_button_ = true;
        icon_.disable();
        text_.setPos(20, 113);
        text_.setSize(Point(350, 50));
      }
      break;
    }

  create(bg_.getSize().x, bg_.getSize().y, bg_.getSDLSurface());
}

void DialogBox::setText(const std::string& msg)
{
  text_.setText(msg);
}

void DialogBox::setActionHandler(DialogBoxCb* handler)
{
  handler_ = handler;
}

void DialogBox::addBlockButton(int picture_index)
{
  Sprite* btn_block;
  assert(button_.size() < 3);

  btn_block = new Sprite("image/dialog/block_dices.jpg");
  btn_block->splitNbFrame(5, 1);
  btn_block->setFrame(picture_index);
  btn_block->setZ(6);
  btn_block->setPos(37 + 60 * button_.size(), 40);
  addChild(btn_block);
  button_.push_back(btn_block);
}

int DialogBox::addRerollLabel()
{
  Sprite* reroll_lbl_ = new Sprite("image/dialog/reroll.jpg");
  reroll_lbl_->setZ(3);
  reroll_lbl_->splitNbFrame(2, 1);
  reroll_lbl_->setPos(75, 130);
  addChild(reroll_lbl_);
  button_.push_back(reroll_lbl_);
  return button_.size() - 1;
}

DialogBoxCb* DialogBox::getActionHandler()
{
  return handler_;
}

void DialogBox::enable()
{
  if (!isEnabled())
    {
      game_.setState(VGS_SHOWDLGBOX);
      VirtualSurface::enable();
    }
}

void DialogBox::disable()
{
  if (isEnabled())
    {
      VirtualSurface::disable();
      game_.unsetState(VGS_SHOWDLGBOX);
    }
}

void DialogBox::update()
{
  Input& inp = game_.getInput();
  Rect sr(getScreenRect());
  Rect bar(sr.x, sr.y, sr.w, 20);

  // Dragging bar.
  if (bar.inside(inp.mouse_) && inp.button_[1] && !dragging_)
    {
      dragging_ = true;
      drag_delta_ = getPos() - inp.mouse_;
    }
  if (!inp.button_[1])
    dragging_ = false;
  if (dragging_)
    setPos(inp.mouse_ + drag_delta_);

  // Buttons
  ButtonList::iterator it;
  int index;
  for (index = 0, it = button_.begin(); it != button_.end(); ++it, index++)
    {
      Surface *btn = *it;
      if (btn->getScreenRect().inside(inp.mouse_))
        {
          btn->show();
          if (inp.button_pressed_[1])
            {
              if (handler_ != NULL)
                handler_->clicked(index);
              disable();
              return;
            }
        }
      else if (!always_display_button_)
        btn->hide();
    }

  VirtualSurface::update();
}

END_NS(sdlvisu);
