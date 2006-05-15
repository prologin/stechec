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

class Input
{
public:
  Input(CmdLineInterface* i, Api* gc);
  ~Input();

  bool getCommand();

private:
  void cmdHelp(const std::string& cmd, const std::string& args);
  void cmdPrint(const std::string& cmd, const std::string& args);
  void cmdSay(const std::string& cmd, const std::string& args);
  void cmdKickOff(const std::string& cmd, const std::string& args);
  void cmdMove(const std::string& cmd, const std::string& args);
  void cmdIllegal(const std::string& cmd, const std::string& args);
  void cmdEnd(const std::string& cmd, const std::string& args);

  Api* api_;
  CmdLineInterface* i_;
};

#endif /* !INPUT_HH_ */
