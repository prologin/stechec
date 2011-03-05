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
// gui.hh : made by Zavie (Julien Guertault)
//
//=============================================================================

#ifndef			GUI_HH
# define		GUI_HH

// {GUI} est une classe virtuelle permettant de s'abstraire du Toolkit
// utilisé.

#include		"Api.hh"
#include		"client_cx.hh"
#include		"rendering.hh"

class			GUI
{
protected:
  typedef		enum
    {
      other_button,
      left_button,
      middle_button,
      right_button
    }			MouseButton;

  typedef		enum
    {
      no_key,
      f1,
      f2,
      f3,
      f4,
      f5,
      f6,
      f7,
      f8,
      f9,
      f10,
      f11,
      f12,
      up_arrow,
      down_arrow,
      left_arrow,
      right_arrow
    }			SpecialKey;

public:
  // Constructor and destructor
  GUI (Api *, ClientCx *);
  virtual ~GUI ();

  // Initialization
  virtual int		Init_GUI (int *,
				  char **,
				  const unsigned int,
				  const unsigned int,
				  const char *) = 0;
  int			Init_files ();
  int			Init_API ();

  virtual void		Run (const int, const int);

  virtual int		Elapsed_time () = 0;

  // Redraw call
  virtual void		Redraw () = 0;

  // Callbacks
  void			RenderTimerCallback ();
  void			FrameTimerCallback ();
  void			IdleCallback ();
  void			DisplayCallback ();
  // Window
  void			ResizeCallback (const int, const int);
  // Keyboard
  void			KeyboardCallback (const unsigned char,
					  const bool,
					  const bool,
					  const bool);
  void			SpecialKeyCallback (const SpecialKey,
					    const bool,
					    const bool,
					    const bool);
  // Mouse
  void			MouseButtonCallback (const MouseButton, const bool,
					     const int, const int);
  void			MouseMotionCallback (const int, const int);
  void			MouseDragCallback (const int, const int);

protected:
  Api *			_api;
  ClientCx *		_ccx;

  int			_window_width;
  int			_window_height;

  int			_xold;
  int			_yold;

  bool			_left_button;
  bool			_middle_button;
  bool			_right_button;

  bool			_pause;
//   bool			_step;

  int			_render_delay;
  int			_frame_delay;
  int			_frame_start_time;
  int			_current_time;
  int			_pause_time;
  int			_pause_start_time;
  Rendering		_rendering;
};

#endif			// GUI_HH
