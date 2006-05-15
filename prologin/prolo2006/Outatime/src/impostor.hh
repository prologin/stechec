//=============================================================================
//
// Impostor class
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
// impostor.hh : made by Zavie (Julien Guertault)
//
//=============================================================================

#ifndef			IMPOSTOR_HH
# define		IMPOSTOR_HH

// {Impostor} inclue les objets de type imposteur. Un imposteur est
// une image remplaçant un objet 3D, affichée grâce à un billboard
// aligné avec le monde.

#include		"worldbillboard.hh"
// #include		"vector3d.hh"

class			Impostor : public WorldBillboard
{
public:
  // Constructor
  Impostor ();

  // Le contenu d'un imposteur doit être remis à jour en fonction de
  // son orientation.
private:
//   vector3d		_reference;
};

#endif			// IMPOSTOR_HH
