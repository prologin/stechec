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
  api_->setEventHandler(this);

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
      if (input_.process())
        return;
    }
}

void CmdLineInterface::printGlobal()
{

  cout << "  - you are coach '" << api_->getTeamId() << "'\n" 
       << "  - current game state: " << api_->getStateString() << "\n"
    //       << "  - the weather is '" << api_->getWeather()->getWeatherStr()
    //       << "' (" << api_->getWeather()->getWeather() << ")\n"
       << "  - ball position: " << Point(api_->ballX(), api_->ballY()) << "\n"
       << "  - our team  : '" << api_->getTeamName() << "', coached by '"
       << api_->getCoachName() << "' (" << api_->getTeam()->getNbPlayer()
       << " players)\n";
  api_->selectTeam(THEM);
  cout << "  - other team: '" << api_->getTeamName() << "', coached by '"
       << api_->getCoachName() << "' (" << api_->getTeam()->getNbPlayer()
       << " players)" << endl;
  api_->selectTeam(US);

}

// Print the field, in ascii art.
// Disclaimer: This code is not intended to be beautiful, nor readable.
void CmdLineInterface::printField()
{
  int row, col;

  cout << "Your are coach " << api_->getTeamId() << ", you are on the ";
  cout << (api_->getTeamId() == 0 ? "top" : "bottom") << " of the field.\n";

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
          if (col == 4 || col == 11)
            cout << "|";
          else
            cout << " ";

          // print square content.
          Position pos(row, col);
          Position ball_pos(api_->ballY(), api_->ballX());
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
	  if (p == NULL || p->getId() < 10)
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
  for (int i = 0; i < team_size; i++)
    {
      p = api_->getPlayer(i);
      cout << "* " << i << ": " << setw(16) << p->getName() << " " << p->getPosition() << "\n";
    }
  api_->selectTeam(THEM);
  team_size = api_->getTeam()->getNbPlayer();
  for (int i = 0; i < team_size; i++)
    {
      p = api_->getPlayer(i);
      cout << "+ " << i << ": " << setw(16) << p->getName() << " " << p->getPosition() << "\n";
    }
  api_->selectTeam(US);
}

void CmdLineInterface::printPlayer(int player_id, int team_id)
{
  api_->selectTeam(team_id);
  if (player_id < 0 || player_id > api_->getTeam()->getNbPlayer())
    {
      cout << "player id " << player_id << ": out of bound." << endl;
      api_->selectTeam(US);
      return;
    }
  cout << *api_->getPlayer(player_id);
  api_->selectTeam(US);
}

//
// Events (virtual methods called from Event).
//

void CmdLineInterface::evIllegal(int was_token)
{
  cout << "An illegal action was tried (token: " << was_token << "). Bouh." << endl;
}

void CmdLineInterface::evNewTurn(bool our_turn)
{
  if (our_turn)
    cout << "It's our turn..." << endl;
  else
    cout << "It's their turn..." << endl;    
  our_turn_ = our_turn;
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

void CmdLineInterface::evPlayerPos(int team_id, int player_id, const Point& pos)
{
  cout << "Player `" << player_id << "' of team `"
       << team_id << "' has moved to `" << pos << "'" << endl;
}

void CmdLineInterface::evPlayerMove(int, int player_id, const Point& pos)
{
  cout << "Player `" << player_id << "' has moved to `" << pos << "'" << endl;
}

void CmdLineInterface::evPlayerKnocked(int, int player_id)
{
  cout << "Player `" << player_id << "' has been knocked down." << endl;
}

void CmdLineInterface::evGiveBall()
{
  cout << "Touchback! receiving team can give the ball to any player on the field." << endl;
}

void CmdLineInterface::evFollow()
{
  cout << "You can 'follow' the opponent player or 'stay' here." << endl;
}

void CmdLineInterface::evBlockPush(Position pos, int nb_choice, Position choices[])
{
  cout << "You can push the player from the square" << pos << " to : " << endl;
  for (int i = 0; i < nb_choice; i++)
    cout << "  'push " << i << " : " << choices[i] << endl;
}

void CmdLineInterface::evResult(int team_id, int player_id, enum eRoll action_type, 
				int result, int modifier, int required, bool reroll)
{
  cout << "Player `" << player_id << "' tried an action : `"
       << api_->getRollString(action_type)
       << "' : roll [" << result << "] + ["<< modifier << "], required : ["
       << required << "]." << endl;
	
  if (result + modifier < required && reroll && api_->getTeamId() == team_id)
    cout << "		You can use a 'reroll' or 'accept' this result." << endl;
}

void CmdLineInterface::evBlockResult(int team_id, int player_id, int opponent_id, 
				     int nb_dice, enum eBlockDiceFace result[3],
				     int choose, bool reroll)
{
  if (team_id != api_->getTeamId())
    cout << "Opponent ";
	
  cout << "Player `" << player_id << "' tried to block " << "player `" 
       << opponent_id << "'" << endl;
	
  cout << "Result : ";
  for (int i = 0; i < nb_dice; i++)
    {
      cout << i << "-";
      switch (result[i])
	{
	case BATTAKER_DOWN :
	  cout << " Attacker Down";
	  break;
	case BBOTH_DOWN :
	  cout << " Both Down";
	  break;
	case BPUSHED :
	  cout << " Defender Pushed";
	  break;
	case BDEFENDER_STUMBLE :
	  cout << " Defender Strumble";
	  break;
	case BDEFENDER_DOWN : 
	  cout << " Defender Down";
	  break;
	}
      if (i != nb_dice - 1)
	cout << ", ";
    }
  cout << endl;

  if (team_id == api_->getTeamId() && choose == api_->getTeamId() && reroll)
    cout << " You can use a 'reroll' or choose a 'dice <n>'" << endl;
  if (team_id == api_->getTeamId() && choose == api_->getTeamId() && !reroll)
    cout << " You must choose a 'dice <n>'" << endl;
  if (team_id == api_->getTeamId() && choose != api_->getTeamId() && reroll)
    cout << "		You can use a 'reroll' or 'accept' this result." << endl;
  if (team_id != api_->getTeamId() && choose != api_->getTeamId() && reroll)
    cout << " Wait for opponent reroll decision" << endl;
  if (team_id != api_->getTeamId() && !reroll
      && choose != api_->getTeamId() && choose != -1)
    cout << " You must choose a 'dice <n>'" << endl;	
}
