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

#include "InternalTraineeMoto.hh"
#include "Utils.hh"
#include "Contest.hh"
#include "GameData.hh"

#include "interface.hh"

/*
 * Wrapper that handles both iterators and reverse_iterators for lists. It is
 * used only for merging two InternalTraineeMoto’s.
 */
template<typename T>
class ListIterator
{
public:
    typedef std::list<T>	nodes_list;

    ListIterator(nodes_list&	list,
		 bool		reverse = false);

    // Using the two next methods is undefined if the iterator has reached the
    // end of the list
    T& operator*();
    T& operator->();

    // Go to the next node (does not raise an error if the end is reached)
    ListIterator<T>& operator++();
    // Return if the end is reached
    operator bool();

protected:
    nodes_list&	list_;

    bool	reverse_;
    typename nodes_list::iterator		it_;
    typename nodes_list::reverse_iterator	rit_;
};

template<typename T>
ListIterator<T>::ListIterator(nodes_list&	list,
			      bool		reverse)
    : list_ (list),
      reverse_ (reverse)
{
    if (reverse_)
	rit_ = list.rbegin();
    else
	it_ = list.begin();
}

template<typename T>
T&
ListIterator<T>::operator*()
{
    if (reverse_)
	return (*rit_);
    else
	return (*it_);
}

template<typename T>
T&
ListIterator<T>::operator->()
{
    if (reverse_)
	return (*rit_);
    else
	return (*it_);
}

template<typename T>
ListIterator<T>&
ListIterator<T>::operator++()
{
    if (reverse_)
	++rit_;
    else
	++it_;
    return (*this);
}

template<typename T>
ListIterator<T>::operator bool()
{
    if (reverse_)
	return (rit_ != list_.rend());
    else
	return (it_ != list_.end());
}

InternalTraineeMoto::InternalTraineeMoto(GameData*	gd,
                                         int		player,
					 int		id,
                                         position	init,
                                         int		max_len)
    : gd_(gd),
      player_(player),
      id_(id),
      len_(1),
      max_len_(max_len)
{
  content_.push_front(init);
  LOG4("trainee_moto constructor");
}

InternalTraineeMoto::InternalTraineeMoto()
{
    ERR("Tried to instanciate InternalTraineeMoto with the default"
	" constructor (this is invalid, and compile just in order to be able"
	" to use std::map)");
}

bool InternalTraineeMoto::begin(position pos){
  return content_.front() == pos;
}

bool InternalTraineeMoto::end(position pos){
  return content_.back() == pos;
}

position InternalTraineeMoto::head(){
  return content_.front();
}

position InternalTraineeMoto::queue(){
  return content_.back();
}

// ne check pas PLUS_DE_PA ni PAS_A_TOI ni ID_INVALIDE
void InternalTraineeMoto::reject_bad_move(position from, position to){
    LOG4("trainee_moto reject bad move");
    if (!a_cote(from, to)){
	LOG4("pas a cote");
	throw POSITION_INVALIDE;
    }
    if (!begin(from) && !end(from)){
	LOG4("pas de tete");
	throw POSITION_INVALIDE;
    }
    Case& c = gd_->get_case(to);
    if (
	(c.type != VIDE || c.nb_trainees_moto != 0) &&
	(c.type != POINT_CROISEMENT))
    {
	LOG4("destination occupee");
	throw POSITION_INVALIDE;
    }
}

void InternalTraineeMoto::take_case(const position&	pos,
				    type_bonus&		taken_bonus)
{
    Case&	case_to = gd_->get_case(pos);

    case_to.nb_trainees_moto += 1;
    taken_bonus = case_to.bonus;
    if (case_to.bonus != PAS_BONUS)
    {
	gd_->joueurs[player_].bonus.push_back(case_to.bonus);
	case_to.bonus = PAS_BONUS;
    }
}

bool InternalTraineeMoto::move(position from, position to,
			       type_bonus&	taken_bonus)
{
    LOG4("trainee_moto move %1 -> %2", from, to);
    if (begin(from))
    {
	if (len_ == max_len_)
	{
	    position pos = content_.back();
	    content_.pop_back();
	    gd_->get_case(pos).nb_trainees_moto -= 1;
	}
	else
	    len_ ++;
	content_.push_front(to);
	take_case(to, taken_bonus);
	return false;
    }
    else if (end(from))
    {
	if (len_ == max_len_)
	{
	    position pos = content_.front();
	    content_.pop_front();
	    gd_->get_case(pos).nb_trainees_moto -= 1;
	}
	else
	    len_ ++;
	content_.push_back(to);
	take_case(to, taken_bonus);
	return true;
    }
    throw POSITION_INVALIDE;
    return false;
}

