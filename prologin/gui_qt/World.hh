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

#ifndef WORLD_HH_
# define WORLD_HH_

# include "MainWindow.hh"
# include "Object2D.hh"

class Gui;

class World
{
public:
  World(Gui* gui);
  virtual ~World();

  virtual Object2D* createObject() = 0;
  virtual void  move(int width, int height) = 0;
  virtual void  resize(int width, int height) = 0;

  int           getCaseLength() const;
  void          setCaseLength(int lenght);

protected:
  Gui*          gui_;

private:
  int           case_length_;
};

inline int World::getCaseLength() const
{
  return case_length_;
}


#endif // !WORLD_HH_
