//=============================================================================
//
// DeLorean class
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
// delorean.hh : made by Zavie (Julien Guertault)
//
//=============================================================================

#ifndef			DELOREAN_HH
# define		DELOREAN_HH

// {DeLorean} représente la de Lorean.

#include		"ownable.hh"

class			DeLorean : public Ownable
{
public:
  // Constructor
  DeLorean (const unsigned int,
	    const unsigned int,
	    const Player *,
	    const bool,
	    const unsigned int);

  // Accessor
  bool			Present () const;
  unsigned int		Autonomy () const;

private:
  const bool		_present;
  const unsigned int	_autonomy;
};

#endif			// DELOREAN_HH
