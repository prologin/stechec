/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2011 Prologin
*/

#include "Utils.hh"
#include "GameData.hh"
#include "Constant.hh"

#include <ctime>
#include <cstdlib>
#include <set>
#include <deque>

// TODO: better, call Init() from BeforeNewGame, etc..
#define INIT()					\
  assert(initialized_);

Case::Case()
    : type(VIDE),
      bonus(PAS_BONUS),
      nb_trainees_moto(0),
      source_id(-1)
{
}

Joueur::Joueur(int id)
    : score(0),
      bonus()
{
    this->id = id;
}

bool Joueur::is_able(type_bonus	b)
{
    return (get_bonus(b) != bonus.end());
}

erreur Joueur::use_capacity(type_bonus	b)
{
    bonus_list::iterator	it;

    it = get_bonus(b);
    if (it == bonus.end())
	return (BONUS_INVALIDE);
    bonus.erase(it);
    return (OK);
}

Joueur::bonus_list::iterator
Joueur::get_bonus(type_bonus	b)
{
    bonus_list::iterator	it;
    for (it = bonus.begin(); it != bonus.end(); ++it)
	if (*it == b)
	    return (it);
    return (bonus.end());
}

source_energie  SourceEnergie::to_source_energie(int indice)
{
    source_energie  s;
    s.id = indice;
    s.pos = pos;
    s.capacite = potentiel_cur;
    s.capacite_max = potentiel_max;
    return (s);
}

void SourceEnergie::set_potentiel(int potentiel)
{
    potentiel_max = potentiel;
    potentiel_cur = potentiel;
}

int SourceEnergie::regenerer()
{
    int old_potentiel = potentiel_cur;
    potentiel_cur = potentiel_max;
    return (old_potentiel);
}

void SourceEnergie::reset(int old_potentiel)
{
    potentiel_cur = old_potentiel;
}

void SourceEnergie::consume(int degree)
{
    if (potentiel_max < 0)
	degree = -degree;
    if ((potentiel_cur - degree) * potentiel_max < 0)
	// If (potentiel_cur - degree) and potentiel_max does not have the same
	// sign and the first one is not null.
	potentiel_cur = 0;
    else
	potentiel_cur -= degree;
}

void SourceEnergie::release()
{
    if (potentiel_cur > potentiel_max)
	--potentiel_cur;
    else if (potentiel_cur < potentiel_max)
	++potentiel_cur;
}

GameData::GameData()
    // FIXME: use a constant to but the last team's id
    : current_player(1),
      can_play(true)
{
    int		taille = TAILLE_TERRAIN * TAILLE_TERRAIN;
    Case	default_case;

    terrain_.reserve(taille);
    for (int i = 0; i < taille; ++i)
	terrain_.push_back(default_case);

    joueurs.reserve(2);
    for (int i = 0; i < 2; ++i)
	joueurs.push_back(Joueur(i));
}

void GameData::Init() {
  LOG2("GameData::Init");
  initialized_ = true;
}

Case&
GameData::get_case(int x, int y)
{
    if (position_invalide(x, y))
	abort();
    return terrain_[x + TAILLE_TERRAIN * y];
}

Case&
GameData::get_case(const position &p)
{
  return get_case(p.x, p.y);
}

int GameData::get_free_moto_id()
{
    // Even if it is inefficient, it should work while there’s no more than
    // 2^(sizeof (int) * 8) motos. :-)

    int result = 0;
    while (moto_valide(result))
	++result;
    return result;
}

void GameData::get_next_pos(const position& p,
			    std::vector<position>& next_pos)
{
    position next_p;
    next_pos.reserve(4);

    next_p.x = p.x;
    next_p.y = p.y - 1;
    next_pos.push_back(next_p);
    next_p.y = p.y + 1;
    next_pos.push_back(next_p);

    next_p.y = p.y;
    next_p.x = p.x - 1;
    next_pos.push_back(next_p);
    next_p.x = p.x + 1;
    next_pos.push_back(next_p);
}

