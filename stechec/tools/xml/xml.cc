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
#include "xml/xml_internal.hh"
#include "xml/xml.hh"

BEGIN_NS(xml);

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
  : xml_internal_(NULL),
    append_node_(0)
{
  if (inst_count_++ == 0)
    xercesc::XMLPlatformUtils::Initialize();
}

XML::~XML()
{
  delete xml_internal_;
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

  xml_internal_ = new XMLInternal;
  try {
    xml_internal_->parseDocument(filename, getExpectedRoot());
  } catch (const XMLError&) {
    delete xml_internal_;
    xml_internal_ = NULL;
    throw;
  }
}

void XML::save()
{
  if (xml_internal_ == NULL)
    return;

  LOG2("Saving xml to file `%1'", filename_);
  xml_internal_->save(filename_);
}

std::ostream& operator<< (std::ostream& os, const XML& xml)
{
  xml.xml_internal_->print(os);
  return os;
}

void XML::setNodeAppend(bool enabled)
{
  if (enabled)
    append_node_ = 0;
  else
    append_node_ = -1;
}

/*
** XML::getData, XML::getAttr specializations
*/
  
template <>
int XML::getData(const std::string& node_name, int index) const
{
  int ret;
  std::istringstream is(xml_internal_->getText(node_name, index));
  is >> ret;
  return ret;
}

template <>
bool XML::getData(const std::string& node_name, int index) const
{
  std::string val = xml_internal_->getText(node_name, index);
  if (val == "true" || val == "t" || val == "yes" || val == "on" || val == "1")
    return true;
  return false;
}

template <>
std::string XML::getData(const std::string& node_name, int index) const
{
  return xml_internal_->getText(node_name, index);
}

template <>
int XML::getAttr(const std::string& node_name,
		 const std::string& attr_name,
		 int index) const
{
  int ret;
  std::istringstream is(xml_internal_->getAttr(node_name, attr_name, index));
  is >> ret;
  return ret;
}

template <>
bool XML::getAttr(const std::string& node_name,
		  const std::string& attr_name,
		  int index) const
{
  std::string val = xml_internal_->getAttr(node_name, attr_name, index);
  if (val == "true" || val == "t" || val == "yes" || val == "on" || val == "1")
    return true;
  return false;
}

template <>
std::string XML::getAttr(const std::string& node_name,
			 const std::string& attr_name,
			 int index) const
{
  return xml_internal_->getAttr(node_name, attr_name, index);
}


  
/*
** XML::setData, XML::setAttr specializations
*/

template <>
void XML::setData(const std::string& node_name,
		  const std::string& value)
{
  xercesc::DOMElement* elt = xml_internal_->findNode(node_name, append_node_);
  xml_internal_->setData(elt, node_name, value);
}

template <>
void XML::setData(const std::string& node_name,
		  const int& value)
{
  std::ostringstream os;
  os << value;
  xercesc::DOMElement* elt = xml_internal_->findNode(node_name, append_node_);
  xml_internal_->setData(elt, node_name, os.str());
}

template <>
void XML::setData(const std::string& node_name,
		  const bool& value)
{
  std::string val("false");
  if (value)
    val = "true";
  xercesc::DOMElement* elt = xml_internal_->findNode(node_name, append_node_);
  xml_internal_->setData(elt, node_name, val);
}

template<>
void XML::setAttr(const std::string& node_name,
		  const std::string& attr_name,
		  const std::string& value)
{
  xercesc::DOMElement* elt = xml_internal_->findNode(node_name, 0);
  xml_internal_->setAttr(elt, node_name, attr_name, value);
}

template<>
void XML::setAttr(const std::string& node_name,
		  const std::string& attr_name,
		  const int& value)
{
  std::ostringstream os;
  os << value;
  xercesc::DOMElement* elt = xml_internal_->findNode(node_name, 0);
  xml_internal_->setAttr(elt, node_name, attr_name, os.str());
}

template<>
void XML::setAttr(const std::string& node_name,
		  const std::string& attr_name,
		  const bool& value)
{
  std::string val("false");
  if (value)
    val = "true";
  xercesc::DOMElement* elt = xml_internal_->findNode(node_name, 0);
  xml_internal_->setAttr(elt, node_name, attr_name, val);
}

END_NS(xml)
