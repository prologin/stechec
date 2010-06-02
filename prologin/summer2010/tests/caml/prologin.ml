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

let (|>) x f = f x;;

let distance (ax, ay) (bx, by) = max (abs (ax - bx ) ) (abs (ay - by ) );;

let cant_go u pos =
  ( distance u.pos_unite pos ) < u.pa

let better_piece u p1 p2 = if p1.valeur > p2.valeur then p1 else p2;; (* todo distance *)

let my_team u = let t = mon_equipe () in u.team == t;;

let clean () =
  begin
    flush stderr;
    flush stdout;
  end


let move_player (u:unite) =
  Printf.printf "recuperation des pieces...\n";
  clean();
  let money = pieces_en_jeu () |> Array.to_list in
    begin
      Printf.printf "depacement...\n";
      clean ();
      let opt = List.fold_left
	(fun opt piece ->
	   if cant_go u piece.pos_piece then None
	   else match opt with
	     | None -> Some piece
	     | Some autre_piece ->
		 Some (better_piece u piece autre_piece)
	) None money
      in match opt with
	| Some p ->
	    begin
	      deplacer u.pos_unite p.pos_piece |> afficher_erreur;
	      Printf.printf "depacement...\n";
	      clean ();
	    end
	| None -> ()
    end
;;

let show_unites u =
  List.iter
    (fun u ->
       let (x, y) = u.pos_unite in
       Printf.printf "id=%d team=%d pos=(%d, %d)\n" u.id u.team x y
    )
    u

let deplacer () =
  let u = unites () |> Array.to_list in
(*   let () = Gc.compact () in *)
  let () = show_unites u in
  let u = u |> List.filter my_team in
    List.iter move_player u;;


(*
** Fonction appellée pour la phase de jeu.
*)
let jouer () =  (* Pose ton code ici *)
  begin
    prerr_endline "jouer !";
    try deplacer ()  with _ -> prerr_endline "erreur...";
    flush stderr; (* Pour que vos sorties s'affichent *)
    flush stdout;
  end;;

(*
** Fonction appellée à la fin de la partie.
*)
let end_game () =  (* Pose ton code ici *)
  flush stderr; flush stdout;; (* Pour que vos sorties s'affichent *)

(* /!\ Ne touche pas a ce qui suit /!\ *)
Callback.register "ml_init_game" init_game;;
Callback.register "ml_jouer" jouer;;
Callback.register "ml_end_game" end_game;;
