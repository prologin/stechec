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

# include "CBall.hh"
# include "CPlayer.hh"
# include "CRules.hh"

CBall::CBall(CRules* rules)
  : r_(rules)
{
}

CBall::~CBall()
{
}

void CBall::setPosition(const MsgBallPos* m)
{
  pos_.row = m->row;
  pos_.col = m->col;
  r_->onEvent(m);
}
