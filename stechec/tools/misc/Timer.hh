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

#ifndef TIMER_HH_
# define TIMER_HH_

/*! @brief Implement a passive timer, with time accuracy in seconds.
**
** You have to start, stop, restart, and check the remaining time yourself.
** The timer can be paused, then re-started.
**
** Currently, the timer has a second precision. Something more accurate (ie
** milliseconds) should be implemented, but for the time being this ssufficient.
**
** @author victor
** @date 28/01/2006
*/
class Timer
{
public:
  Timer(int allowed_time);
  Timer();
  ~Timer();

  //! @brief Set the maximum allowed time, in second.
  //! @note This function does nothing if the timer is started.
  void        setAllowedTime(int allowed_time);
  //! @brief Get the remaining time, in seconds.
  //! @note If called when the timer is stopped, it will return a high value (> 0).
  int        getTimeRemaining() const;
  //! @brief Check is the time is elapsed.
  //! @note If called when the timer is stopped, it will return false.
  bool        isTimeElapsed() const;

  //! @brief Start the timer.
  void        start();
  //! @brief Stop the timer.
  void        stop();
  //! @brief Restart the timer. This is equivalent to calling stop then start.
  void        restart();
  //! @brief Do a pause.
  //! @note To restart at the current position, call \c start, not \c restart.
  void        pause();

private:
  bool        started_;        ///< True if the timer is active.
  int        begin_;         ///< Start time, since epoch.
  int        begin_pause_;   ///< Pause start time, since epoch.
  int        time_allowed_;  ///< Time allowed, in seconds.
};

#endif /* !TIMER_HH_ */
