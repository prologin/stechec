/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2007 Prologin
*/

#include "GameData.hh"
#include <ctime>
#include <cstdlib>
#include <deque>
#include <map>

//todo, better, call Init() from BeforeNewGame, etc..
#define INIT()					\
  assert(initialized_);

static const int directions4[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
static const int directions8[8][2] = {{0,1},{0,-1},{1,0},{-1,0},
				      {1,1},{1,-1},{-1,-1},{-1,1}};

static bool dans_les_bornes(int y, int x) {
  return y >= 0 && x >= 0 && x < TAILLE_CARTE && y < TAILLE_CARTE;
}

GameData::GameData()
{
  srand(time(0));

  start_team_ = -1;
  nb_virtual_turns_ = -1;
  nb_monuments_ = -1;

  // fill:
  std::fill(argent_, argent_ + MAX_TEAMS, ARGENT_INITIAL);
  std::fill(score_, score_ + MAX_TEAMS, 0);
  std::fill(*constructions_, *constructions_ + TAILLE_CARTE * TAILLE_CARTE, std::make_pair(VIDE, -1));
  std::fill(*valeurs_cases_, *valeurs_cases_ + TAILLE_CARTE * TAILLE_CARTE, 0);
  constructions_[TAILLE_CARTE / 2][TAILLE_CARTE / 2].first = ROUTE;

  monument_en_cours_ = -1;
  won_auction_ = -1;
  std::fill(encheres_, encheres_ + MAX_TEAMS, 0);
}

void GameData::Init() {
  assert(getNbTeam() <= MAX_TEAMS);
  nb_virtual_turns_ = getNbTeam() * 2 + 1;
  LOG4("There are %1 virtual turns", nb_virtual_turns_);
  initialized_ = true;
}

void GameData::InitDistances() {
  for (int i = 0 ; i < TAILLE_CARTE ; ++i) {
    for (int j = 0 ; j < TAILLE_CARTE ; ++j) {      
      for (int l = 0 ; l <= MAX_DISTANCE ; ++l) {
	for (int c = 0 ; c <= MAX_DISTANCE * 2 ; ++c) {
	  distances_[i][j][l][c] = static_cast<unsigned char>(l + std::abs(c - MAX_DISTANCE));
	}
      }
    }
  }
  NotifyCellModified(TAILLE_CARTE / 2, TAILLE_CARTE / 2);

  ComputeNonBlockingCells();
}


void GameData::NotifyCellModified(int x, int y) {
  assert(dans_les_bornes(x, y));
  for (int l = 0 ; l <= MAX_DISTANCE * 2; ++l) {
    for (int c = 0 ; c <= MAX_DISTANCE * 2 ; ++c) {
      if (std::abs(l - MAX_DISTANCE) + std::abs(c - MAX_DISTANCE) > MAX_DISTANCE) {
	continue;
      }
      int yy = y + l - MAX_DISTANCE;
      int xx = x + c - MAX_DISTANCE;
      if (dans_les_bornes(xx, yy)) {
	RecomputeDistanceFrom(xx, yy);
      }
    }
  }
}

struct Pos {
  int l,c,d;
};

void GameData::RecomputeDistanceFrom(int x, int y) {
  assert(dans_les_bornes(x, y));
  
  for (int l = 0 ; l <= MAX_DISTANCE ; ++l) {
    for (int c = 0 ; c <= MAX_DISTANCE * 2 + 1 ; ++c) {
      distances_[y][x][l][c] = MAX_DISTANCE + 1;
    }
  }

  const int type_case = constructions_[y][x].first;
  // Only empty cells and cells that we have reserved can be used.
  if (type_case != VIDE &&
      (type_case != RESERVATION || constructions_[y][x].second != getTeamId()) && 
      type_case != ROUTE) {
    return;
  }  
  
  // BFS from (y, x)
  bool visited[MAX_DISTANCE * 2 + 1][MAX_DISTANCE * 2 + 1];
  std::fill(*visited, *visited + (MAX_DISTANCE * 2 + 1) * (MAX_DISTANCE * 2 + 1), false);
  // positions are stored as (l,c). Real position is (y + l - MAX_DISTANCE, x + c - MAX_DISTANCE).
  // Do not forget to change l before filling out the distances_ array!
  std::deque<Pos> file;
  file.push_back((Pos) {MAX_DISTANCE, MAX_DISTANCE, 0});

  while (!file.empty()) {
    const Pos cur = file.front();
    file.pop_front();
    if (cur.l < 0 || cur.c < 0 || cur.l > MAX_DISTANCE * 2 || cur.c > MAX_DISTANCE * 2) {
      continue;
    }
    if (std::abs(cur.l - MAX_DISTANCE) + std::abs(cur.c - MAX_DISTANCE) > MAX_DISTANCE) { // Too far from (y,x).
      continue;
    }
    const int cury = y + cur.l - MAX_DISTANCE;
    const int curx = x + cur.c - MAX_DISTANCE;
    if (cury < 0 || curx < 0 || cury >= TAILLE_CARTE || curx >= TAILLE_CARTE) {
	continue;
    }
    const int type_case = constructions_[cury][curx].first;
    // Seulement les cases vides et que l'on a réservées peuvent être parcourues (ainsi que
    // les routes).
    if (type_case != VIDE &&
       (type_case != RESERVATION || constructions_[cury][curx].second != getTeamId()) &&
	type_case != ROUTE) {
      continue;
    }
    if (visited[cur.l][cur.c]) {
      continue;
    }
    visited[cur.l][cur.c] = true;
    if (cur.l >= MAX_DISTANCE && cur.d <= MAX_DISTANCE) { // update the distance only when we are in the right zone (triangle under (x,y)).
      distances_[y][x][cur.l - MAX_DISTANCE][cur.c] = cur.d;
    }
    for (int k = 0 ; k < 4 ; k++) {
      const Pos next_pos = {cur.l + directions4[k][0],
			    cur.c + directions4[k][1],
			    cur.d + 1};
      file.push_back(next_pos);
    }
  }
}

// [0, nb_team) -> phase 1
// [nb_team,nb_team+1) -> phase 2
// [nb_team+1, nb_team+1+nb_team) -> phase 3
int GameData::GetCurrentPhase() {
  INIT();
  int t = getNbTeam();
  int k = getCurrentTurn();
  k = (k-1) % nb_virtual_turns_;
  if (k < t) return 1;
  if (k == t) return 2;
  if (k < 2 * t + 1) return 3;
  assert(0);
}

int GameData::GetRealTurn() {
  INIT();
  int k = getCurrentTurn();
  return 1 + (k - 1) / nb_virtual_turns_;
}

bool GameData::IsPhase1Ending() {
  INIT();
  if (GetCurrentPhase() != 1) return false;
  int k = (getCurrentTurn() - 1) % nb_virtual_turns_;
  return k == getNbTeam() - 1;
}

int GameData::GetCurrentPlayer() {
  INIT();
  if (GetCurrentPhase() == 2)
    return -1;
  int t = getNbTeam();
  int k = getCurrentTurn();
  k = (k-1) % nb_virtual_turns_;
  if (GetCurrentPhase() == 3) {
    // todo, faire en sorte que celui qui a gagné l'enchère soit toujours le premier/le dernier ?
    k -= t + 1;
    return k;
  }
  if (GetCurrentPhase() == 1) {
    assert(k >= 0 && k < getNbTeam());
    int res = (start_team_ + k + GetRealTurn() - 1) % getNbTeam();
    return res;
  }
  assert(0);
}

int GameData::CoutConstructions(int routes, int maisons) {
  INIT();
  if (maisons >= 3) return INFINI;
  maisons = std::max(maisons - 1, 0);
  routes = std::max(routes - 1, 0);
  return maisons + (routes - maisons + 1) / 2;
}


// A couple of the following functions computes the non blocking cells.

// Do a DFS from the outside of the game, and see which road cells are accessible.
// Get the paths to go to these road cells. Blocking cells are those through which go
// all these paths.

typedef std::pair<int, int> pii;
typedef std::set<pii> spii;
typedef std::vector<pii> vpii;

static int visited[TAILLE_CARTE][TAILLE_CARTE];
static pii prev[TAILLE_CARTE][TAILLE_CARTE];
static spii blocking_candidates;

struct PosBlock {
  short y,x,py,px;
};

static void bfs(GameData *g) {
  std::deque<PosBlock> file;
  std::fill(*visited, *visited + TAILLE_CARTE * TAILLE_CARTE, 0);
  std::fill(*prev, *prev + TAILLE_CARTE * TAILLE_CARTE, std::make_pair(-1, -1));
  for (int i = 0 ; i < TAILLE_CARTE ; i++) {
    file.push_back((PosBlock){0, i, -1,-1});
    file.push_back((PosBlock){i, 0, -1,-1});
    file.push_back((PosBlock){i, TAILLE_CARTE - 1, -1,-1});
    file.push_back((PosBlock){TAILLE_CARTE - 1, i, -1,-1});
  }

  // Visiter toutes les cases (Les seules cases franchissables sont les cases vides)
  // Et construire l'arborescence retour.
  while (!file.empty()) {
    const PosBlock cur = file.front();
    file.pop_front();
    if (g->constructions_[cur.y][cur.x].first != VIDE) {
      continue;
    }
    int& v = visited[cur.y][cur.x];
    if (v) continue;
    v = true;
    prev[cur.y][cur.x] = std::make_pair(cur.py, cur.px);
    for (int k = 0 ; k < 4 ; k++) {
      const int yy = cur.y + directions4[k][0];
      const int xx = cur.x + directions4[k][1];
      if (dans_les_bornes(yy, xx)
	  && !visited[yy][xx] && g->constructions_[yy][xx].first == VIDE) {
	file.push_back((PosBlock){yy, xx, cur.y, cur.x});
      }
    }
  }
}

static void GetBlockingCandidates(GameData *g) {
  // A partir des routes, on regarde tous les chemins retours possibles.
  // Une case visitée par ce processus est mis à 2.
  bool found_one_road = false;
  blocking_candidates.clear();
  for (int y = 0 ; y < TAILLE_CARTE && (!found_one_road || !blocking_candidates.empty()) ; ++y) {
    for (int x = 0 ; x < TAILLE_CARTE ; ++x) {
      if (g->constructions_[y][x].first == ROUTE) {
	for (int k = 0 ; k < 4 ; k++) {
	  int yy = y + directions4[k][0];
	  int xx = x + directions4[k][1];
	  if (dans_les_bornes(yy, xx) && visited[yy][xx] == 1) {
	    assert(g->constructions_[yy][xx].first == VIDE);
	    spii path_cells;
	    // Suivre le chemin à partir de (yy,xx).
	    while (yy != -1 && xx != -1 && visited[yy][xx] == 1) {
	      visited[yy][xx] = 2;
	      // Prune cells that are sourrounded by 8 empty cells. (hence, they can't be blocking cells).
	      bool prune = true;
	      for (int p = 0 ; p < 8 ; ++p) {
		int l = yy + directions8[p][0];
		int c = xx + directions8[p][1];
		if (!dans_les_bornes(l, c) || g->constructions_[l][c].first != VIDE) {
		  prune = false;
		}
		  
	      }
	      if (!prune) {
		path_cells.insert(std::make_pair(yy, xx));
	      }
	      int nxx = prev[yy][xx].second;
	      int nyy = prev[yy][xx].first;
	      yy = nyy, xx = nxx;
	    }
	    // intersecting blocking_candidates and path_cells.
	    if (found_one_road) {
	      vpii out;
	      std::set_intersection(path_cells.begin(), path_cells.end(), blocking_candidates.begin(), blocking_candidates.end(),
				    std::back_inserter(out));
	      blocking_candidates = std::set<std::pair<int, int> >(out.begin(), out.end());
	    } else {
	      found_one_road = true;
	      blocking_candidates = path_cells;
	    }
	  }
	}
      }
    }
  }

}

static bool CanReachRoad(int x, int y, int forbiddenx, int forbiddeny, GameData *g) {
  if (x == forbiddenx && y == forbiddeny)
    return false;
  if (visited[y][x]) return false;
  visited[y][x] = true;
  if (g->constructions_[y][x].first == ROUTE) {
    return true;
  }
  if (g->constructions_[y][x].first != VIDE) {
    return false;
  }

  for (int k = 0 ; k < 4; k++) {
    const int xx = x + directions4[k][0];
    const int yy = y + directions4[k][1];
    if (dans_les_bornes(xx, yy) && 
	!visited[yy][xx] &&
	(g->constructions_[yy][xx].first == VIDE || g->constructions_[yy][xx].first == ROUTE)) {
      bool can = CanReachRoad(xx, yy, forbiddenx, forbiddeny, g);
      if (can) {
	return true;
      }
    }
  }
  return false;
}

void GameData::ComputeNonBlockingCells() {
  INIT();

  bfs(this);

  GetBlockingCandidates(this);

  std::fill(*cases_non_blocantes_, *cases_non_blocantes_ + TAILLE_CARTE * TAILLE_CARTE, true);

  // For each candidate, explicitely check whether the cell is blocking or not.
  for (spii::const_iterator it = blocking_candidates.begin() ; it != blocking_candidates.end() ; ++it) {
    const pii& cur = *it;
    std::fill(*visited, *visited + TAILLE_CARTE * TAILLE_CARTE, 0);
    bool can = false;
    for (int k = 0 ; k < TAILLE_CARTE && !can; ++k) {
      can = can || CanReachRoad(0, k, cur.second, cur.first, this)	
	|| CanReachRoad(k, 0, cur.second, cur.first, this)
	|| CanReachRoad(TAILLE_CARTE - 1, k, cur.second, cur.first, this)
	|| CanReachRoad(k, TAILLE_CARTE - 1, cur.second, cur.first, this);
    }
    if (!can) {
      // The cell is blocking!
      LOG4("Cell y=%1, x=%2 is blocking", cur.first, cur.second);
      cases_non_blocantes_[cur.first][cur.second] = false;
    }
  }
}

void GameData::InitTemporaryData() {
  INIT();
  ordre_ = 0;
  maisons_construires_ = 0;
  routes_construites_ = 0;
  did_auction_ = false;
}

void GameData::ComputeSecondaryData() {
  INIT();
  // First, valeurs_cases_ :
  std::fill(*valeurs_cases_, *valeurs_cases_ + TAILLE_CARTE * TAILLE_CARTE, 0);
  for (int y = 0 ; y < TAILLE_CARTE ; ++y) {
    for (int x = 0 ; x < TAILLE_CARTE ; ++x) {
      if (constructions_[y][x].first == MONUMENT) {
	int type = constructions_[y][x].second;
	int prestige = monuments_[type].first;
	int reach = monuments_[type].second;
	for (int i = y - reach ; i <= y + reach ; ++i) {
	  for (int j = x - reach ; j <= x + reach ; ++j) {
	    if (i < 0 || j < 0 || i >= TAILLE_CARTE || j >= TAILLE_CARTE)
	      continue;
            valeurs_cases_[i][j] += prestige;
	  }
	}
      }
    }
  }
  //score_ :
  for (int t = 0 ; t < MAX_TEAMS ; ++t)
    score_[t] = 0;
  for (int y = 0 ; y < TAILLE_CARTE ; ++y) {
    for (int x = 0 ; x < TAILLE_CARTE ; ++x) {
      if (constructions_[y][x].first == MAISON) {
	score_[constructions_[y][x].second] += std::max(1, valeurs_cases_[y][x] + 1);
      }
    }
  }
}

void GameData::ApplyBuildOrder(e_com_type type, int param[10]) {
  INIT();
  int x = param[0];
  int y = param[1];
  assert(dans_les_bornes(x, y));
  switch (type) {
  case BUILD_ROAD:
    constructions_[y][x] = std::make_pair(ROUTE, -1);
    break;
  case BUILD_HOUSE: 
    {
      int team = param[2];
      assert(team >= 0 && team < getNbTeam());
      constructions_[y][x] = std::make_pair(MAISON, team);
      break;
    }
  case RESERVE_CELL:
    {
      int team = param[2];
      assert(team >= 0 && team < getNbTeam());
      constructions_[y][x] = std::make_pair(RESERVATION, team);
      break;
    }
  case DESTROY_HOUSE:    
    constructions_[y][x] = std::make_pair(VIDE, -1);
    break;
      
  case SELL_HOUSE:
    constructions_[y][x] = std::make_pair(MAISON, MAIRIE);
    break;
    
  case BUILD_MONUMENT:
    { 
      int m = param[2];
      assert(m >= 0 && m < MAX_MONUMENTS);
      constructions_[y][x] = std::make_pair(MONUMENT, m);
      break;
    }
  default:
    assert(0);
  }
}

int GameData::GetFirstPlayerPhase1() {
  INIT();
  int team = start_team_ + GetRealTurn() - 1 + (GetCurrentPhase() != 1);
  team %= getNbTeam();
  return team;
}

bool GameData::MakeChecks(bool server) {
  INIT();
#ifndef NDEBUG
  assert(start_team_ >= 0 && start_team_ < getNbTeam());
  
  for (int t = 0 ; t < getNbTeam() ; ++t) {
    assert(argent_[t] >= 0);
    assert(score_[t] >= 0);
  }

  assert(GetRealTurn() == 1 || monument_en_cours_ >= 0 && monument_en_cours_ < MAX_MONUMENTS);
  
  if (!server) {
    assert(GetRealTurn() == 1 || won_auction_ >= 0 && won_auction_ < getNbTeam());

    // Check the map
    for (int i = 0 ; i < TAILLE_CARTE ; i++) {
      for (int j = 0 ; j < TAILLE_CARTE ; j++) {
	const int t = constructions_[i][j].first;
	const int a = constructions_[i][j].second;
	if (t == MAISON)
	  assert(a >= 0 && a < getNbTeam() || a == MAIRIE);
	if (t == RESERVATION)
	  assert(a>=0 && a < getNbTeam());
	if (t != MONUMENT && t != MAISON && t != RESERVATION)
	  assert(a == -1);
	if (t == MONUMENT)
	  assert(a >= 0 && a < MAX_MONUMENTS);
      }
    }

    // Check if the scores match.
    static int values[MAX_TEAMS][TAILLE_CARTE][TAILLE_CARTE];
    std::fill(**values, **values + MAX_TEAMS * TAILLE_CARTE * TAILLE_CARTE, 0);
    int score[MAX_TEAMS] = {0};
    for (int i = 0 ; i < TAILLE_CARTE ; i++) {
      for (int j = 0 ; j < TAILLE_CARTE ; j++) {
	if (constructions_[i][j].first == MAISON)
	  values[constructions_[i][j].second][i][j]++;
	if (constructions_[i][j].first == MONUMENT) {
	  const int m = constructions_[i][j].second;
	  const int l = monuments_[m].second;
	  const int p = monuments_[m].first;
	  for (int y = std::max(0, i-l) ; y < std::min(TAILLE_CARTE, i+l+1) ; y++)
	    for (int x = std::max(0,j-l) ; x < std::min(TAILLE_CARTE, j+l+1) ; x++)
	      if (constructions_[y][x].first == MAISON) {
		values[constructions_[y][x].second][y][x] += p;
	      }
	}
      }
    }
    for (int t = 0 ; t < MAX_TEAMS ; t++) {
      for (int i = 0 ; i < TAILLE_CARTE ; i++) {
	for (int j = 0 ; j < TAILLE_CARTE ; j++) {
	  if (constructions_[i][j].first == MAISON && constructions_[i][j].second == t)
	    score[t] += std::max(1, values[t][i][j]);
	}
      }
    }
    for (int t = 0 ; t < MAX_TEAMS ; t++) {
      //      ERR("score team %1 by GameData : %2, by computation : %3", t, score_[t], score[t]);
      assert(score[t] == score_[t]);
    }
  }

  // Check that there is at most one monument of each kind in the map.
  bool seen[MAX_MONUMENTS] = {0};
  for (int i = 0 ; i < TAILLE_CARTE ; i++) {
    for (int j = 0 ; j < TAILLE_CARTE ; j++) {
      if (constructions_[i][j].first == MONUMENT) {
	const int k = constructions_[i][j].second;
	assert(k >= 0 && k < MAX_MONUMENTS);
	assert(seen[k] == false);
	seen[k] = true;
      }
    }
  }
#endif
}
