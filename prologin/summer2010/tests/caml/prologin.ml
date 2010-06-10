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
      ) [] l1;;

  let find_opt f li =
    try
      Some (List.find f li)
    with Not_found -> None
end

module Tuple = struct
  let switch (a, b) = b, a
  let apply_clot (a, b) = a b
  let map f (a, b) = (f a), (f b)
  let map2 (f, g) (a, b) = (f a), (g b)
  let combine (a, b) (c, d) = (a, c), (b, d)
  let add (a, b) c = (a, b, c)
  let to_list (a, b) = [b, a]
  let fold f acc0 (a, b) = f a (f acc0 b)
end


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
(*  let () = show_unites u in *)
    List.exists (fun u -> u.pos_unite = p) u

let player_at p =
  let u = unites () |> Array.to_list in
(*  let () = show_unites u in *)
    List.find_opt (fun u -> u.pos_unite = p) u

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

let manger_simple u money =
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
let prix objet = (proprietes_objet objet).cout;;

let utiliser_objet u money =
  let unites = unites () |> Array.to_list in
  let upos = u.pos_unite in
  let (ux, uy) = upos in
  let lec f = List.filter (fun p -> (f p.pos_piece) = (f upos)) money in
  let ligne (a, b) = b in
  let colone (a, b) = a in
  let gauche, droite = List.partition (fun p -> (colone p.pos_piece) < (colone upos) ) (lec ligne) in
  let haut, bas = List.partition (fun p -> (ligne p.pos_piece) < (ligne upos) ) (lec colone) in
  let hd = ((fst upos), 0) in
  let bd = ((fst upos), taille_terrain - 1) in
  let gd = (0, (snd upos)) in
  let dd = (taille_terrain - 1, (snd upos)) in
  let li = [
    (hd, haut, (fun (x, y) -> x = ux && y < uy));
    (bd, bas, (fun (x, y) -> x = ux && y > uy));
    (gd, gauche, (fun (x, y) -> x < ux && y = uy));
    (dd, droite, (fun (x, y) -> x > ux && y = uy))
  ] in
    List.fold_left
      (fun acc (direction, pieces, filtre) ->
	 acc || (
	   let unite_touchee =
	     List.fold_left
	       (fun opt u ->
		  if filtre u.pos_unite then
		    match opt with
		      | None -> Some u
		      | Some u2 ->
			  Some (
			    if distance upos u.pos_unite > distance upos u2.pos_unite
			    then u2
			    else u)
		  else opt
	       )
	       None
	       unites
	   in
	   let moi_touche, lui_touche = match unite_touchee with
	     | None -> false, false
	     | Some u -> let t = mon_equipe () in (t = u.team, t <> u.team)
	   in
	   let pieces =
	     match unite_touchee with
	       | Some u2 ->
		   let dref = distance upos u2.pos_unite in
		     List.filter
		       (fun p -> distance upos p.pos_piece < dref )
		       pieces
	       | None -> pieces
	   in
	   let () =
	     begin
	       if moi_touche then
		 Printf.printf "MOI TOUCHE !\n"
	     end in
	     if moi_touche then false else
	       let gain =
		 let pieces = List.map (fun p -> p.valeur) pieces in
		   List.fold_left (+) 0 pieces in
	       let () = Printf.printf "gain = %d prix = %d\n" gain (prix Filet)
	       in
		 if gain > (prix Filet) then
		   let () = begin
		     Printf.printf "utiliser objet ! (de %d, %d en %d, %d)\n" ux uy (fst direction) (snd direction);
		     List.iter (fun p -> Printf.printf "on capture la piece %d, %d\n" (fst p.pos_piece) (snd p.pos_piece) ) pieces;
		     clean();
		     acheter_objet upos Filet |> afficher_erreur;
		     utiliser upos direction |> afficher_erreur;
		     clean();
		   end
	       in true
		 else false
	 )
      ) false li

let move_player (u:unite) =
  Printf.printf "recuperation des pieces...\n";
  clean();
  let money = pieces_en_jeu () |> Array.to_list in
    if not (utiliser_objet u money) then manger_simple u money
;;

let deplacer () =
  let u = unites () |> Array.to_list in
    (* let () = show_unites u in *)
  let u = u |> List.filter (fun u -> (my_team u) && u.ko = 0) in
    begin
      Printf.printf "nbr utites : %d\n" (List.length u);
      clean();
      List.iter move_player u
    end;;

let try_deplacer () =
  begin
    prerr_endline "jouer !";
    try deplacer ()  with e ->
      begin
	prerr_endline (Printexc.to_string e);
	prerr_endline (Printexc.get_backtrace ());
      end;
  end;;

let try_achever () =
  begin
    let mon_score, son_score =
      let mon_equipe = mon_equipe () in
      let () = Printf.printf "mon equipe = %d\n" mon_equipe in
      let score0 = score 0
      and score1 = score 1
      in let () = begin
	  Printf.printf "score equipe 0 = %d\n" score0;
	  Printf.printf "score equipe 1 = %d\n" score1;
	  clean ();
	end
      in let tuple = (score0, score1)
      in if mon_equipe == 0
	then tuple
	else Tuple.switch tuple
    in let () = begin
	Printf.printf "mon score = %d\n" mon_score;
	Printf.printf "son score = %d\n" son_score;
	clean ();
      end
    in if mon_score > son_score + prix_fin_partie then
	begin
	  Printf.printf "ACHEVER LA PARTIE !\n";
	  Printf.printf "tour = %d\n" (tour_actuel ());
	  clean ();
	  achever_la_partie () |> afficher_erreur;
	end
  end;;


(*
** Fonction appellée au début de la partie.
*)
let init_game () =  (* Pose ton code ici *)
  Printf.printf "init game";
  clean ();;

(*
** Fonction appellée pour la phase de jeu.
*)
let jouer () =  (* Pose ton code ici *)
    begin
      try_deplacer ();
      try_achever ();
      clean ();
    end;;

(*
** Fonction appellée à la fin de la partie.
*)
let end_game () =  (* Pose ton code ici *)
  Printf.printf "ocaml end game";;
  clean ();;

(* /!\ Ne touche pas a ce qui suit /!\ *)
Callback.register "ml_init_game" init_game;;
Callback.register "ml_jouer" jouer;;
Callback.register "ml_end_game" end_game;;
