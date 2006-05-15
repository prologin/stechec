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

#include "Weather.hh"

Weather::Weather()
{
}

void Weather::serialize(std::ostream& os) const
{
  os << weather_ << std::endl;
}

void Weather::unserialize(std::istream& is)
{
  is >> weather_;
}


const char* Weather::getWeatherStr() const
{
  if (weather_ == 2)
    return "Sweltering Heat";
  if (weather_ == 3)
    return "Very Sunny";
  if (weather_ == 11)
    return "Pouring Rain";
  if (weather_ == 12)
    return "Blizzard";

  return "Nice";
}

void Weather::setWeather(const MsgWeather* m)
{
  weather_ = m->weather;
}
