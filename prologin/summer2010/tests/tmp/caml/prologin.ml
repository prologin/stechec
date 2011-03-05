(*
** This file has been generated, if you wish to
** modify it in a permanent way, please refer
** to the script file : gen/generator_caml.rb
*)

open Api;;


(*
** Fonction appellée au début de la partie.
*)
let init_game () =  (* Pose ton code ici *)
  flush stderr; flush stdout;; (* Pour que vos sorties s'affichent *)

(*
** Fonction appellée pour la phase de jeu.
*)
let jouer () =  (* Pose ton code ici *)
  flush stderr; flush stdout;; (* Pour que vos sorties s'affichent *)

(*
** Fonction appellée à la fin de la partie.
*)
let end_game () =  (* Pose ton code ici *)
  flush stderr; flush stdout;; (* Pour que vos sorties s'affichent *)

(*
** Affiche le contenu d'une valeur de type erreur
*)

(*
** Affiche le contenu d'une valeur de type type_objet
*)

(*
** Affiche le contenu d'une valeur de type position
*)

(*
** Affiche le contenu d'une valeur de type caracteristiques_objet
*)

(*
** Affiche le contenu d'une valeur de type unite
*)

(*
** Affiche le contenu d'une valeur de type piece
*)

(* /!\ Ne touche pas a ce qui suit /!\ *)
Callback.register "ml_init_game" init_game;;Callback.register "ml_jouer" jouer;;Callback.register "ml_end_game" end_game;;