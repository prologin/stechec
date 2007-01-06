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

#include "tools.hh"
#include "ClientApp.hh"
#include "Api.hh"
#include "interface.hh"

#ifdef CONSOLE_BINARY

class ConsoleApp : public ClientApp
{
public:
  ConsoleApp(int argc, char** argv);
  virtual ~ConsoleApp();

private:
  virtual void showVersion();
  virtual int onPlay(bool replay);
};


ConsoleApp::ConsoleApp(int argc, char** argv)
  : ClientApp(argc, argv, ".tbt/tbtrc", PKGDATADIR)
{
}

ConsoleApp::~ConsoleApp()
{
}

void ConsoleApp::showVersion()
{
  std::cout << "TowBowlTactics console client v" PACKAGE_VERSION << "\n"
	    << "Copyright (C) 2006 TBT Team.\n";
}

int ConsoleApp::onPlay(bool)
{
  bool use_readline = true;
  try {
    cfg_.getAttr<int>("client", "redirection", "stdin");
    use_readline = false;
  } catch (...) {}

  CmdLineInterface i(&cfg_, rules_->getApi(), &ccx_, use_readline);

  i.hello();
  i.init();
  i.run();

  return -1; // Don't want to play a second time.
}

int main(int argc, char** argv)
{
  ConsoleApp app(argc, argv);

  return app.runApp();
}


#else

extern "C" int run(xml::XMLConfig* cfg, Api* api, ClientCx* ccx)
{
  bool use_readline = true;
  try {
    cfg->getAttr<int>("client", "redirection", "stdin");
    use_readline = false;
  } catch (...) {}

  CmdLineInterface i(cfg, api, ccx, use_readline);

  i.hello();
  i.init();
  i.run();

  return 0;
}

#endif /* !CONSOLE_MODULE */
