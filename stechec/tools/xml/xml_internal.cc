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
#include <xercesc/dom/DOMWriter.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/framework/MemBufFormatTarget.hpp>

#include "tools.hh"
#include "xml/strings.hh"
#include "xml/xml.hh"
#include "xml/xml_internal.hh"

BEGIN_NS(xml);

XERCES_CPP_NAMESPACE_USE

/*!
** @brief Really throw something on error.
**
** Since HandlerBase ignore error, throw it ourself.
*/
class ValidationHandler : public HandlerBase
{
public:
  ValidationHandler() {}
  virtual ~ValidationHandler() {}
  
  virtual void error(const SAXParseException &exc)
  {
    throw exc;
  };
  virtual void fatalError(const SAXParseException &exc)
  {
    throw exc;
  };
};

  
XMLInternal::XMLInternal()
  : curr_node_(NULL)
{
  parser_ = new XercesDOMParser();

  parser_->setValidationScheme(xercesc::XercesDOMParser::Val_Auto);
  parser_->setDoNamespaces(true);
  parser_->setDoSchema(true);
  parser_->setValidationConstraintFatal(false);
  parser_->setExitOnFirstFatalError(true);
  parser_->setLoadExternalDTD(true);
  parser_->setDoValidation(true); // set this to false to skip dtd validation.
}

XMLInternal::~XMLInternal()
{
  std::for_each(conv_val_.begin(), conv_val_.end(), ArrayDeleter());
  delete parser_;
}

XMLCh* XMLInternal::str2xml(const std::string& str)
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


/*
** Parser
*/

void XMLInternal::parseDocument(const std::string& filename,
			      const std::string& expected_root)
{
  ErrorHandler* err_handler;
  xercesc::DOMDocument* doc;

  err_handler = new ValidationHandler();
  parser_->setErrorHandler(err_handler);

  try {
    parser_->parse(filename.c_str());
    doc = parser_->getDocument();
  }
  catch (const SAXParseException& e) {
    delete err_handler;
    PRINT_AND_THROW(XMLError, "l" << e.getLineNumber() << ",c"
		    << e.getColumnNumber() << ": " << e.getMessage());
  }
  catch (const XMLException& toCatch) {
    delete err_handler;
    PRINT_AND_THROW(XMLError, "XML Exception message: "
		    << toCatch.getMessage());
  }
  catch (const DOMException& toCatch) {
    delete err_handler;
    PRINT_AND_THROW(XMLError, "DOM Exception message: "
		    << toCatch.getMessage());
  }

  root_ = doc->getDocumentElement();
  delete err_handler;

  // Check if it is the document we really want.
  if (xml2str(root_->getNodeName()) != expected_root)
    {
      PRINT_AND_THROW(XMLError,
		      "A wrong xml document were given (have root node: '"
		      << root_->getNodeName() << "', expected root node: "
		      << expected_root << "')");
    }
}

// Set the current node to the part of the xml we whish to
// process later.
void XMLInternal::setCurrentNode(const std::string& node_name)
{
  if (xml2str(root_->getNodeName()) == node_name)
    {
      curr_node_ = root_;
      return;
    }
  DOMNodeList* nl = root_->getElementsByTagName(str2xml(node_name));
  if (!nl->getLength())
    THROW(XMLError, "Failed to catch `" << node_name <<"' in current document");

  if (nl->getLength() != 1)
    {
      // use setCurrentNodeByAttr instead.
      THROW(XMLError, "More than one occurence of `" << node_name
	    << "' in current document");
    }
    
  DOMNode* n = nl->item(0);
  curr_node_ = dynamic_cast<DOMElement*>(n);
}

bool XMLInternal::setCurrentNodeByAttr(const std::string& node_name,
				       const std::string& attr_name,
				       const std::string& attr_value)
{
  DOMNodeList* nl = root_->getElementsByTagName(str2xml(node_name));
  if (!nl->getLength())
    THROW(XMLError, "Failed to catch `" << node_name <<"' in current document");

  curr_node_ = NULL;
  const XMLCh* search_attr = str2xml(attr_name);
  for (unsigned i = 0; i < nl->getLength(); i++)
    if (nl->item(i)->getNodeType() == DOMNode::ELEMENT_NODE)
      {
	DOMElement* elt = dynamic_cast<DOMElement*>(nl->item(i));
	if (elt->hasAttribute(search_attr) &&
	    attr_value == xml2str(elt->getAttribute(search_attr)))
	  {
	    curr_node_ = elt;
	    return true;
	  }
      }

  // Failed. Not a fatal error, though. Be careful that curr_node_ is NULL now.
  return false;
}

// Find a specific DOMElement, starting from curr_node
// Return NULL if not found. (It's not a throw yet :).
DOMElement* XMLInternal::findNode(const std::string& node_name, int index)
{
  if (curr_node_ == NULL)
    THROW(XMLError, "You forgot to set the current node. Abort.");

  if (xml2str(curr_node_->getNodeName()) == node_name)
    return curr_node_;

  DOMNodeList* nl = curr_node_->getElementsByTagName(str2xml(node_name));
  if ((int)nl->getLength() < index)
    return NULL;
  return dynamic_cast<DOMElement*>(nl->item(index));
}

std::string XMLInternal::getText(const std::string& node_name, int index)
{
  DOMNode* n = findNode(node_name, index);
  if (n == NULL)
    {
      if (index == 0)
	THROW(XMLError, "Node `" << node_name << "' must exists.");
      else
	THROW(XMLError, "Node `" << node_name
	      << "' (number " << index << ") must exists.");
    }
  DOMNode* tn = n->getFirstChild();
  if (tn == NULL)
    THROW(XMLError, "Node `" << node_name << "' is not a text node. "
	  << "Seems like it is empty.");
  if (tn->getNodeType() != DOMNode::TEXT_NODE)
    THROW(XMLError, "Node `" << node_name << "' is not text. "
	  << "Seems it contains something else.");

  return xml2str(tn->getNodeValue());
}


std::string XMLInternal::getAttr(const std::string& node_name,
				 const std::string& attr_name,
				 int index)
{
  DOMElement* elt = findNode(node_name, index);
  if (elt == NULL)
    {
      if (index == 0)
	THROW(XMLError, "Node '" << node_name << "' must exists.");
      else
	THROW(XMLError, "Node '" << node_name
	      << "' (number " << index << ") must exists.");
    }
  if (!elt->hasAttribute(str2xml(attr_name)))
    THROW(XMLError, "Node '" << node_name
	  << "' doesn't have attribute '" << attr_name << "'");
  return xml2str(elt->getAttribute(str2xml(attr_name)));
}


// Formation.xml specific stuff.
bool XMLInternal::getFormationAttr(const std::string& id,
				 const std::string& player_id,
				 std::string& row,
				 std::string& col)
{
  DOMNode* tmp = curr_node_->getFirstChild();

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

DOMElement* XMLInternal::getCurrentNode()
{
  return curr_node_;
}

XercesDOMParser* XMLInternal::getXercesParser()
{
  return parser_;
}



/*
** Writer
*/

void XMLInternal::setData(xercesc::DOMElement* elt,
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
  
void XMLInternal::setAttr(xercesc::DOMElement* elt,
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

void XMLInternal::dump(xercesc::XMLFormatTarget* outfile)
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

void XMLInternal::save(const std::string& filename)
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

void XMLInternal::print(std::ostream& os)
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



END_NS(xml);
