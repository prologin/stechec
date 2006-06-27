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

#include <xercesc/dom/DOMWriter.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/framework/MemBufFormatTarget.hpp>
#include <xercesc/dom/DOMDocument.hpp>

#include "tools.hh"
#include "xml/strings.hh"
#include "xml/xml.hh"
#include "xml/xml_writer.hh"

namespace xml
{
  XERCES_CPP_NAMESPACE_USE

  XMLWriter::XMLWriter()
  {
  }

  XMLWriter::~XMLWriter()
  {
    std::for_each(conv_val_.begin(), conv_val_.end(), ArrayDeleter());
  }

  XMLCh* XMLWriter::str2xml(const std::string& str)
  {
    XMLCh* val = xercesc::XMLString::transcode(str.c_str());
    conv_val_.push_back(val);
    if (conv_val_.size() > 30)
      {
        std::for_each(conv_val_.begin(), conv_val_.begin() + 20, ArrayDeleter());
        conv_val_.erase(conv_val_.begin(), conv_val_.begin() + 20);
      }
    return val;
  }

  void XMLWriter::setCurrentNode(xercesc::DOMElement* elt)
  {
    curr_node_ = elt;
  }

  void XMLWriter::setXercesParser(xercesc::XercesDOMParser* parser)
  {
    parser_ = parser;
  }
  
  void XMLWriter::setData(xercesc::DOMElement* elt,
                          const std::string& node_name,
                          const std::string& value)
  {
    assert(parser_);
    if (!elt)
      {
        elt = parser_->getDocument()->createElement(str2xml(node_name));
        curr_node_->appendChild(elt);
      }
    DOMNode* n = parser_->getDocument()->createTextNode(str2xml(value));
    elt->appendChild(n);
  }
  
  void XMLWriter::setAttr(xercesc::DOMElement* elt,
                          const std::string& node_name,
                          const std::string& attr_name,
                          const std::string& value)
  {
    assert(parser_);
    if (!elt)
      {
        elt = parser_->getDocument()->createElement(str2xml(node_name));
        curr_node_->appendChild(elt);
      }
    elt->setAttribute(str2xml(attr_name), str2xml(value));
  }

  void XMLWriter::dump(xercesc::XMLFormatTarget* outfile)
  {
    DOMWriter* writer = NULL;
    assert(parser_);

    try {
          xercesc::DOMDocument* xmlDoc = parser_->getDocument();
      DOMImplementation* impl = xmlDoc->getImplementation();
      writer = impl->createDOMWriter();

      // add spacing and such for human-readable output
      if (writer->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true))
        writer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint , true);
      
      xmlDoc->normalizeDocument() ;
      writer->writeNode(outfile , *xmlDoc);
        } catch (const XMLException& e) {
      delete writer;
      PRINT_AND_THROW(XMLError, "XML Exception message: " << e.getMessage());
    }
    delete writer;
  }

  void XMLWriter::save(const std::string& filename)
  {
    LocalFileFormatTarget* outf = NULL;
    try {
      outf = new LocalFileFormatTarget(str2xml(filename));
      dump(outf);
    } catch (const XMLError&) {
      delete outf;
      throw;
    }
    delete outf;
  }

  void XMLWriter::print(std::ostream& os)
  {
    MemBufFormatTarget* memb = NULL;
    try {
      memb = new MemBufFormatTarget();
      dump(memb);
    } catch (const XMLError&) {
      delete memb;
      throw;
    }
    os << memb->getRawBuffer();
    delete memb;
  }
  
} // !namespace xml
