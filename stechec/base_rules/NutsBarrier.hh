#ifndef NUTSBARRIER_HH_
# define NUTSBARRIER_HH_

# include <sys/times.h>
# include <signal.h>
# include <cassert>
# include "tools.hh"

/*!
** @file
** @author victor
** @date 05/03/05
*/

# define LOOP_TO_TURN_RATIO 5

// receive signals
class SignalHandler
{
protected:
  SignalHandler();
  virtual ~SignalHandler();

  virtual void sigSegv() = 0;
  virtual void sigBus() = 0;
  virtual void sigAlrm() = 0;

private:
  friend class NutsBarrier;
  static void catchSignal(int signal);
  static SignalHandler* inst_;
};

class ConfSection;

/*!
** @brief Extra memory/time limitation, mainly for AIs.
**
** Prologin contest puts a strict limitation on memory and time
** champions are allowed to use.
**
** Also, put signal handlers to catch some nasty signals (SEGV, ...),
** to point out this is _their_ code that is badly programmed...
**
** Currently, it doesn't work with multi-threaded process.
*/
class NutsBarrier
{
public:
  NutsBarrier();
  ~NutsBarrier();

  
  int           getTimeRemaining() const;
  int           getExtraTimeRemaining() const;
  
  void          setConfigLimits(const ConfSection& cfg);
  void          setTimeLimitValue(int time, int reserve); // in millisecond
  void          setMemoryLimitValue(int memory); // in kbytes.
  void          setSignalHandler(SignalHandler* hdl);
  
  void          setTimeLimit();
  void          unsetTimeLimit();
  void          setSignalCatch();
  void          unsetSignalCatch();

private:
  int           time_limit_;
  int           time_reserve_;

  struct sigaction prev_sigbus_;
  struct sigaction prev_sigsegv_;

  int           autoTimeSelect() const;
  int           timerSub(const struct itimerval *start, const struct itimerval *end) const;
  void          dumbLoop() const;

  static void   reachHardLimit(int);
  struct itimerval time_start_;
  
  mutable int   ref_speed_;
  SignalHandler* hdl_;
};


#endif /* !NUTSBARRIER_HH_ */

