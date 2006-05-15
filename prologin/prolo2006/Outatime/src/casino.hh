//=============================================================================
//
// Casino class
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
// casino.hh : made by Zavie (Julien Guertault)
//
//=============================================================================

#ifndef			CASINO_HH
# define		CASINO_HH

// {Casino} se limite aux bâtiments dans lesquels il est possible de
// jouer.

#include		<vector>
#include		"building.hh"

class			Casino : public Building
{
public:
  // Constructor
  Casino (const unsigned int,
	  const unsigned int,
	  const unsigned int,
	  const bool);

  // Accessors
  unsigned int		Id () const;
  bool			Open () const;

private:
  const unsigned int	_id;
  bool			_open;
};

typedef
std::vector<Casino *>	Casinos_t;

#endif			// CASINO_HH
