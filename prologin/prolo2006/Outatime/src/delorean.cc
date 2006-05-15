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
// delorean.cc : made by Zavie (Julien Guertault)
//
//=============================================================================

#include		"delorean.hh"

// ================================================================ Constructor

DeLorean::DeLorean (const unsigned int x,
		    const unsigned int y,
		    const Player * owner,
		    const bool present,
		    const unsigned int autonomy):
  Ownable(x, y, owner), _present(present), _autonomy(autonomy)
{}


// =================================================================== Accessor

bool			DeLorean::Present () const	{ return _present; }
unsigned int		DeLorean::Autonomy () const	{ return _autonomy; }

// ============================================================================
