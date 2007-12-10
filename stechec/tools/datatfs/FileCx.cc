/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2007 Prologin
*/

#include "tools.hh"
#include "FileCx.hh"

void FileCx::open(const std::string& filename, int mode)
{
  if (mode == CX_READ)
    file_.open(filename.c_str(), std::ios_base::in | std::ios_base::binary);
  else if (mode == CX_WRITE)
    file_.open(filename.c_str(), std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
  else
    THROW(Exception, "bad file mode");
  if (file_.fail())
    THROW(Exception, "cannot open '" << filename << "'");
  state_ = mode;
}

bool FileCx::pollInternal(int* timeout)
{
  (void) timeout;
  return false;
}

int FileCx::recvData(bool use_exception)
{
  (void) use_exception;
  return 0;
}

void FileCx::sendData(unsigned char* data, unsigned size)
{
  file_.write(reinterpret_cast<const char *>(data), size);
}
