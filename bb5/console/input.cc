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

#include <cstdio>
#include <sys/types.h>
#include <iomanip>

#include "tools.hh"

//#ifdef HAVE_READLINE_H
//# include <readline.h>
//# include <history.h>
//#elif HAVE_READLINE_READLINE_H
# include <readline/readline.h>
# include <readline/history.h>
//#else
//# error Sorry, fallback where not using readline is not implemented 
//yet.
//#endif

#include "interface.hh"
#include "input.hh"

using namespace std;

//
// Realine forward declaration.
//

// Yes, easier.
static Input* input_inst = NULL;

void get_line(char* line);
char** cmd_completion(const char* text, int start, int end);
char* cmd_completion_foo(const char*, int);


//
// List of commands.
// You are free to add other.
//

Input::InputCommand Input::main_cmd_[] = {
  {"quit", &Input::cmdQuit, "exit the game, CU"},
  {"help", &Input::cmdHelp, "print this help"},
  {"print", &Input::cmdPrint, "<subcmd>|print some informations ('help print')"},
  {"say", &Input::cmdSay, "<s>|chat with others"},
  {"kickoff", &Input::cmdKickOff, "<r> <c>|place the ball on <r, c>"},
  {"move", &Input::cmdMove, "<subcmd>|move something ('help move')"},
  {"illegal", &Input::cmdIllegal, "ask for an illegal procedure"},
  {"end", &Input::cmdEnd, "end turn"},
  {NULL, NULL, NULL}
};

Input::InputSubCommand Input::print_cmd_[] = {
  {"global", &Input::cmdPrintGlobal, "print game globals"},
  {"field", &Input::cmdPrintField, "print field in ascii art"},
  {"players", &Input::cmdPrintPlayers, "print player list"},
  {"us", &Input::cmdPrintUs, "<n>|print info for our team player <n>"},
  {"them", &Input::cmdPrintThem, "<n>|print info for other team player <n>"},
  {"", &Input::cmdPrintString, "<s>|simply display the string <s>"},
  {NULL, NULL, NULL}
};

Input::InputSubCommand Input::move_cmd_[] = {
  {"turnmarker", &Input::cmdMoveTurnMarker, "move the turn marker"},
  {"", &Input::cmdMovePlayer, "<p> <r> <c>|move player <p> to <r,c>"},
  {NULL, NULL, NULL}
};




//
// Main commands
//

void Input::cmdQuit(const std::string&, const std::string&)
{
  want_exit_ = true;
}

void Input::cmdHelp(const string& cmd, const string&)
{
  std::string cmd_name;
  std::string cmd_doc;

  // Print head
  if (cmd == "print")
    cout << "Command list for 'print':\n";
  else if (cmd == "move")
    cout << "Command list for 'move':\n";
  else
    cout << "Quick command list:\n";
  cout << setiosflags(ios::left);
  
  for (int i = 0; ; i++)
    {
      // Get command name and doc.
      if (cmd == "print")
        {
          if (print_cmd_[i].name == NULL)
            return;
          cmd_name = print_cmd_[i].name;
          cmd_doc = print_cmd_[i].doc;
        }      
      else if (cmd == "move")
        {
          if (move_cmd_[i].name == NULL)
            return;
          cmd_name = move_cmd_[i].name;
          cmd_doc = move_cmd_[i].doc;
        }
      else
        {
          if (main_cmd_[i].name == NULL)
            return;
          cmd_name = main_cmd_[i].name;
          cmd_doc = main_cmd_[i].doc;
        }

      // Print help line.
      string::size_type pipe = cmd_doc.find('|');
      if (pipe != string::npos)
        {
          cmd_name = cmd_name + " " + trim(cmd_doc.substr(0, pipe));
          cmd_doc = cmd_doc.substr(pipe + 1, 180);
        }
      cout << " - " << setw(16) << cmd_name << " " << trim(cmd_doc) << endl;
    }
}

void Input::cmdPrint(const string& cmd, const string& args)
{
  int i;
  for (i = 0; print_cmd_[i].name != NULL; i++)
    if (cmd == print_cmd_[i].name)
      {
        (this->*print_cmd_[i].fun)(args);
        break;
      }
  if (print_cmd_[i].name == NULL)
    cmdPrintString(cmd + " " + args);
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
  if (cmd == "turnmarker")
    cmdMoveTurnMarker(args);
  else
    cmdMovePlayer(cmd + " " + args);
}

void Input::cmdIllegal(const string&, const string&)
{
  api_->doAskIllegalProcedure();
}

void Input::cmdEnd(const string&, const string&)
{
  api_->doEndTurn();
}

//
// Print commands
//

void Input::cmdPrintGlobal(const std::string&)
{
  i_->printGlobal();
}

void Input::cmdPrintField(const std::string&)
{
  i_->printField();
}

void Input::cmdPrintPlayers(const std::string&)
{
  i_->printPlayerList();
}

void Input::cmdPrintUs(const std::string& args)
{
  istringstream is(args);
  int id = -1;
  is >> id;
  i_->printPlayer(id, US);
}