bool GameData::is_crossable_pos(const position& p)
{
    if (position_invalide(p))
	return false;
    Case& c = get_case(p);
    if ((c.type == VIDE && c.nb_trainees_moto == 0)
	|| c.type == POINT_CROISEMENT)
	return true;
    return false;
}

void GameData::build_from_reverse_path(const position& reverse_begin,
				       const position& reverse_end,
				       std::map<pair_position, position>& back_path,
				       std::vector<position>& path)
{
    std::vector<position> reverse_path;

    int length = 0;
    path.clear();
    reverse_path.push_back(reverse_begin);
    while (reverse_path[length] != reverse_end)
    {
	pair_position current_pos = pos_to_pair(reverse_path[length]);
	reverse_path.push_back(back_path[current_pos]);
	++length;
    }
    path.reserve(reverse_path.size());
    for (int i = length - 1; i >= 0; --i)
	path.push_back(reverse_path[i]);
    //LOG4("Path's length: %1", reverse_path.size());
}

/*
 * Build a shortest path betwee 'begin' and 'end' in 'path', or make 'path'
 * empty if there's no such path.
 */
void GameData::get_shortest_path(const position& begin,
				 const position& end,
				 std::vector<position>& path)
{
  /*LOG4("Processing a shortest path betwee (%1, %2) and (%3, %4)",
	 begin.x, begin.y,
	 end.x, end.y);
  */
    // Proceed a BFS and keep in memory for each cell the previous cell
    // (building a reverse path)
    std::deque<position> fifo;
    std::map<pair_position, position> back_path;

    path.clear();
    // We don't mind if the beginning is not crossable
    if (!is_crossable_pos(end))
	// One end is not crossable: there's no path
	return;
    LOG5("The goal position is crossable: continue...");

    fifo.push_back(begin);
    back_path[pos_to_pair(begin)] = begin;
    while (!fifo.empty())
    {
	position p = fifo[0];
	fifo.pop_front();
	LOG5("Processing the position (%1, %2)", p.x, p.y);

	// Build the set of the adjacent positions
	std::vector<position> next_pos;
	get_next_pos(p, next_pos);

	for (int i = 0; i < next_pos.size(); ++i)
	{
	    pair_position next_p = pos_to_pair(next_pos[i]);

	    if (back_path.find(next_p) != back_path.end() ||
		!is_crossable_pos(next_pos[i]))
		// If this position is already visited or if this position is
		// not crossable, do not process it
		continue;
	    LOG5("The next position (%1, %2) is valid", next_p.first, next_p.second);

	    // The previous position of the next one is the current one
	    back_path[next_p] = p;

	    // If we reached the end, clear and exit. Continue otherwise.
	    if (next_pos[i] == end)
	    {
		LOG5("We reached the end!");
		fifo.clear();
		break;
	    }
	    else
		fifo.push_back(next_pos[i]);
	}
    }
    if (back_path.find(pos_to_pair(end)) == back_path.end())
	// If the end could not be reached, there's no path
	return;
    build_from_reverse_path(end, begin, back_path, path);
}

InternalTraineeMoto&
GameData::creer_trainee_moto(int player, position init, int max_len)
{
    int	id = get_free_moto_id();
    InternalTraineeMoto moto(this, player, id, init, max_len);
    motos.insert(std::pair<int, InternalTraineeMoto>(id, moto));
    return motos[id];
}

bool GameData::moto_valide(int id)
{
    return (motos.find(id) != motos.end());
}

void GameData::supprimer_moto(int id)
{
    motos.erase(id);
}

void GameData::lookup_trainee_case(const position& pos,
				   std::vector<int>& trainees)
{
    motos_type::const_iterator it;

    for (it = motos.begin(); it != motos.end(); ++it)
	if (it->second.contains(pos))
	    trainees.push_back(it->first);
}

bool GameData::source_valide(int id)
{
    return (0 <= id && id < sources.size());
}

