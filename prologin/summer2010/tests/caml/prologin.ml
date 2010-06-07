(*
** This file has been generated, if you wish to
** modify it in a permanent way, please refer
** to the script file : gen/generator_caml.rb
*)

open Api;;

module List = struct
  include List;;
  
  let init f i =
    if i < 0 then [] else
      let rec aux n =
	if n = i then [f n]
	else (f n) :: (aux (n + 1))
      in aux 0;;
  let carthesien l1 l2 =
    List.fold_left
      (fun acc i1 ->
	 List.fold_left
	   (fun acc i2 ->
	      (i1, i2) :: acc
	   ) acc l2
      ) [] l1
end


(*
** Fonction appellée au début de la partie.
*)
let init_game () =  (* Pose ton code ici *)
  flush stderr; flush stdout;; (* Pour que vos sorties s'affichent *)

let show_unites u =
  List.iter
    (fun u ->
       let (x, y) = u.pos_unite in
       Printf.printf "id=%d team=%d pos=(%d, %d)\n" u.id u.team x y
    )
    u

let (|>) x f = f x;;

let distance (ax, ay) (bx, by) = max (abs (ax - bx ) ) (abs (ay - by ) );;

let distance_user_piece u p = distance u.pos_unite p.pos_piece;;

let cant_go u pos =
  ( distance u.pos_unite pos ) > u.pa

let better_piece u p1 p2 = if p1.valeur > p2.valeur then p1 else p2;; (* todo distance *)

let plus_proche u p1 p2 =
  let d1 = distance_user_piece u p1 in
  let d2 = distance_user_piece u p2 in
  if d1 < d2 then p1 else p2;; (* todo distance *)

let my_team u = let t = mon_equipe () in u.team == t;;

let clean () =
  begin
    flush stderr;
    flush stdout;
    Gc.compact ();
  end

let deplacer u p =
  let (ux, uy) = u.pos_unite in
  let (px, py) = p in
  begin
    Printf.printf "depacement (%d, %d) -> (%d, %d) u.pa = %d ...\n" ux uy px py u.pa;
    clean ();
    deplacer u.pos_unite p |> afficher_erreur;
    clean ();
  end

let position_occupee p =
  let u = unites () |> Array.to_list in
  let () = show_unites u in
    List.exists (fun u -> u.pos_unite = p) u

let direction u p =
  let (ux, uy) = u.pos_unite in
  let (px, py) = p.pos_piece in
  let position (dx, dy) =
    let x = if ux < px then ux + dx else ux - dx in
    let y = if uy < py then uy + dy else uy - dy
    in (x, y)
  in
  let position_libre dx dy =
    let list_dx = List.init (fun i -> i + 1) (dx - 1) |> List.rev in
    let list_dy = List.init (fun i -> i + 1) (dy - 1) |> List.rev in
    let p_list = List.carthesien list_dx list_dy in
    let p_list = List.map position p_list in
      List.fold_left
	(fun opt p ->
	   match opt with
	     | Some s -> opt
	     | None ->
		 if position_occupee p then None else Some p
	)
	None
	p_list
  in
  let pa = u.pa in
  let dx = min pa (abs (ux - px)) in
  let dy = min pa (abs (uy - py)) in
  match position_libre dx dy with
    | Some (x, y) ->
	let () =
	  begin
	    Printf.printf "from (%d, %d) to (%d, %d) => (%d, %d) (pa = %d)\n" ux uy px py x y pa;
	    clean();
	  end
	in Some (x, y)
    | None -> None

let move_player (u:unite) =
  Printf.printf "recuperation des pieces...\n";
  clean();
  let money = pieces_en_jeu () |> Array.to_list in
    begin
      clean ();
      let opt = List.fold_left
	(fun opt piece ->
	   if cant_go u piece.pos_piece then opt
	   else match opt with
	     | None -> Some piece
	     | Some autre_piece ->
		 Some (better_piece u piece autre_piece)
	) None money
      in match opt with
	| Some p ->
	    begin
	      Printf.printf "manger une piece\n";
	      clean();
	      deplacer u p.pos_piece;
	    end
	| None ->
	    let () = Printf.printf "aller vers la piece la plus proche\n" in
	    let () = clean() in
	    let opt =
	      List.fold_left
		(fun opt piece ->
		   match opt with
		     | None -> Some piece
		     | Some autre_piece ->
			 Some (plus_proche u piece autre_piece)
		) None money
	    in match opt with
	      | None -> ()
	      | Some p ->
		  match direction u p with
		    | Some p -> deplacer u p
		    | None -> ()
    end
;;

let deplacer () =
  let u = unites () |> Array.to_list in
(*   let () = Gc.compact () in *)
  let () = show_unites u in
  let u = u |> List.filter (fun u -> (my_team u) && u.ko = 0) in
    begin
      Printf.printf "nbr utites : %d\n" (List.length u);
      clean();
      List.iter move_player u
    end;;


(*
** Fonction appellée pour la phase de jeu.
*)
let jouer () =  (* Pose ton code ici *)
  begin
    prerr_endline "jouer !";
    try deplacer ()  with e ->
      begin
	prerr_endline (Printexc.to_string e);
	prerr_endline (Printexc.get_backtrace ());
      end;
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
