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
** Fonction appellée pour la phase de retrait de KO.
*)
let retirer_ko () =
  let get_ennemis = Array.fold_left (fun a u -> if u.ennemi then a else u::a) []
  and mycomp u1 u2 = 
    if u1.ko = u2.ko then 0
    else if u1.ko > u2.ko then -1
    else 1
  in let ue_list = get_ennemis (unites ())
  in (List.hd (List.sort mycomp ue_list)).pos;;

(*
** Fonction appellée pour la phase de jeu.
*)
let jouer () =  (* Pose ton code ici *)
  let us = unites () in
  let my_titi = Array.fold_left 
    (fun a u -> if u.vrai_type_unite = Perroquet && not u.ennemi then u else a) us.(0) us in
  if my_titi.pos = pos_renfort false then
    (let x,y = my_titi.pos in
       deplacer (x,y) (x,y-1);
       renfort Chat)
  else
    (let x,y = pos_renfort false in
    attaquer (x,y-1) (x,y));
  flush stderr; flush stdout;; (* Pour que vos sorties s'affichent *)

(*
** Fonction appellée à la fin de la partie.
*)
let end_game () =  (* Pose ton code ici *)
  flush stderr; flush stdout;; (* Pour que vos sorties s'affichent *)

(* /!\ Ne touche pas a ce qui suit /!\ *)
Callback.register "ml_init_game" init_game;;
Callback.register "ml_retirer_ko" retirer_ko;;
Callback.register "ml_jouer" jouer;;
Callback.register "ml_end_game" end_game;;