void GameData::team_switched(){
    LOG4("GameData::team_switched");
    for (motos_type::const_iterator it = motos.begin(); it != motos.end(); ++it){
      if (it->second.player_ == current_player){
	motos[it->second.id_].actif = false;
      }
    }
    // Update scores and potentiels each time everybody have played
    if (current_player % 2 == 0)
    {
	apply_connections(true);
	LOG4("Scores and potentiels have been updated!");
	for (int i = 0; i < 2; ++i)
	    LOG4("  - Team %1 : %2", i, joueurs[i].score);
    }

    actions_stockees = actions_stockees_buffer;
    actions_stockees_buffer.clear();

    can_play = true;
    current_player = (current_player + 1 ) % 2;
    // FIXED by PM: reset point d'actions
    remaining_pa_ = MAX_PA;

    /* This code tests the path finding
    static int tour = 0;
    ++tour;
    if (tour != 6)
	return;
    std::cout << "Shortest path from (0, 0) to (49, 49):" << std::endl;
    std::vector<position> path;
    position begin;
    begin.x = 0;
    begin.y = 0;
    position end;
    end.x = TAILLE_TERRAIN - 1;
    end.y = TAILLE_TERRAIN - 1;
    get_shortest_path(begin, end, path);
    for (std::vector<position>::iterator it = path.begin();
	 it != path.end();
	 ++it)
	std::cout << "-> (" << it->x << ", " << it->y << ")" << std::endl;
    */

    // decrementation des sources

    for (int x = 0; x < TAILLE_TERRAIN; x++){
      for (int y = 0; y < TAILLE_TERRAIN; y++){
	position p = {x, y};
	Case& c = get_case(p);
	if (c.source_id != -1){
	  SourceEnergie& src = sources[c.source_id];
	  position p2;
	  bool connected = false;
	  for (p2.x = x - 1; p2.x <= x + 1; p2.x++){
	    for (p2.y = y - 1 ; p2.y <= y + 1; p2.y++){
	      Case& c2 = get_case(p2);
	      connected = connected || ( c2.nb_trainees_moto != 0 && actif(p) );
	    }
	  }
	  if (!connected) src.release();
	}
      }
    }
}

bool GameData::actif(position pos){
  bool actif = false;
  for (motos_type::const_iterator it = motos.begin(); it != motos.end(); ++it){
    if (it->second.contains(pos)){
      actif = actif || it->second.actif;
    }
  }
  return actif;
}

void GameData::check(const char * file, int line){
  LOG3("%1:%2", file, line );
  // TODO
}

int GameData::mon_equipe(){
  LOG4("mon_equipe %1", getTeamId());
  return getTeamId();
}
bool GameData::mon_tour()
{
  unsigned int curr_player = (getCurrentTurn() - 1) % 2;
  LOG4("curr_player = %1, team id = %2, current_player = %3", curr_player, getTeamId(), current_player);
  return curr_player == getTeamId();
}
int GameData::get_current_player(){
  return current_player;
}

void GameData::get_scores(std::vector<int>& scores)
{
    scores.reserve(joueurs.size());
    for (int i = 0; i < joueurs.size(); ++i)
        scores[i] = joueurs[i].score;
}

void
GameData::get_sources(std::vector<source_energie>& srcs)
{
    srcs.reserve(sources.size());
    for (int i = 0; i < sources.size(); ++i)
      srcs[i] = sources[i].to_source_energie(i);
}
void
GameData::get_bonus_joueur(int joueur, std::vector<type_bonus>& bonus)
{
    if (joueur < 0 || joueur >= joueurs.size())
	return;

    Joueur::bonus_list& blist = joueurs[joueur].bonus;
    Joueur::bonus_list::iterator it;

    bonus.clear();
    bonus.reserve(bonus.size());
    for (it = blist.begin(); it != blist.end(); ++it)
	bonus.push_back(*it);
}

int GameData::get_real_turn()
{
  return (getCurrentTurn() - 1) / 2 + 1;
}

bool GameData::isMatchFinished(){
  return getCurrentTurn() == FIN_PARTIE * 2;
}


// actions
void GameData::appliquer_action(Action* a){
  actions.push_back(a);
  a->appliquer(this);
}

