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

#ifndef INPUT_HH_
# define INPUT_HH_

class Api;
class CmdLineInterface;

/*!
** @brief Quick and light command line processer.
*/
class Input
{
public:
  Input(CmdLineInterface* i, Api* gc, bool use_readline);
  ~Input();

  ///! @brief Main loop, check input, process commands... call it frequently.
  ///! @note Block at most 50ms.
  ///! @return true if user wants to exit from app (EOF, quit, etc...).
  bool process();

  ///! @brief Process a command.
  ///! @param s The command line, without trailing '\n'.
  void processCommand(const std::string& s);

  ///! @brief Wants exit, next call to process() will return true.
  void wantExit();

  ///! @brief Tell to process input again.
  void stopWaiting();

  ///! @brief Server has finished to process our input.
  void syncDone();
  
public:

  struct InputCommand
  {
    const char* name;
    void (Input::*fun)(const std::string&, const std::string&);
    const char* doc;
  };

  struct InputSubCommand
  {
    const char* name;
    void (Input::*fun)(const std::string&);
    const char* doc;
  };

  static InputCommand main_cmd_[];
  static InputSubCommand print_cmd_[];
  static InputSubCommand move_cmd_[];
  static InputSubCommand block_cmd_[];
  static InputSubCommand declare_cmd_[];

private:

  // Main commands
  void cmdQuit(const std::string& cmd, const std::string& args);
  void cmdHelp(const std::string& cmd, const std::string& args);
  void cmdPrint(const std::string& cmd, const std::string& args);
  void cmdSay(const std::string& cmd, const std::string& args);
  void cmdKickOff(const std::string& cmd, const std::string& args);
  void cmdIllegal(const std::string& cmd, const std::string& args);
  void cmdEnd(const std::string& cmd, const std::string& args);
  void cmdReroll(const std::string& cmd, const std::string& args);
  void cmdAccept(const std::string& cmd, const std::string& args);
  void cmdGiveBall(const std::string& cmd, const std::string& args);
  void cmdDice(const std::string& cmd, const std::string& args);
  void cmdCheat(const std::string& cmd, const std::string& args);
  void cmdWait(const std::string& cmd, const std::string& args);

  // Action command
  void cmdDeclare(const std::string& cmd, const std::string& args);
  void cmdMove(const std::string& cmd, const std::string& args);
  void cmdStandUp(const std::string& cmd, const std::string& args);
  void cmdBlock(const std::string& cmd, const std::string& args);
  void cmdPass(const std::string& cmd, const std::string& args);

  // Print commands
  void cmdPrintGlobal(const std::string& args);
  void cmdPrintField(const std::string& args);
  void cmdPrintPlayers(const std::string& args);
  void cmdPrintUs(const std::string& args);
  void cmdPrintThem(const std::string& args);
  void cmdPrintString(const std::string& args);

  // Move commands
  void cmdMoveTurnMarker(const std::string& args);
  void cmdMovePlayer(const std::string& args);

  // Block commands
  void cmdBlockBlock(const std::string& args);
  void cmdBlockStay(const std::string& args);
  void cmdBlockFollow(const std::string& args);
  void cmdBlockPush(const std::string& args);

  // Declare commands
  void cmdDeclareMove(const std::string& args);
  void cmdDeclareBlock(const std::string& args);
  void cmdDeclareBlitz(const std::string& args);
  void cmdDeclarePass(const std::string& args);

  Api*                  api_;
  CmdLineInterface*     i_;
  bool                  want_exit_;
  bool			cmd_processed_;   ///< Stop processing input if a command was executed.
  int			wait_;            ///< Don't process input util it is our turn.
  int			sync_;		  ///< Number of 'sync' from server to receive before proceding input.
  bool			use_readline_;    ///< Use readline, if it was compiled (default).

  // for read().
  std::string		cmd_;             ///< What we read from stdin, when no using libreadline.
  int			read_size_;
  int			read_index_;
  char			buf_[1024];
};

#endif /* !INPUT_HH_ */
