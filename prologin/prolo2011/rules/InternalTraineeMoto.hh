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

#ifndef INTERNAL_TRAINEE_MOTO_HH_
#define INTERNAL_TRAINEE_MOTO_HH_

#include "Constant.hh"
#include <list>

class GameData;

class InternalTraineeMoto
{
public:
    typedef std::list<position> nodes_list;

    struct MotoData
    {
	typedef std::list<position> nodes_list;
	nodes_list content;
	int len;
	int max_len;
	bool last_end_moved;
    };

    InternalTraineeMoto(GameData* gd,
			int player, int id,
			position init, int max_len);
    // Will fail. It is defined only to be able to put InternalTraineeMoto’s in
    // std::map
    InternalTraineeMoto();

    bool begin(position pos);
    bool end(position pos);

    void load_data(const MotoData& data);
    trainee_moto to_trainee_moto() const;

    void take_case(const position&	pos,
		   type_bonus&		taken_bonus);
    bool move(position from, position to,
	      type_bonus&	taken_bonus);
    void reject_bad_move(position from, position to);

    erreur couper(position entre, position et,
		  InternalTraineeMoto** moitie,
		  int incr_size);
    void reject_bad_coupe(position entre, position et,
			 int incr_size);

    erreur fusionner(InternalTraineeMoto	&autre,
		     position			entre,
		     position			et);
    void reject_bad_fusion(InternalTraineeMoto	&autre,
			   position		entre,
			   position		et);

    erreur enrouler(position	point,
		    MotoData	&data);
    void reject_bad_enroule(position	point);

    int length();
    position queue(position head);
    position head();
    position queue();
    bool contains(const position& pos) const;

    GameData*	gd_;
    int		player_;
    int		id_;

    nodes_list content_;
    int len_;
    int max_len_;
  bool actif;
protected:
    void save_data(MotoData& data);
};

#endif // !INTERNAL_TRAINEE_MOTO_HH_