extern Api* api;
void GameData::send_actions()
{
  for (std::vector<Action*>::iterator it = actions.begin();
       it != actions.end(); ++it)
  {
    (*it)->envoyer(api);
    delete *it;
  }
  actions.resize(0);
}

bool GameData::annuler(){
  if (actions.empty()){
    LOG3("annuler : empty stack");
    return false;
  }
  else
  {
    Action* act = actions.back();
    actions.pop_back();
    LOG3("annuler : process...");
    act->annuler(this);
    delete act;
    return true;
  }
}

bool GameData::poll_pa(int pa)
{
    if (pa < 0)
	return false;
    if (remaining_pa_ < pa)
	return false;
    return true;
}

bool GameData::take_pa(int pa)
{
    if (poll_pa(pa))
    {
	remaining_pa_ -= pa;
	return true;
    }
    return false;
}

void GameData::give_pa(int pa)
{
    if (pa < 0)
	return;
    remaining_pa_ += pa;
}

/*
 * Look for an energy source at a (potentially invalid) position, and categorize
 * it (positive & negative).
 */
void GameData::categorize_case(const position& p,
			       std::set<SourceEnergie*>& src_p,
			       std::set<SourceEnergie*>& src_n,
			       std::deque<InternalTraineeMoto*> &a_traiter,
			       int player,
			       int map[TAILLE_TERRAIN][TAILLE_TERRAIN][4],
			       bool diagonale)
{
    if (position_invalide(p))
	return;
    Case& c = get_case(p);
    if (c.source_id != -1)
    {
	SourceEnergie&	src = sources[c.source_id];
	
	if (src_p.count(&src) != 0) return;
	if (src_n.count(&src) != 0) return;
	LOG3("source energie : %1 - %1", src.pos, src.potentiel_cur);
	if (src.potentiel_cur < 0)
	    src_n.insert(&src);
	else if (src.potentiel_cur > 0)
	    src_p.insert(&src);
	for (int x = p.x - 1; x <= p.x + 1; x ++)
	  for (int y = p.y - 1; y <= p.y + 1; y ++){
	    if (x != p.x || y != p.y){
	      position p2 = {x, y};
	      categorize_case(p2, src_p, src_n, a_traiter, player, map, false);
	    }
	  }
    }else if (!diagonale && c.nb_trainees_moto != 0){
      for (int i = 0; map[p.x][p.y][i] != -1 && i < 4; i++){
	int indice = map[p.x][p.y][i];
	InternalTraineeMoto *moto = &motos[indice];
	if (moto->player_ == player){
	  // LOG2("moto a traiter %1 in %1", indice, p);
	  a_traiter.push_back(moto);
	}
      }
    }
}

