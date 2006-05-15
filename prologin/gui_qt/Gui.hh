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

#ifndef GUI_HH_
# define GUI_HH_

# include <qapplication.h>

# include "tools.hh"
# include "client_cx.hh"
# include "MainWindow.hh"
# include "Object2D.hh"
# include "World.hh"


/*!
** Main class. Created and hold by the specific QT class in
** rules.
**
** Manual:
**  - inherit from this class
**  - set height_, width_, case_length_, path_ to good values
**  - call init()
**  - prepare needed Object for your application
**  - call run()
*/
class Gui : public QApplication
{
  Q_OBJECT

public:
  Gui(ClientCx* ccx);
  ~Gui();

  //! @brief Print a message on status bar.
  void printStatusMessage(const std::string& msg);

  //! @brief Retrieve gfx data path.
  const std::string& getPath() const;

  //! @brief Retrieve the main QT widget (menu, status bar, ...).
  MainWindow* getMainWindow();
  
  //! @brief Initialize the system.
  void init();

  //! @brief Run the gui.
  //! @note This function return when the user quits the application.
  void run();

public slots:
  virtual void slotRun();
  virtual void slotStop();
  virtual void slotStepForward();
  virtual void slotStepBackward();
  virtual void slotGoTo();

protected slots:
  virtual void slotRunningStep();
  virtual void slotPeriodic();

private:
  MainWindow*   main_window_;
  bool          running_;

protected:
  World*        world_;

  int           height_;
  int           width_;
  int           case_length_;
  std::string   path_;

  ClientCx*     ccx_;           ///< Connection with the server.
};


#endif // !GUI_HH_
