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
// vector3d.hh : made by Zavie (Julien Guertault)
//
//=============================================================================

#ifndef			VECTOR3D_HH
# define		VECTOR3D_HH

// {Vector3D} est un vecteur en trois dimensions, avec les opérations
// habituelles d'algèbre linéaire.

// Note : ma méthode pour le calcul de la norme ne me plaît pas. Pour
// éviter les calculs inutiles, la norme est sauvée, et son calcul est
// retardé. Malheureusement cela fait que la méthode Norm () n'est pas
// constante. Si quelqu'un a une idée pour garder l'optimisation tout
// en gardant une méthode constante, elle est bienvenue.

#include		<iostream>

class			Vector3D
{
public :

  // Constructors
  Vector3D ();
  Vector3D (const float, const float, const float);
  Vector3D (const float, const float, const float, const float);

  // Accessors
  const float *		operator () () const;
  float			X () const;
  float			Y () const;
  float			Z () const;

  float			Norm ();
  float			Square_norm ();

  // Modifiers
  void			Set (const float,
			     const float,
			     const float);
  void			Set (const float,
			     const float,
			     const float,
			     const float);
  void			X (const float);
  void			Y (const float);
  void			Z (const float);
  void			Normalize ();
  void			Normalize (const float);
  void			Rotate (const Vector3D &, const float);

  // Operators
  Vector3D		operator + (const Vector3D &) const;
  Vector3D		operator - (const Vector3D &) const;
  Vector3D		operator - () const;
  float			operator * (const Vector3D &) const;
  Vector3D		operator ^ (const Vector3D &) const;

  Vector3D		operator * (const float) const;
  Vector3D		operator / (const float) const;

protected :
  float			_v[3];
  float			_norm;
  float			_square_norm;
  bool			_norm_ok;
  bool			_square_norm_ok;
};

Vector3D		operator * (const float, const Vector3D &);

std::ostream&		operator << (std::ostream &, const Vector3D &);

#endif			// VECTOR_HH
