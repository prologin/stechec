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

#include <cstring>

#include <sys/time.h>
#include <sys/times.h>
#include <sys/resource.h>
#include <dlfcn.h>
#include <signal.h>
#include "misc/Conf.hh"

#include "tools.hh"
#include "NutsBarrier.hh"

/*
** Signal handler
*/
SignalHandler* SignalHandler::inst_ = NULL;

SignalHandler::SignalHandler()
{
  // Can't have two instance of SignalHandler - it is _not_
  // designed to be thread-safe.
  assert(inst_ == NULL);
  inst_ = this;
}

SignalHandler::~SignalHandler()
{
  inst_ = NULL;
}

void SignalHandler::catchSignal(int signal)
{
  struct timeval t;

  assert(inst_);
  if (signal == SIGVTALRM)
    {
      gettimeofday(&t, NULL);
      ERR("Time limit exceeded. Go fix your (slow) program.");
      LOG3("Timestamp: %1:%2", t.tv_sec, t.tv_usec);
      inst_->sigAlrm();
      exit(1); // yeah. easier.
    }
  else if (signal == SIGSEGV)
    {
      ERR("Segfault catched. Go fix your program.");
      inst_->sigSegv();
      exit(1);
    }
  else if (signal == SIGBUS)
    {
      ERR("BusError catched. Go fix your program.");
      inst_->sigBus();
      exit(1);
    }
  else
    WARN("??? unknown catched signal: %1", signal);
}

/*
** NutsBarrier.
*/

NutsBarrier::NutsBarrier() :
  time_limit_(0),
  time_reserve_(0),
  ref_speed_(0),
  hdl_(NULL)
{
}

NutsBarrier::~NutsBarrier()
{
}

void    NutsBarrier::setConfigLimits(const ConfSection& cfg)
{

  if (cfg.getValue<bool>("valgrind") || cfg.getValue<bool>("spectator"))
    return;

  int time = cfg.getValue<int>("time");
  int reserve = cfg.getValue<int>("time_reserve");
  if (reserve < 0)
    reserve = 0;
  if (time > 10)
    setTimeLimitValue(time, reserve);

  int mem = cfg.getValue<int>("memory");
  if (mem > 100)
    setMemoryLimitValue(mem);
}

void    NutsBarrier::setMemoryLimitValue(int mem)
{
  struct rlimit rl;

#ifdef HOSTTYPE_LINUX
  // The maximum size of the process's virtual memory (address space) in bytes
  // say that stack + non-heap data takes approximatly 3 Mo.
  getrlimit(RLIMIT_AS,&rl);
  rl.rlim_cur = (mem * 1024) + (15 * 1024 * 1024); // FIXME: currently 15 mo.
  setrlimit(RLIMIT_AS,&rl);
  getrlimit(RLIMIT_AS,&rl);
  LOG3("Set memory restriction to %1 kb.", rl.rlim_cur / 1024);
#else
  // The maximum size of the process's data segment.
  getrlimit(RLIMIT_DATA, &rl);
  rl.rlim_cur = mem * 1024 + (15 * 1024 * 1024);
  setrlimit(RLIMIT_DATA, &rl);
  getrlimit(RLIMIT_DATA,&rl);
  LOG3("Set memory restriction to %1 kb.", rl.rlim_cur / 1024);
#endif

  // The maximum size of the process stack, in bytes.
  getrlimit(RLIMIT_STACK,&rl);
  rl.rlim_cur = 20 * 1024;
  setrlimit(RLIMIT_STACK, &rl);

  // maximum file descriptor number that can be opened
  getrlimit(RLIMIT_NOFILE,&rl);
  rl.rlim_cur = 10; // FIXME: can open max 10 files.
  //setrlimit(RLIMIT_NOFILE,&rl);

  // The maximum number of processes that can be created
#ifndef HOSTTYPE_CYGWIN
  getrlimit(RLIMIT_NPROC,&rl);
  rl.rlim_cur = 0; // can't fork
  setrlimit(RLIMIT_NPROC,&rl);
#endif
}

int        NutsBarrier::getTimeRemaining() const
{
  if (time_limit_ > 0)
    {
      struct itimerval time;

      getitimer(ITIMER_VIRTUAL, &time);
      int real_ms = timerSub(&time_start_, &time);
      return (int)((time_limit_ - real_ms) * (1. / (double)autoTimeSelect() * 1000));
    }
  return 99999;
}

int        NutsBarrier::getExtraTimeRemaining() const
{
  if (time_limit_ > 0)
    return (int)(time_reserve_ * (1. / (double)autoTimeSelect() * 1000));
  return 99999;
}

