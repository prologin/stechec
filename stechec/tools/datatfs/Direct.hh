/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2006, 2007 Prologin
*/

#ifndef DIRECT_HH_
# define DIRECT_HH_

# include "Cx.hh"

//! @ingroup tools_net
enum eCxWhoIAm {
  CXD_IS_SERVER,
  CXD_IS_COACH1,
  CXD_IS_COACH2
};

//! @ingroup tools_net
class DirectCx : public Cx
{
public:
  DirectCx(eCxWhoIAm w);
  virtual ~DirectCx();

protected:
  virtual bool pollInternal(int* timeout = 0);
  virtual int recvData(bool use_exception);
  virtual void sendData(unsigned char* data, unsigned size);
  
private:
  void lock(bool lock_server);
  void unlock(bool unlock_server);

  eCxWhoIAm w_;

  static DirectCx* inst_client_[2];
  static DirectCx* inst_server_;
};

#endif /* !DIRECT_HH_ */
