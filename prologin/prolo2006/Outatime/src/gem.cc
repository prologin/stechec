//=============================================================================
//
// Gem class
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
// gem.cc : made by Zavie (Julien Guertault)
//
//=============================================================================

#include		"GL/gl.h"
#include		"gem.hh"

// ================================================= Constructor and destructor

Gem::Gem ():
  _kind(object),
  _position(0, 0, 0)
{}

Gem::~Gem ()
{}

// ====================================================================== Calls

void			Gem::Update (const Matrix &)
{}

void			Gem::Load_position () const
{
  glTranslatef (_position.X (), _position.Y (), _position.Z ());
}


// =================================================================== Accessor

Gem::kind		Gem::Kind () const	{ return _kind; }
const Vector3D &	Gem::Position () const	{ return _position; }


// =================================================================== Modifier

void			Gem::Kind (const kind kind)
{
  _kind = kind;
}

void			Gem::Position (const Vector3D & position)
{
  _position = position;
}

// ============================================================================
