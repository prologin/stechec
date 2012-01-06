/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2007 Prologin
*/

#ifndef FILECX_HH_
# define FILECX_HH_

# include <fstream>
# include "Cx.hh"

//! @ingroup tools_net
class FileCx : public Cx
{
public:
  FileCx() {}
  virtual ~FileCx() {}

  void open(const std::string& filename, int mode);
  
protected:
  virtual bool pollInternal(int* timeout = 0);
  virtual int recvData(bool use_exception);
  virtual void sendData(unsigned char* data, unsigned size);

private:
  std::fstream file_;
};

#endif /* !FILECX_HH_ */
