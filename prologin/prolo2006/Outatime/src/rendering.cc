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
// rendering.cc : made by Zavie (Julien Guertault)
//
//=============================================================================

#include		<GL/gl.h>
#include		<GL/glu.h>
#include		<iostream>
#include		<cstdio>
#include		<cmath>
#include		"3ds.hh"
#include		"datacenter.hh"
#include		"rendering.hh"
#include		"texturemanager.hh"
#include		"vector3d.hh"

//================================================== Constructor and destructor

Rendering::Rendering ():
  _window_width(0),
  _window_height(0),
  _last_tick(0),
  _average_delta_tick(0),
  _debug_camera(false),
  _auto_camera(true),
  _lighting(true),
  _fog(false),
  _texturing(true),
  _filtering(true),
  _mipmapping(true),
  _filling(true),
  _display_almanach(true),
  _display_delorean(true),
  _display_buildings(true),
  _display_players(true),
  _update_projection(true),
  _letter_base(0),
  _console_texture(0),
  _time_axis(0),
  _team_axis(1, 0, 0),
  _center(0, 0, 0),
  _delorean_representation(0)
{
  for (unsigned int i = 0; i < 6; ++i)
    _weights[i] = new Vector3D (0, 0, 0);
}

Rendering::~Rendering ()
{
  glDeleteLists (_letter_base, 95);

  for (unsigned int i = 0; i < 6; ++i)
    delete _weights[i];

  if (_delorean_representation != 0)
    delete _delorean_representation;
}


//============================================================== Initialization

bool			Rendering::Init_objects ()
{
  _console_texture = TextureManager::Instance ()->Load_texture ("console.jpg",
								true,
								false,
								false);

  // Load the 3DS in the messy structure
  t3DModel delorean_3ds;
  CLoad3DS loader;
  if (loader.Import3DS (&delorean_3ds, "delorean.3ds"))
    // Import it into a clean object
    _delorean_representation = new Composite (delorean_3ds);

  // Clean up all the mess
  for (std::vector<t3DObject>::iterator it = delorean_3ds.pObject.begin ();
       it != delorean_3ds.pObject.end ();
       ++it)
    {
      delete it->pVerts;
      delete it->pNormals;
      delete it->pTexVerts;
      delete it->pFaces;
    }

  return (_delorean_representation != 0);
}

// int	Rendering::Load_textures ()
// {
//   unsigned char texture_height[256 * 256];

//   if (!Load_texture (DataCenter::Instance ()->Map (),
// 		     "Height", 256, 1, texture_height))
//     return 1;

//   _terrain.Height (texture_height);
//   std::cerr << "Height map loaded!" << std::endl;

//   if (!(Texture::Instance ()->Load ()))
//     return 1;

//   std::cerr << "Textures loaded!" << std::endl;

//   // Arbres
//   for (unsigned int i = 0; i < TREES; ++i)
//     _trees.push_back (new TreeGfx (_terrain));

//   return 0;
// }

