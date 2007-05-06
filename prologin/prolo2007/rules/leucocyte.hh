
#ifndef LEUCOCYTE_HH_
# define LEUCOCYTE_HH_

# include "Contest.hh"
# include "Object.hh"

enum Competence
  {
    PHAGO_SPEED = 0,
    ANTIBODY_NB,
    MESSAGES_NB,
    VISION,
    LAST_COMPETENCE
  };

class GameData;

class Leucocyte : public Object
{
public:
  Leucocyte();
  int	getScore () {return score_;}
  int	getState () {return state_;}
  void	setState (int state) {state_ = state;}

  void	setGameData (GameData*g);

  int	GetRealUid () {return r_uid_;}
  void	SetRealUid (int uid) {r_uid_ = uid;}

  void	ResetSentMessages () {nb_sent_messages_ = 0;}
  int	NbSentMessages () {return nb_sent_messages_;}
  void	SendMessage () {nb_sent_messages_++;}

  void	SetMaxMessagesSendable (int nb) {competences[MESSAGES_NB] = nb;}
  int	MaxMessagesSendable() {return competences[MESSAGES_NB];}

  int	GetLastMessage () {return last_message_;}
  void	MessageReaded () {last_message_++;}

  bool	action; // to be reinitialized each turn
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
  std::vector<int>	known_types;
  int			knows_type (int type)
  {
    for (std::vector<int>::iterator it = known_types.begin ();
	 it != known_types.end (); ++it)
      if (*it == type)
	return SUCCESS;
    return UNKNOWN;
  }
private:
  int	r_uid_;
  int	score_;
  int	state_;
  int	nb_sent_messages_; // to be reinitialized each turn
  int	max_messages_sendable_;
  int	last_message_;
  int	phagocytose_turn_;
  Virus* v_;
  Cellule* c_;
  Leucocyte* l_;
  GameData* g_;
  //fixme
};

#endif // !LEUCOCYTE_HH_

