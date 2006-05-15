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

#ifndef BASESRULES_HH_
# define BASESRULES_HH_

# include "BaseRules.hh"

class ClientStatistic;
class CoachErrorCustom;

class BaseSRules : public BaseRules
{
public:
  BaseSRules();
  virtual ~BaseSRules();

  //! @brief Get the viewer state.
  int           getViewerState() const;
  //! @brief Set the viewer state.
  void          setViewerState(int value);

  //! @brief Called when the game is about to start, and all clients have
  //! joined (server-side only).
  virtual void  serverStartup() {}
  //! @brief Periodically called when the game is launched.
  virtual void  serverProcess() {}

  //! @brief Called when a coach is disconnected.
  //! @param cec Custom data that SRules could save, it will be retourned
  //!   back on the call of outputStat.
  //! @return true if the game should go on, false if must stop now.
  virtual bool  coachKilled(int coach_id, CoachErrorCustom*& cec);

  //! @brief Called after the game is finished, to output some game statistics.
  //!  Called for each coach.
  virtual void  outputStat(int coach_id, ClientStatistic& client_stat);
  
private:
  int           viewer_state_;
};

#endif /* !BASESRULES_HH_ */
