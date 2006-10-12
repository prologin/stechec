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

#ifdef HAVE_READLINE_H
# include <readline.h>
# include <history.h>
# define USE_READLINE 1
#elif HAVE_READLINE_READLINE_H
# include <readline/readline.h>
# include <readline/history.h>
# define USE_READLINE 1
#endif

#include "interface.hh"
#include "input.hh"

using namespace std;

//
// Realine forward declaration.
//

#ifdef USE_READLINE
// Yes, easier.
static Input* input_inst = NULL;

void get_line(char* line);
char** cmd_completion(const char* text, int start, int end);
char* cmd_completion_foo(const char*, int);
#endif // !USE_READLINE

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
  {"giveBall", &Input::cmdGiveBall, "<id>|give the ball to the player'id'"},
  {"illegal", &Input::cmdIllegal, "ask for an illegal procedure"},
  {"reroll", &Input::cmdReroll, "reroll the dice(s)"},
  {"accept", &Input::cmdAccept, "accept result of the dice(s)"},
  {"end", &Input::cmdEnd, "end turn"},
  {"declare", &Input::cmdDeclare, "<subcmd>|print some informations ('help declare')"},
  {"move", &Input::cmdMove, "<subcmd>|move something ('help move')"},
  {"standup", &Input::cmdStandUp, "<id>|stand up the player'id'"},
  {"block", &Input::cmdBlock, "<id> <d_id>|block with player 'id' player 'd_id'"},
  {"pass", &Input::cmdPass, "<p> <r> <c>|pass the ball with player 'id' at specified position"},
  {"dice", &Input::cmdDice, "<n>|choose the dice number <n> for the block."},
  {"follow", &Input::cmdFollow, "follow after a block."},
  {"stay", &Input::cmdStay, "stay after a block."},
  {"push", &Input::cmdPush, "<n>|choose the square to push the player in."},
  {"cheat", &Input::cmdCheat, "<n> <n> <...>|force next dice rolls to give these results (cheat)."},
  {"wait", &Input::cmdWait, "do not process input util it's your turn."},
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

Input::InputSubCommand Input::declare_cmd_[] = {
  {"move", &Input::cmdDeclareMove, "<p>|declare a move action for player <p>"},
  {"block", &Input::cmdDeclareBlock, "<p>|declare a block action for player <p>"},
  {"blitz", &Input::cmdDeclareBlitz, "<p>|declare a blitz action for player <p>"},
  {"pass", &Input::cmdDeclarePass, "<p>|declare a pass action for player <p>"},
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
  if (api_->doPlaceBall(pb))
    sync_++;
}

void Input::cmdReroll(const string&, const string&)
{
  if (api_->doReroll())
    sync_++;
}

void Input::cmdAccept(const string&, const string&)
{
  if (api_->doAccept())
    sync_++;
}

void Input::cmdMove(const string& cmd, const string& args)
{
  if (cmd == "turnmarker")
    cmdMoveTurnMarker(args);
  else
    cmdMovePlayer(cmd + " " + args);
}

void Input::cmdDeclare(const string& cmd, const string& args)
{
  if (cmd == "move")
    cmdDeclareMove(args);
  else if (cmd == "block")
    cmdDeclareBlock(args);
  else if (cmd == "blitz")
    cmdDeclareBlitz(args);
  else if (cmd == "pass")
    cmdDeclarePass(args);
}

void Input::cmdStandUp(const string& cmd, const string& args)
{
  istringstream is(cmd + " " + args);
  int p = -1;
  is >> p;
  if (api_->doStandUpPlayer(p))
    sync_++;
}

void Input::cmdBlock(const string& cmd, const string& args)
{
  istringstream is(cmd + " " + args);
  int p_id = -1;
  int p_did = -1;
  is >> p_id;
  is >> p_did;
  if (api_->doBlockPlayer(p_id, p_did))
    sync_++;
}

void Input::cmdPass(const string& cmd, const string& args)
{
  istringstream is(cmd + " " + args);
  int p_id = -1;
  Position pos;
  is >> p_id;
  is >> pos.row;
  is >> pos.col;
  if (api_->doPassPlayer(p_id, pos))
    sync_++;
}

void Input::cmdIllegal(const string&, const string&)
{
  api_->doAskIllegalProcedure();
  sync_++;
}

void Input::cmdEnd(const string&, const string&)
{
  api_->doEndTurn();
  sync_++;
}

void Input::cmdDice(const string& cmd, const string& args)
{
  istringstream is(cmd + " " + args);
  int n = -1;
  is >> n;
  if (api_->doChooseBlockDice(n))
    sync_++;
}

void Input::cmdFollow(const string&, const string&)
{
  if (api_->doFollow(true))
    sync_++;
}

void Input::cmdStay(const string&, const string&)
{
  if (api_->doFollow(false))
    sync_++;
}

