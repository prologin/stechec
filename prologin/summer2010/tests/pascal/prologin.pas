library champion;

uses prolo_interface;

(*
  Fonction appellée au début de la partie.
*)
procedure init_game(); cdecl; export;
begin
	(* fonction a completer *)
end;

(*
  Fonction appellée pour la phase de jeu.
*)
procedure jouer(); cdecl; export;
begin
	(* fonction a completer *)
end;

(*
  Fonction appellée à la fin de la partie.
*)
procedure end_game(); cdecl; export;
begin
	(* fonction a completer *)
end;


exports
  init_game
,  jouer
,  end_game
;

begin
end.
