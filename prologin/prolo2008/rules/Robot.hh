/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2008 Prologin
*/

#ifndef ROBOT_HH_
# define ROBOT_HH_

class Robot
{
public:
  Robot(void): _pos_x(-1), _pos_y(-1), _turbo(0), _hook(-1), _has_ball(false), _team_id(-1) {}

  void		Init(int x, int y, int team) {
    _pos_x = x;
    _pos_y = y;
    _team_id = team;
  }

  int		GetXPos(void)		{ return _pos_x; }
  int		GetYPos(void)		{ return _pos_y; }

  void		SetPos(int x, int y)	{ _pos_x = x; _pos_y = y; }

  void		BoostTurbo(void)	{ _turbo++; }
  void		ResetTurbo(void)	{ _turbo = 0; }

  void		SetHook(int id)		{ _hook = id; }
  void		ResetHook(void)		{ _hook = -1; }

  void		PickBall(void)		{ _has_ball = true; }
  void		ReleaseBall(void)	{ _has_ball = false; }

private:
  int		_pos_x;
  int		_pos_y;
  unsigned int	_turbo;
  int		_hook;		/// Grapin -- -1 if empty
  bool		_has_ball;
  int		_team_id;
};

#endif // !ROBOT_HH_