void        NutsBarrier::setTimeLimitValue(int time, int reserve)
{
  time_limit_ = (time * autoTimeSelect()) / 1000;
  time_reserve_ = (reserve * autoTimeSelect()) / 1000;
  LOG2("Set time limit to %1 ms (real: %2 ms), extra time to %3 ms (real: %4 ms).", time, time_limit_, reserve, time_reserve_);
}


void        NutsBarrier::setSignalCatch()
{
  struct sigaction act;
  int netbsdsuck;

  netbsdsuck = sigemptyset(&act.sa_mask);
  act.sa_handler = &SignalHandler::catchSignal;
  act.sa_flags = 0;
  sigaction(SIGBUS, &act, &prev_sigbus_);
  sigaction(SIGSEGV, &act, &prev_sigsegv_);
}

void        NutsBarrier::unsetSignalCatch()
{
  sigaction(SIGBUS, &prev_sigbus_, NULL);
  sigaction(SIGSEGV, &prev_sigsegv_, NULL);
}

void        NutsBarrier::setTimeLimit()
{
  if (time_limit_ > 0)
    {
      struct sigaction          act;
      int                       foo;
      foo = sigemptyset(&act.sa_mask);
      act.sa_handler = &SignalHandler::catchSignal;
      act.sa_flags = 0;
      sigaction(SIGVTALRM, &act, NULL);

      memset(&time_start_, 0, sizeof (struct itimerval));
      time_start_.it_value.tv_sec = ((time_limit_ + time_reserve_)) / 1000;
      time_start_.it_value.tv_usec = (((time_limit_ + time_reserve_)) * 1000) % 1000000;
      setitimer(ITIMER_VIRTUAL, &time_start_, NULL);
    }
}

void        NutsBarrier::unsetTimeLimit()
{
  struct itimerval        alr;
  struct sigaction        act;

  if (time_limit_ > 0)
    {
      struct itimerval time;

      getitimer(ITIMER_VIRTUAL, &time);
      int real_ms_delta = timerSub(&time_start_, &time);
      
      if (real_ms_delta > time_limit_)
        {
          time_reserve_ -= real_ms_delta - time_limit_;
          LOG1("Used %1 ms of extra time (%2 ms remain).", real_ms_delta - time_limit_, time_reserve_);
          if (time_reserve_ < 0)
            hdl_->sigAlrm(); // Should never happen now.
        }

      int bar;
      bar = sigemptyset(&act.sa_mask);
      act.sa_handler = SIG_IGN;
      act.sa_flags = 0;
      sigaction(SIGVTALRM, &act, NULL);
      memset(&alr, 0, sizeof (struct itimerval));
      setitimer(ITIMER_VIRTUAL, &alr, NULL);
    }
}


void                NutsBarrier::reachHardLimit(int)
{
  // about 200 MHz
  ERR("This computer is too slow, I can't run any program at this speed !\n");
  exit(1);
}

/*
** hope that no compiler will be too intelligent to optimize that :)
*/
void                NutsBarrier::dumbLoop() const
{
  volatile int	    i, j, k;

  k = 42;
  for (i = 0; i < 10000; ++i)
    for (j = 0; j <= 6000; j++)
      k += i * j;
}

// return in ms.
int                NutsBarrier::timerSub(const struct itimerval* start,
                                         const struct itimerval* end) const
{
  int                ret;

  ret = (start->it_value.tv_sec - end->it_value.tv_sec) * 1000;
  ret += (start->it_value.tv_usec - end->it_value.tv_usec) / 1000;
  return ret;
}

int                NutsBarrier::autoTimeSelect() const
{
  struct itimerval        itstart;
  struct itimerval        itdone;
  struct itimerval        zero_timer;

  if (ref_speed_ == 0)
    {
      memset(&itstart, 0, sizeof (struct itimerval));
      memset(&zero_timer, 0, sizeof (struct itimerval));
      itstart.it_value.tv_sec = 2;
      signal(SIGVTALRM, reachHardLimit);
      setitimer(ITIMER_VIRTUAL, &itstart, NULL);
      dumbLoop();
      setitimer(ITIMER_VIRTUAL, &zero_timer, &itdone);
      ref_speed_ = (timerSub(&itstart, &itdone) * LOOP_TO_TURN_RATIO);
      signal(SIGVTALRM, SIG_DFL);
      LOG3("Time adjustment: %1 / 1000", ref_speed_);
    }
  return ref_speed_;
}

void        NutsBarrier::setSignalHandler(SignalHandler* hdl)
{
  hdl_ = hdl;
}
