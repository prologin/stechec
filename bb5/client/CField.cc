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

#include "CField.hh"

struct ScorePoint : public Position
{
  int                score;
  int                ma;        ///< Player movement remaining.
  ScorePoint*        from;      ///< Previous position, to retrieve the path.
};


CField::CField()
{
  path_tab_ = new ScorePoint[COLS * ROWS];

  for (int i = 0; i < ROWS; i++)
    for (int j = 0; j < COLS; j++)
      {
        tab_[i * COLS + j] = NULL;
        path_tab_[i * COLS + j].row = i;
        path_tab_[i * COLS + j].col = j;
      }
}

CField::~CField()
{
  delete[] path_tab_;
}

ScorePoint* CField::extractMin()
{
  ScorePoint*        min = cur_pt_list_.begin()->second;
  cur_pt_list_.erase(cur_pt_list_.begin());
  LOG6("--- extract min: %1", *min);
  return min;
}

int CField::getScoreModifier(const ScorePoint& cur,
			     const ScorePoint& prev,
			     int team_id)
{
  int        mod = 0;

  // Check if a player of the other team can tackles.
  mod += getNbTackleZone(team_id == 1 ? 2 : 1, cur) * 5;
 
  // Decrease probability if we go farther.
  // (speed up algorithm - do not change result)
  if (cur.distance(*goal_) > prev.distance(*goal_))
    mod += 3;

  return mod;
}


// Dijkstra like, with a pinch of A star.
bool CField::getMinPath(int team_id)
{
  ScorePoint*        m = extractMin();

  // We reach the goal.
  if (*m == *goal_)
    return true;
  
  // Can't move any more.
  if (!m->ma)
    return false;

  // Try to move on all adjacent points.
  for (int di = -1, i; di <= 1; di++)
    for (int dj = -1, j; dj <= 1; dj++)
      if ((di != 0 || dj != 0) &&
          (i = m->row + di) >= 0 && i < ROWS &&
          (j = m->col + dj) >= 0 && j < COLS &&
          !getPlayer(Position(i, j)))
        {
          ScorePoint* cur = &path_tab_[i * COLS + j];
          int new_score = m->score + 1 + getScoreModifier(*cur, *m, team_id);
          LOG6("new score: %1 prev: %2", new_score, m->score);
          if (new_score < cur->score)
            {
              cur->from = m;
              cur->score = new_score;
              cur->ma = m->ma - 1;
              LOG6("+ push %1 ma %2", *cur, cur->ma);
              cur_pt_list_.insert(std::make_pair(new_score, cur));
            }
        }
  return false;
}

const PosList& CField::getPath(const Position& start,
			       const Position& dest,
			       CPlayer* p)
{
  // Reinit path_tab_.
  for (int i = 0; i < COLS * ROWS; i++)
    {
      path_tab_[i].score = 99999;
      path_tab_[i].from = NULL;
    }

  // Initialize start and dest points.
  ScorePoint* sp_start = &path_tab_[start.row * COLS + start.col];
  sp_start->score = 0;
  sp_start->ma = p->getMaRemain();
  cur_pt_list_.insert(std::make_pair(0, sp_start));
  goal_ = &path_tab_[dest.row * COLS + dest.col];

  path_.clear();
  while (!cur_pt_list_.empty())
    {
      if (getMinPath(p->getTeamId()))
        {
          // Yeah, found it ! Recover the used path, then return.
          ScorePoint* cur = goal_;
          while (*cur != start)
            {
              path_.push_front(Position(cur->row, cur->col));
              cur = cur->from;
            }
          cur_pt_list_.clear();
          break;
        }
    }
  return path_;
}