void Input::cmdPrintThem(const std::string& args)
{
  istringstream is(args);
  int id = -1;
  is >> id;
  i_->printPlayer(id, THEM);
}

void Input::cmdPrintString(const std::string& args)
{
  cout << args << endl;
}

//
// Move commands
//

void Input::cmdMoveTurnMarker(const std::string&)
{
  api_->doMoveTurnMarker();
}

void Input::cmdMovePlayer(const std::string& args)
{
  istringstream is(args);
  int p = -1;
  Position pos;
  is >> p;
  is >> pos.row;
  is >> pos.col;
  api_->doMovePlayer(p, pos);
}


//
// Input main functions.
//

Input::Input(CmdLineInterface* i, Api* gc)
  : api_(gc),
    i_(i),
    want_exit_(false)
{
  ios_base::sync_with_stdio(true);

  // Initialize readline
  rl_callback_handler_install(NULL, &get_line);
  rl_attempted_completion_function = cmd_completion;
  rl_completion_entry_function = cmd_completion_foo;

  assert(input_inst == NULL);
  input_inst = this;
}

Input::~Input()
{
  input_inst = NULL;
  rl_callback_handler_remove();
}

bool Input::process()
{
  // #$^#$%^#$^ poll() on MacOsX (at least 10.4) doesn't accept STDIN_FILENO
  // ... or how to lost 3 days on a stupid problem. Revert back to select().
  struct timeval tval;
  tval.tv_sec  = 0;
  tval.tv_usec = 50 * 1000;
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);

  int ret = 42;

  // Poll stdin, fill readline with characters if any.
  // Block around 50ms.
  while (ret > 0)
    {
      ret = select(STDIN_FILENO + 1, &fds, NULL, NULL, &tval);
      if (ret < 0)
        PRINT_AND_THROW(Exception, "stdin select");
      if (ret > 0)
        rl_callback_read_char();
    }
  return want_exit_;
}

void Input::processCommand(const std::string& s)
{
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

  int i;
  for (i = 0; main_cmd_[i].name != NULL; i++)
    if (cmd == main_cmd_[i].name)
      {
        (this->*main_cmd_[i].fun)(sub_cmd, args);
        break;
      }
  if (main_cmd_[i].name == NULL)
    cout << "Unknown command: `" << cmd << "', try again, or type 'help'." << endl;
}


//
// Realine related functions.
//

// Callback. Called by readline when a line is completed.
void get_line(char* line)
{
  if (line && *line != 0)
    add_history(line);
  std::string cmd(line);
  free(line);
  input_inst->processCommand(cmd);
}

// Get next matching word in 'main' commands
char* cmd_generator(const char* text, int state)
{
  static int list_index;
  static int len;
  const char* name;

  if (state == 0)
    {
      list_index = 0;
      len = strlen(text);
    }
  while ((name = input_inst->main_cmd_[list_index++].name) != NULL)
    if (strncmp(name, text, len) == 0)
      return strdup(name);
  return NULL;
}

// Get next matching word in 'move' command
char* cmd_generator_print(const char* text, int state)
{
  static int list_index;
  static int len;
  const char* name;

  if (state == 0)
    {
      list_index = 0;
      len = strlen(text);
    }
  while ((name = input_inst->print_cmd_[list_index++].name) != NULL)
    if (strncmp(name, text, len) == 0)
      return strdup(name);
  return NULL;
}

// Get next matching word in 'move' command
char* cmd_generator_move(const char* text, int state)
{
  static int list_index;
  static int len;
  const char* name;

  if (state == 0)
    {
      list_index = 0;
      len = strlen(text);
    }
  while ((name = input_inst->move_cmd_[list_index++].name) != NULL)
    if (strncmp(name, text, len) == 0)
      return strdup(name);
  return NULL;
}

// Get next matching word in 'help' command
char* cmd_generator_help(const char* text, int state)
{
  static int list_index;
  static int len;
  const char* name;
  const char* help_list[] = {
    "move",
    "print",
    NULL
  };

  if (state == 0)
    {
      list_index = 0;
      len = strlen(text);
    }
  while ((name = help_list[list_index++]) != NULL)
    if (strncmp(name, text, len) == 0)
      return strdup(name);
  return NULL;
}

// Called by readline, when user type 'tab'
// Must return the list of words that can complete start of 'text'.
char** cmd_completion(const char* text, int start, int)
{
  char **matches = NULL;   

  // FIXME: is 'completion_matches' on mac.
  if (start == 0)
    matches = rl_completion_matches(text, cmd_generator);
  else if (strncmp(rl_line_buffer, "print", 5) == 0)
    matches = rl_completion_matches(text, cmd_generator_print);
  else if (strncmp(rl_line_buffer, "move", 4) == 0)
    matches = rl_completion_matches(text, cmd_generator_move);
  else if (strncmp(rl_line_buffer, "help", 4) == 0)
    matches = rl_completion_matches(text, cmd_generator_help);

  return matches;
}

// Override the default completion, on filename and users.
char* cmd_completion_foo(const char*, int)
{
  return NULL;
}
