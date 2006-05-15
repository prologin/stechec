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

#ifndef FILE_HH_
# define FILE_HH_

# include <fstream>
# include "cx.hh"

class FileCx : public Cx
{
public:
  FileCx();
  virtual ~FileCx();

  void open(const std::string& filename, int mode);
  
  virtual void          send(const Packet* pkt);
  virtual Packet*       receive();
  virtual bool          poll(int timeout = 0);

private:
  std::fstream          f_;
  int                   mode_;
};

#endif /* !FILE_HH_ */
