library champion;

uses prolo_interface;

type
   pos = record
	    x, y : integer;
	 end;

var
   team	    : integer;
   direction : array[0..3] of pos;

procedure print_map(); forward;

{
  Fonction de debut
}
procedure init_game(); export; cdecl;
begin
   direction[0].x := -1;
   direction[0].y := 0;
   direction[1].x := 0;
   direction[1].y := -1;
   direction[2].x := 1;
   direction[2].y := 0;
   direction[3].x := 0;
   direction[3].y := 1;
   writeln('kikoo je suis un champion ecrit en pascal');
   team := get_my_team;
   writeln('ma team de RoXXoR : ', team);
   writeln('ma fourmiliere mega-grande de ouf : ', get_anthill_pos_x(team),
	   ', ', get_anthill_pos_y(team));
   writeln('la map de bouseu : ', get_landscape_width, 'x',
	   get_landscape_height);
   print_map;
   writeln('on va chercher a manger');
end;

{
  Fonction appele a chaque tour
}
procedure new_turn(); cdecl; export;
var
   i, x, y : integer;
begin
   for i := 0 to 3 do
   begin
      x := get_ant_pos_x(i);
      y := get_ant_pos_y(i);

      if get_land_type(x, y) = FOOD then
      begin
	 carry_food(i);
	 continue;
      end;

      if (get_anthill_pos_x(team) = x) and (get_anthill_pos_y(team) = y) then
      begin
	 if get_ant_luggage(i) > 0 then
	 begin
	    writeln('a manger !');
	    let_food(i);
	    continue;
	 end;
	 direction[i].x := direction[i].x * -1;
	 direction[i].y := direction[i].y * -1;
      end;
      x := x + direction[i].x;
      y := y + direction[i].y;

      if ((get_land_type(x, y) = OBSTACLE) or
	  (get_land_type(x, y) = BAD_ARGUMENT) or
	  ((get_land_type(x, y) = ANTHILL) and
	  ((get_anthill_pos_x(team) <> x) or
	  (get_anthill_pos_y(team) <> y)))) then
      begin
	 direction[i].x := direction[i].x * -1;
	 direction[i].y := direction[i].y * -1;
      end;
      move_ant(i, x, y);
   end;
   print_map;
end;

procedure print_map();
var
   i, j, v : integer;
   l	   : string;
   c	   : char;
begin
   for i := 0 to (get_landscape_width - 1) do
   begin
      l := '';
      for j := 0 to (get_landscape_height - 1) do
      begin
	 v := get_land_type(i, j);
	 case v of
	    UNKNOWN:
		    c := '?';
	    NOTHING:
		    c := '.';
	    OBSTACLE:
		     c := 'x';
	    ANTHILL:
		    c := '#';
	    FOOD:
		 c := 'B';
	 else
	    c := 'u';
	 end;
	 if (get_land_ant(i, j) <> NO_ANT) then
	    c := 'F';
	 l := l + c;
      end;
      writeln(l);
   end;
end;

exports
  init_game,
  new_turn;

begin

end.

