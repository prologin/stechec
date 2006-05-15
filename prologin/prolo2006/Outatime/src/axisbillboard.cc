//=============================================================================
//
// AxisBillboard class
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
// axisbillboard.cc : made by Zavie (Julien Guertault)
//
//=============================================================================

#include		"axisbillboard.hh"

// =============================================================== Constructors

AxisBillboard::AxisBillboard ():
  Billboard(), _length(1), _axis(0, 1, 0)
{}

AxisBillboard::AxisBillboard (const float length):
  Billboard(), _length(length), _axis(0, 1, 0)
{}


// ================================================================== Accessors

float			AxisBillboard::Length () const
{
  return _length;
}

const Vector3D &	AxisBillboard::Axis () const
{
  return _axis;
}


// ================================================================== Modifiers

void			AxisBillboard::Length (const float length)
{
  _length = length;
}

void			AxisBillboard::Axis (const Vector3D & axis)
{
  _axis = axis;
}

void			AxisBillboard::ReAlign (const Matrix & camera)
{
  Vector3D Vy (_axis);
  Vector3D Vx (camera.Vy () ^ camera.Vz ());
  Vx.Normalize ();
  Vy.Normalize (_length);

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
