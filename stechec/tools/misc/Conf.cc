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

#include <string>
#include <sstream>
#include <fstream>
#include <cstring>

#include "tools.hh"
#include "misc/Conf.hh"

/*
** ConfException
*/

ConfException::ConfException(const std::string& reason)
  : Exception(reason)
{
}



/*
** ConfSection
*/

ConfSection::ConfSection(const std::string& section)
  : section_(section)
{
}

ConfSection::~ConfSection()
{
}

bool ConfSection::exist(const std::string& key) const
{
  return reg_.find(key) != reg_.end();
}

template <>
int ConfSection::getValue(const std::string& key) const
{
  RegIterConst it = reg_.find(key);
  if (it == reg_.end())
    THROW(ConfException, "unknown key '" << key << "'");

  int ret;
  std::istringstream is(it->second);
  is >> ret;
  return ret;
}

template <>
bool ConfSection::getValue(const std::string& key) const
{
  RegIterConst it = reg_.find(key);
  if (it == reg_.end())
    THROW(ConfException, "unknown key '" << key << "'");

  std::string val = it->second;
  if (val == "true" || val == "t" || val == "yes" || val == "on" || val == "1")
    return true;
  return false;
}

template <>
std::string ConfSection::getValue(const std::string& key) const
{
  RegIterConst it = reg_.find(key);
  if (it == reg_.end())
    THROW(ConfException, "unknown key '" << key << "'");

  return it->second;
}


template <>
void ConfSection::setValue(const std::string& key, const std::string& value)
{
  RegIter it = reg_.find(key);
  if (it == reg_.end())
    THROW(ConfException, "unknown key '" << key << "'");

  it->second = value;
}

template <>
void ConfSection::setValue(const std::string& key, char* const& value)
{
  RegIter it = reg_.find(key);
  if (it == reg_.end())
    THROW(ConfException, "unknown key '" << key << "'");

  it->second = value;
}

template <>
void ConfSection::setValue(const std::string& key, const int& value)
{
  RegIter it = reg_.find(key);
  if (it == reg_.end())
    THROW(ConfException, "unknown key '" << key << "'");

  std::ostringstream os;
  os << value;
  it->second = os.str();
}

template <>
void ConfSection::setValue(const std::string& key, const bool& value)
{
  RegIter it = reg_.find(key);
  if (it == reg_.end())
    THROW(ConfException, "unknown key '" << key << "'");

  it->second = value ? "true" : "false";
}


bool operator==(const ConfSection* lhs, const std::string& rhs)
{
  return lhs->section_ == rhs;
}



/*
** ConfFile
*/

ConfFile::ConfFile(int argc, char **argv)
  : argc_(argc),
    argv_(argv)
{
}

ConfFile::~ConfFile()
{
  SectionList::iterator it;

  for (it = section_list_.begin(); it != section_list_.end(); ++it)
    delete it->second;
}

void ConfFile::parse(const std::string& file)
{
  std::ifstream f(file.c_str());
  std::string s;
  std::string section;
  ConfSection *cur = NULL;

  if (!f)
    {
      THROW(ConfException,
            "cannot open '" << file << "'");
    }

  while (!f.eof())
    {
      std::getline(f, s);
      if (s.length() == 0 || s[0] == '#')
        continue;
      if (s[0] == '[')
        {
          unsigned int end = s.find_first_of(']');
          section = s.substr(1, end - 1);
          if (end == std::string::npos || section.length() == 0)
            {
              f.close();
              THROW(ConfException,
                    "invalid section entry in '" << file << "'");
            }

          AliasList::iterator ait;
          ait = alias_.find(section);
          if (ait != alias_.end())
            section = ait->second;
          
          SectionList::iterator it;
          it = section_list_.find(section);
          if (it == section_list_.end())
            {
              cur = new ConfSection(section);
              section_list_[section] = cur;
            }
          else
            cur = it->second;
        }
      else if (cur != NULL)
        {
          unsigned int eq = s.find_first_of('=');
          if (eq == 0 || eq == std::string::npos)
            continue;
          std::string key = s.substr(0, eq);
          std::string value = s.substr(eq + 1);

          cur->reg_[key] = value;
        }
    }
  f.close();
}


