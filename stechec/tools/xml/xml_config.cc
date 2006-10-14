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

#include <sys/types.h>
#include <sys/stat.h>

#include "tools.hh"
#include "xml/xml_config.hh"
#include "xml/xml_internal.hh"

BEGIN_NS(xml);

XMLConfig::XMLConfig()
  : client_section_("0")
{
}

void XMLConfig::parse(const std::string& filename)
{
  struct stat st;
  char resolved_path[PATH_MAX + 1];
  std::string fn;

  // First see if the user supplied file exists.
  if (filename != "")
    {
      LOG4("Looking for %1 ...", filename);
      if (stat(filename.c_str(), &st) >= 0)
	{
	  realpath(filename.c_str(), resolved_path);
	  fn = resolved_path;
	  goto parse;
	}
    }

  // Search for "tbtrc" in the current directory
  fn = "tbtrc";
  LOG4("Looking for %1 ...", fn);
  if (fn != filename && stat(fn.c_str(), &st) >= 0)
    {
      realpath(fn.c_str(), resolved_path);
      fn = resolved_path;
      goto parse;
    }

  // Search in the user home directory for ~/.tbtrc
  fn = std::string(getenv("HOME")) + "/.tbtrc";
  LOG4("Looking for %1 ...", fn );
  if (stat(fn.c_str(), &st) >= 0)
    {
      realpath(fn.c_str(), resolved_path);
      fn = resolved_path;
      goto parse;
    }

  // Hu. Sorry. Prepare to catch XMLError.
  // FIXME: should provide a default one.
  fn = filename;

 parse:
  XML::parse(fn);
}

void XMLConfig::switchClientSection(int client_gid) const
{
  std::ostringstream os;
  os << client_gid;
  client_section_ = os.str();
}

bool XMLConfig::switchToSection(const std::string& section_name,
				bool first_time) const
{
  if (section_name == "client")
    {
      if (first_time)
	{
	  // try specific (or generic if specific was not set) section.
	  xml_internal_->setCurrentNodeByAttr("client", "id", client_section_);
	  return true;
	}
      if (client_section_ != "0")
	{
	  // try generic section
	  xml_internal_->setCurrentNodeByAttr("client", "id", "0");
	  return true;
	}
      return false; // already tried.
    }

  if (!first_time)
    return false;
  xml_internal_->setCurrentNode(section_name);
  return true;
}

const char* XMLConfig::getExpectedRoot() const
{
  return "config";
}

END_NS(xml);
