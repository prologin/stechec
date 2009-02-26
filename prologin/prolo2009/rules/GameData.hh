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

#ifndef GAMEDATA_HH_
# define GAMEDATA_HH_

# include <list>
# include <map>
# include <algorithm>
# include <map>
# include "Contest.hh"
# include "Constant.hh"

/*!
** This class is meant to contain all data, accessible from
** everywhere.
*/
class GameData: public StechecGameData
{
public:
  GameData();
  // Call this before everything else.
  void Init();

  // only for the server:
  int nb_monuments_;
  int monument_order_[MAX_MONUMENTS];

  // orders that will be broadcasted to other clients, in the intended order.
  // No two orders will affect the same cell (that is, they can be applied in any order)
  // (they are filtered in ServerResolver and re-transmitted in ServerEntry)
  // [][0] -> type
  // [][i], i>0 -> parameters
  // Only for the server.
  // The last getNbTeam() orders will always be of type auction when
  // in phase 2.
  int pass_threw_orders_[MAX_ORDERS][MAX_PARAM_PER_ORDER + 1]; 
  int nb_pass_threw_orders_;

  // To manage the turns.
  // A game's turn is divided into 7 (3+1+3) turns.
  int start_team_;
  int nb_virtual_turns_; // number of virtual turn per real game's turn (3+1+3 with 3 players).
  // Returns 1,2 or 3 : the current phase of the game.
  int GetCurrentPhase();
  bool IsPhase1Ending();
  int GetRealTurn();
  // Returns the current player during the first phase (according to the game
  // order), -1 during the second, and sequentially 0,1,2 during the third
  // phase (anyways, only the player winning the auction will be allowed to
  // play).
  int GetCurrentPlayer();
  int GetFirstPlayerPhase1();

  // Primary game data (that cannot be deduced from others)
  // updated by ClientDiffer *and* during the turn.
  // Note that for the currently playing player, they will be updated twice :
  // first during its turn, and secondly when the server broadcasts the orders to everybody.
  // Of course, argent_ will be updated correctly.
  int argent_[MAX_TEAMS];
  // (type de construction, appartenance pour les maisons ou réservations | type de monument | -1).
  std::pair<int,int> constructions_[TAILLE_CARTE][TAILLE_CARTE];
  // (prestige, portee)
  std::pair<int,int> monuments_[MAX_MONUMENTS];

  int encheres_[MAX_TEAMS]; // 0 if not set
  int monument_en_cours_; // -1 if not set
  int won_auction_; // -1 if not set.

  // Secondary game data (only to speedup/simplify).
  // client's side
  // Updated at the beginning of each turn (and during the turn)
  // by clients.
  // Although this could be re-computed at each turn, it is only updated incrementally, during the player's
  // turn and also by the server.
  int score_[MAX_TEAMS]; // updated during turns by the clients, tracked by the server and broadcasted
  // to everybody thanks to NEW_SCORE messages.

  // peut être négatif. C'est seulement la somme des prestiges des batiments à portée. (ie, vaut 0 lorsqu'il n'y a rien).
  int valeurs_cases_[TAILLE_CARTE][TAILLE_CARTE];
  // true iff we can build a house/monument without blocking the game.
  // (ie : we can always access a road by 4-connectedness from the outside.)
  // Only used for empty cells. (if the cell is not empty, the value is ignored).
  bool cases_non_blocantes_[TAILLE_CARTE][TAILLE_CARTE];

  // distance array, only used by clients.
  // [y][x][l][c] represents the distance between (x,y) and (x - MAX_DISTANCE + c, y + l).
  // so 0 <= c <= MAX_DISTANCE * 2, 0 <= l <= MAX_DISTANCE.
  // with TAILLE_CARTE = 100 and MAX_DISTANCE = 20, occupies < 8.5MB.
  // Contains something >= MAX_DISTANCE + 1 if any of the two endpoints are in an occupied cell.
  unsigned char distances_[TAILLE_CARTE][TAILLE_CARTE][MAX_DISTANCE + 1][MAX_DISTANCE * 2 + 1];

  // Temporary data for a turn (for client).
  // updated at the beginning of each turn (and during the turn)
  // by the client.
  // Compteur des ordres envoyés, juste pour les appliquer dans le bon ordre côté serveur.
  int ordre_;
  int maisons_construires_;
  int routes_construites_;
  int CoutConstructions(int routes, int maisons);
  bool did_auction_;

  void ComputeNonBlockingCells();
  // Update functions, to be called before each turn.
  void InitTemporaryData();
  void ComputeSecondaryData();

  // Init distances, to be called once by the clients at the beginning of the game.
  // Assumes that the games starts with one road cell at TAILLE_CARTE/2, TAILLE_CARTE/2
  // and every other cell empty.
  void InitDistances();

  // To be called everytime a cell (x,y) is modified (build/destroy/reserved)
  // This will update the distances accordingly.
  // Cost : 2 * MAX_DISTANCE^2 * Cost(RecomputeDistanceFrom)
  // = 16 * MAX_DISTANCE^4 = 160000
  void NotifyCellModified(int x, int y);

  // Recomputes all the distances from the point (x,y).
  // Cost : 8 * MAX_DISTANCE^2 
  void RecomputeDistanceFrom(int x, int y);

  // Perform some checks to see if all is going well.
  // Quite fast and not harmful : you can do this whenever you want.
  bool MakeChecks(bool server = false);

  // performs some tests for the detection of the blocking cells (ComputeNonBlockingCells function).
  // Crashs if there is a bug
  // Call this only for testing purposes, it modifies the game !
  void UnitTestBlockingCells();

private:
  bool initialized_;

};

#endif // !GAMEDATA_HH_
