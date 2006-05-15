//=============================================================================
//
// Building class
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
// building.hh : made by Zavie (Julien Guertault)
//
//=============================================================================

#ifndef			BUILDING_HH
# define		BUILDING_HH

// {Building} inclue les {Element} immobiles, c'est à dire les
// bâtiments, qu'il soit possible d'y jouer ou non.

#include		<vector>
#include		"element.hh"

class			Building : public Element
{
public:
  // Constructor
  Building (const unsigned int, const unsigned int);
};

typedef
std::vector<Building *>	Buildings_t;

#endif			// BUILDING_HH
