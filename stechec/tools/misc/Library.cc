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

#include <dlfcn.h>
#include "tools.hh"
#include "Library.hh"

Library::Library()
  : hndl_(NULL)
{
}

Library::Library(const std::string& library_file)
  : hndl_(NULL)
{
  open(library_file);
}

// Find and load the shared library
void Library::open(const std::string& library_file)
{
  if (hndl_ != NULL)
    {
      LOG5("Unload dynamic library `" << library_file_ << "'");
      dlclose(hndl_);
    }

  // FIXME: add .dll or other on windows.
  library_file_ = library_file + ".so";
  LOG5("Opening dynamic library `" << library_file_ << "'");
  hndl_ = dlopen(library_file_.c_str(), RTLD_LAZY | RTLD_GLOBAL);
  if (hndl_ == NULL)
    {
      std::ostringstream err_msg;
      err_msg << dlerror();
      ERR(err_msg.str());
      throw LibraryError(err_msg.str().c_str());
    }
  LOG3("Library `" << library_file_ << "' opened.");
}

Library::~Library()
{
  if (hndl_ != NULL)
    {
      LOG5("Unload dynamic library `" << library_file_ << "'");
      dlclose(hndl_);
    }
}

void*  Library::getSymbol(const char* sym, bool required)
{
  assert(hndl_ != NULL);

  void *mkr = dlsym(hndl_, sym);
  if (mkr == NULL && required)
    {
      std::ostringstream err_msg;
      err_msg << "dlsym: " << dlerror();
      ERR(err_msg.str());
      throw LibraryError(err_msg.str().c_str());
    }
  if (mkr != NULL)
    LOG6("Symbol `" << sym << "' loaded from `" << library_file_ << "'");
  return mkr;
}
