/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2006 Prologin
*/

#include "tools.hh"
#include "ClientApp.hh"

class Stechec : public ClientApp
{
public:
  Stechec(int argc, char** argv);
  virtual ~Stechec();

private:
  virtual int onPlay(bool is_replay);
};

Stechec::Stechec(int argc, char** argv)
  : ClientApp(argc, argv)
{
}

Stechec::~Stechec()
{
}

int Stechec::onPlay(bool)
{
  // Call a hook in rules, before UI is loaded.
  if (!rules_loader_.initRules(&ccx_))
    {
      LOG3("Sorry, rules initialization failed, cannot go further.");
      return 1;
    }

  int ret = runChampion();

  if (ret == 0)
    return -1; // Don't want to run again.
  return ret;
}

int main(int argc, char** argv)
{
  Stechec c(argc, argv);

  return c.runApp();
}
