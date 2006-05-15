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
// moving.cc : made by Zavie (Julien Guertault)
//
//=============================================================================

#include		"moving.hh"

// ================================================================ Constructor

Moving::Moving (const unsigned int x, const unsigned int y):
  Element(x, y)
{
  for (unsigned int i = 0; i < MAX_TEAM; ++i)
    _visible_by_team[i] = false;
}


// =================================================================== Accessor

bool			Moving::Visible_by_team (const unsigned int team) const
{
  if (team >= MAX_TEAM)
    return false;
  return _visible_by_team[team];
}


// =================================================================== Modifier

void			Moving::Visible_by_team (const unsigned int team,
						 const bool visible)
{
  if (team >= MAX_TEAM)
    return;
  _visible_by_team[team] = visible;
}

// ============================================================================
