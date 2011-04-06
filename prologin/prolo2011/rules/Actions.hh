/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2010 Prologin
*/

#ifndef ACTIONS_HH_
# define ACTIONS_HH_

# include "Api.hh"
# include "Contest.hh"
# include "Constant.hh"
# include "GameData.hh"

# include "InternalTraineeMoto.hh"

# include <map>
# include <vector>

class Action
{
public:
    Action(int player) : player_(player) {}

    virtual ~Action() {}

    virtual void appliquer(GameData* g);
    virtual void envoyer(Api* api) = 0;
    virtual void annuler(GameData* g);
    virtual void verifier(GameData* g) = 0;
    virtual void print() = 0;

    void verifier_pa(GameData* g, int pa);

protected:
  int player_;
};

#define ACTIONS(Nom)				  \
    virtual void appliquer(GameData* g);	  \
    virtual void envoyer(Api* api);		  \
    static  Nom* recevoir(const StechecPkt* pkt); \
    virtual void annuler(GameData* g);		  \
    virtual void verifier(GameData* g);		  \
    virtual void print();

class ActionDeplacer : public Action
{
public:
    ActionDeplacer(int player, int id, position from, position to) :
	Action(player), id_(id), from_(from), to_(to)
    {
    }

    ACTIONS(ActionDeplacer);

protected:
    position	from_;
    position	to_;
    int		id_;
    int		old_len_;
    position	old_queue_;
    position	new_queue_;
    type_bonus	taken_bonus_;
    bool	last_end_moved_;
};

class ActionCouperTraineeMoto : public Action
{
public:
    ActionCouperTraineeMoto(int player, int id,
			    position entre, position et) :
	Action(player), id_(id), entre_(entre), et_(et),
	new_id_(-1)
    {
    }

    ACTIONS(ActionCouperTraineeMoto);

protected:
    int		id_;
    int		new_id_;
    position	entre_;
    position	et_;
};

class ActionFusionner : public Action
{
public:
    ActionFusionner(int player,
		    int	id1,	position pos1,
		    int	id2,	position pos2) :
	Action(player),
	id1_(id1), pos1_(pos1),
	id2_(id2), pos2_(pos2)
    {
    }

    ACTIONS(ActionFusionner);

protected:
    int		id1_;
    int		id2_;
    position	pos1_;
    position	pos2_;
};

class ActionEnrouler : public Action
{
public:
    ActionEnrouler(int player,
		   int		id,
		   position	point) :
	Action(player),
	id_(id), point_(point)
    {
    }

    ACTIONS(ActionEnrouler);

protected:
    int					id_;
    position				point_;
    InternalTraineeMoto::MotoData	data_;
};

class ActionRegenererSourceEnergie : public Action
{
public:
    ActionRegenererSourceEnergie(int player, int id)
	: Action(player),
	  id_(id)
    {
    }

    ACTIONS(ActionRegenererSourceEnergie);

protected:
    int	id_;
    int	old_potentiel_;
};


class ActionAgrandirTraineeMoto : public Action
{
public:
    ActionAgrandirTraineeMoto(int player, int id, int longueur)
	: Action(player),
	  id_ (id),
	  longueur_ (longueur)
    {
    }

    ACTIONS(ActionAgrandirTraineeMoto);

protected:
    int	id_;
    int longueur_;
};

class ActionPoserPointCroisement : public Action
{
public:
    ActionPoserPointCroisement(int player, position point)
	: Action(player),
	  point_(point)
    {
    }

    ACTIONS(ActionPoserPointCroisement);

protected:
    position point_;
    type_case old_type_;
};

Action* act_from_pkt(int type, const StechecPkt* pkt);

#endif
