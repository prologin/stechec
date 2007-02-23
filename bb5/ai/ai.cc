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

#include "tools.hh"
#include "ai.hh"


/* initialize a standard tbt client */
AIApp::AIApp(int argc, char** argv)
  : ClientApp(argc, argv, ".tbt/tbtrc", PKGDATADIR)
{
}

AIApp::~AIApp()
{
}

/* override version message */
void AIApp::showVersion()
{
  std::cout << "TowBowlTactics AI client v" PACKAGE_VERSION << "\n"
            << "Copyright (C) 2007 TBT Team.\n";
}

/* block until rules is ready to process another command */
void AIApp::waitUntilReady()
{
  while (true)
    {
      while (ccx_.process())
        ;
      if (!api_->isBusy())
        return;

      // Wait a little, do not eat 100% cpu.
      usleep(25 * 1000);
    }
}

void AIApp::handleDrawKicker()
{
  // Choose to receive.
  api_->doChooseKickoff(false);
}

void AIApp::handleKickOff()
{
  // Try to place the ball somewhere.
  // This is _definitively_ not the right way to do this, maybe
  // it's not to us to place the ball, maybe there will be a touchback
  // (not handled here).
  //
  // But, sometimes, this is working...
  api_->doPlaceBall(Point(7, 5));
}

void AIApp::handleMyTurn()
{
  // This is our turn. select player 10 (this is the 11th, api use base 0).
  api_->selectPlayer(10);
  // Get its position.
  const CPlayer *p = api_->getPlayer();
  Position pos = p->getPosition();
  LOG2("player 10 is at %1", pos);

  // Say we'll be moving player 10.
  pos.row += 1;
  api_->doDeclare(DCL_MOVE);

  // Wait until rules process this command.
  waitUntilReady();
  
  // Really move it.
  api_->doMovePlayer(pos);

  // And end our turn here.
  api_->doEndTurn();
}

/* after 'app.runApp()' from main() finish its dirty job
 * (parse xml, connect to server, ...), this function is called.
 * this is something like _your_ main().
 */
int AIApp::onPlay(bool)
{
  // Get the game api, and register to be able to catch events.
  api_ = rules_->getApi();
  api_->setEventHandler(this);
    
  // Wait until game is fully initialized.
  while (api_->getState() == GS_WAIT || api_->getState() == GS_INITGAME)
    ccx_.process(true);

  // At this point, we are about to place team and do the kickoff.
  // Jump into the main loop, until the end of the game.
  int cur_state = 0;
  while (api_->getState() != GS_END)
    {
      // Process every incoming packets.
      while (ccx_.process())
        ;

      // Tracks game state change.
      if (api_->getState() != cur_state)
        {
          LOG2("new game state: %1", api_->gameStateString());
          cur_state = api_->getState();
        }

      switch (cur_state)
        {
          case GS_DRAWKICKER:
            handleDrawKicker();
            break;
          case GS_INITKICKOFF:
            handleKickOff();
            break;
          case GS_COACH1:
            if (api_->myTeamId() == 0)
              handleMyTurn();
            break;
          case GS_COACH2:
            if (api_->myTeamId() == 1)
              handleMyTurn();
            break;
        }

      // Wait a little (25ms). Otherwise, we would run an active loop and take 100% cpu.
      usleep(25 * 1000);
    }

  return -1; // Don't want to play a second time.
}



/* entry point. don't modify it. */
int main(int argc, char** argv)
{
  AIApp app(argc, argv);

  return app.runApp();
}
