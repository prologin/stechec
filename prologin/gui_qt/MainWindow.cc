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

#include <qaction.h>
#include <qcanvas.h>
#include <qdockwindow.h>
#include <qkeysequence.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qpushbutton.h>
#include <qstatusbar.h>
#include <qtoolbutton.h>

#include "MainWindow.hh"
#include "Gui.hh"
#include "World.hh"

MainWindow::MainWindow(Gui* gui)
  : QMainWindow(),
    gui_(gui),
    world_(NULL),
    world_state_(false)
{
  gui_->setMainWidget(this);

  QFont font;
  font.setPointSize(10);
  gui_->setFont(font, true);

  // Create File menu
  QPopupMenu* file_menu = new QPopupMenu(this);
  menuBar()->insertItem("&File", file_menu);

  QAction* open_file = new QAction(QPixmap(PIXMAP_BASE "/open_file.png"),
                                   "&Open File...",
				    CTRL + Key_F, this, "Open File...");
  connect(open_file, SIGNAL(activated()), this, SLOT(open_file_()));
  open_file->addTo(file_menu);

  file_menu->insertSeparator();

  QAction* quit = new QAction(QPixmap(PIXMAP_BASE "/quit.png"), "&Quit",
			       CTRL + Key_W, this, "Quit");
  connect(quit, SIGNAL(activated()), gui, SLOT(quit()));
  quit->addTo(file_menu);

  // Create Actions menu
  QPopupMenu* actions_menu = new QPopupMenu(this);
  menuBar()->insertItem("&Actions", actions_menu);

  QAction* run = new QAction(QPixmap(PIXMAP_BASE "/run.png"), "&Run",
			      CTRL + Key_R, this, "Run");
  connect(run, SIGNAL(activated()), gui, SLOT(slotRun()));
  run->addTo(actions_menu);

  QAction* stop = new QAction(QPixmap(PIXMAP_BASE "/stop.png"), "&Stop",
			      CTRL + Key_S, this, "Stop");
  connect(stop, SIGNAL(activated()), gui, SLOT(slotStop()));
  stop->addTo(actions_menu);

  actions_menu->insertSeparator();

  QAction* step_forward = new QAction(QPixmap(PIXMAP_BASE "/step_forward.png"),
                                      "Step &Forward",
				       CTRL + Key_N, this, "Step Forward");
  connect(step_forward, SIGNAL(activated()), gui, SLOT(slotStepForward()));
  step_forward->addTo(actions_menu);

  QAction* step_backward = new QAction(QPixmap(PIXMAP_BASE "/step_backward.png"),
                                       "Step &Backward",
					CTRL + Key_B, this, "Step Backward");
  connect(step_backward, SIGNAL(activated()), gui, SLOT(slotStepBackward()));
  step_backward->addTo(actions_menu);

  QAction* go_to = new QAction(QPixmap(PIXMAP_BASE "/go_to.png"),
                               "&Go To...",
				CTRL + Key_G, this, "Go To...");
  connect(go_to, SIGNAL(activated()), gui, SLOT(slotGoTo()));
  go_to->addTo(actions_menu);

  // Create View menu
  QPopupMenu* view_menu = new QPopupMenu(this);
  menuBar()->insertItem("&View", view_menu);

  QAction* zoom_in = new QAction(QPixmap(PIXMAP_BASE "/zoom_in.png"),
                                 "Zoom &In",
				  Key_Plus, this, "Zoom In");
  zoom_in->addTo(view_menu);

  QAction* zoom_out = new QAction(QPixmap(PIXMAP_BASE "/zoom_out.png"),
                                  "Zoom &Out",
				   Key_Minus, this, "Zoom Out");
  zoom_out->addTo(view_menu);

  view_menu->insertSeparator();

  QAction* switch_world = new QAction(QPixmap(PIXMAP_BASE "/ok.png"),
                                      "&Switch World",
				       CTRL + Key_V, this, "Switch World");
  connect(switch_world, SIGNAL(activated()), this, SLOT(switch_world_()));
  switch_world->addTo(view_menu);

  // Create Help menu
  QPopupMenu* help_menu = new QPopupMenu(this);
  menuBar()->insertItem("&Help", help_menu);

  QAction* contents = new QAction(QPixmap(PIXMAP_BASE "/contents.png"),
                                  "&Contents...",
				   Key_F1, this, "Contents...");
  connect(contents, SIGNAL(activated()), this, SLOT(contents_()));
  contents->addTo(help_menu);

  QAction* about = new QAction(QPixmap(PIXMAP_BASE "/about.png"),
                               "&About...",
				Key_F2, this, "About...");
  connect(about, SIGNAL(activated()), this, SLOT(about_()));
  about->addTo(help_menu);

  // Create the tool bar
  QToolBar* tool_bar = new QToolBar(this, "Control Panel");
  tool_bar->setLabel("Control Panel");

  open_file->addTo(tool_bar);
  action_map_[open_file->name()] = open_file;

  tool_bar->addSeparator();

  step_backward->addTo(tool_bar);
  action_map_[step_backward->name()] = step_backward;
  step_forward->addTo(tool_bar);
  action_map_[step_forward->name()] = step_forward;
  go_to->addTo(tool_bar);
  action_map_[go_to->name()] = go_to;

  tool_bar->addSeparator();

  run->addTo(tool_bar);
  action_map_[run->name()] = run;
  stop->addTo(tool_bar);
  action_map_[stop->name()] = stop;

  tool_bar->addSeparator();

  zoom_in->addTo(tool_bar);
  action_map_[zoom_in->name()] = zoom_in;
  zoom_out->addTo(tool_bar);
  action_map_[zoom_out->name()] = zoom_out;

  tool_bar->addSeparator();

  switch_world->addTo(tool_bar);
  action_map_[switch_world->name()] = switch_world;

  // Create the status bar
  font.setPointSize(9);
  statusBar()->setFont(font);

  // Ends up window creation
  setCaption("Visio");
  show();

  statusBar()->message("Ready");
}

MainWindow::~MainWindow()
{
}

World* MainWindow::getWorld()
{
  return world_;
}

QAction* MainWindow::action(std::string name)
{
  ActionList::iterator i = action_map_.find(name);

  if (i == action_map_.end())
    return NULL;
  return i->second;
}


void MainWindow::switch_world_()
{
  // FIXME: do multiple world
  if (world_state_)
    {
//       world_->hide();
//       world_->show();
    }
  else
    {
//       world_->hide();
//       world_->show();
    }

  world_state_ = !world_state_;
}

void MainWindow::open_file_()
{
  statusBar()->message("Action:: Open File...");
}

void MainWindow::contents_()
{
  statusBar()->message("Action:: Contents...");
}

void MainWindow::about_()
{
  statusBar()->message("Action:: About...");
}
