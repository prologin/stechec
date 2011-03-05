//=============================================================================
//
// Matrix class
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
// matrix.cc : made by Zavie (Julien Guertault)
//
//=============================================================================

// Important note:
// ---------------
// Operators use OpenGL, in order to use hardware acceleration if available.
// The matrix mode is assumed to be MODELVIEW.

#include		<GL/gl.h>
#include		<iostream>
#include		"matrix.hh"

// =============================================================== Constructors

Matrix :: Matrix ()
{
  for (int i = 0; i < 16; ++i)
    A[i] = (i % 5 == 0 ? 1 : 0);
}

Matrix :: Matrix (const float m[16])
{
  for (int i = 0; i < 16; ++i)
    A[i] = m[i];
}

Matrix :: Matrix(const Vector3D & v)
{
  for (int i = 0; i < 12; ++i)
    A[i] = (i % 5 == 0 ? 1 : 0);
  A[12] = v.X ();
  A[13] = v.Y ();
  A[14] = v.Z ();
  A[15] = 1;
}


// ==================================================================== Vectors

Vector3D		Matrix::T () const
{
  return Vector3D (A[12], A[13], A[14]);
}

Vector3D		Matrix::Vx () const
{
  return Vector3D (A[0], A[4], A[8]);
}

Vector3D		Matrix::Vy () const
{
  return Vector3D (A[1], A[5], A[9]);
}

Vector3D		Matrix::Vz () const
{
  return Vector3D (A[2], A[6], A[10]);
}


// ======================================================================= Load

void			Matrix::Load () const
{
  glMultMatrixf (A);
}

// =================================================================== Modifers

void			Matrix::Rotate (const float angle,
					const Vector3D &axis)
{
  Rotate (angle, axis.X (), axis.Y (), axis.Z ());
}

void			Matrix::Rotate (const float angle,
					const float x,
					const float y,
					const float z)
{
  glPushMatrix ();
//   glLoadMatrixf (A);
//   glRotatef (angle, x, y, z);
  glLoadIdentity ();
  glRotatef (angle, x, y, z);
  glMultMatrixf (A);
  glGetFloatv(GL_MODELVIEW_MATRIX, A);
  glPopMatrix ();
}

void			Matrix::Translate (const Vector3D &v)
{
  Translate (v.X (), v.Y (), v.Z ());
}

void			Matrix::Translate (const float x,
					   const float y,
					   const float z)
{
  glPushMatrix ();
//   glLoadMatrixf (A);
//   glTranslatef (x, y, z);
  glLoadIdentity ();
  glTranslatef (x, y, z);
  glMultMatrixf (A);
  glGetFloatv(GL_MODELVIEW_MATRIX, A);
  glPopMatrix ();
}

void			Matrix::Set_origin (const Vector3D &v)
{
  A[12] = v.X ();
  A[13] = v.Y ();
  A[14] = v.Z ();
}

void			Matrix::Ortho_normalise ()
{
  Vector3D vx(Vx ());
  Vector3D vy(Vy ());

  Vector3D vz(vx ^ vy);
  vz.Normalize ();
  vx.Normalize ();
  vy = vz ^ vx;

  A[0] = vx.X ();
  A[1] = vy.X ();
  A[2] = vz.X ();
  A[4] = vx.Y ();
  A[5] = vy.Y ();
  A[6] = vz.Y ();
  A[8] = vx.Z ();
  A[9] = vy.Z ();
  A[10] = vz.Z ();
}


// ================================================================== Operators

float			Matrix::operator () (const unsigned int i,
					     const unsigned int j) const
{
  if (i > 3 || j > 3)
    return 0;
  return A[i + 3 * j];
}

float			Matrix::operator () (const unsigned int i) const
{
  if (i > 15)
    return 0;
  return A[i];
}

const float *		Matrix::operator () () const
{
  return A;
}

Vector3D		Matrix::operator * (const Vector3D & v) const
{
  Matrix m(v);

  glPushMatrix ();
  glLoadMatrixf (A);
  glMultMatrixf (m.A);
  glGetFloatv(GL_MODELVIEW_MATRIX, m.A);
  glPopMatrix ();

  return Vector3D (m.T ());
}

Matrix			Matrix::operator * (const Matrix & source) const
{
  Matrix m;

  glPushMatrix ();
  glLoadMatrixf (A);
  glMultMatrixf (source.A);
  glGetFloatv(GL_MODELVIEW_MATRIX, m.A);
  glPopMatrix ();

  return m;
}


// ================================================================== IO stream

std::ostream &		operator << (std::ostream & out, const Matrix & m)
{
//   out << "[["
//       << m(0) << ", " << m(4) << ", " << m(8) << ", " << m(12)
//       << "], ["
//       << m(1) << ", " << m(5) << ", " << m(9) << ", " << m(13)
//       << "], ["
//       << m(2) << ", " << m(6) << ", " << m(10) << ", " << m(14)
//       << "], ["
//       << m(3) << ", " << m(7) << ", " << m(11) << ", " << m(15)
//       << "]]";
  out << "[["
      << m(0) << ", " << m(4) << ", " << m(8) << ", " << m(12)
      << "]," << std::endl << "["
      << m(1) << ", " << m(5) << ", " << m(9) << ", " << m(13)
      << "]," << std::endl << "["
      << m(2) << ", " << m(6) << ", " << m(10) << ", " << m(14)
      << "]," << std::endl << "["
      << m(3) << ", " << m(7) << ", " << m(11) << ", " << m(15)
      << "]]";
  return out;
}

// ============================================================================
