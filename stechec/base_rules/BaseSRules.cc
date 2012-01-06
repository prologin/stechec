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
** `COPYING' file in the root directory.
**
** The TBT Team consists of people listed in the `AUTHORS' file.
*/

# include "BaseSRules.hh"

BaseSRules::BaseSRules(int team_number)
  : BaseRules(team_number),
    viewer_state_(0)
{
}

BaseSRules::~BaseSRules()
{
}

int     BaseSRules::getViewerState() const
{
  return viewer_state_;
}

void    BaseSRules::setViewerState(int value)
{
  viewer_state_ = value;
}

bool    BaseSRules::coachKilled(int, CoachErrorCustom*&)
{
  return false;
}

void    BaseSRules::outputStat(int, ClientStatistic&)
{
}
