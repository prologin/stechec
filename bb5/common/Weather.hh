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

#ifndef WEATHER_HH_
# define WEATHER_HH_

# include "Constants.hh"
# include "PacketHandler.hh"

//! @brief Server -> Client. Send the current weather, once in the game.
DECLARE_PACKET(MSG_WEATHER, MsgWeather)
  int weather;
END_PACKET

/*!
** @brief Base class for storing current match weather.
*/
class Weather
{
public:
  Weather();
  virtual ~Weather() {}

  virtual void serialize(std::ostream& os) const;
  virtual void unserialize(std::istream& is);

  //! @brief Get the current weather (between 2 and 12).
  //! @return The current weather.
  int getWeather() const;
  //! @brief Get the current weather as a string, eg: <tt>Pouring rain</tt>.
  //! @return A static string describing the current weather.
  const char* getWeatherStr() const;

  //! @brief Set the current weather
  void setWeather(const MsgWeather* m);
  
protected:

  int weather_; ///< The current weather [| 2, 12 |].
};

inline int Weather::getWeather() const
{
  return weather_;
}

#endif /* !WEATHER_HH_ */