//
// Set OpenGL settings and compile the various display lists
//
int			Rendering::Init_API (const int width, const int height)
{
  LOG1("Rendering::Init_API");

  _window_width = width;
  _window_height = height;

  // OpenGL general settings
  glClearColor (0, 0, 0, 0);
  glLineWidth (3);
//   glClearColor (0.87, 1, 0.77, 0);
  const float fogcolor[4] = {0.8, 0.8, 0.8, 1};
  glFogfv (GL_FOG_COLOR, fogcolor);
//   glFogi (GL_FOG_MODE, GL_LINEAR);
//   glFogf (GL_FOG_START, 0.3);
//   glFogf (GL_FOG_END, 2);
  glFogi (GL_FOG_MODE, GL_EXP2);
  glFogf (GL_FOG_DENSITY, 0.5);

  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
  glEnable (GL_CULL_FACE);
  glCullFace (GL_BACK);
  glEnable (GL_ALPHA_TEST);
  glAlphaFunc (GL_GREATER, 0);
  glPointSize (5);

  // Light
  GLfloat L0pos[] = { 0.0, 1.0, 1.0, 0 };
  GLfloat L0amb[] = { 0.7, 0.7, 0.7 };
  GLfloat L0dif[] = { 1, 0.95, 0.8 };
  GLfloat L0spe[] = { 1, 1, 1 };
  glLightModeli (GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
//  glLightModeli (GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
  glLightfv (GL_LIGHT0, GL_POSITION, L0pos);
  glLightfv (GL_LIGHT0, GL_AMBIENT, L0amb);
  glLightfv (GL_LIGHT0, GL_DIFFUSE, L0dif);
  glLightfv (GL_LIGHT0, GL_SPECULAR, L0spe);
  glEnable (GL_LIGHT0);
  glEnable (GL_RESCALE_NORMAL);

  _camera.Translate (0, -0.25, -1);
  Rendering_update ();

  // Letters display list for displaying text
  _letter_base = glGenLists (95);
  for (unsigned int i = ' '; i <= '~'; ++i)
    {
      glNewList (_letter_base + i - ' ', GL_COMPILE);
      const int c = i - ' ';
      const int row = c / 13;
      const int column = c % 13;

      const float x1 = column / 13.;
      const float x2 = (column + 1) / 13.;
      const float y1 = row / 8.;
      const float y2 = (row + 1) / 8.;

      glBegin (GL_TRIANGLE_FAN);
      glTexCoord2f(x1, y1); glVertex3f (0, 1.5, 0);
      glTexCoord2f(x1, y2); glVertex3f (0, 0, 0);
      glTexCoord2f(x2, y2); glVertex3f (1, 0, 0);
      glTexCoord2f(x2, y1); glVertex3f (1, 1.5, 0);
      glEnd ();

      glTranslatef (1, 0, 0);
      glEndList ();
    }

  std::cerr << "OpenGL ready!" << std::endl;

  return 0;
}


//=================================================================== Rendering

void			Rendering::Rendering_update ()
{
  LOG1("Rendering::Rendering_update");

  TextureManager::Instance ()->Update ();

  if (_update_projection)
    {
      glMatrixMode (GL_PROJECTION);

      const float zmin = 0.01;
      const float zmax = _camera.T ().Norm () + 2;
      glLoadIdentity ();
      gluPerspective (40,
		      float (_window_width) / float (_window_height),
		      zmin,
		      zmax);
      glMatrixMode (GL_MODELVIEW);
      _update_projection = false;
    }
}

void			Rendering::Animation_update (const float state)
{
  LOG1("Rendering::Animation_update");

  DataCenter * data_center = DataCenter::Instance ();

  LOG1("test");


#warning "FIXME: c'est ici qu'il faut mettre à jour les éléments graphiques"
//   for (Gems_t::iterator it = _gems.begin (); etc.


  // Animation of the water
//   _water.Update_shape (_last_tick);

  // Animation of the healing sources
//   for (Healings_t::const_iterator it =
// 	 data_center->Healing_sources ().begin ();
//        it != data_center->Healing_sources ().end ();
//        ++it)
//     (*it)->Update ();
//   for (B14773s_t::const_iterator it =
// 	 data_center->Coleopteres ().begin ();
//        it != data_center->Coleopteres ().end ();
//        ++it)
//     (*it)->Update (_terrain, state);

  // Automatic camera
  if (_auto_camera)
    {
      Players_t::const_iterator begin(data_center->Players ().begin ());
      Players_t::const_iterator end(data_center->Players ().end ());

//       LOG1("test (" << data_center->Players ().size () << " joueurs)");

      if (begin != end)
	{
	  Vector3D p((*begin)->X (), (*begin)->Y (), 0);
	  _min_x = p.X ();
	  _min_y = p.Y ();
	  _min_z = p.Z ();
	  _max_x = p.X ();
	  _max_y = p.Y ();
	  _max_z = p.Z ();
	}
      else
	{
	  _min_x = 0;
	  _min_y = 0;
	  _min_z = 0;
	  _max_x = 0;
	  _max_y = 0;
	  _max_z = 0;
	}

      LOG1("test");

      int count[MAX_TEAM];
      for (unsigned int i = 0; i < MAX_TEAM; ++i)
	{
	  count[i] = 0;
	  _weights[i]->Set (0, 0, 0);
	}

      LOG1("test");

      // Bounding-box
      for (Players_t::const_iterator it = begin; it != end; ++it)
	{
	  Vector3D p((*it)->X (), (*it)-> Y (), 0);

	  const unsigned int i((*it)->Team ());
	  ++count[i];
	  (*_weights[i]) = (*_weights[i]) + p;

	  if (_min_x > p.X ())
	    _min_x = p.X ();
	  if (_min_y > p.Y ())
	    _min_y = p.Y ();
	  if (_min_z > p.Z ())
	    _min_z = p.Z ();
	  if (_max_x < p.X ())
	    _max_x = p.X ();
	  if (_max_y < p.Y ())
	    _max_y = p.Y ();
	  if (_max_z < p.Z ())
	    _max_z = p.Z ();
	}

      LOG1("test");

      Vector3D old_center(_center);
      // Center of the bounding-box
      _center.Set ((_min_x + _max_x) * 0.5f,
		   (_min_y + _max_y) * 0.5f,
		   (_min_z + _max_z) * 0.5f);
      // Size of the bounding box
      const float dx(_max_x - _min_x);
      const float dy(_max_y - _min_y);
      const float dz(_max_z - _min_z);
      float length(0.5 * sqrtf (dx * dx + dy * dy + dz * dz));
      if (length < 0.5f)
	length = 0.5f;

      LOG1("test");

      // Weights of each team
      unsigned int teams = 0;
      for (unsigned int i = 0; i < 6; ++i)
	if (count[i] != 0)
	  {
	    (*_weights[i]) = (*_weights[i]) / count[i];
	    ++teams;
	  }

      LOG1("test");

      Vector3D old_axis(_team_axis);

      // Best horizontal axis
      _team_axis = Vector3D (1, 0, 0);
      if (teams > 1)
	{
	  float max_len = 0;
	  for (unsigned int i = 0; i < 5; ++i)
	    if (count[i] != 0)
	      for (unsigned int j = i; j < 5; ++j)
		if (count[j] != 0)
		  {
		    Vector3D v = (*_weights[i]) - (*_weights[j]);
		    v.Y (0);
		    if (v.Norm () > max_len)
		      {
			max_len = v.Norm ();
			_team_axis = v;
		      }
		  }
	}

      LOG1("test");

      // Special case: only one team...
      if (1 == teams)
	{
	  float max_len = 0;
	  for (Players_t::const_iterator it = begin; it != end; ++it)
	    for (Players_t::const_iterator jt = it; jt != end; ++jt)
	    {
#warning "FIXME: cas où il n'y a qu'une unité, pour la caméra."
	      Vector3D v = Vector3D ((*it)->X () - (*jt)->X (),
				     (*it)->Y () - (*jt)->Y (),
				     0);
	      v.Y (0);
	      if (v.Norm () > max_len)
		{
		  max_len = v.Norm ();
		  _team_axis = v;
		}
	    }
	}

      LOG1("test");

      // Filtering, to decrease shaking
      _team_axis.Normalize ();
      if (_team_axis * old_axis > 0.9)
	{
	  _team_axis = (_team_axis + old_axis * 19) * 0.05;
	  _center = (_center + old_center * 19) * 0.05;
	  _time_axis = _last_tick;
	}

      LOG1("test");

      // Up vector
      Vector3D up(0, 0.1 + 0.5 * length * length, 0);

      // At vector
      Vector3D at(_team_axis ^ up);
      at.Normalize (length * 1.4);
      at = at + up;

      const Matrix translation(-_center - at);
      at.Normalize ();
      Vector3D right = _team_axis;
      right.Normalize ();
      up = at ^ right;
      const float m[16] = { right.X (), up.X (), at.X (), 0,
			    right.Y (), up.Y (), at.Y (), 0,
			    right.Z (), up.Z (), at.Z (), 0,
			    0, 0, 0, 1 };
      const Matrix rotation(m);
      _camera = rotation * translation;
      _camera.Ortho_normalise ();
      _update_projection = true;
    }
  LOG1(" * plip-plop *");
}

//
// Returns a string telling the framerate
//
static char fps_string[22]; // "Ough to be enough for everyone" (c)
static char *			fps (const float value)
{
  if (value > 1000)
    {
      if (value > 10000)
	sprintf (fps_string, "< 1 fps (%.1f s)", value / 1000);
      else
	sprintf (fps_string, "< 1 fps (%.1f ms)", value);
    }
  else
    sprintf (fps_string, "%.1f fps (%.1f ms)", 1000 / value, value);
  return fps_string;
}

//
// Called when a redraw is requested
//
void			Rendering::Render (const int tick, const float state)
{
  LOG1("Rendering::Render");

  glMatrixMode (GL_MODELVIEW);

  _average_delta_tick = (4 * _average_delta_tick + (tick - _last_tick)) / 5;
  _last_tick = tick;

  Rendering_update ();

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  if (!_filling)
    glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

  // Camera
//   display_list->Full_view ();

//   // Skydome
//   glColor3f (1, 1, 1);
// //   display_list->Skydome ();
//   glPopMatrix ();
//   glEnable (GL_DEPTH_TEST);


  DataCenter * data_center = DataCenter::Instance ();
  TextureManager * texture_manager = TextureManager::Instance ();

  if (_lighting)
    glEnable (GL_LIGHTING);
  if (_fog)
    glEnable (GL_FOG);


#warning "FIXME: Afficher ici !"


  if (_fog)
    glDisable (GL_FOG);
  if (_lighting)
    glDisable (GL_LIGHTING);


  // Camera wires
  // -------------------------------------
  if (_debug_camera)
    {
      glDisable (GL_BLEND);
      texture_manager->Disactivate_texturing ();
      glDisable (GL_DEPTH_TEST);

      glColor3f (1, 1, 0);
      glBegin (GL_LINES);
      glVertex3f (_min_x, _min_y, _min_z); glVertex3f (_max_x, _min_y, _min_z);
      glVertex3f (_min_x, _min_y, _max_z); glVertex3f (_max_x, _min_y, _max_z);
      glVertex3f (_min_x, _max_y, _min_z); glVertex3f (_max_x, _max_y, _min_z);
      glVertex3f (_min_x, _max_y, _max_z); glVertex3f (_max_x, _max_y, _max_z);

      glVertex3f (_min_x, _min_y, _min_z); glVertex3f (_min_x, _max_y, _min_z);
      glVertex3f (_min_x, _min_y, _max_z); glVertex3f (_min_x, _max_y, _max_z);
      glVertex3f (_max_x, _min_y, _min_z); glVertex3f (_max_x, _max_y, _min_z);
      glVertex3f (_max_x, _min_y, _max_z); glVertex3f (_max_x, _max_y, _max_z);

      glVertex3f (_min_x, _min_y, _min_z); glVertex3f (_min_x, _min_y, _max_z);
      glVertex3f (_min_x, _max_y, _min_z); glVertex3f (_min_x, _max_y, _max_z);
      glVertex3f (_max_x, _min_y, _min_z); glVertex3f (_max_x, _min_y, _max_z);
      glVertex3f (_max_x, _max_y, _min_z); glVertex3f (_max_x, _max_y, _max_z);

      glColor3f (1, 1, 0);
      for (unsigned int i = 0; i < 5; ++i)
	for (unsigned int j = 0; j < 5; ++j)
	  {
	    glVertex3fv ((*_weights[i])());
	    glVertex3fv ((*_weights[j])());
	  }

      glVertex3fv (_center());
      glVertex3fv ((_center + _team_axis)());
      glEnd ();

      glBegin (GL_POINTS);
      glVertex3fv (_center());
      for (unsigned int i = 0; i < 6; ++i)
	{
	  glColor3fv (colors[i]);
	  glVertex3fv ((*_weights[i])());
	}
      glEnd ();
      glEnable (GL_DEPTH_TEST);
      glEnable (GL_BLEND);
    }


  // OSD
  // -------------------------------------
  texture_manager->Activate_texture (42 /* FIXME */);

  glDisable (GL_DEPTH_TEST);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  if (!_filling)
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

  glMatrixMode (GL_PROJECTION);
  glPushMatrix ();
  glOrtho (0, _window_width - 1, 0, _window_height - 1, -1, 1);
  glMatrixMode (GL_MODELVIEW);

  glPushMatrix ();

  glLoadIdentity ();

  static float osd_color[2][4] =
    {
      // Disabled
      {0.5, 0.5, 0.5, 0.6},
      // Enabled
      {1, 1, 1, 0.6}
    };

  // Prologin
  glColor4fv (osd_color[1]);
  glTranslatef (10, _window_height - 20, 0);
  Display_text (12, "Prologin 2006");

  // FPS
  glTranslatef (10, -20, 0);
  Display_text (12, fps (_average_delta_tick));

  // Frame
  {
    char buffer[20];
    sprintf (buffer, "Frame %d/%d",
	     data_center->Current_frame (),
	     data_center->Frames ());
    glTranslatef (10, -20, 0);
    Display_text (12, buffer);
  }

  glLoadIdentity ();
  glTranslatef (10, 15, 0);

  // Features activated
  glColor4fv (osd_color[(_debug_camera ? 1 : 0)]);
  Display_text (10, "Debug-cam");
  glTranslatef (90, 0, 0);

  glColor4fv (osd_color[(_auto_camera ? 1 : 0)]);
  Display_text (10, "Auto-cam");
  glTranslatef (90, 0, 0);

  glColor4fv (osd_color[(_lighting ? 1 : 0)]);
  Display_text (10, "Light");
  glTranslatef (60, 0, 0);

  glColor4fv (osd_color[(_fog ? 1 : 0)]);
  Display_text (10, "Fog");
  glTranslatef (40, 0, 0);

  glColor4fv (osd_color[(_texturing ? 1 : 0)]);
  Display_text (10, "Textures");
  glTranslatef (90, 0, 0);

  glColor4fv (osd_color[(_filtering ? 1 : 0)]);
  Display_text (10, "Filter");
  glTranslatef (70, 0, 0);

  glColor4fv (osd_color[(_mipmapping ? 1 : 0)]);
  Display_text (10, "Mipmap");
  glTranslatef (70, 0, 0);

  glColor4fv (osd_color[(_filling ? 1 : 0)]);
  Display_text (10, "Fill");
  glTranslatef (50, 0, 0);

  // Elements displayed
  glColor4fv (osd_color[(_display_almanach ? 1 : 0)]);
  Display_text (10, "Almanach");
  glTranslatef (90, 0, 0);

  glColor4fv (osd_color[(_display_delorean ? 1 : 0)]);
  Display_text (10, "De Lorean");
  glTranslatef (100, 0, 0);

  glColor4fv (osd_color[(_display_buildings ? 1 : 0)]);
  Display_text (10, "Buildings");
  glTranslatef (100, 0, 0);

  glColor4fv (osd_color[(_display_players ? 1 : 0)]);
  Display_text (10, "Players");

  glPopMatrix ();

  glMatrixMode (GL_PROJECTION);
  glPopMatrix ();
  glMatrixMode (GL_MODELVIEW);
  // -------------------------------------

  // Update the screen
  glFlush ();
}

void			Rendering::Display_text (const float size,
						 const char * str) const
{
  unsigned int	length = 0;
  for (length = 0; str[length] != '\0'; ++length);

  glPushMatrix ();
  glScalef (size, size, size);
  glListBase (_letter_base - ' ');
  glCallLists (length, GL_BYTE, str);
  glListBase (0);
  glPopMatrix ();
}


//====================================================================== States

void			Rendering::Resize (const int width, const int height)
{
  LOG1("Rendering::Resize");

  _window_width = width;
  _window_height = height;
  glViewport (0, 0, _window_width, _window_height);
  _update_projection = true;
}

void			Rendering::Rotate (const float x,
					   const float y,
					   const float z)
{
  if (_auto_camera)
    return;
  glMatrixMode (GL_MODELVIEW);
  if (y != 0)
    _camera.Rotate (y, 1, 0, 0);
  if (x != 0)
    _camera.Rotate (x, 0, 1, 0);
  if (z != 0)
    _camera.Rotate (z, 0, 0, 1);
  _camera.Ortho_normalise ();
}

void			Rendering::Translate (const float x,
					      const float y,
					      const float z)
{
  if (_auto_camera)
    return;
  glMatrixMode (GL_MODELVIEW);
  _camera.Translate (x, y, z);
  _update_projection = true;
}

void			Rendering::Toggle_debug_camera ()
{
  _debug_camera = !_debug_camera;
}

void			Rendering::Toggle_auto_camera ()
{
  _auto_camera = !_auto_camera;
}

void			Rendering::Toggle_lighting ()
{
  _lighting = !_lighting;
}

void			Rendering::Toggle_fog ()
{
  _fog = !_fog;
}

void			Rendering::Toggle_texturing ()
{
  TextureManager::Instance()->Toggle_texturing ();
}

void			Rendering::Toggle_filtering ()
{
  TextureManager::Instance()->Toggle_filtering ();
}

void			Rendering::Toggle_mipmapping ()
{
  TextureManager::Instance()->Toggle_mipmapping ();
}

void			Rendering::Toggle_filling ()
{
  _filling = !_filling;
}

void			Rendering::Toggle_almanach ()
{
  _display_almanach = !_display_almanach;
}

void			Rendering::Toggle_deLorean ()
{
  _display_delorean = !_display_delorean;
}

void			Rendering::Toggle_buildings ()
{

  _display_buildings = !_display_buildings;
}

void			Rendering::Toggle_players ()
{
  _display_players = !_display_players;
}

//=============================================================================
