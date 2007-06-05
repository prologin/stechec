/*
** TowBowlTactics, an adaptation of the tabletop game Blood Bowl.
** 
** Copyright (C) 2006, 2007 The TBT Team.
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
                                   ClientCx* client_cx,
                                   bool use_readline)
  : cfg_(cfg),
    api_(api),
    ccx_(client_cx),
    standalone_(false),
    input_(this, api_, use_readline),
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
  // FIXME: actually, it _must_ have connected us. remove this code.
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
  const CTeam *t;
  api_->selectTeam(US);
  t = api_->getTeam();
  cout << "  - you are coach '" << api_->getTeamId() << "'\n" 
       << "  - current game state: " << api_->gameStateString() << "\n"
    //       << "  - the weather is '" << api_->getWeather()->getWeatherStr()
    //       << "' (" << api_->getWeather()->getWeather() << ")\n"
       << "  - ball position: " << Position(api_->ball()) << "\n"
       << "  - our team  : '" << t->getTeamName() << "', coached by '"
       << t->getCoachName() << "' (" << t->getNbPlayer()
       << " players), scored " << t->getScore() << " point(s).\n";

  api_->selectTeam(THEM);
  t = api_->getTeam();
  cout << "  - other team: '" << t->getTeamName() << "', coached by '"
       << t->getCoachName() << "' (" << t->getNbPlayer()
       << " players), scored " << t->getScore() << " point(s)." << endl;
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
          Point pos(col, row);
          int team_id = api_->teamId(pos);
          int player_id = api_->playerId(pos);
          api_->selectTeam(team_id);
          api_->selectPlayer(player_id);
          if (player_id > -1)
            {
              char pdb, pde;
              if (api_->getPlayer()->getStatus() != STA_STANDING)
                pdb = '[';
              else
              pdb = team_id == api_->myTeamId() ? '<' : '{';
              pde = team_id == api_->myTeamId() ? '>' : '}';
              if (api_->ball() == pos)
                pde = '@';
              cout << pdb << player_id << pde;
            }
          else
            if (api_->ball() == pos)
              cout << " @ ";
            else
              cout << " . ";

          // Keep at least one space as separator.
          if (player_id < 10)
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
  api_->selectTeam(US);
}

void CmdLineInterface::printPlayerList()
{
  int justify_name;
  int team_size;
  const CPlayer* p;

  cout << setiosflags(ios::left);

  for (int k = 0; k < 2; k++)
    {
      api_->selectTeam(k == 0 ? US : THEM);
      team_size = api_->getTeam()->getNbPlayer();
      for (int i = 0; i < team_size; i++)
        {
          p = api_->getPlayer(i);
          justify_name = i >= 10 ? 15 : 16;
          cout << (k == 0 ? "* " : "+ ") << i << ": " << setw(justify_name)
               << p->getName() << " ";
          if (p->getStatus() != STA_STANDING &&
              p->getStatus() != STA_PRONE &&
              p->getStatus() != STA_STUNNED)
            cout << "out of the field";
          else
            cout << p->getPosition();
          cout << "  " << p->stringify(p->getStatus())
               << endl;
        }
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

void CmdLineInterface::evIllegal(int team_id, int was_token)
{
  cout << "An illegal action (token: " << bb5_token_str[was_token]
    << ") was tried by the coach `" << team_id << "'. Bouh." << endl;
}

void CmdLineInterface::evInitGame()
{
  // Cheat, player 0 will always choose first.
  if (api_->myTeamId() == 0)
    api_->doCheatDice(1);
}

void CmdLineInterface::evDrawKicker(int team_id, bool is_a_question)
{
  if (is_a_question)
    if (team_id == api_->myTeamId())
      {
        cout << "Arbiter is asking us to choose to either kick or receive the ball."
          << " (use 'choose kickoff|receive')" << endl;
        input_.stopWaiting();
      }
    else
      cout << "Wait that the other team chooses to either kick or receive the ball." << endl;
  else
    cout << ((team_id == api_->myTeamId()) ? "Our" : "Other" )
      << " team will kick off the ball." << endl; 
}

void CmdLineInterface::evNewTurn(int player_id, int cur_half, int cur_turn)
{
  if (player_id == api_->myTeamId())
    {
      cout << "It's our turn (turn " << cur_turn << ", half "
           << cur_half << ")..." << endl;
      input_.stopWaiting();
    }
  else
    cout << "It's their turn (turn " << cur_turn << ")..." << endl;    
  our_turn_ = player_id == api_->myTeamId();
}

void CmdLineInterface::evPlaceTeam(int team_id)
{
    if (team_id == api_->myTeamId())
    {
      cout << "Arbiter is asking us to set up on the field. (use 'place')" << endl;
      input_.stopWaiting();
    }
    else
      cout << "Wait that the other team sets up on the field." << endl;
}

void CmdLineInterface::evKickOff(int team_id)
{
    if (team_id == api_->myTeamId())
    {
      cout << "Arbiter is asking us to place the ball. (use 'kickoff')" << endl;
      input_.stopWaiting();
    }
    else
      cout << "Wait that the other team places the ball." << endl;
}

void CmdLineInterface::evMoveTurnMarker()
{
  cout << "The turn marker has been moved." << endl;
}

void CmdLineInterface::evTouchdooown(int team_id, int player_id)
{
  if (player_id == -1)
    cout << "Team `" << team_id << "' scores a penalty shoot-out." << endl;
  else
    cout << "Player `" << player_id << "' of team `"
      << team_id << "' scores a touchdooown!" << endl;
}

void CmdLineInterface::evChat(const std::string& msg)
{
  cout << "<CHAT> " << msg << endl;
}

void CmdLineInterface::evPlayerStatus(int team_id, int player_id, enum eStatus status)
{
  cout << "Player `" << player_id << "' of team `"
       << team_id << "' is now marked as `" << Player::stringify(status) << "'." << endl;
}

void CmdLineInterface::evPlayerPos(int team_id, int player_id, const Point& pos)
{
  cout << "Player `" << player_id << "' of team `"
       << team_id << "' has moved to `" << pos << "'." << endl;
}

void CmdLineInterface::evPlayerMove(int team_id, int player_id, const Point& pos)
{
  cout << "Player `" << player_id << "' of team `"
    << team_id << "' has moved to `" << pos << "'." << endl;
}

void CmdLineInterface::evPlayerKnocked(int team_id, int player_id)
{
  cout << "Player `" << player_id << "' of team `"
    << team_id << "' has been knocked down." << endl;
}

void CmdLineInterface::evGiveBall(int team_id, int player_id)
{
  if (team_id == -1)
    cout << "The ball is left alone." << endl;
  else if (player_id == -1)
    {
      if (team_id == api_->myTeamId())
        {
          cout << "Touchback! Arbiter is asking us to place the ball. (use 'giveBall')" << endl;
          input_.stopWaiting();
        }
      else
        cout << "Touchback! Wait that the other team gives the ball and plays his turn." << endl;
    }
  else
    cout << "Player `" << player_id << "' of team `" << team_id << "' gets the ball." << endl;
}

void CmdLineInterface::evFollow()
{
  cout << "You can 'follow' the opponent player or 'stay' here." << endl;
}

void CmdLineInterface::evBlockPush(const Position& pos, int nb_choice, const Position choices[])
{
  cout << "You can push the player from the square" << pos << " to : " << endl;
  for (int i = 0; i < nb_choice; i++)
    cout << "  'push " << i << " : " << choices[i] << endl;
}

void CmdLineInterface::evResult(int team_id, int player_id, enum eRoll action_type, int result,
                                int modifier, int required, bool reroll, enum eSkill skill)
{
  cout << "Player `" << player_id << "' tried an action : `"
       << Dice::stringify(action_type)
       << "' : roll [" << result << "] + ["<< modifier << "], required : ["
       << required << "]." << endl;

  if (api_->getTeamId() == team_id && (reroll || (skill != SK_UNASSIGNED)))
    {
      api_->selectSkilledPlayer(player_id);
      cout << "You can"
        << ((reroll) ? " use a team 'reroll' or" : "")
        << ((skill != SK_UNASSIGNED) ? " use the 'skill " : "")
        << ((skill != SK_UNASSIGNED) ? Player::stringify(skill) : "")
        << ((skill != SK_UNASSIGNED) ? "' or" : "")
        << " 'accept' this result." << endl;
      input_.stopWaiting();
    }
}

void CmdLineInterface::evBlock(int team_id, int player_id, int opponent_id) 
{
  if (team_id != api_->getTeamId())
    cout << "Opponent ";

  cout << "Player `" << player_id << "' tries to block " << "player `" 
       << opponent_id << "'" << endl;
}

void CmdLineInterface::evBlockResult(int team_id, int player_id, int opponent_id, 
                                     int nb_dice, enum eBlockDiceFace result[3],
                                     int choose, bool reroll)
{
  cout << "Block result : ";
  for (int i = 0; i < nb_dice; i++)
    {
      cout << i << "- " << Dice::stringify(result[i]);
      if (i != nb_dice - 1)
        cout << ", ";
    }
  cout << endl;

  if (team_id == api_->getTeamId() && choose == api_->getTeamId() && reroll)
    cout << " You can use a 'reroll' or choose a 'block dice <n>'." << endl;
  if (team_id == api_->getTeamId() && choose == api_->getTeamId() && !reroll)
    cout << " You must choose a 'block dice <n>'." << endl;
  if (team_id == api_->getTeamId() && choose != api_->getTeamId() && reroll)
    cout << " You can use a 'reroll' or 'accept' this block result." << endl;
  if (team_id != api_->getTeamId() && choose == api_->getTeamId() && reroll)
    cout << " Wait for opponent reroll decision" << endl;
  if (team_id != api_->getTeamId() && choose == api_->getTeamId() && !reroll)
    cout << " You must choose a 'block dice <n>'." << endl;
}

void CmdLineInterface::evSkill(int team_id, int player_id, enum eSkill skill, int choice)
{
  if (team_id != api_->getTeamId())
    cout << "Opponent ";
  cout << "Player `" << player_id << "' ";
  if (choice == -1)
    cout << "can use ";
  else if (choice == 0)
    cout << "doesn't use ";
  else
    cout << "uses ";
  cout << "the skill `" << Player::stringify(skill) << "'." << endl;

  if (choice == -1)
    if (team_id != api_->getTeamId())
      cout << "Wait for opponent skill decision." << endl;
    else
      {
        api_->selectSkilledPlayer(player_id);
        cout << "You must choose to either use or ignore this skill ('help skill')." << endl;
        input_.stopWaiting();
      }
  else if ((team_id != api_->getTeamId()) && our_turn_ &&
      ((skill == SK_BLOCK) || (skill == SK_CATCH) || (skill == SK_DODGE)))
    input_.stopWaiting();
}

