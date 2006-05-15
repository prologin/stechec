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

#include <fcntl.h>
#include <sys/types.h>
#include <sys/poll.h>

#include "interface.hh"
#include "input.hh"

using namespace std;

/*
** I tried hard getting SIGIO works for STDIN_FILENO.
** It would allow us to block in process(), and get back as soon as
** possible when a user typed something. But linux was more powerful
** than me... :/ So I ended back using my friend poll().
**
** Now network messages may wait at most 150ms to be processed. Not a
** big problem.
*/

Input::Input(CmdLineInterface* i, Api* gc)
  : api_(gc),
    i_(i)
{
  ios_base::sync_with_stdio(true);
  fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
}

Input::~Input()
{
}

inline std::string trim(const std::string& s)
{
  if (s.length() == 0)
    return s;
  int b = s.find_first_not_of(" \t");
  int e = s.find_last_not_of(" \t");
  if (b == -1) // No non-spaces
    return "";
  return std::string(s, b, e - b + 1);
}

bool Input::getCommand()
{
  struct pollfd         pf[1] = { { STDIN_FILENO, POLLIN, 0 } };

  int ret = poll(pf, 1, 50); // block 50 ms.
  if (ret < 0)
    throw "stdin poll";
  if (ret <= 0)
    return false;
  
  string s;
  getline(cin, s);
  if (cin.fail())
    {
      // poll returning 1 doesn't mean there is something to read...
      cin.clear();
      errno = 0; // otherwise it will blow up network code

      // sig :/ BSD sucks. Or linux. Or both.
      struct timespec tm = {0, 50 * 1000000};
      nanosleep(&tm, NULL);
      return false;
    }

  // Lightly parser for our line into: <cmd> <subcmd> <args>
  string cmd, sub_cmd, args;
  cmd = trim(s);
  string::size_type first_space = cmd.find(' ');
  if (first_space != string::npos)
    {
      sub_cmd = trim(cmd.substr(first_space, 128));
      cmd = cmd.substr(0, first_space);
      string::size_type second_space = sub_cmd.find(' ');
      if (second_space != string::npos)
        {
          args = trim(sub_cmd.substr(second_space, 128));
          sub_cmd = sub_cmd.substr(0, second_space);
        }
    }

  if (cmd == "exit" || cmd == "quit")
    return true;

  static struct {
    const char* name;
    void (Input::*fun)(const string&, const string&);
  } cmd_fcts[] = {
    {"help", &Input::cmdHelp},
    {"print", &Input::cmdPrint},
    {"say", &Input::cmdSay},
    {"kickoff", &Input::cmdKickOff},
    {"move", &Input::cmdMove},
    {"illegal", &Input::cmdIllegal},
    {"end", &Input::cmdEnd},
    {NULL, NULL}
  };

  int i;
  for (i = 0; cmd_fcts[i].name != NULL; i++)
    if (cmd == cmd_fcts[i].name)
      {
        (this->*cmd_fcts[i].fun)(sub_cmd, args);
        break;
      }
  if (cmd_fcts[i].name == NULL)
    cout << "Unknown command: `" << cmd << "', try again, or type 'help'." << endl;
  return false;
}



//
// Commands
//

void Input::cmdHelp(const string& cmd, const string&)
{
  if (cmd == "print")
    {
      cout << "Command list for 'print':\n"
           << " - global          print game globals\n"
           << " - field           print field in ascii art\n"
           << " - players         print player list\n"
           << " - us <n>          print info for our team player <n>\n"
           << " - them <n>        print info for other team player <n>\n"
           << " - <s>             simply display the string <s>\n";
      return;
    }
  if (cmd == "move")
    {
      cout << "Command list for 'move':\n"
           << " - teammarker      move the turn marker\n"
           << " - <p> <r> <c>     move player <p> to <r,c>\n";
      return;
    }

  cout << "Quick command list:\n"
       << " - quit|exit         exit the game. CU.\n"
       << " - print <subcmd>    print some informations ('help print')\n"
       << " - say <s>           chat with others\n"
       << " - kickoff <r> <c>   place the ball on <r, c>\n"
       << " - move <subcmd>     move something ('help move')\n"
       << " - illegal           ask for an illegal procedure\n"
       << " - end               end turn" << endl;
}

void Input::cmdPrint(const string& cmd, const string& args)
{
  if (cmd == "global")
    i_->printGlobal();
  else if (cmd == "field")
    i_->printField();
  else if (cmd == "players")
    i_->printPlayerList();
  else if (cmd == "us")
    {
      istringstream is(args);
      int id = -1;
      is >> id;
      i_->printPlayer(id, US);
    }
  else if (cmd == "them")
    {
      istringstream is(args);
      int id = -1;
      is >> id;
      i_->printPlayer(id, THEM);
    }
  else
    cout << cmd << endl;
}


void Input::cmdSay(const string& cmd, const string& args)
{
  api_->sendChatMessage(cmd + " " + args);
}

void Input::cmdKickOff(const string& cmd, const string& args)
{
  istringstream is(cmd + ' ' + args);
  Position pb;
  is >> pb.row;
  is >> pb.col;
  api_->doPlaceBall(pb);
}

void Input::cmdMove(const string& cmd, const string& args)
{
  if (cmd == "teammarker")
    {
      api_->doMoveTurnMarker();
      return;
    }
  
  istringstream is(cmd + " " + args);
  int p = -1;
  Position pos;
  is >> p;
  is >> pos.row;
  is >> pos.col;
  api_->doMovePlayer(p, pos);
}

void Input::cmdIllegal(const string&, const string&)
{
  api_->doAskIllegalProcedure();
}

void Input::cmdEnd(const string&, const string&)
{
  api_->doEndTurn();
}
