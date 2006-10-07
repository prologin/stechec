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

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOM.hpp>

#include <sys/types.h>
#include <sys/stat.h>

#include "tools.hh"
#include "xml/xml_parser.hh"
#include "xml/xml_writer.hh"
#include "xml/xml.hh"

namespace xml
{

  /*
  ** XMLError
  */

  XMLError::XMLError(const std::string& reason)
    : Exception(reason)
  {
  }


  /*
  ** XML
  */

  int XML::inst_count_ = 0;

  XML::XML()
    : xml_parser_(NULL),
      xml_writer_(NULL)
  {
    if (inst_count_++ == 0)
      xercesc::XMLPlatformUtils::Initialize();
  }

  XML::~XML()
  {
    delete xml_parser_;
    delete xml_writer_;
    if (--inst_count_ == 0)
      xercesc::XMLPlatformUtils::Terminate();
  }

  void XML::parse(const std::string& filename)
  {
    LOG2("Parsing xml file '%1`", filename);
    filename_ = filename;

    // Check if the file exists before doing anything. Otherwise, the SAX
    // exception is everything but not helpful.
    struct stat st;
    if (stat(filename.c_str(), &st) < 0)
      {
        // FIXME: not thread-safe, but strerror_r gives strange results...
        PRINT_AND_THROW(XMLError, filename << ": " << strerror(errno));
      }

    xml_parser_ = new XMLParser;
    try {
      xml_parser_->parseDocument(filename, getExpectedRoot());
    } catch (const XMLError&) {
      delete xml_parser_;
      xml_parser_ = NULL;
      throw;
    }
    xml_writer_ = new XMLWriter;
    xml_writer_->setXercesParser(xml_parser_->getXercesParser());
  }

  void XML::save()
  {
    LOG2("Saving xml to file '%1`", filename_);

    xml_writer_->save(filename_);
  }

  std::ostream& operator<< (std::ostream& os, const XML& xml)
  {
    xml.xml_writer_->print(os);
    return os;
  }

  
  /*
  ** XML::getData, XML::getAttr specializations
  */
  
  template <>
  int XML::getData(const std::string& node_name) const
  {
    int ret;
    std::istringstream is(xml_parser_->getText(node_name));
    is >> ret;
    return ret;
  }

  template <>
  bool XML::getData(const std::string& node_name) const
  {
    std::string val = xml_parser_->getText(node_name);
    if (val == "true" || val == "t" || val == "yes" || val == "on" || val == "1")
      return true;
    return false;
  }

  template <>
  std::string XML::getData(const std::string& node_name) const
  {
    return xml_parser_->getText(node_name);
  }

  template <>
  int XML::getAttr(const std::string& node_name,
                              const std::string& attr_name) const
  {
    int ret;
    std::istringstream is(xml_parser_->getAttr(node_name, attr_name));
    is >> ret;
    return ret;
  }

  template <>
  bool XML::getAttr(const std::string& node_name,
                    const std::string& attr_name) const
  {
    std::string val = xml_parser_->getAttr(node_name, attr_name);
    if (val == "true" || val == "t" || val == "yes" || val == "on" || val == "1")
      return true;
    return false;
  }

  template <>
  std::string XML::getAttr(const std::string& node_name,
                           const std::string& attr_name) const
  {
    return xml_parser_->getAttr(node_name, attr_name);
  }


  
  /*
  ** XML::setData, XML::setAttr specializations
  */

  template <>
  void XML::setData(const std::string& node_name,
                    const std::string& value)
  {
    xercesc::DOMElement* elt = xml_parser_->findNode(node_name);
    xml_writer_->setData(elt, node_name, value);
  }

  template <>
  void XML::setData(const std::string& node_name,
                    const int& value)
  {
    std::ostringstream os;
    os << value;
    xercesc::DOMElement* elt = xml_parser_->findNode(node_name);
    xml_writer_->setData(elt, node_name, os.str());
  }

  template <>
  void XML::setData(const std::string& node_name,
                    const bool& value)
  {
    std::string val("false");
    if (value)
      val = "true";
    xercesc::DOMElement* elt = xml_parser_->findNode(node_name);
    xml_writer_->setData(elt, node_name, val);
  }

  template<>
  void XML::setAttr(const std::string& node_name,
                    const std::string& attr_name,
                    const std::string& value)
  {
    xercesc::DOMElement* elt = xml_parser_->findNode(node_name);
    xml_writer_->setAttr(elt, node_name, attr_name, value);
  }

  template<>
  void XML::setAttr(const std::string& node_name,
                    const std::string& attr_name,
                    const int& value)
  {
    std::ostringstream os;
    os << value;
    xercesc::DOMElement* elt = xml_parser_->findNode(node_name);
    xml_writer_->setAttr(elt, node_name, attr_name, os.str());
  }

  template<>
  void XML::setAttr(const std::string& node_name,
                    const std::string& attr_name,
                    const bool& value)
  {
    std::string val("false");
    if (value)
      val = "true";
    xercesc::DOMElement* elt = xml_parser_->findNode(node_name);
    xml_writer_->setAttr(elt, node_name, attr_name, val);
  }



  
  /*
  ** XMLTeam
  */

  void XMLTeam::switchToTeamGlobals()
  {
    xml_parser_->setCurrentNode("team");
    xml_writer_->setCurrentNode(xml_parser_->getCurrentNode());
  }

  bool XMLTeam::switchToPlayer(int index)
  {
    return xml_parser_->setCurrentIndexNode("player", index);
  }

  const char* XMLTeam::getExpectedRoot() const
  {
    return "team";
  }


  /*
  ** XMLFormation
  */

  XMLFormation::XMLFormation()
    : formation_id_(-1)
  {
  }

  bool XMLFormation::switchFormationId(int formation_id)
  {
    formation_id_ = formation_id;
    return true;
  }

  Position XMLFormation::getPos(int player_id)
  {
    std::string srow, scol;
    std::ostringstream sid, splayer_id;
    sid << formation_id_;
    splayer_id << player_id;
    if (!xml_parser_->getFormationAttr(sid.str(), splayer_id.str(), srow, scol))
      {
        PRINT_AND_THROW(XMLError, "Can't get position for player `"
                        << player_id << "' in xml file `" << filename_ << "'.");
      }
    std::istringstream isrow(srow), iscol(scol);
    Position pos;
    isrow >> pos.row;
    iscol >> pos.col;
    return pos;
  }

  void XMLFormation::parse(const std::string& filename)
  {
    XML::parse(filename);
    xml_parser_->setCurrentNode("formation");
    xml_writer_->setCurrentNode(xml_parser_->getCurrentNode());
  }

  const char* XMLFormation::getExpectedRoot() const
  {
    return "formations";
  }


  /*
  ** XMLconfig
  */

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

  void XMLConfig::switchSection(const char* sec) const
  {
    xml_parser_->setCurrentNode(sec);
    xml_writer_->setCurrentNode(xml_parser_->getCurrentNode());
  }

  void XMLConfig::switchClientSection(int client_gid) const
  {
    if (client_gid == -1)
      client_gid = client_section_;
    else
      client_section_ = client_gid;
      
    std::ostringstream os;
    os << "client_" << client_gid;
    xml_parser_->setCurrentNode(os.str());
    xml_writer_->setCurrentNode(xml_parser_->getCurrentNode());
  }

  const char* XMLConfig::getExpectedRoot() const
  {
    return "config";
  }

} // !namespace xml
