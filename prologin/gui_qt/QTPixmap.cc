/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2005, 2006 Prologin
*/

#include <fstream>

#include "tools.hh"
#include "QTPixmap.hh"

QTPixmap::QTPixmap()
  : pixname_("ERROR_NOT_LOADED"),
    data_(NULL)
{
}

bool QTPixmap::load(const std::string& path, const std::string& pixname)
{
  std::string filepath = path + "/" + pixname;
  std::ifstream info;
  int n = 1;

  info.open((filepath + "/info").c_str());
  if (info)
    {
      info >> n;
      data_ = new QCanvasPixmapArray(filepath + "/%1.png", n);
      info.close();
    }
  else
    data_ = new QCanvasPixmapArray(filepath + ".png");

  if (!data_->isValid())
    {
      ERR("Loading pixmap '" << filepath << ".png' failed");
      delete data_;
      data_ = NULL;
      return false;
    }
  return true;
}

QTPixmap::~QTPixmap()
{
  delete data_;
}
