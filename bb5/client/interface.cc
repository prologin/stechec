/*
** TowBowlTactics, an adaptation of the tabletop game Blood Bowl.
** 
** Copyright (C) 2006 The TBT Team.
** 
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
** 
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
** 
** The TBT Team consists of people listed in the `AUTHORS' file.
*/

#include "Api.hh"

// FIXME: declare me.
extern Api* api;

extern "C" {

int my_team_id()
{
  return api->myTeamId();
}

int ball_x()
{
  return api->ball().x;
}

int ball_y()
{
  return api->ball().y;
}

int team_id(int x, int y)
{
  return api->teamId(Point(x, y));
}

int player_id(int x, int y)
{
  return api->playerId(Point(x, y));
}

int tackle_zone_number(int x, int y)
{
  return api->tackle_zone_number(x, y); // XXX
}

int select_team(int team_id)
{
  return api->selectTeam(team_id);
}

int team_reroll_number()
{
  return api->team_reroll_number(); // XXX
}

int reroll_used()
{
  return api->reroll_used(); // XXX
}

int player_number()
{
  return api->player_number(); // XXX
}

int select_player(int team_id)
{
  return api->selectPlayer(team_id);
}

int player_status()
{
  return (int)api->playerStatus();
}

int player_played()
{
  return api->player_played(); // XXX
}

int player_x()
{
  return api->player_x();
}

int player_y()
{
  return api->player_y();
}

int player_ma()
{
  return api->player_ma();
}

int player_ma_remain()
{
  return api->player_ma_remain();
}

int player_st()
{
  return api->player_st();
}

int player_ag()
{
  return api->player_ag();
}

int player_av()
{
  return api->player_av();
}

int player_skill_number()
{
  return api->player_skill_number();
}

int player_skill(int index)
{
  return api->player_skill(index);
}

int move_possible(int x, int y)
{
  return api->move_possible(x, y);
}

int move_length(int x, int y)
{
  return api->move_length(x, y);
}

int move_difficulty(int step)
{
  return api->move_difficulty(step);
}

int move_path_x(int step)
{
  return api->move_path_x(step);
}

int move_path_y(int step)
{
  return api->move_path_y(step);
}

void end_turn()
{
  return api->end_turn();
}

void move_turn_marker()
{
  return api->move_turn_marker();
}

void ask_illegal_procedure()
{
  return api->ask_illegal_procedure();
}

int reroll(int value)
{
  return api->reroll(value);
}

int choose_kickoff(int value)
{
  return api->choose_kickoff(value);
}

int place_ball()
{
  return api->place_ball();
}

int give_ball()
{
  return api->give_ball();
}

int choose_block_dice()
{
  return api->choose_block_dice();
}

int block_push()
{
  return api->block_push();
}

int follow(int value)
{
  return api->follow(value);
}

int declare(int action)
{
  return api->declare(action);
}

int move_player(int x, int y)
{
  return api->move_player(x, y);
}

int stand_up_player()
{
  return api->stand_up_player();
}

int block_player(int id)
{
  return api->block_player(id);
}

int pass_ball(int x, int y)
{
  return api->pass_ball(x, y);
}


}
