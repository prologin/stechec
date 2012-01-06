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

#ifndef INIFILE_HH_
# define INIFILE_HH_

# include <string>
# include <vector>
# include <map>
# include "misc/Exception.hh"

class ConfException : public Exception
{
public:
  ConfException(const std::string& reason);
};


class ConfSection
{
public:
  ~ConfSection();

  //! @brief check if a key exists
  bool exist(const std::string& key) const;
  
  //! @brief get value
  template <typename T>
  T getValue(const std::string& key) const;

  //! @brief set value
  template <typename T>
  void setValue(const std::string& key, const T& value);

  typedef std::map<std::string, std::string> RegList;
  typedef RegList::const_iterator RegIterConst;
  typedef RegList::iterator RegIter;

private:
  ConfSection(const std::string& section);

  RegList reg_;
  std::string section_;

  friend bool operator==(const ConfSection* lhs, const std::string& rhs);
  friend class ConfFile;
};


class ConfFile
{
public:
  ConfFile(int argc, char **argv);
  ~ConfFile();

  void parse(const std::string& file);
  ConfSection* parseCmdLine(const std::string& section,
                            const struct ConfCmdLineOpt opt[],
                            int chunk_index = 1);
  ConfSection* setDefaultEntries(const std::string& section,
                                 const ConfSection::RegList& def_ent);
  ConfSection* getSection(const std::string& section);
  void addAlias(const std::string& orig, const std::string& link);
  void printHelpOption(const struct ConfCmdLineOpt opt[]);

private:
  typedef std::map<std::string, ConfSection*> SectionList;
  typedef std::map<std::string, std::string> AliasList;

  SectionList section_list_;
  AliasList alias_;
  int argc_;
  char **argv_;
};


struct ConfCmdLineOpt
{
  unsigned char short_opt;
  const char* long_opt;
  int have_arg;
  const char* help;
};

#endif // !INIFILE_HH_
