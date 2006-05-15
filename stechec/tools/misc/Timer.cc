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

#include <cstdlib>
#include <time.h>
#include "Timer.hh"

Timer::Timer(int allowed_time)
  : started_(false),
    begin_(0),
    begin_pause_(0),
    time_allowed_(allowed_time)
{
}

Timer::Timer()
  : started_(false),
    begin_(0),
    begin_pause_(0),
    time_allowed_(30)
{
}

Timer::~Timer()
{
}

void Timer::setAllowedTime(int time)
{
  time_allowed_ = time;
}

int  Timer::getTimeRemaining() const
{
  time_t tv;
  int time_elapsed;

  if (!started_)
    return 0xFFFF;
  tv = time(NULL);
  time_elapsed = tv - begin_;
  return time_allowed_ - time_elapsed;
}

bool Timer::isTimeElapsed() const
{
  return getTimeRemaining() <= 0;
}


void Timer::start()
{
  if (!started_)
    {
      begin_ = time(NULL);
    }
  if (begin_pause_ != 0)
    {
      begin_ += time(NULL) - begin_pause_;
      begin_pause_ = 0;
    }
  started_ = true;
}

void Timer::stop()
{
  started_ = false;
}

void Timer::restart()
{
  stop();
  start();
}

void Timer::pause()
{
  if (started_)
    {
      begin_pause_ = time(NULL);
    }
}
