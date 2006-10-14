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

#ifndef XML_HH_
# define XML_HH_

BEGIN_NS(xml);

class XMLInternal;

//! @brief Class thrown on error.
class XMLError : public Exception
{
public:
  XMLError(const std::string& reason);
};

/*!
** @brief Application interface for XML document reading/writing.
**
** This class can manage a very small subset of XML domain.
** Indeed, this is all TBT requires, to keep the code using it as
** simple as possible.
**
** Always call @c parse before doing anything, not doing it will invoke
** strange things. Be prepared to catch XMLError, as it can be sent by
** approximatively all functions. Other xerces exceptions may be thrown,
** but consider it as a bug and report it instead of catching it.
**
** Do not instanciate this class directly, use XML*, which have more
** knowledge about the document you access.
**
** @see There are some examples of its usage in stechec/tools/xml_test.
**
** @author victor
** @date 21/01/2006
*/
class XML
{
public:
  //! @brief Parse an xml file.
  //! @param filename File to parse.
  //! @throw XMLError Thrown on any kind of error.
  virtual void parse(const std::string& filename);

  //! @brief Save the resulting xml file to the hard drive,
  //!        replacing the file previously saved.
  //! @throw XMLError Thrown on any kind of error.
  void save();

  //! @brief Dump current xml data into ostream. Useful for debbugging.
  friend std::ostream& operator<< (std::ostream& os, const XML& xml);

  //! @brief Append node, instead of replacing the first, when
  //!  using setData.
  void setNodeAppend(bool enabled);
  
  //! @brief Get a text value. (ex: @verbatim<roll>2</rool>@endverbatim, return "2").
  //! @param node_name The node to get text from. (from the exemple above, "roll").
  //! @return Return the value of the desired type. Currently, \c int, \c bool, and
  //    \c string are supported.
  template <typename T>
  T getData(const std::string& node_name, int index = 0) const;

  //! @brief Get an attribute value. (ex: @verbatim<roll val="2" />@endverbatim,
  //!    return "2").
  //! @param node_name The node to get value from. (from the exemple
  //!        above, "roll").
  //! @param attr_name The attribute name to get value from. (from the
  //!        example above, "val").
  template <typename T>
  T getAttr(const std::string& node_name,
	    const std::string& attr_name,
	    int index = 0) const;

  //! @brief Replace text of node \c node_name by \c value. It creates a
  //! new node if \c node_name is not found.
  template <typename T>
  void setData(const std::string& node_name,
	       const T& value);

  //! @brief Replace an attribute of the node \c node_name by \c value. It
  //!   create a new attribute if \c node_name is not found.
  template <typename T>
  void setAttr(const std::string& node_name,
	       const std::string& attr_name,
	       const T& value);

protected:
  XML();
  virtual ~XML();

  //! @brief Root node name we expect when loading the document.
  virtual const char* getExpectedRoot() const = 0;

  mutable XMLInternal* xml_internal_;

  std::string filename_;

private:
  // Do not allow copies of this object.
  XML(const XML&);
  XML& operator= (const XML&);

  static int inst_count_;
  mutable int append_node_;
};


END_NS(xml);

#endif /* !XML_HH_ */