erreur InternalTraineeMoto::couper(position entre, position et,
				   InternalTraineeMoto** moitie,
				   int incr_size)
{
    // First find the two adjectent nodes to split
    nodes_list::iterator it;
    nodes_list::iterator it2;
    int i = 1;

    // True if "entre" belong to the current internal moto
    bool entre_remains = false;

    for (it = content_.begin(); it != content_.end(); ++it, ++i)
        if (*it == entre)
        {
	    it2 = it;
	    ++it2;
            if (it2 == content_.end() || *it2 != et)
                throw POSITION_INVALIDE;
	    entre_remains = true;
            break;
        }
        else if (*it == et)
        {
	    it2 = it;
	    ++it2;
            if (it2 == content_.end() || *it2 != entre)
                throw POSITION_INVALIDE;
            break;
        }

    /*
     * Now, entre can belong to the current trainee_moto or to the old one,
     * there’s no determinism about this
     */

    // Then create the second TraineeMoto
    int sub_max_len = 0;
    if (entre_remains)
	sub_max_len = max_len_ - incr_size - i;
    else
	sub_max_len = incr_size;
    InternalTraineeMoto& moto = gd_->creer_trainee_moto(player_,
                                                        *it2,
                                                        sub_max_len);
    *moitie = &moto;
    max_len_ -= incr_size;
    len_ -= 1;
    while ((it2 = content_.erase(it2)) != content_.end())
    {
        moto.content_.push_back(*it2);
        moto.len_ += 1;
        len_ -= 1;
    }
    return OK;
}

/*
 * Just check that "entre" and "et" are two positions owned by the TraineeMoto
 * and that they are adjacent. Check also that the TraineeMoto can still
 * increase its length with "incr_size" units.
 */
void InternalTraineeMoto::reject_bad_coupe(position entre, position et, int incr_size)
{
    nodes_list::const_iterator it;
    nodes_list::const_iterator it2;

    if (incr_size < 0 || incr_size > max_len_ - len_)
	throw POSITION_INVALIDE;

    for (it = content_.begin(); it != content_.end(); ++it)
        if (*it == entre)
        {
	    it2 = it;
	    ++it2;
            if (it2 == content_.end() || *it2 != et)
                throw POSITION_INVALIDE;
	    else
		return;
        }
        else if (*it == et)
        {
	    it2 = it;
	    ++it2;
            if (it2 == content_.end() || *it2 != entre)
                throw POSITION_INVALIDE;
	    else
		return;
        }
}

/*
 * Make the "autre" moto empty steeling all its nodes, and delete it.
 * "entre" must be owned by *this, and "et" must be owned by autre. Of course,
 * each of "entre" and "et" must be ends of the Motos.
 */
erreur InternalTraineeMoto::fusionner(InternalTraineeMoto	&autre,
				      position			entre,
				      position			et)
{
    // Check which ends will be merged
    bool			my_end;

    if (head() == entre)
	my_end = false;
    else if (queue() == entre)
	my_end = true;

    // Determine if we browse the moto in the direct or the reverse order
    ListIterator<position>	it(autre.content_, autre.queue() == et);

    // Move the position from autre to *this
    while (it)
    {
	position	buffer = *it;
	++it;
	if (my_end)
	    content_.push_back(buffer);
	else
	    content_.push_front(buffer);
    }
    len_ += autre.len_;
    max_len_ += autre.max_len_;
    gd_->supprimer_moto(autre.id_);
}

/*
 * Just check that "entre" and "et" are two positions owned by the TraineeMoto,
 * that they are adjacent and that they belong to the same team.
 */

void InternalTraineeMoto::reject_bad_fusion(InternalTraineeMoto	&autre,
					    position		entre,
					    position		et)
{
    if (player_ != autre.player_)
	throw ID_INVALIDE;
    if ((head() != entre && queue() != entre) ||
	(autre.head() != et && autre.queue() != et))
	throw POSITION_INVALIDE;
    if (!a_cote(entre, et))
	throw POSITION_INVALIDE;
}

erreur InternalTraineeMoto::enrouler(position	point,
				     MotoData&	data)
{
    save_data(data);
    content_.push_back(point);
    len_ = 1;
    gd_->get_case(point).nb_trainees_moto += 1;
    return OK;
}

void InternalTraineeMoto::reject_bad_enroule(position	point)
{
    nodes_list::const_iterator	it;

    for (it = content_.begin(); it != content_.end(); ++it)
	if (*it == point)
	    return;
    throw POSITION_INVALIDE;
}

int InternalTraineeMoto::length(){
  return len_;
}

position InternalTraineeMoto::queue(position head__)
{
    LOG4("trainee_moto queue");
    if (begin(head__))
	return queue();
    else if (end(head__))
	return head();
    else
    {
      abort();
    }
}

bool InternalTraineeMoto::contains(const position& pos) const
{
    nodes_list::const_iterator it;

    for (it = content_.begin(); it != content_.end(); ++it)
    {
	if (*it == pos)
	    return true;
    }
    return false;
}

void InternalTraineeMoto::save_data(MotoData& data)
{
    data.content = content_;
    data.len = len_;
    data.max_len = max_len_;

    // Remove all the node from the map
    nodes_list::const_iterator	it;
    for (it = content_.begin(); it != content_.end(); ++it)
	gd_->get_case(*it).nb_trainees_moto -= 1;
    content_.clear();
}

void InternalTraineeMoto::load_data(const MotoData& data)
{
    content_ = data.content;
    // Put all the node on the map
    nodes_list::const_iterator	it;
    for (it = content_.begin(); it != content_.end(); ++it)
	gd_->get_case(*it).nb_trainees_moto += 1;
    len_ = data.len;
    max_len_ = data.max_len;
}

trainee_moto InternalTraineeMoto::to_trainee_moto() const
{
    trainee_moto		out;
    nodes_list::const_iterator	it;

    out.id = id_;
    out.emplacement.reserve(len_);
    out.team = player_;
    out.longueur_max = max_len_;
    for (it = content_.begin(); it != content_.end(); ++it)
	out.emplacement.push_back(*it); // TODO
    return out;
}
