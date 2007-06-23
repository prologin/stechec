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

#ifndef FILECX_HH_
# define FILECX_HH_

# include "Cx.hh"

//! @ingroup tools_net
class FileCx : public Cx
{
public:
  FileCx() {}
  virtual ~FileCx() {}

protected:
  virtual bool pollInternal(int* timeout = 0);
  virtual int recvData(bool use_exception);
  virtual void sendData(unsigned char* data, unsigned size);
};

inline bool FileCx::pollInternal(int*) { return false; }
inline int FileCx::recvData(bool) { return 0; }
inline void FileCx::sendData(unsigned char*, unsigned) {}

#endif /* !FILECX_HH_ */
