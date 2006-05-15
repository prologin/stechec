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

#ifndef MAIN_WINDOW_HH_
# define MAIN_WINDOW_HH_

# include <qaction.h>
# include <qmainwindow.h>

class World;
class Gui;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(Gui* gui);
  ~MainWindow();

  template <typename T>
  void          addWorld(T* world);
  
  World*        getWorld();
  QAction*      action(std::string name);

private:
  Gui*          gui_;
  World*        world_;

  typedef std::map<std::string, QAction*> ActionList;
  ActionList    action_map_;

private slots:
  void          switch_world_();
  void          open_file_();
  void          contents_();
  void          about_();

private:
  bool          world_state_;
};

# include "Gui.hh"

template <typename T>
inline void MainWindow::addWorld(T* world)
{
  world_ = world;

  setCentralWidget(world);
  connect(action("Zoom In"), SIGNAL(activated()), world, SLOT(slotZoomIn()));
  connect(action("Zoom Out"), SIGNAL(activated()), world, SLOT(slotZoomOut()));

  world->show();
}


#endif // !MAIN_WINDOW_HH_
