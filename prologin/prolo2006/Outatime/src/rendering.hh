//=============================================================================
//
// Rendering class
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
// rendering.hh : made by Zavie (Julien Guertault)
//
//=============================================================================

#ifndef			RENDERING_HH
# define		RENDERING_HH

// {Rendering} est le noyau de l'interface graphique.

#include		"Api.hh"
#include		"matrix.hh"


const float colors[7][3] =
  {
    {0.2, 0.4, 1},
    {0.2, 1, 0.4},
    {1, 0.4, 0.2},
    {1, 0.2, 0.4},
    {0.4, 1, 0.2},
    {0.4, 0.2, 1}
  };

class			Rendering
{
public:
  // Constructor and destructor
  Rendering ();
  ~Rendering ();

  // Init
//   int			Load_textures ();
  int			Init_API (const int, const int);

  // Rendering
  void			Rendering_update ();
  void			Animation_update (const float);
  void			Render (const int, const float);
  void			Display_text (const float, const char *) const;

  // States
  void			Resize (const int, const int);
  void			Rotate (const float, const float, const float);
  void			Translate (const float, const float, const float);

  void			Toggle_debug_camera ();
  void			Toggle_auto_camera ();
  void			Toggle_lighting ();
  void			Toggle_fog ();
  void			Toggle_texturing ();
  void			Toggle_filtering ();
  void			Toggle_mipmapping ();
  void			Toggle_filling ();

  void			Toggle_almanach ();
  void			Toggle_deLorean ();
  void			Toggle_buildings ();
  void			Toggle_players ();

protected:
  char *		Fps (const float);

  Matrix		_camera;

  int			_window_width;
  int			_window_height;

  int			_last_tick;
  float			_average_delta_tick;

  bool			_debug_camera;
  bool			_auto_camera;
  bool			_lighting;
  bool			_fog;
  bool			_texturing;
  bool			_filtering;
  bool			_mipmapping;
  bool			_filling;

  bool			_display_almanach;
  bool			_display_delorean;
  bool			_display_buildings;
  bool			_display_players;

  bool			_update_projection;

  unsigned int		_letter_base;

  float			_min_x;
  float			_min_y;
  float			_min_z;
  float			_max_x;
  float			_max_y;
  float			_max_z;
  int			_time_axis;
  Vector3D		_team_axis;
  Vector3D		_center;
  Vector3D *		_weights[MAX_TEAM];
};

#endif			// RENDERING_HH
