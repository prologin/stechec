// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : make_player/make_java.rb

class point
{
    public point(int x, int y)
    {
	this.x = x;
	this.y = y;
    }
    public int x;
    public int y;
};


public class Prologin extends Interface
{
    private int team;
    private point direction[] = new point[4];

    /*!
    ** Fonction de debut
    */
    public void init_game()
    {
	System.err.println("Hello World from java application !");
	team = Prologin.get_my_team();
	System.err.println("Team : " + team);
	System.err.println("Anthill : " + get_anthill_pos_x(team) +  ", " + get_anthill_pos_y(team));
	print_map();
	System.err.println("Map : " + get_landscape_width() + "x" + get_landscape_height());
	direction[0] = new point(-1, 0);
	direction[1] = new point(0, -1);
	direction[2] = new point(1, 0);
	direction[3] = new point(0, 1);
    }


    /*!
    ** Fonction appele a chaque tour
    */
    public void new_turn()
    {
	int i;

	for (i = 0; i < 4; i++)
	{
	    int x = get_ant_pos_x(i);
	    int y = get_ant_pos_y(i);

	    // nourriture !
	    if (get_land_type(x, y) == FOOD)
		{
		    carry_food(i);
		    continue;
		}

	    // on est arrive a destination ?
	    if (get_anthill_pos_x(get_my_team()) == x
		&& get_anthill_pos_y(get_my_team()) == y)
		{
		    if (get_ant_luggage(i) > 0)
			{
			    System.err.println("Manger !!");
			    let_food(i);
			    continue;
			}
		    // demi tour !
 		    direction[i].x *= -1;
 		    direction[i].y *= -1;
		}

	    x += direction[i].x;
	    y += direction[i].y;

	    // obstacle, demi tour (et on y perd un tour) :/
	    if (get_land_type(x, y) == OBSTACLE
		|| get_land_type(x, y) == BAD_ARGUMENT
		|| (get_land_type(x, y) == ANTHILL
		    && !(get_anthill_pos_x(get_my_team()) == x
			 && get_anthill_pos_y(get_my_team()) == y)))
		{
		    direction[i].x *= -1;
		    direction[i].y *= -1;
		}

	    move_ant(i, x, y);
	}
	print_map();
    }

    /*!
    ** Affiche la map
    */
    public void print_map()
    {
    int i, j;

    for (i = 0; i < get_landscape_width(); i++)
	{
	    for (j = 0; j < get_landscape_height(); j++)
		{
		    char c = (char)get_land_type(i, j);
		    switch (c)
			{
			case UNKNOWN: c = '?'; break;
			case NOTHING: c = '.'; break;
			case OBSTACLE: c = 'x'; break;
			case ANTHILL: c = '#'; break;
			case FOOD: c = 'B'; break;
			}
		    if (get_land_ant(i, j) != NO_ANT)
			c = 'F';
		    System.err.print(c);
		}
	    System.err.println();
	}
    }
}

