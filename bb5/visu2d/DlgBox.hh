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

#ifndef DLGBOX_HH_
# define DLGBOX_HH_

# include "VirtualSurface.hh"
# include "Sprite.hh"
# include "TextSurface.hh"

BEGIN_NS(sdlvisu);

enum eDlgBoxIcon {
  eDlgError = 2,
  eDlgWarning = 3,
  eDlgQuestion = 6,
  eDlgInfo = 7,
};

enum eDlgBoxButton {
  eDlgBoxYesNo,
  eDlgBoxOk,
  eDlgBoxBlock,
};

class Game;

/*
** @brief Base class for DialogBox button handler.
*/
class DialogBoxCb
{
public:
  DialogBoxCb() {}
  virtual ~DialogBoxCb() {}

  virtual void clicked(int btn_index) = 0;
};

/*!
** @brief Victor reimplementation of dialog box.
**
** Sorry poltuiu I can't reuse your... my system is too different :)
**
** DialogBox handles itself on the renderer list, so you don't need to use addChild on it.
** It adds itself to the screen list on creation, and remove itself on deletion.
*/
class DialogBox : public VirtualSurface
{
public:
  DialogBox(Game& game);
  DialogBox(Game& game, enum eDlgBoxButton style, enum eDlgBoxIcon icon);
  virtual ~DialogBox();

  void setStyle(enum eDlgBoxButton style, enum eDlgBoxIcon icon);
  void setText(const std::string& msg);
  void setActionHandler(DialogBoxCb* handler);

  //! @brief Add a block button on the dialog box.
  //! @param dice_face The face of the dice to display in the range [1-6].
  //! @note Only work with eDlgBoxBlock.
  void addBlockButton(int dice_face);
  
  virtual void enable();
  virtual void disable();
  virtual void update();

private:
  Game& game_;
  DialogBoxCb* handler_;

  Surface bg_;
  TextSurface text_;
  Sprite icon_;
  typedef std::vector<Surface*> ButtonList;
  ButtonList button_;

  bool dragging_;
  Point drag_delta_;
  bool always_display_button_;
};

END_NS(sdlvisu);

#endif /* !DLGBOX_HH_ */
