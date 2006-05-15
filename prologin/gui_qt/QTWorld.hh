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

#ifndef QTWORLD_HH_
# define QTWORLD_HH_

# include <qgl.h>
# include <qwidget.h>
# include <qcanvas.h>

# include "QTPixmap.hh"
# include "World.hh"

class Gui;

/*!
** Display the world using QT sprites.
** Not really fast nor beautiful... but, eh, it is working !
*/
class QTWorld : public QCanvasView, public World
{
  Q_OBJECT

public:
  QTWorld(Gui* gui, QWidget* parent);
  ~QTWorld();

  Object2D*     createObject();
  void          move(int width, int height);
  void          resize(int width, int height);

  const QTPixmap* getPixmap(const std::string& pixname);

private slots:
  void          slotZoomIn();
  void          slotZoomOut();

private:
  void          zoom();

  typedef std::map<std::string, const QTPixmap*> PixmapMap;
  PixmapMap     pixmap_list_;

  int max_x_;
  int max_y_;
  int x_;
  int y_;
  int scale_;

};

#endif // !QTWORLD_HH_
