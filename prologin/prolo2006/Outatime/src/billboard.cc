//=============================================================================
//
// Billboard class
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
// billboard.cc : made by Zavie (Julien Guertault)
//
//=============================================================================

#include		"GL/gl.h"
#include		"billboard.hh"

// ================================================= Constructor and destructor

Billboard::Billboard ()
{
  _texture_uv[0] = 1;
  _texture_uv[1] = 0;
  _texture_uv[2] = 0;
  _texture_uv[3] = 0;
  _texture_uv[4] = 0;
  _texture_uv[5] = 1;
  _texture_uv[6] = 1;
  _texture_uv[7] = 1;

  for (unsigned int i = 0; i < 12; ++i)
    _vertices[i] = 0;
}

Billboard::~Billboard ()
{}

// ====================================================================== Calls

void			Billboard::Render (const float) const
{
  glEnableClientState (GL_TEXTURE_COORD_ARRAY);
  glEnableClientState (GL_VERTEX_ARRAY);

  glTexCoordPointer (2, GL_INT, 0, _texture_uv);
  glVertexPointer (3, GL_FLOAT, 0, _vertices);

  glDrawArrays (GL_TRIANGLE_FAN, 0, 4);

  glDisableClientState (GL_TEXTURE_COORD_ARRAY);
  glDisableClientState (GL_VERTEX_ARRAY);
}

void			Billboard::Update (const Matrix & camera)
{
  ReAlign (camera);
}

// ============================================================================
