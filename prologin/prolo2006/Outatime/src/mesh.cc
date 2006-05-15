//=============================================================================
//
// Mesh class
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
// mesh.cc : made by Zavie (Julien Guertault)
//
//=============================================================================

#include		"GL/gl.h"
#include		"mesh.hh"

// ================================================= Constructor and destructor

Mesh::Mesh ():
  Solid (),
  _loaded(false),
  _number_vertices(0),
  _number_faces(0),
  _number_texcoord(0),
  _material(0),
  _name(0),
  _vertices(0),
  _normals(0),
  _texcoords(0),
  _faces(0)
{}

Mesh::~Mesh ()
{
  if (!_loaded)
    return;
  delete _vertices;
  delete _normals;
  delete _texcoords;
  delete _faces;
}

// ======================================================================= Call

void			Mesh::Render () const
{
  if (!_loaded)
    return;

  glPushMatrix ();
  Load_position ();
  Load_orientation ();

#warning "FIXME: envoyer les tableaux a OpenGL"

  glPopMatrix ();
}

// ============================================================================
