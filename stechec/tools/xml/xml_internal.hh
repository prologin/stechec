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

#ifndef XML_INTERNAL_HH_
# define XML_INTERNAL_HH_

# include <deque>
# include <xercesc/dom/DOM.hpp>
# include <xercesc/dom/DOMDocument.hpp>
# include <xercesc/parsers/XercesDOMParser.hpp>

BEGIN_NS(xml);

/*!
** @brief Our internal utilisation of Xerces-C++.
** @ingroup tools_xml
*/
class XMLInternal
{
public:
  XMLInternal();
  ~XMLInternal();

  /*
  ** Parser
  */
  void parseDocument(const std::string& filename,
		     const std::string& expected_root);

  void setCurrentNode(const std::string& node_name);
  bool setCurrentNodeByAttr(const std::string& node_name,
			    const std::string& attr_name,
			    const std::string& attr_value);

  std::string getText(const std::string& node_name,
		      int index);
  std::string getAttr(const std::string& node_name,
		      const std::string& attr_name,
		      int index);

  bool getFormationAttr(const std::string& id,
			const std::string& player_id,
			std::string& row,
			std::string& col);

  xercesc::DOMElement* findNode(const std::string& node_name,
				int index);

  /*
  ** Writer
  */
  void setCurrentNode(xercesc::DOMElement* elt);
  void setXercesParser(xercesc::XercesDOMParser* parser);

  void setData(xercesc::DOMElement* elt,
	       const std::string& node_name,
	       const std::string& value);
  
  void setAttr(xercesc::DOMElement* elt,
	       const std::string& node_name,
	       const std::string& attr_name,
	       const std::string& value);
  
  void dump(xercesc::XMLFormatTarget* outfile);
  void save(const std::string& filename);
  void print(std::ostream& os);

  xercesc::DOMElement*		getCurrentNode();
  xercesc::XercesDOMParser*	getXercesParser();
  
private:
  xercesc::XercesDOMParser*	parser_;
  xercesc::DOMElement*		root_;
  xercesc::DOMElement*		curr_node_;

  // Keep returned value from str2xml some time, then trash it.
  // Otherwise, it will leak.
  XMLCh*			str2xml(const std::string& str);
  std::deque<XMLCh*>		conv_val_;
};

END_NS(xml);

#endif /* !XML_INTERNAL_HH_ */
