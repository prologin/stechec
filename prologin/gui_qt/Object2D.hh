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

#ifndef OBJECT2D_HH
# define OBJECT2D_HH

# include <qcanvas.h>
# include <qobject.h>

class Object2D : public QObject
{
  Q_OBJECT

public:
  Object2D() {}
  ~Object2D() {}

  virtual void move(int x, int y) = 0;
  virtual void jump(int x, int y) = 0;
  virtual void show(bool state = true) = 0;
  virtual void setZ(int z) = 0;
  virtual void setAnim(const std::string& pixname, bool loop = 0, int ms = 0) = 0;
  virtual void setKilled() = 0;
  virtual void setKillAnim(const std::string& pixname, int ms = 0) = 0;
  virtual bool isDead() const = 0;
};

#endif // !OBJECT2D_HH
