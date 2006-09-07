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

/*
** Quick and light command line processer.
*/
class Input
{
public:
  Input(CmdLineInterface* i, Api* gc);
  ~Input();

  bool process();
  void processCommand(const std::string& s);

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

	// Action command
	void cmdMove(const std::string& cmd, const std::string& args);
	void cmdMoveBz(const std::string& cmd, const std::string& args);
	void cmdMoveP(const std::string& cmd, const std::string& args);
	void cmdStandUpM(const std::string& cmd, const std::string& args);
	void cmdStandUpBk(const std::string& cmd, const std::string& args);
	void cmdStandUpBz(const std::string& cmd, const std::string& args);
	void cmdStandUpP(const std::string& cmd, const std::string& args);
	void cmdBlock(const std::string& cmd, const std::string& args);
	void cmdBlockBz(const std::string& cmd, const std::string& args);
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

  Api*                  api_;
  CmdLineInterface*     i_;
  bool                  want_exit_;
};

#endif /* !INPUT_HH_ */
