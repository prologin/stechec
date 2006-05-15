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

#include <sys/types.h>
#include <sys/poll.h>

#include <iomanip>
#include "interface.hh"

using namespace std;

CmdLineInterface::CmdLineInterface(xml::XMLConfig* cfg,
                                   Api* api,
                                   ClientCx* client_cx)
  : cfg_(cfg),
    api_(api),
    ccx_(client_cx),
    standalone_(false),
    input_(this, api_),
    paused_(false)
{
}

void CmdLineInterface::hello()
{
  cout << "Welcome to " PACKAGE_NAME " v" PACKAGE_VERSION << endl
       << "I'm a stupid command-line interface, that will allow you to play"
       << endl << "in ascii art using your keyboard." << endl << endl;
}

void CmdLineInterface::init()
{
  // Generic client may have already connected us.
  if (!ccx_->isConnected())
    {
      if (!standalone_)
        cout << "I'm connecting to the server... wait before typing something.";
      else
        cout << "Running in standalone mode. Trying to sync with the server.";
      cout << endl;
  
      if (!ccx_->connect(*cfg_))
        {
          ERR("Can't connect to server, aborting.");
          exit(1);
        }
      cout << "Connected. Now exchanging init informations." << endl;
    }

  while (api_->getState() == GS_WAIT || api_->getState() == GS_INITGAME)
    ccx_->process(true);

  cout << "Init done. Time To Play !\n"
       << "You can get all available commands by typing 'help'."
       << endl;
}

void CmdLineInterface::run()
{    
  while (api_->getState() != GS_END)
    {
      // Process every incoming packets
      while (ccx_->process())
        ;
      // Parse and handle a command, if any.
      if (input_.getCommand())
        return;
    }
}

void CmdLineInterface::printGlobal()
{
  cout << "  - current game state: " << api_->getState() << "\n"
       << "  - the weather is '" << api_->getWeather()->getWeatherStr()
       << "' (" << api_->getWeather()->getWeather() << ")\n"
       << "  - ball position: " << api_->getBallPosition() << "\n"
       << "  - our team  : '" << api_->getTeamName() << "', coached by '"
       << api_->getCoachName() << "' (" << api_->getTeam()->getNbPlayer()
       << " players)\n";
  api_->switchTeam(THEM);
  cout << "  - other team: '" << api_->getTeamName() << "', coached by '"
       << api_->getCoachName() << "' (" << api_->getTeam()->getNbPlayer()
       << " players)" << endl;
  api_->switchTeam(US);

}

// Print the field, in ascii art.
// Disclaimer: This code is not intended to be beautiful, nor readable.
void CmdLineInterface::printField()
{
  int row, col;

  // top (+ col count)
  cout << "      " << setiosflags(ios::left);
  for (col = 0; col < 15; col++)
    cout << setw(5) << col;
  cout << "\n";
  cout << "    " << setw(76) << setfill('-') << "-" << setfill(' ') << "\n";

  // field
  for (row = 0; row < 26; row++)
    {
      if (row == 1 || row == 13 || row == 25)
        cout << "    - - - - - - - - - - - - - - - - - - - - - - - - - "
             << "- - - - - - - - - - - - -\n";

      if (row % 5 == 0 || row == 1 || row == 24)
        cout << setw(3) << row << "|";
      else
        cout << "   |";
      for (col = 0; col < 15; col++)
        {
          if (col == 3 || col == 12)
            cout << "|";
          else
            cout << " ";

          // print square content.
          Position pos(row, col);
          Position ball_pos(api_->getBallPosition());
          const CPlayer* p = api_->getPlayer(pos);
          if (p != NULL)
            {
              char pdb = p->getTeamId() == api_->getTeamId() ? '<' : '{';
              char pde = p->getTeamId() == api_->getTeamId() ? '>' : '}';
              if (ball_pos == pos)
                pde = '@';
              cout << pdb << p->getId() << pde;
            }
          else
            if (ball_pos == pos)
              cout << " @ ";
            else
              cout << "   ";

          // Keep at least one space as separator.
          cout << " ";
        }
      cout << "|\n";
    }

  // bottom (+ col count)
  cout << "    " << setw(76) << setfill('-') << "-" << setfill(' ') << "\n";
  cout << "      ";
  for (col = 0; col < 15; col++)
    cout << setw(5) << col;
  cout << endl;
}

void CmdLineInterface::printPlayerList()
{
  int team_size;
  const CPlayer* p;

  team_size = api_->getTeam()->getNbPlayer();
  cout << setiosflags(ios::left);
  for (int i = 1; i <= team_size; i++)
    {
      p = api_->getPlayer(i);
      cout << "* " << i << ": " << setw(16) << p->getName() << " " << p->getPosition() << "\n";
    }
  api_->switchTeam(THEM);
  team_size = api_->getTeam()->getNbPlayer();
  for (int i = 1; i <= team_size; i++)
    {
      p = api_->getPlayer(i);
      cout << "+ " << i << ": " << setw(16) << p->getName() << " " << p->getPosition() << "\n";
    }
  api_->switchTeam(US);
}

void CmdLineInterface::printPlayer(int player_id, int team_id)
{
  api_->switchTeam(team_id);
  if (player_id <= 0 || player_id > api_->getTeam()->getNbPlayer())
    {
      cout << "player id " << player_id << ": out of bound." << endl;
      return;
    }
  cout << *api_->getPlayer(player_id);
  api_->switchTeam(team_id);
}

//
// Events (virtual methods called from Event).
//

void CmdLineInterface::evIllegal()
{
  cout << "An illegal action was tried. Bouh." << endl;
}

void CmdLineInterface::evNewTurn()
{
  cout << "It's our turn..." << endl;
  our_turn_ = true;
}

void CmdLineInterface::evKickOff()
{
  cout << "Arbiter is asking us to place the ball. (use 'kickoff')" << endl;
}

void CmdLineInterface::evMoveTurnMarker()
{
  cout << "The turn marker has been moved." << endl;
}

void CmdLineInterface::evTimeExceeded()
{
  cout << "You were too slow, slug !" << endl;
}

void CmdLineInterface::evChat(const std::string& msg)
{
  cout << "<CHAT> " << msg << endl;
}

void CmdLineInterface::evPlayerMove(int team_id, int player_id, const Point& pos)
{
  cout << "A player has moved." << endl;
}

void CmdLineInterface::evPlayerKnocked(int team_id, int player_id)
{
  cout << "A player has been knocked down." << endl;
}
