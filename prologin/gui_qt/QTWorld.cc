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

#include <qwmatrix.h>

#include "QTObject.hh"
#include "QTWorld.hh"


QTWorld::QTWorld(Gui* gui, QWidget* parent)
  : QCanvasView(new QCanvas(0, "World Map Canvas"), parent, "World Map"),
    World(gui),
    max_x_(0),
    max_y_(0),
    x_(0),
    y_(0),
    scale_(4)
{
  canvas()->resize(1280, 1024);
  canvas()->setBackgroundColor(QColor(0, 0, 0));
  canvas()->setAdvancePeriod(50);
  zoom();

  show();
}

QTWorld::~QTWorld()
{
}

Object2D* QTWorld::createObject()
{
  return new QTObject(this);
}


void QTWorld::move(int x, int y)
{
  x_ = x;
  y_ = y;
}

void QTWorld::resize(int x, int y)
{
  canvas()->resize(x, y);
}

const QTPixmap* QTWorld::getPixmap(const std::string& pixname)
{
  if (pixname == "")
    return NULL;
  
  PixmapMap::const_iterator it;
  it = pixmap_list_.find(pixname);
  if (it != pixmap_list_.end())
    return it->second;

  // Not found ? Try to create a new one.
  QTPixmap* pix = new QTPixmap;
  if (!pix->load(gui_->getPath(), pixname))
    {
      // FIXME: write error on status bar
      return NULL;
    }

  pixmap_list_[pixname] = pix;
  return pix;
}

void QTWorld::slotZoomIn()
{
  if (scale_ > 1)
    scale_--;
  zoom();
}

void QTWorld::slotZoomOut()
{
  if (scale_ < 15)
    scale_++;
  zoom();
}

void QTWorld::zoom()
{
  QWMatrix matrix;
  matrix.scale(1.0 / (double) scale_ * 2.0, 1.0 / (double) scale_ * 2.0);
  setWorldMatrix(matrix);
}