int GameData::apply_connections_group(int id_trainee, std::vector<int> &degrees, std::set<int> &deja_traitees, int map[TAILLE_TERRAIN][TAILLE_TERRAIN][4], bool apply){
  LOG3("APPLY CONNECTIONS GROUP");
  if (deja_traitees.count(id_trainee) == 1) return 0;

  typedef InternalTraineeMoto::nodes_list::iterator nodes_it;
  typedef std::set<SourceEnergie*>::iterator sources_it;
  typedef std::set<InternalTraineeMoto*>::iterator network_it;

  std::set<SourceEnergie*>	src_p;
  std::set<SourceEnergie*>	src_n;
  std::set<InternalTraineeMoto*> motos_network;

  std::deque<InternalTraineeMoto *> a_traiter;
  a_traiter.push_front(&motos[id_trainee]);
  int player = motos[id_trainee].player_;
  if (player != current_player) return 0;
  while( 0 != a_traiter.size()){
    InternalTraineeMoto *moto = a_traiter.front();
    a_traiter.pop_front();
    if (deja_traitees.count(moto->id_) == 1){
      // LOG3("moto : %1 deja traitee...", moto->id_);
      continue;
    }

    
    motos_network.insert(moto);
    LOG3("moto : %1", moto->id_);
    deja_traitees.insert(moto->id_);

    InternalTraineeMoto::nodes_list& nodes = moto->content_;
    for (nodes_it it = nodes.begin(); it != nodes.end(); ++it)
    {
	position p;
	for (p.x = it->x - 1; p.x <= it->x + 1; p.x ++)
	  for (p.y = it->y - 1; p.y <= it->y + 1; p.y ++)
	    if (p.x != it->x || p.y != it->y)
	      categorize_case(p, src_p, src_n, a_traiter, player, map, p.x == it->x || p.y == it->y );
    }
  }
  if (src_p.empty() || src_n.empty())
    return 0;
  int sum_neg = 0, sum_pos = 0;
  for (sources_it it = src_p.begin(); it != src_p.end(); ++it)
    {
      sum_pos += (*it)->potentiel_cur;
    }
  for (sources_it it = src_n.begin(); it != src_n.end(); ++it)
    {
      sum_neg -= (*it)->potentiel_cur;
    }

  LOG3("sum_pos = %1, sum_neg = %2", sum_pos, sum_neg);
  
  Joueur& joueur = joueurs[player];
  int diff = (sum_neg > sum_pos) ? sum_pos : sum_neg;
  
  if (diff == 0) return 0;
  
  if (apply){

    for (network_it it = motos_network.begin(); it != motos_network.end(); ++it)
      {
	(*it)->actif = true;
      }

    for (sources_it it = src_p.begin(); it != src_p.end(); ++it)
      {
	LOG3("consomme : %1 (%2) ", (*it)->id, (*it)->potentiel_cur);
	degrees[(*it)->id] += 1;
      }
    for (sources_it it = src_n.begin(); it != src_n.end(); ++it)
      {
	LOG3("consomme : %1 (%2)", (*it)->id , (*it)->potentiel_cur);
	degrees[(*it)->id] += 1;
      }
    joueur.score += diff;
  }
  return diff;
}

/*
 * Look for every connection between trainees_moto and sources, compute the
 * sources potentiel’s changes and the increase the scores.
 */
int GameData::apply_connections(bool apply)
{
  typedef InternalTraineeMoto::nodes_list::iterator nodes_it;
    motos_type::const_iterator	it;
    std::vector<int>		degrees;

    std::set<int> deja_traitees;

    int map[TAILLE_TERRAIN][TAILLE_TERRAIN][4];

    for (int x = 0; x < TAILLE_TERRAIN; x ++){
      for (int y = 0; y < TAILLE_TERRAIN; y ++){
	for (int i = 0; i < 4; i ++){
	  map[x][y][i] = -1;
	}
      }
    }
    for (it = motos.begin(); it != motos.end(); ++it){
      int indice = it->first;
      if (deja_traitees.count(indice) != 0) continue;
      InternalTraineeMoto &moto = motos[indice];
      InternalTraineeMoto::nodes_list& nodes = moto.content_;
      for (nodes_it it2 = nodes.begin(); it2 != nodes.end(); ++it2) {
	int i;
	for (i = 0; map[it2->x][it2->y][i] != -1; i ++){
	}
	map[it2->x][it2->y][i] = indice;
      }
    }
    int sum = 0;
    degrees.resize(sources.size(), 0);
    for (motos_type::const_iterator it = motos.begin(); it != motos.end(); ++it)
      sum += apply_connections_group(it->first, degrees, deja_traitees, map, apply);

    if (apply){
    // Change the sources’ potentiel
    for (int i = 0; i < degrees.size(); ++i)
      if (degrees[i] != 0){
	LOG3("consommer energie : %1 - %1", sources[i].pos, sources[i].potentiel_cur);
	sources[i].consume(degrees[i]);
	LOG3("apres consommer energie : %1 - %1", sources[i].pos, sources[i].potentiel_cur);
      }
    }
    return sum;
}

void GameData::stocker_action(Action* act)
{
    // WARN: We cannot store the pointer, it is deleted just after the
    // action is done. Serialize, and store that.
    std::vector<int> ser = act->serialiser();
    ser.insert(ser.begin(), 1, act->type());
    actions_stockees_buffer.push_back(ser);
}
