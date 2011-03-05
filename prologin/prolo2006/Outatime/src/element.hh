//=============================================================================
//
// Element class
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
// element.hh : made by Zavie (Julien Guertault)
//
//=============================================================================

#ifndef			ELEMENT_HH
# define		ELEMENT_HH

// {Element} inclue tout ce à quoi il est fait référence dans le sujet
// : bâtiments, joueurs, de Lorean, Almanach.

class			Element
{
public:
  // Constructor
  Element (const unsigned int, const unsigned int);

  // Accessors
  unsigned int		X () const;
  unsigned int		Y () const;

private:
  const unsigned int	_x;
  const unsigned int	_y;
};

#endif			// ELEMENT_HH
