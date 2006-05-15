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

#include "GLWorld.hh"

GLWorld::GLWorld(Gui* gui, QWidget* parent)
  : QGLWidget(parent),
    World(gui)
{
}

GLWorld::~GLWorld()
{
}

Object2D* GLWorld::createObject()
{
  return NULL;
}

void GLWorld::move(int width, int height)
{
}

void GLWorld::resize(int width, int height)
{
}

void GLWorld::zoom()
{

}
