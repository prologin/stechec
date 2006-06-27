//=============================================================================
//
// GUI virtual class
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
// gui.cc : made by Zavie (Julien Guertault)
//
//=============================================================================

#include	<iostream>
#include	"datacenter.hh"
#include	"gui.hh"
#include	"texturemanager.hh"

// ================================================================ Constructor

GUI::GUI (Api * api, ClientCx * ccx):
  _api(api),
  _ccx(ccx),
  _window_width(0),
  _window_height(0),
  _left_button(false),
  _middle_button(false),
  _right_button(false),
  _pause(false),
//   _step(false),
  _render_delay(100),
  _frame_delay(500),
  _frame_start_time(0),
  _current_time(0),
  _pause_time(0),
  _pause_start_time(0),
  _rendering()
{}

GUI::~GUI ()
{}


//============================================================== Initialization

int	GUI::Init_files ()
{
  LOG1("GUI::Init_files");
  return (_rendering.Init_objects () ? 0 : 1);
}

int	GUI::Init_API ()
{
  LOG1("GUI::Init_API");
  if (_rendering.Init_API (_window_width, _window_height) != 0)
    return 2;
  return 0;
}

void	GUI::Run (const int render_delay, const int frame_delay)
{
  LOG1("GUI::Run");
  _render_delay = render_delay;
  _frame_delay = frame_delay;
//   std::cerr << "Running..." << std::endl;
}


// ================================================================== Callbacks

//
// Called at a specific frequency
//
void	GUI::RenderTimerCallback ()
{
  LOG1("GUI::RenderTimerCallback");

  if (!_pause)//  || _step)
    {
      const float state((_current_time - _frame_start_time - _pause_time) /
			(float)_render_delay);
      _rendering.Animation_update ((state > 1) ? 1 : state);
    }
  _rendering.Rendering_update ();

  Redraw ();
}

void	GUI::FrameTimerCallback ()
{
  LOG1("GUI::FrameTimerCallback");

  // We load the next frame
  _ccx->setReady ();
  DataCenter::Instance ()->New_frame (_api);

  if (!_pause)//  || _step)
    {
      _frame_start_time = Elapsed_time ();
      _pause_time = 0;
      DataCenter::Instance ()->Next_frame ();
    }
//   if (_step)
//     {
//       _pause_start_time = _current_time;
//       _pause = true;
//       _step = false;
//     }
}

//
// Called when the application is idle
//
void	GUI::IdleCallback ()
{
}

//
// Called when the window is resized
//
void	GUI::ResizeCallback (const int width, const int height)
{
  LOG1("GUI::ResizeCallback");
//   std::cerr << "ResizeCallback" << std::endl;

  _window_width = width;
  _window_height = height;
  _rendering.Resize (width, height);
  Redraw ();
}

//
// Called when a redraw is requested
//
void	GUI::DisplayCallback ()
{
  LOG1("GUI::DisplayCallback");
  _current_time = Elapsed_time ();
  if (_pause)//  && !_step)
    {
      _pause_time = _pause_time + (_current_time - _pause_start_time);
      _pause_start_time = _current_time;
    }
  const float state((_current_time - _frame_start_time - _pause_time) /
		    (float)_render_delay);
  _rendering.Render (_current_time, (state > 1) ? 1 : state);
}

//
// Called when a key is pressed
//
void	GUI::KeyboardCallback (const unsigned char key,
			       const bool /* shift */,
			       const bool /* ctrl */,
			       const bool /* alt */)
{
  LOG1("GUI::KeyboardCallback");
//   std::cerr << "KeyboardCallback" << std::endl;

  switch (key)
    {
    case 'q':
    case 'Q':
    case 27:
      // Tout détruire
      TextureManager::Destroy ();
      DataCenter::Destroy ();

      std::cerr << "Bye =)" << std::endl;
      exit (0);
      break;
    case 'p':
    case 'P':
    case ' ':
      _pause = !_pause;
      if (_pause)
	_pause_start_time = _current_time;
      break;
//     case 's':
//     case 'S':
//     case ' ':
//       _step = true;
//       break;
    }
  Redraw ();
}


//
// Called when an extended key is pressed
//
void	GUI::SpecialKeyCallback (const SpecialKey specialkey,
				 const bool /* shift */,
				 const bool /* ctrl */,
				 const bool /* alt */)
{
  LOG1("GUI::SpecialKeyCallback");
//   std::cerr << "SpecialKeyCallback" << std::endl;

  switch (specialkey)
    {
    case left_arrow:
      DataCenter::Instance ()->Previous_frame ();
      _pause_start_time = _current_time;
      _pause = true;
      break;
    case right_arrow:
      DataCenter::Instance ()->Next_frame ();
      break;
    case up_arrow:
      DataCenter::Instance ()->First_frame ();
      _pause_start_time = _current_time;
      _pause = true;
      break;
    case down_arrow:
      DataCenter::Instance ()->Last_frame ();
      break;
    case f1:
      _rendering.Toggle_debug_camera ();
      break;
    case f2:
      _rendering.Toggle_auto_camera ();
      break;
    case f3:
      _rendering.Toggle_lighting ();
      break;
    case f4:
      _rendering.Toggle_fog ();
      break;
    case f5:
      _rendering.Toggle_texturing ();
      break;
    case f6:
      _rendering.Toggle_filtering ();
      break;
    case f7:
      _rendering.Toggle_mipmapping ();
      break;
    case f8:
      _rendering.Toggle_filling ();
      break;
    case f9:
      _rendering.Toggle_almanach ();
      break;
    case f10:
      _rendering.Toggle_deLorean ();
      break;
    case f11:
      _rendering.Toggle_buildings ();
      break;
    case f12:
      _rendering.Toggle_players ();
      break;
    default:
      break;
    }
  Redraw ();
}

//
// Called when a button is pressed
//
void	GUI::MouseButtonCallback (const MouseButton button,
				  const bool state,
				  const int x, const int y)
{
  LOG1("GUI::MouseButtonCallback");
//   std::cerr << "MouseButtonCallback" << std::endl;

  if (left_button == button)
    _left_button = state;
  if (middle_button == button)
    _middle_button = state;
  if (right_button == button)
    _right_button = state;
  _xold = x;
  _yold = y;
}

//
// Called when the mouse is moved
//
void	GUI::MouseMotionCallback (const int x, const int y)
{
  LOG1("GUI::MouseMotionCallback");
//   std::cerr << "MouseMotionCallback" << std::endl;

  if (_left_button)
    _rendering.Rotate ((x - _xold) / 5, (y - _yold) / 5., 0);

  if (_middle_button)
    _rendering.Rotate (0, 0, (x - _xold) / 5);

  if (_right_button)
      _rendering.Translate ((_xold - x) / 600., 0, (_yold - y) / 400.);

  _xold = x;
  _yold = y;
}

//=============================================================================
