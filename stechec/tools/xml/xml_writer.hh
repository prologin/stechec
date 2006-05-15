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

#ifndef XML_WRITER_HH_
# define XML_WRITER_HH_

# include <xercesc/dom/DOM.hpp>
# include <xercesc/parsers/XercesDOMParser.hpp>

namespace xml
{  
  class XMLWriter
  {
  public:
    XMLWriter();
    ~XMLWriter();
 
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

  private:
    xercesc::DOMElement* curr_node_;
    xercesc::XercesDOMParser* parser_;

    // Keep returned value from str2xml some time, then trash it.
    // Otherwise, it will leak.
    // FIXME: merge with xml_parser.
    XMLCh* str2xml(const std::string& str);
    std::deque<XMLCh*> conv_val_;
  };

} // !namespace xml

#endif /* !XML_WRITER_HH_ */
