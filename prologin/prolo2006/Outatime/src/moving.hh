//=============================================================================
//
// Moving class
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
// moving.hh : made by Zavie (Julien Guertault)
//
//=============================================================================

#ifndef			MOVING_HH
# define		MOVING_HH

// {Moving} inclue les {Element} mobiles : joueurs, de Lorean,
// Almanach.

#include		"Api.hh"
#include		"element.hh"

class			Moving : public Element
{
public:
  // Constructor
  Moving (const unsigned int, const unsigned int);

  // Accessor
  bool			Visible_by_team (const unsigned int) const;

  // Modifier
  void			Visible_by_team (const unsigned int, const bool);

private:
  bool			_visible_by_team[MAX_TEAM];
};

#endif			// MOVING_HH
