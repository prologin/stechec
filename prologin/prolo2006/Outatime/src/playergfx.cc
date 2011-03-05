//=============================================================================
//
// PlayerGfx class
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
// playergfx.cc : made by Zavie (Julien Guertault)
//
//=============================================================================

#include		"playergfx.hh"
#include		"texturemanager.hh"

static std::vector<int>	_idle[4];
static std::vector<int>	_bored[4];
static std::vector<int>	_ko[4];
static std::vector<int>	_moving[4];
static std::vector<int>	_attacking[4];
static std::vector<int>	_attacked[4];
static bool		_textures_loaded = false;

static int		Load_texture (const char * name)
{
  return TextureManager::Instance ()->Load_texture (name, true, true, false);
}

// ================================================================ Constructor

PlayerGfx::PlayerGfx ():
  WorldBillboard(), _direction(0), _state()
{
  if (_textures_loaded)
    return;

  _idle[0].push_back (Load_texture ("immobile-dos.jpg"));
  _idle[1].push_back (Load_texture ("immobile-droite.jpg"));
  _idle[2].push_back (Load_texture ("immobile-face.jpg"));
  _idle[3].push_back (Load_texture ("immobile-gauche.jpg"));

  _bored[0].push_back (Load_texture ("ennui-dos.jpg"));
  _bored[1].push_back (Load_texture ("ennui-droite.jpg"));
  _bored[2].push_back (Load_texture ("ennui-face.jpg"));
  _bored[3].push_back (Load_texture ("ennui-gauche.jpg"));

  _ko[0].push_back (Load_texture ("ko-dos.jpg"));
  _ko[1].push_back (Load_texture ("ko-droite.jpg"));
  _ko[2].push_back (Load_texture ("ko-face.jpg"));
  _ko[3].push_back (Load_texture ("ko-gauche.jpg"));

  _moving[0].push_back (Load_texture ("marche-dos-1.jpg"));
  _moving[0].push_back (Load_texture ("marche-dos-2.jpg"));
  _moving[0].push_back (Load_texture ("marche-dos-3.jpg"));
  _moving[0].push_back (Load_texture ("marche-dos-4.jpg"));
  _moving[1].push_back (Load_texture ("marche-droite-1.jpg"));
  _moving[1].push_back (Load_texture ("marche-droite-2.jpg"));
  _moving[1].push_back (Load_texture ("marche-droite-3.jpg"));
  _moving[1].push_back (Load_texture ("marche-droite-4.jpg"));
  _moving[2].push_back (Load_texture ("marche-face-1.jpg"));
  _moving[2].push_back (Load_texture ("marche-face-2.jpg"));
  _moving[3].push_back (Load_texture ("marche-gauche-1.jpg"));
  _moving[3].push_back (Load_texture ("marche-gauche-2.jpg"));
  _moving[3].push_back (Load_texture ("marche-gauche-3.jpg"));
  _moving[3].push_back (Load_texture ("marche-gauche-4.jpg"));

  _attacking[0].push_back (Load_texture ("frappe-dos-1.jpg"));
  _attacking[0].push_back (Load_texture ("frappe-dos-2.jpg"));
  _attacking[0].push_back (Load_texture ("frappe-dos-3.jpg"));
  _attacking[0].push_back (Load_texture ("frappe-dos-4.jpg"));
  _attacking[1].push_back (Load_texture ("frappe-droite-1.jpg"));
  _attacking[1].push_back (Load_texture ("frappe-droite-2.jpg"));
  _attacking[1].push_back (Load_texture ("frappe-droite-3.jpg"));
  _attacking[1].push_back (Load_texture ("frappe-droite-4.jpg"));
  _attacking[2].push_back (Load_texture ("frappe-face-1.jpg"));
  _attacking[2].push_back (Load_texture ("frappe-face-2.jpg"));
  _attacking[2].push_back (Load_texture ("frappe-face-3.jpg"));
  _attacking[2].push_back (Load_texture ("frappe-face-4.jpg"));
  _attacking[3].push_back (Load_texture ("frappe-gauche-1.jpg"));
  _attacking[3].push_back (Load_texture ("frappe-gauche-2.jpg"));
  _attacking[3].push_back (Load_texture ("frappe-gauche-3.jpg"));
  _attacking[3].push_back (Load_texture ("frappe-gauche-4.jpg"));

  _attacked[0].push_back (Load_texture ("victime-dos-1.jpg"));
  _attacked[0].push_back (Load_texture ("victime-dos-2.jpg"));
  _attacked[0].push_back (Load_texture ("victime-dos-3.jpg"));
  _attacked[1].push_back (Load_texture ("victime-droite-1.jpg"));
  _attacked[1].push_back (Load_texture ("victime-droite-2.jpg"));
  _attacked[1].push_back (Load_texture ("victime-droite-3.jpg"));
  _attacked[1].push_back (Load_texture ("victime-droite-4.jpg"));
  _attacked[2].push_back (Load_texture ("victime-face-1.jpg"));
  _attacked[2].push_back (Load_texture ("victime-face-2.jpg"));
  _attacked[2].push_back (Load_texture ("victime-face-3.jpg"));
  _attacked[2].push_back (Load_texture ("victime-face-4.jpg"));
  _attacked[3].push_back (Load_texture ("victime-gauche-1.jpg"));
  _attacked[3].push_back (Load_texture ("victime-gauche-2.jpg"));
  _attacked[3].push_back (Load_texture ("victime-gauche-3.jpg"));

  _textures_loaded = true;
}


