//=============================================================================
//
// Singleton class
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
// singleton.hh : made by Zavie (Julien Guertault)
//
//=============================================================================

#ifndef			SINGLETON_HH
# define		SINGLETON_HH

// {Singleton} est un design pattern garantissant l'unicité de l'instance.

template <typename T>
class			Singleton
{
protected:

  // Constructor and destructor
  Singleton () {};
  ~Singleton () {};

public:

  // Instance
  static T *		Instance ()
  {
    if (0 == _instance)
      _instance = new T;
    return ((T *)_instance);
  }

  static void		Destroy ()
  {
    if (_instance != 0)
      delete _instance;
  }

private:
  static T *		_instance;
};

template <typename T>
T *			Singleton<T>::_instance = 0;

#endif			// SINGLETON_HH
