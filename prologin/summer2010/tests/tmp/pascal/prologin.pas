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

(*
  Affiche le contenu d'une valeur de type erreur
*)

(*
  Affiche le contenu d'une valeur de type type_objet
*)

(*
  Affiche le contenu d'une valeur de type position
*)

(*
  Affiche le contenu d'une valeur de type caracteristiques_objet
*)

(*
  Affiche le contenu d'une valeur de type unite
*)

(*
  Affiche le contenu d'une valeur de type piece
*)


exports
  init_game,  jouer,  end_game;

begin
end.
