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

#ifndef LIBRARY_HH_
# define LIBRARY_HH_

# include <string>

/*!
** @brief Exception class for dynamic library loader errors.
** @ingroup tools_misc
*/
class LibraryError : public Exception
{
public:
  
  //! @brief Standard constructor. Output of dlerror() stands for the reason.
  LibraryError();

  //! @brief Standard constructor. Output of dlerror() stands for the reason.
  //! @param msg The failing message.
  LibraryError(const std::string& msg);
};

/*!
** @brief Dynamic library loader.
** @ingroup tools_misc
*/
class Library
{
public:
  Library();
  Library(const std::string& library_file);
  ~Library();

  void          open(const std::string& library_file);

  //! @brief Get a symbol from this dynamic library.
  //! @throw LibraryError Thrown when the symbol was not found, and
  //!   it is required.
  void*         getSymbol(const char* sym, bool required = true);

private:

  void*         hndl_;
  std::string   library_file_; ///< Keep lib filename, for debug messages.
};

#endif /* !LIBRARY_HH_ */
