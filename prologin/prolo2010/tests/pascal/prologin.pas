library champion;

uses prolo_interface;

(*
  Fonction appell�e au d�but de la partie.
*)
procedure init_game(); cdecl; export;
begin
	(* fonction a completer *)
end;

(*
  Fonction appell�e pour la phase de retrait de KO.
*)
function retirer_ko() : position; cdecl; export;
begin
	(* fonction a completer *)
end;

(*
  Fonction appell�e pour la phase de jeu.
*)
procedure jouer(); cdecl; export;
begin
	(* fonction a completer *)
end;

(*
  Fonction appell�e � la fin de la partie.
*)
procedure end_game(); cdecl; export;
begin
	(* fonction a completer *)
end;


exports
  init_game
,  retirer_ko
,  jouer
,  end_game
;

begin
end.