ConfSection* ConfFile::parseCmdLine(const std::string& section,
                                    const struct ConfCmdLineOpt opt[],
                                    int chunk_index)
{
  SectionList::iterator it;
  ConfSection *conf;
  int i;

  it = section_list_.find(section);
  if (it == section_list_.end())
    conf = section_list_[section] = new ConfSection(section);
  else
    conf = it->second;

  for (i = 1; i < argc_ && chunk_index > 1; )
    if (!strcmp(argv_[i++], "--"))
      chunk_index--;
    
  // merge command line options
  for ( ; i < argc_; i++)
    {
      if (!strcmp(argv_[i], "--"))
        break;
      for (int j = 0; opt[j].long_opt; j++)
        {
          std::string key;
          std::string value;

          if (argv_[i][0] == '-' && argv_[i][1] != 0 &&
              argv_[i][1] == opt[j].short_opt)
            key = opt[j].long_opt;

          if (argv_[i][0] == '-' && argv_[i][1] == '-' &&
              ((opt[j].have_arg &&
                !strncmp(opt[j].long_opt, argv_[i] + 2, strlen(opt[j].long_opt))) ||
               (!opt[j].have_arg &&
                !strcmp(opt[j].long_opt, argv_[i] + 2))))
            {
              unsigned int eq;
              key = argv_[i] + 2;
              if ((eq = key.find_first_of('=')) != std::string::npos)
                {
                  value = key.substr(eq + 1);
                  key = key.substr(0, eq);
                }
            }

          if (key != "")
            {
              if (value == "" && opt[j].have_arg && i + 1 < argc_)
                value = argv_[++i];
              else if (value == "" && opt[j].have_arg)
                break;

              unsigned int kid = key.find_first_of('-', 0);
              while (kid != std::string::npos)
                {
                  key.replace(kid, 1, "_");
                  kid = key.find_first_of('-', kid);
                }

              if (opt[j].have_arg)
                conf->reg_[key] = value;
              else
                conf->reg_[key] = "true";
              break;
            }
        }
    }
  return conf;
}


ConfSection* ConfFile::setDefaultEntries(const std::string& section,
                                         const ConfSection::RegList& def_ent)
{
  ConfSection::RegIterConst src_it;
  SectionList::iterator it;
  ConfSection *conf;

  it = section_list_.find(section);
  if (it == section_list_.end())
    conf = section_list_[section] = new ConfSection(section);
  else
    conf = it->second;

  // check for unauthorized (== unknown) values in conf file section.
  for (src_it = conf->reg_.begin(); src_it != conf->reg_.end(); ++src_it)
    {
      const std::string& key = src_it->first;
      if (def_ent.find(key) == def_ent.end())
        WARN("key '%1' in section '%2' is not allowed", key, section);
    }

  // set default entries
  for (src_it = def_ent.begin(); src_it != def_ent.end(); ++src_it)
    {
      const std::string& key = src_it->first;
      if (conf->reg_.find(key) == conf->reg_.end())
        conf->reg_[key] = src_it->second;
    }

  // KLUDGE: make it easier to retrieve used [client] section
  if (section.substr(0, 6) == std::string("client"))
    section_list_["client"] = conf;

  return conf;
}

void ConfFile::addAlias(const std::string& orig, const std::string& link)
{
  alias_[orig] = link;
}

ConfSection* ConfFile::getSection(const std::string& section)
{
  SectionList::iterator it;

  it = section_list_.find(section);
  if (it == section_list_.end())
    THROW(ConfException, "section '" << section << "' not found");
  return it->second;
}

void ConfFile::printHelpOption(const struct ConfCmdLineOpt opt[])
{
  const struct ConfCmdLineOpt* o = opt;

  while (o->long_opt)
    {
      std::cout << "  ";
      if (o->short_opt)
        std::cout << "-" << (char)(o->short_opt);
      else
        std::cout << "  ";
      std::cout << " --" << o->long_opt;

      // print help
      int opt_len = strlen(o->long_opt) + 7;
      int space_len = 25 - opt_len;
      if (space_len >= 0)
        {
          std::string space(space_len, ' ');
          std::cout << space;
        }
      else
        {
          std::string space(25, ' ');
          std::cout << std::endl << space;
        }
      std::cout << " " << o->help;
      std::cout << std::endl;
      ++o;
    }
}
