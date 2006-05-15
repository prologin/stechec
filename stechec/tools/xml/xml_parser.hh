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

#ifndef XML_PARSER_HH_
# define XML_PARSER_HH_

# include <string>
# include <deque>
# include <xercesc/dom/DOM.hpp>
# include <xercesc/dom/DOMDocument.hpp>
# include <xercesc/parsers/XercesDOMParser.hpp>

namespace xml
{

  class XMLParser
  {
  public:
    XMLParser();
    ~XMLParser();

    void parseDocument(const std::string& filename,
                       const std::string& expected_root);

    void setCurrentNode(const std::string& node_name);
    bool setCurrentIndexNode(const std::string& node_name, int number);

    std::string getText(const std::string& node_name);
    std::string getAttr(const std::string& node_name,
                        const std::string& attr_name);

    bool getFormationAttr(const std::string& id,
                          const std::string& player_id,
                          std::string& row,
                          std::string& col);

    xercesc::DOMElement* findNode(const std::string& node_name);

    // For XMLWriter.
    xercesc::DOMElement* getCurrentNode();
    xercesc::XercesDOMParser* getXercesParser();

  private:
    xercesc::XercesDOMParser parser_;
    xercesc::DOMElement* root_;
    xercesc::DOMElement* curr_node_;

    // Keep returned value from str2xml some time, then trash it.
    // Otherwise, it will leak.
    XMLCh* str2xml(const std::string& str);
    std::deque<XMLCh*> conv_val_;
  };
}

#endif /* !XML_PARSER_HH_ */
