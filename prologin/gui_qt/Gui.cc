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
#include <qstatusbar.h>
#include <qtimer.h>

#include "QTWorld.hh"
#include "Gui.hh"

static char* qt_app_argv[] = { "visio" };
static int qt_app_argc = 0;

Gui::Gui(ClientCx* ccx)
  : QApplication(qt_app_argc, qt_app_argv),
    main_window_(NULL),
    running_(false),
    world_(NULL),
    height_(-1),
    width_(-1),
    case_length_(-1),
    path_("PATH_ERROR"),
    ccx_(ccx)
{
}

Gui::~Gui()
{
}

void Gui::printStatusMessage(const std::string& msg)
{
  LOG3("Status bar msg: " << msg);
  if (main_window_ != NULL)
    main_window_->statusBar()->message(msg);
}

const std::string& Gui::getPath() const
{
  return path_;
}

MainWindow* Gui::getMainWindow()
{
  return main_window_;
}

void Gui::init()
{
  if (width_ < 0 || height_ < 0 || case_length_ < 0)
    {
      WARN("Please initialize some variables before initialize the gui.");
      return;
    }

  main_window_ = new MainWindow(this);
  QTWorld* qt_world = new QTWorld(this, main_window_);
  qt_world->setCaseLength(case_length_);
  main_window_->addWorld(qt_world);

  // set default world;
  world_ = qt_world;

  main_window_->action("Open File...")->setDisabled(true);
  main_window_->action("Step Backward")->setDisabled(true);
  main_window_->action("Go To...")->setDisabled(true);
  main_window_->action("Switch World")->setDisabled(true);

  main_window_->getWorld()->resize((width_ + 1) * case_length_,
                                    (height_ + 1) * case_length_);
  //main_window_->getWorld()->zoom();
}

void Gui::run()
{
  if (width_ < 0 || height_ < 0 || case_length_ < 0)
    {
      WARN("Please initialize some variables before running gui.");
      return;
    }
  
  QTimer* timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(slotPeriodic()));
  timer->start(150, 0);
  exec();
}



void Gui::slotRun()
{
  printStatusMessage("Action:: Run");

  slotStepForward();

  QTimer* timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(slotRunningStep()));
  timer->start(200, 1);

  running_ = true;
}

void Gui::slotStop()
{
  printStatusMessage("Action:: Stop");
  running_ = false;
}

void Gui::slotStepForward()
{
  printStatusMessage("Action:: Step Forward");

  // next turn
  ccx_->setReady();
  
  // FIXME: hide button if finish
  if (false)
    {
      running_ = false;
      main_window_->action("Step Forward")->setDisabled(true);
      main_window_->action("Run")->setDisabled(true);
      main_window_->action("Stop")->setDisabled(true);
    }
}

void Gui::slotStepBackward()
{
  printStatusMessage("Action:: Step Backward (not implemented)");
}

void Gui::slotGoTo()
{
  printStatusMessage("Action:: Go To... (not implemented)");
}


void Gui::slotRunningStep()
{
  if (running_)
    {
      slotStepForward();

      // FIXME: really must recreate timers ?
      QTimer* timer = new QTimer(this);
      connect(timer, SIGNAL(timeout()), this, SLOT(slotRunningStep()));
      timer->start(200, 1);
    }
}

void Gui::slotPeriodic()
{
  while (ccx_->process())
    ;
}
