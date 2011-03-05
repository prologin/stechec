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
// matrix.hh : made by Zavie (Julien Guertault)
//
//=============================================================================

#ifndef			MATRIX_HH
# define		MATRIX_HH

// {Matrix} est une matrice 4x4, avec les opérations habituelles
// d'algèbre linéaire.

// Important note:
// ---------------
// Operators use OpenGL, in order to use hardware acceleration if available.
// The matrix mode is assumed to be MODELVIEW.

#include		<iostream>
#include		"vector3d.hh"

class			Matrix
{
public :

  // Constructors
  Matrix ();
  Matrix (const float m[16]);
  Matrix (const Vector3D &);

  // Vectors
  Vector3D		T () const;
  Vector3D		Vx () const;
  Vector3D		Vy () const;
  Vector3D		Vz () const;

  // Load
  void			Load () const;

  // Modifiers
  void			Rotate (const float, const Vector3D &);
  void			Rotate (const float,
				const float,
				const float,
				const float);
  void			Translate (const Vector3D &);
  void			Translate (const float, const float, const float);
  void			Set_origin (const Vector3D &);
  void			Ortho_normalise ();

  // Operator
  float			operator () (const unsigned int,
				     const unsigned int) const;
  float			operator () (const unsigned int) const;
  const float *		operator () () const;
  Matrix		operator * (const Matrix &) const;
  Vector3D		operator * (const Vector3D &) const;

protected:
  float			A[16];
};

std::ostream&		operator << (std::ostream &, const Matrix &);

#endif			// MATRIX_HH
