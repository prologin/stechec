//=============================================================================
//
// GUI implementation with Glut
// Copyright (C) 2006  Julien Guertault
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
//=============================================================================
//
// API for Prologin 2006
//
// glutgui.cc : made by Zavie (Julien Guertault)
//
//=============================================================================

#include	<GL/glut.h>
#include	<iostream>
#include	"glutgui.hh"

//================================================================= Constructor

GlutGUI::GlutGUI (Api * api, ClientCx * ccx):
  GUI(api, ccx)
{}

GlutGUI::~GlutGUI ()
{}


// =================================================================== Instance

GlutGUI *	GlutGUI::_glut_instance = 0;

GlutGUI *	GlutGUI::Instance (Api * api, ClientCx * ccx)
{
  if (0 == _glut_instance)
    {
      if (0 == api || 0 == ccx)
	std::cerr << "Not fair NULL API T_T" << std::endl;
      else
	_glut_instance = new GlutGUI (api, ccx);
    }
  return _glut_instance;
}

GlutGUI *	GlutGUI::Instance ()
{
  return GlutGUI::Instance (0, 0);
}


void		GlutGUI::Destroy ()
{
  if (_glut_instance != 0)
    delete _glut_instance;
}


//============================================================== Initialization

int	GlutGUI::Init_GUI (int * argc,
			   char ** argv,
			   const unsigned int width,
			   const unsigned int height,
			   const char * title)
{
  _window_width = width;
  _window_height = height;

  // Render window setting
  glutInit (argc, argv);
  glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize (width, height);
  glutCreateWindow (title);

  // Callbacks settings
  glutIdleFunc (&GlutIdleFunc);
  glutDisplayFunc (&GlutDisplayFunc);
  glutReshapeFunc (&GlutReshapeFunc);
  glutKeyboardFunc (&GlutKeyboardFunc);
  glutSpecialFunc (&GlutSpecialFunc);
  glutMouseFunc (&GlutMouseFunc);
  glutPassiveMotionFunc (&GlutPassiveMotionFunc);
  glutMotionFunc (&GlutMotionFunc);
//   glutEntryFunc (EntryCallback);

  std::cerr << "Window created!" << std::endl;
  return 0;
}

void	GlutGUI::Run (const int delay, const int frame_delay)
{
  GUI::Run (delay, frame_delay);
  glutTimerFunc (delay, &GlutTimerFunc, delay);
  glutTimerFunc (frame_delay, &GlutFrameTimerFunc, frame_delay);
  glutMainLoop ();
}

//
// Called to tell the scene is ok (swap buffers)
//
void	GlutGUI::Redraw ()
{
  glutPostRedisplay ();
}

//
// Called to know the elapsed time
//
int	GlutGUI::Elapsed_time ()
{
  return glutGet (GLUT_ELAPSED_TIME);
}


//=================================================================== Callbacks

//
// Called at a specific frequency
//
void	GlutGUI::GlutTimerFunc (int delay)
{
  GlutGUI::Instance ()->TimerCallback ();
  glutTimerFunc (delay, &GlutTimerFunc, delay);
}

void	GlutGUI::GlutFrameTimerFunc (int delay)
{
  GlutGUI::Instance ()->FrameTimerCallback ();
  glutTimerFunc (delay, &GlutFrameTimerFunc, delay);
}

//
// Called when the application is idle
//
void	GlutGUI::GlutIdleFunc ()
{
  GlutGUI::Instance ()->IdleCallback ();
}

//
// Called when a redraw is requested
//
void	GlutGUI::GlutDisplayFunc ()
{
  GlutGUI::Instance ()->DisplayCallback ();
  glutSwapBuffers ();
}

//
// Called when the window is resized
//
void	GlutGUI::GlutReshapeFunc (int width, int height)
{
  GlutGUI::Instance ()->ResizeCallback (width, height);
}

//
// Called when a key is pressed
//
void	GlutGUI::GlutKeyboardFunc (unsigned char key, int, int)
{
  const int mod = glutGetModifiers ();
  GlutGUI::Instance ()->KeyboardCallback (key,
					  ((mod & GLUT_ACTIVE_SHIFT) != 0),
					  ((mod & GLUT_ACTIVE_CTRL) != 0),
					  ((mod & GLUT_ACTIVE_ALT) != 0));
}

//
// Called when an extended key is pressed
//
void	GlutGUI::GlutSpecialFunc (int key, int, int)
{
  const int mod = glutGetModifiers ();
  SpecialKey specialkey = no_key;
  switch (key)
    {
    case GLUT_KEY_F1:
      specialkey = f1;
      break;
    case GLUT_KEY_F2:
      specialkey = f2;
      break;
    case GLUT_KEY_F3:
      specialkey = f3;
      break;
    case GLUT_KEY_F4:
      specialkey = f4;
      break;
    case GLUT_KEY_F5:
      specialkey = f5;
      break;
    case GLUT_KEY_F6:
      specialkey = f6;
      break;
    case GLUT_KEY_F7:
      specialkey = f7;
      break;
    case GLUT_KEY_F8:
      specialkey = f8;
      break;
    case GLUT_KEY_F9:
      specialkey = f9;
      break;
    case GLUT_KEY_F10:
      specialkey = f10;
      break;
    case GLUT_KEY_F11:
      specialkey = f11;
      break;
    case GLUT_KEY_F12:
      specialkey = f12;
      break;
    }
  GlutGUI::Instance ()->SpecialKeyCallback (specialkey,
					    ((mod & GLUT_ACTIVE_SHIFT) != 0),
					    ((mod & GLUT_ACTIVE_CTRL) != 0),
					    ((mod & GLUT_ACTIVE_ALT) != 0));
}

//
// Called when a button is pressed
//
void	GlutGUI::GlutMouseFunc (int button, int state, int x, int y)
{
  const bool my_state = (GLUT_DOWN == state);
  const MouseButton my_button = (GLUT_LEFT_BUTTON == button ?
				 left_button :
				 (GLUT_MIDDLE_BUTTON == button ?
				  middle_button :
				  (GLUT_RIGHT_BUTTON == button ?
				   right_button :
				   other_button)));
  GlutGUI::Instance ()->MouseButtonCallback (my_button, my_state, x, y);
}

//
// Called when the mouse is moved
//
void	GlutGUI::GlutPassiveMotionFunc (int x, int y)
{
  GlutGUI::Instance ()->MouseMotionCallback (x, y);
}

//
// Called when the mouse is moved while a button is pressed
//
void	GlutGUI::GlutMotionFunc (int x, int y)
{
//   GlutGUI::Instance ()->MouseDragCallback (x, y);
  GlutGUI::Instance ()->MouseMotionCallback (x, y);
}

//=============================================================================
