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

#ifndef LEUCOCYTE_HH_
# define LEUCOCYTE_HH_

# include "Contest.hh"
# include "Object.hh"

enum Competence {
  PHAGO_SPEED = 0,
  ANTIBODY_NB,
  MESSAGES_NB,
  VISION,
  LAST_COMPETENCE
};

class GameData;

# define MULT_ANTIBODY 5


class Leucocyte : public Object
{
public:
  Leucocyte();
  int	getScore() { return score_; }

  int	getState() { return state_; }
  void	setState(int state) { state_ = state; }

  void	setGameData(GameData* g);

  int	GetRealUid () {return r_uid_;}
  void	SetRealUid (int uid) {r_uid_ = uid;}

  // Actions
  bool	alreadyPlayed() { return action_done_; }
  void	setPlayed() { action_done_ = true; }
  
  // Messages
  int	canSendMessage() { return nb_sent_messages_ < competences[MESSAGES_NB]; }
  int	getLastMessage() { return last_message_; }
  void	setLastMessageReaded() { last_message_++; }
  void	sendMessage() { nb_sent_messages_++; }

  void	resetTurn();	// to be called on beforeTurn()
  

  int	competences[LAST_COMPETENCE];

  void	addAntibody ();

  void  Phagocyte(int y, int x, Virus&);

  void  Phagocyte(int y, int x, Cellule&);

  void  Phagocyte(int y, int x, Leucocyte&);

  void	Phagocyte ()
  {
    state_ = STATE_PHAGOCYTOSING;
    phagocytose_turn_ = PHAGOCYTOSIS_DURATION;
  }

  virtual void		StopActions ();

  virtual void	PlayTurn ();

  unsigned int antibodies[MAX_MAP_SIZE][MAX_MAP_SIZE];

  int isKnownDisease(int type);

  int	score_;

private:
  int	r_uid_;
  int	state_;
  int	nb_sent_messages_;	// to be reinitialized each turn
  int	max_messages_sendable_;
  int	last_message_;
  int	phagocytose_turn_;
  bool	action_done_;		// to be reinitialized each turn

  std::vector<int> known_types_;

  Virus* v_;
  Cellule* c_;
  Leucocyte* l_;
  GameData* g_;
  //fixme
};

#endif // !LEUCOCYTE_HH_

