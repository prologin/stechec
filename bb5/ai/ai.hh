/*
** TowBowlTactics, an adaptation of the tabletop game Blood Bowl.
**
** Copyright (C) 2007 The TBT Team.
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

#ifndef AI_HH_
# define AI_HH_

# include "ClientApp.hh"
# include "Event.hh"
# include "Api.hh"

/*
** @brief Test for a TBT Artificial Intelligence client.
*/
class AIApp : public ClientApp, public Event
{
public:
  AIApp(int argc, char** argv);
  virtual ~AIApp();

private:
  virtual void showVersion();
  virtual int onPlay(bool replay);

  void handleKickOff();
  void handleMyTurn();
  
  Api* api_;
};

#endif /* !AI_HH_ */
