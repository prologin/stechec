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

#ifndef GLWORLD_HH_
# define GLWORLD_HH_

# include <qgl.h>
# include <qwidget.h>

# include "Object2D.hh"
# include "World.hh"

class GLWorld : public QGLWidget, public World
{
  Q_OBJECT

public:
  GLWorld(Gui* gui, QWidget* parent);
  ~GLWorld();

  virtual Object2D*     createObject();
  virtual void          move(int width, int height);
  virtual void          resize(int width, int height);

private:

  virtual void          zoom();
};


#endif // !GLWORLD_HH_
