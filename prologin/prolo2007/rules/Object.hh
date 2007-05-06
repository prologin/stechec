#ifndef OBJECT_HH_
# define OBJECT_HH_

class                Object : public Position
{
public:
  Object() :
    id_(-42),
    player_id_(-42),
    killed_by_ (-1)
  {}
  Object(int row, int col) :
    Position (row, col),
    id_(-42),
    player_id_(-42),
    killed_by_ (-1)
  {}
  unsigned		get_id() const { return id_; }
  unsigned		get_player() const { return player_id_; }

  void			set_id(int id) { id_ = id; }
  void			set_player(int player_id) { player_id_ = player_id; }

  virtual void		PlayTurn () = 0;

  virtual void		StopActions () = 0;

  void			killedBy (int id) {killed_by_ = id;}

  int			getKilledBy () {return killed_by_;}

private:
  unsigned		id_;		/* Object's identifier */
  unsigned		player_id_;	/* Player's identifier */
  int			killed_by_;
};

#endif // !OBJECT_HH_

