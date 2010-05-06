(*
** This file has been generated, if you wish to
** modify it in a permanent way, please refer
** to the script file : gen/generator_caml.rb
*)

open Api

(*
** Fonction appell�e au d�but de la partie.
*)
val init_game : unit -> unit

(*
** Fonction appell�e pour la phase de retrait de KO.
*)
val retirer_ko : unit -> position

(*
** Fonction appell�e pour la phase de jeu.
*)
val jouer : unit -> unit

(*
** Fonction appell�e � la fin de la partie.
*)
val end_game : unit -> unit