void Input::cmdPush(const string& cmd, const string& args)
{
  istringstream is(cmd + " " + args);
  int n = -1;
  is >> n;
  if (api_->doBlockPush(n))
    sync_++;
}

void Input::cmdGiveBall(const string& cmd, const string& args)
{
  istringstream is(cmd + " " + args);
  int p = -1;
  is >> p;
  if (api_->doGiveBall(p))
    sync_++;
}

void Input::cmdCheat(const string& cmd, const string& args)
{
  istringstream is(cmd + " " + args);
  int roll = -1;
  is >> roll;
  while (roll != -1)
    {
      api_->doCheatDice(roll);
      sync_++;
      roll = -1;
      is >> roll;
    }
}

void Input::cmdWait(const string&, const string&)
{
  wait_++;
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
  sync_++;
}

void Input::cmdMovePlayer(const std::string& args)
{
  istringstream is(args);
  int p = -1;
  Position pos;
  is >> p;
  is >> pos.row;
  is >> pos.col;
  if (api_->doMovePlayer(p, pos))
    sync_++;
}

//
// Declare commands
//

void Input::cmdDeclareMove(const std::string& args)
{
  istringstream is(args);
  int p = -1;
  is >> p;
  if (api_->doDeclareMove(p))
    sync_++;
}

void Input::cmdDeclareBlock(const std::string& args)
{
  istringstream is(args);
  int p = -1;
  is >> p;
  if (api_->doDeclareBlock(p))
    sync_++;
}

void Input::cmdDeclareBlitz(const std::string& args)
{
  istringstream is(args);
  int p = -1;
  is >> p;
  if (api_->doDeclareBlitz(p))
    sync_++;
}

void Input::cmdDeclarePass(const std::string& args)
{
  istringstream is(args);
  int p = -1;
  is >> p;
  if (api_->doDeclarePass(p))
    sync_++;
}


//
// Input main functions.
//

Input::Input(CmdLineInterface* i, Api* gc, bool use_readline)
  : api_(gc),
    i_(i),
    want_exit_(false),
    wait_(0),
    sync_(0),
    use_readline_(use_readline),
    read_size_(0)
{
  ios_base::sync_with_stdio(true);

#ifdef USE_READLINE
  if (use_readline_)
    {
      // Initialize readline
      rl_callback_handler_install(NULL, &get_line);
      rl_attempted_completion_function = cmd_completion;
      rl_completion_entry_function = cmd_completion_foo;

      assert(input_inst == NULL);
      input_inst = this;
    }
#endif // !NO_READLINE
}

Input::~Input()
{
#ifdef USE_READLINE
  if (use_readline_)
    {
      input_inst = NULL;
      rl_callback_handler_remove();
    }
#endif // !USE_READLINE
}

void Input::wantExit()
{
  want_exit_ = true;
}

void Input::stopWaiting()
{
  wait_--;
}

void Input::syncDone()
{
  // FIXME: in fact, counting it doesn't work.
  if (--sync_ < 0)
    sync_ = 0;
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
  if (wait_ <= 0 && sync_ == 0)
    FD_SET(STDIN_FILENO, &fds);

  int ret = 42;
  cmd_processed_ = false;

  // Poll stdin, fill readline with characters if any.
  // Block around 50ms.
  // Stop if one command is executed (avoid reading "< file.cmd" at once).
  while (ret > 0 && !cmd_processed_ && !want_exit_)
    {
      ret = select(STDIN_FILENO + 1, &fds, NULL, NULL, &tval);
      if (ret < 0 && errno != EINTR)
        PRINT_AND_THROW(Exception, "stdin select");
      if (ret > 0
#ifdef USE_READLINE
	  && use_readline_)
	{
	  rl_callback_read_char();
	}
      else if (ret > 0
#endif // !USE_READLINE
	       )
	{
	  if (read_size_ == 0)
	    {
	      read_size_ = read(STDIN_FILENO, buf_, 1024);
	      read_index_ = 0;
	    }
	  if (read_size_ < 0)
	    PRINT_AND_THROW(Exception, "stdin read");
	  if (read_size_ == 0)
	    return true;

	  for (; read_index_ < read_size_; read_index_++)
	    {
	      if (buf_[read_index_] == '\n')
		{
		  if (cmd_ != "")
		    processCommand(cmd_);
		  cmd_ = "";
		  read_index_++;
		  break;
		}
	      else
		cmd_ += buf_[read_index_];
	    }
	  if (read_index_ == read_size_)
	    read_size_ = 0;

	}
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

  cmd_processed_ = true;
}


//
// Realine related functions.
//

#ifdef USE_READLINE

// Callback. Called by readline when a line is completed.
void get_line(char* line)
{
  // EOF
  if (line == NULL)
    {
      input_inst->wantExit();
      return;
    }

  if (*line != 0)
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


#endif // !USE_READLINE
