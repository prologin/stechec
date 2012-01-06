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
** `COPYING' file in the root directory.
** 
** The TBT Team consists of people listed in the `AUTHORS' file.
*/

#include <dlfcn.h>
#include "tools.hh"
#include "Library.hh"

LibraryError::LibraryError()
  : Exception(dlerror())
{
}

LibraryError::LibraryError(const std::string& msg)
  : Exception(msg + ": " + dlerror())
{
}

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
      LOG5("Unload dynamic library `%1`", library_file);
      dlclose(hndl_);
    }

  // FIXME: add .dll or other on windows.
  library_file_ = library_file + ".so";
  LOG5("Opening dynamic library `%1`", library_file_);
  hndl_ = dlopen(library_file_.c_str(), RTLD_LAZY | RTLD_GLOBAL);
  if (hndl_ == NULL)
    PRINT_AND_THROW(LibraryError, "");
  LOG3("Library `%1` opened.", library_file_ );
}

Library::~Library()
{
  if (hndl_ != NULL)
    {
      LOG5("Unload dynamic library `%1`", library_file_);
      dlclose(hndl_);
    }
}

void*  Library::getSymbol(const char* sym, bool required)
{
  assert(hndl_ != NULL);

  void *mkr = dlsym(hndl_, sym);
  if (mkr == NULL && required)
    PRINT_AND_THROW(LibraryError, "dlsym");
  if (mkr != NULL)
    LOG6("Symbol `%1` loaded from `%2`",sym, library_file_);
  return mkr;
}
