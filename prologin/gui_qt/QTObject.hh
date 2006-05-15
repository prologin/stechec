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

#ifndef QTOBJECT_HH
# define QTOBJECT_HH

# include <qcanvas.h>
# include "Object2D.hh"

class QTWorld;

class QTObject : public Object2D
{
  Q_OBJECT

public:
  QTObject(QTWorld* world);
  ~QTObject();

  virtual void move(int x, int y);
  virtual void jump(int x, int y);
  virtual void show(bool state = true);
  virtual void setZ(int z);
  virtual void setAnim(const std::string& pixname, bool loop = 0, int ms = 0);
  virtual void setKilled();
  virtual void setKillAnim(const std::string& pixname, int ms = 0);
  virtual bool isDead() const;

private:
  QTWorld*              world_;

  int                   x_;
  int                   y_;
  int                   x_goal_;
  int                   y_goal_;
  int                   speed_;
  bool                  dead_;

  QCanvasSprite*        sprite_;
  QCanvasPixmapArray*   origin_;

private slots:
  void                  slotBackToOrigin();
  void                  slotStopMoving();
  void                  slotSetKilled();
};


inline bool QTObject::isDead() const
{
  return dead_;
}

#endif // !QTOBJECT_HH
