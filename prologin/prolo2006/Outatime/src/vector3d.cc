//=============================================================================
//
// Vector3D class
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
// vector3d.cc : made by Zavie (Julien Guertault)
//
//=============================================================================

#include		<iostream>
#include		<cmath>
#include		"vector3d.hh"

// =============================================================== Constructors

Vector3D::Vector3D ():
  _norm(0), _square_norm(0),
  _norm_ok(true), _square_norm_ok(true)
{
  _v[0] = 0;
  _v[1] = 0;
  _v[2] = 0;
}

Vector3D::Vector3D (const float x, const float y, const float z):
  _norm_ok(false), _square_norm_ok(false)
{
  _v[0] = x;
  _v[1] = y;
  _v[2] = z;
}

Vector3D::Vector3D (const float x,
		    const float y,
		    const float z,
		    const float norm):
  _norm(norm),
  _norm_ok(true), _square_norm_ok(false)
{
  _v[0] = x;
  _v[1] = y;
  _v[2] = z;
}


// ================================================================== Accessors

const float *		Vector3D::operator () () const	{ return _v; }
float			Vector3D::X () const		{ return _v[0]; }
float			Vector3D::Y () const		{ return _v[1]; }
float			Vector3D::Z () const		{ return _v[2]; }

float			Vector3D::Norm ()
{
  if (!_norm_ok)
    {
      _norm = sqrtf (Square_norm ());
      _norm_ok = true;
    }
  return _norm;
}

float			Vector3D::Square_norm ()
{
  if (!_square_norm_ok)
    {
      if (_norm_ok)
	_square_norm = _norm * _norm;
      else
	_square_norm = _v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2];
      _square_norm_ok = true;
    }
  return _square_norm;
}

// ================================================================== Modifiers

void			Vector3D::Set (const float x,
				       const float y,
				       const float z)
{
  _v[0] = x;
  _v[1] = y;
  _v[2] = z;
  _norm_ok = false;
  _square_norm_ok = false;
}

void			Vector3D::Set (const float x,
				       const float y,
				       const float z,
				       const float norm)
{
  _v[0] = x;
  _v[1] = y;
  _v[2] = z;
  _norm = norm;
  _norm_ok = true;
  _square_norm_ok = false;
}

void			Vector3D::X (const float x)
{
  _v[0] = x;
  _norm_ok = false;
  _square_norm_ok = false;
}

void			Vector3D::Y (const float y)
{
  _v[1] = y;
  _norm_ok = false;
  _square_norm_ok = false;
}

void			Vector3D::Z (const float z)
{
  _v[2] = z;
  _norm_ok = false;
  _square_norm_ok = false;
}

void			Vector3D::Normalize ()
{
  Normalize (1);
}

void			Vector3D::Normalize (const float length)
{
  const float norm = Norm ();
  if (norm != 0)
    {
      const float coef(length / norm);
      _v[0] = coef * _v[0];
      _v[1] = coef * _v[1];
      _v[2] = coef * _v[2];
    }
  else
    {
      _v[0] = length;
      _v[1] = 0;
      _v[2] = 0;
    }
  _norm = 1;
}

void			Vector3D::Rotate (const Vector3D & axis,
					  const float angle)
{
  const float cosinus = cosf (angle);
  const float sinus = sinf (angle);
  const float cos_bis = (1 - cosinus) * axis.X ();
  const float cos_third = (1 - cosinus) * axis.Y ();

  float x = (cosinus + cos_bis * axis.X ()) * _v[0];
  x = x + (cos_bis * axis.Y () - axis.Z () * sinus) * _v[1];
  x = x + (cos_bis * axis.Z () + axis.Y () * sinus) * _v[2];

  float y = (cos_bis * axis.Y () + axis.Z () * sinus) * _v[0];
  y = y + (cosinus + cos_third * axis.Y ()) * _v[1];
  y = y + (cos_third * axis.Z () - axis.X () * sinus) * _v[2];

  float z = (cos_bis * axis.Z () - axis.Y () * sinus) * _v[0];
  z = z + (cos_third * axis.Z () + axis.X () * sinus) * _v[1];
  z = z + (cosinus + (1 - cosinus) * axis.Z () * axis.Z ()) * _v[2];

  _v[0] = x;
  _v[1] = y;
  _v[2] = z;
}


// ================================================================== Operators

//    @
//    @
//  @@@@@
//    @
//    @
Vector3D		Vector3D::operator + (const Vector3D & v) const
{
  return Vector3D(_v[0] + v._v[0],
		  _v[1] + v._v[1],
		  _v[2] + v._v[2]);
}

//
//
//  @@@@@
//
//
Vector3D		Vector3D::operator - (const Vector3D & v) const
{
  return Vector3D(_v[0] - v._v[0],
		  _v[1] - v._v[1],
		  _v[2] - v._v[2]);
}

Vector3D		Vector3D::operator - () const
{
  return Vector3D(-_v[0], -_v[1], -_v[2]);
}

//
//    @
//   @@@
//    @
//
float			Vector3D::operator * (const Vector3D & v) const
{
  return (_v[0] * v._v[0] +
	  _v[1] * v._v[1] +
	  _v[2] * v._v[2]);
}

//   @
//  @ @
// @   @
//
//
Vector3D		Vector3D::operator ^ (const Vector3D & v) const
{
  return Vector3D((_v[1] * v._v[2]) - (_v[2] * v._v[1]),
		  (_v[2] * v._v[0]) - (_v[0] * v._v[2]),
		  (_v[0] * v._v[1]) - (_v[1] * v._v[0]));
}


//        @@@
//   @       @
//  @@@   @@@@
//   @   @   @
//        @@@ @
Vector3D		Vector3D::operator * (const float a) const
{
  if (_norm_ok)
    return Vector3D(_v[0] * a,
		    _v[1] * a,
		    _v[2] * a,
		  _norm * a);
  return Vector3D(_v[0] * a,
		  _v[1] * a,
		  _v[2] * a);
}

Vector3D		operator * (const float a, const Vector3D & v)
{
  return v * a;
}

//     @  @@@
//    @      @
//   @    @@@@
//  @    @   @
// @      @@@ @
Vector3D		Vector3D::operator / (const float a) const
{
  if (0 == a)
    return *this;

  const float inv(1 / a);

  if (_norm_ok)
    return Vector3D(_v[0] * inv,
		    _v[1] * inv,
		    _v[2] * inv,
		    _norm * inv);
  return Vector3D(_v[0] * inv,
		  _v[1] * inv,
		  _v[2] * inv);
}


// ================================================================== IO stream

std::ostream &		operator << (std::ostream & out, const Vector3D & v)
{
  out << "("
      << v.X () << ", "
      << v.Y () << ", "
      << v.Z () << ")";
  return out;
}

// ============================================================================
