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

#ifndef STECHEC_HH_
# define STECHEC_HH_

# include "ClientApp.hh"

/*!
** @brief Stechec generic binary client.
**
** This is by far the most simple client available: it presents no menu, loads
** a gui/champion/whatever, and runs it once.
*/
class Stechec : public ClientApp
{
public:
  Stechec(int argc, char** argv);
  virtual ~Stechec();

private:
  virtual int onPlay(bool is_replay);
};


#endif /* !STECHEC_HH_ */