// ======================================================================= Call

void			PlayerGfx::Render (const float state)
{
  TextureManager * texture_manager = TextureManager::Instance ();
  unsigned int direction = _direction + _camera_direction;

  while (direction >= 4)
    direction = direction - 4;
  switch (_state)
    {
    case Player::idle:
      {
	const unsigned int i = (unsigned int)(state *
					      _idle[direction].size ());
	texture_manager->Activate_texture (_idle[direction][i]);
	break;
      }
    case Player::bored:
      {
	const unsigned int i = (unsigned int)(state *
					      _bored[direction].size ());
	texture_manager->Activate_texture (_bored[direction][i]);
	break;
      }
    case Player::ko:
      {
	const unsigned int i = (unsigned int)(state *
					      _ko[direction].size ());
	texture_manager->Activate_texture (_ko[direction][i]);
	break;
      }
    case Player::moving:
      {
	const unsigned int i = (unsigned int)(state *
					      _moving[direction].size ());
	texture_manager->Activate_texture (_moving[direction][i]);
	break;
      }
    case Player::attacking:
      {
	const unsigned int i = (unsigned int)(state *
					      _attacking[direction].size ());
	texture_manager->Activate_texture (_attacking[direction][i]);
	break;
      }
    case Player::attacked:
      {
	const unsigned int i = (unsigned int)(state *
					      _attacked[direction].size ());
	texture_manager->Activate_texture (_attacked[direction][i]);
	break;
      }
    }
  WorldBillboard::Render ();
}


// =================================================================== Modifier

void			PlayerGfx::Direction (const unsigned int direction)
{
  _direction = direction;
}

void			PlayerGfx::State (const Player::state_t state)
{
  _state = state;
}

void			PlayerGfx::ReAlign (const Matrix & camera)
{
  const float x(camera.Vz ().X ());
  const float y(camera.Vz ().Y ());
  const float alpha(atan2f (y, x));
  _camera_direction = (int)(((alpha + M_PI) * 4) / M_PI);

//   WorldBillboard::ReAlign (camera);

  Vector3D Vy (0, 1, 0);
  Vector3D Vx (camera.Vy () ^ camera.Vz ());
  Vy = camera.Vz () ^ Vx;
  Vx.Normalize ();
  Vy.Normalize ();

  const Vector3D A( Vx + Vy);
  const Vector3D B(-Vx + Vy);
  const Vector3D C(-Vx - Vy);
  const Vector3D D( Vx - Vy);

  _vertices[0] = A.X ();
  _vertices[1] = A.Y ();
  _vertices[2] = A.Z ();

  _vertices[3] = B.X ();
  _vertices[4] = B.Y ();
  _vertices[5] = B.Z ();

  _vertices[6] = C.X ();
  _vertices[7] = C.Y ();
  _vertices[8] = C.Z ();

  _vertices[9] = D.X ();
  _vertices[10] = D.Y ();
  _vertices[11] = D.Z ();
}

// ============================================================================
