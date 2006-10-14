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

#ifndef XML_CONFIG_HH_
# define XML_CONFIG_HH_

# include "xml/xml.hh"

BEGIN_NS(xml);

/*!
** @ingroup tools
** @brief XML parser/writer tailored for the configuration file.
**
** Here the xml structure used:
** @code
** <config>
**   <section1>
**     <node1 attr1="..." attr2="...">value 1</node1>
**     <node2 attr1="..." attr2="..." />
**   </section1>
**   <section2>
**     [...]
**   </section2>
** </config>
** @endcode
**
** Configuration options are organized into sections (like game, client, server, ...).
** Inside them, there are one or more node, for a specific configuration item.
**
** To retrieve them, first parse the config file, then use:
**    * getData<return type wanted>(section, node)
**    * getAttr<return type wanted>(section, node, attr)
**
** To modify or add new, use:
**    * setData(section, node, value)
**    * setAttr(section, node, attr, value)
**
** Then use @c save to dump configuration into a file.
**
** The section @c client is a little special. There is more than one client section,
** identified by attribute @c id. Each one with @c id > 0 identify a particular client
** configuration, so it is possible to use the same configuration file to launch multiple
** instance of stechec with different client configuration (in a single process, only
** one section is used at all). You only have to choose the correct client @c id at the
** process startup. Client section can be chosen using @c switchClientSection.
**
** There is also a special client section, whose @c id is 0. It defined configuration items
** for all client section, in case they were not found in there specific sections.
** (yes, this is some xml inheritance kind). This is used to factorize client configuration.
**
** To get client section value and attribute, be sure to call @c switchClientSection once
** in your program, then always use "client" as section:
** @code
** int val = cfg.getAttr<int>("client", "debug", "verbose");
** @endcode
**
** You can refer to @c config.dtd to see the exact file definition, and
** what section and node it can accept.
*/
class XMLConfig : public XML
{
public:
  XMLConfig();
  
  void switchClientSection(int client_gid) const;

  //! @brief Get a text value.
  //! @param section_name Section to fetch value from (client, server, game).
  //! @param node_name The node to get text from.
  //! @return Return the value of the desired type.
  template <typename T>
  T getData(const std::string& section_name,
	    const std::string& node_name) const;

  //! @brief Get an attribute value.
  //! @param section_name Section to fetch value from (client, server, game).
  //! @param node_name The node to get value from.
  //! @param attr_name The attribute name to get value from.
  template <typename T>
  T getAttr(const std::string& section_name,
	    const std::string& node_name,
	    const std::string& attr_name) const;

  //! @brief Replace text of node \c node_name by \c value. It creates a
  //! new node if \c node_name is not found.
  template <typename T>
  void setData(const std::string& section_name,
	       const std::string& node_name,
	       const T& value);

  //! @brief Replace an attribute of the node \c node_name by \c value. It
  //!   create a new attribute if \c node_name is not found.
  template <typename T>
  void setAttr(const std::string& section_name,
	       const std::string& node_name,
	       const std::string& attr_name,
	       const T& value);
  
  //! @brief Try to find config file in various place before parsing.
  virtual void parse(const std::string& filename);

private:
  virtual const char* getExpectedRoot() const;

  bool switchToSection(const std::string& section_name, bool first_time) const;
  mutable std::string client_section_;
};

template <typename T>
inline T XMLConfig::getData(const std::string& section_name,
			    const std::string& node_name) const
{
  switchToSection(section_name, true);
  try {
    return XML::getData<T>(node_name);
  } catch (const XMLError&) {
    if (!switchToSection(section_name, false))
      throw;
    return XML::getData<T>(node_name);
  }
}

template <typename T>
inline T XMLConfig::getAttr(const std::string& section_name,
			    const std::string& node_name,
			    const std::string& attr_name) const
{
  switchToSection(section_name, true);
  try {
    return XML::getAttr<T>(node_name, attr_name);
  } catch (const XMLError&) {
    if (!switchToSection(section_name, false))
      throw;
    return XML::getAttr<T>(node_name, attr_name);
  }
}

template <typename T>
inline void XMLConfig::setData(const std::string& section_name,
			       const std::string& node_name,
			       const T& value)
{
  switchToSection(section_name, true);
  XML::setData<T>(node_name, value);
}

template <typename T>
inline void XMLConfig::setAttr(const std::string& section_name,
			const std::string& node_name,
			const std::string& attr_name,
			const T& value)
{
  switchToSection(section_name, true);
  XML::setAttr<T>(node_name, attr_name, value);
}


END_NS(xml);

#endif /* !XML_CONFIG_HH_ */
