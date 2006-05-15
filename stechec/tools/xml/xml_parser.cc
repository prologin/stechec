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

#include <xercesc/sax/HandlerBase.hpp>

#include "tools.hh"
#include "xml/strings.hh"
#include "xml/xml.hh"
#include "xml/xml_parser.hh"

namespace xml
{

  XERCES_CPP_NAMESPACE_USE

  XMLParser::XMLParser()
    : curr_node_(NULL)
  {
    parser_.setValidationScheme(xercesc::XercesDOMParser::Val_Never);
    parser_.setDoNamespaces(false);
    parser_.setDoSchema(false);
    parser_.setLoadExternalDTD(false);
  }

  XMLParser::~XMLParser()
  {
    std::for_each(conv_val_.begin(), conv_val_.end(), ArrayDeleter());
  }

  XMLCh* XMLParser::str2xml(const std::string& str)
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

  void XMLParser::parseDocument(const std::string& filename,
                                const std::string& expected_root)
  {
    ErrorHandler* err_handler;
        xercesc::DOMDocument* doc;

    err_handler = new HandlerBase();
    parser_.setErrorHandler(err_handler);

    try {
      parser_.parse(filename.c_str());
      doc = parser_.getDocument();
    }
    catch (const SAXParseException& e) {
      delete err_handler;
      ERR(e.getLineNumber() << "," << e.getColumnNumber() << ": "
          << e.getMessage());
      throw XMLError(Log::getLastMessage());
    }
    catch (const XMLException& toCatch) {
      delete err_handler;
      ERR("XML Exception message: " << toCatch.getMessage());
      throw XMLError(Log::getLastMessage());
    }
    catch (const DOMException& toCatch) {
      delete err_handler;
      ERR("DOM Exception message: " << toCatch.getMessage());
      throw XMLError(Log::getLastMessage());
    }

    root_ = doc->getDocumentElement();
    delete err_handler;

    // Check it is the document we really want.
    if (xml2str(root_->getNodeName()) != expected_root)
      {
        ERR("A wrong xml document were given (have root node: '"
            << root_->getNodeName() << "', expected root node: "
            << expected_root << "')");
        throw XMLError(Log::getLastMessage());
      }
  }

  // Set the current node to the part of the xml we whish to
  // process later.
  void XMLParser::setCurrentNode(const std::string& node_name)
  {
    if (xml2str(root_->getNodeName()) == node_name)
      {
        curr_node_ = root_;
        return;
      }
    DOMNodeList* nl = root_->getElementsByTagName(str2xml(node_name));
    if (!nl->getLength())
      {
        ERR("Failed to catch '"<< node_name <<"' in current document");
        throw XMLError(Log::getLastMessage());
      }

    // If there is more than one item... errr... pick the first,
    // and hope that specifics parsers will know what they are
    // doing (this is the case for XMLFormation).
    DOMNode* n = nl->item(0);
        curr_node_ = dynamic_cast<DOMElement*>(n);
  }

  bool XMLParser::setCurrentIndexNode(const std::string& node_name, int number)
  {
    DOMNodeList* nl = root_->getElementsByTagName(str2xml(node_name));
    if (!nl->getLength())
      {
        ERR("Failed to catch '"<< node_name <<"' in current document");
        throw XMLError(Log::getLastMessage());
      }

    const XMLCh* number_ch = str2xml("number");
    for (unsigned i = 0; i < nl->getLength(); i++)
      if (nl->item(i)->getNodeType() == DOMNode::ELEMENT_NODE)
        {
          DOMElement* elt = dynamic_cast<DOMElement*>(nl->item(i));
          if (elt->hasAttribute(number_ch))
          {
            int nb;
            std::istringstream is(xml2str(elt->getAttribute(number_ch)));
            is >> nb;
            if (nb == number)
              {
                curr_node_ = elt;
                return true;
              }
          }
      }
    return false;
  }

  // Find a specific DOMElement, starting from curr_node
  // Return NULL if not found. (It's not a throw yet :).
  DOMElement* XMLParser::findNode(const std::string& node_name)
  {
    if (curr_node_ == NULL)
      {
        ERR("You forgot to set the current node. Abort.");
        throw XMLError(Log::getLastMessage());
      }
    if (xml2str(curr_node_->getNodeName()) == node_name)
      return curr_node_;
    DOMNodeList* nl = curr_node_->getElementsByTagName(str2xml(node_name));
    if (nl->getLength() != 1)
      return NULL;
    return dynamic_cast<DOMElement*>(nl->item(0));
  }

  std::string XMLParser::getText(const std::string& node_name)
  {
    DOMNode* n = findNode(node_name);
    if (n == NULL)
      {
        ERR("Node '" << node_name << "' must exists, and must not be duplicated.");
        throw XMLError(Log::getLastMessage());
      }
    DOMNode* tn = n->getFirstChild();
    assert(tn != NULL && tn->getNodeType() == DOMNode::TEXT_NODE);
    return xml2str(tn->getNodeValue());
  }


  std::string XMLParser::getAttr(const std::string& node_name,
                                 const std::string& attr_name)
  {
    DOMElement* elt = findNode(node_name);
    if (elt == NULL)
      {
        ERR("Node '" << node_name << "' must exists, and must not be duplicated.");
        throw XMLError(Log::getLastMessage());
      }
    if (!elt->hasAttribute(str2xml(attr_name)))
      {
        ERR("Node '" << node_name << "' doesn't have attribute '" << attr_name << "'");
        throw XMLError(Log::getLastMessage());
      }
    return xml2str(elt->getAttribute(str2xml(attr_name)));
  }


  // Formation.xml specific stuff.
  bool XMLParser::getFormationAttr(const std::string& id,
                                   const std::string& player_id,
                                   std::string& row,
                                   std::string& col)
  {
    DOMNode* tmp = curr_node_;

    while (tmp != NULL)
      {
        if (tmp->getNodeType() == DOMNode::ELEMENT_NODE)
          {
            DOMElement* e = dynamic_cast<DOMElement*>(tmp);
            if (xml2str(e->getAttribute(str2xml("id"))) == id
                && xml2str(e->getAttribute(str2xml("n"))) == player_id)
              {
                if (e->hasAttribute(str2xml("row")))
                  row = xml2str(e->getAttribute(str2xml("row")));
                else
                  row = xml2str(e->getAttribute(str2xml("y")));
                if (e->hasAttribute(str2xml("col")))
                  col = xml2str(e->getAttribute(str2xml("col")));
                else
                  col = xml2str(e->getAttribute(str2xml("x")));
                return true;
              }
          }
        tmp = tmp->getNextSibling();
      }
    return false;
  }

  DOMElement* XMLParser::getCurrentNode()
  {
    return curr_node_;
  }

  XercesDOMParser* XMLParser::getXercesParser()
  {
    return &parser_;
  }

} // !namespace xml
