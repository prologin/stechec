(*
** This file has been generated, if you wish to
** modify it in a permanent way, please refer
** to the script file : gen/generator_caml.rb
*)

open Api;;

let debug = true

let ( |> ) f x = x f

let annuler () = cancel ()

let moto_by_id id =
  let motos =
    trainees_moto ()
    |> Array.to_list
    |> List.filter (fun m -> m.id == id)
  in match motos with
    | [] -> begin
      Array.iter afficher_trainee_moto (trainees_moto ()) ;
      Printf.printf "ID RECHERCHE = %i\n%!" id ;
      failwith "pas de motos" end
    | [x] -> x
    | _ -> failwith "trop de motos"

module Coefs = struct
  let pas_cool = 0.02;
end

let unsome = function None -> assert false | Some x -> x
let unsome_list xs = List.map (function None -> [] | Some x -> [x]) xs |> List.flatten
let sum = List.fold_left ( +. ) 0.0
let max_by f a b = if f a > f b then a else b
let maximum_by f = function
  | [] -> None
  | x::xs -> Some (List.fold_left (max_by f) x xs)

let dist a b = chemin a b |> Array.length

let ( ++ ) (x, y) (x', y') = (x + x', y + y')

let near xs p = List.map (( ++ ) p) xs
let dxys = [ 1, 0 ; -1, 0 ; 0, 1 ; 0, -1 ]
let neighbours = near dxys
let dneighbours = near (dxys @ [ -1,-1 ; -1,1 ; 1,-1; 1,1 ])

type id_snake = int

module H = Hashtbl
let cache : (id_snake, float) H.t = H.create 2
let snake_map : id_snake list array array =
  Array.init taille_terrain (fun _ ->
  Array.init taille_terrain (fun _ -> [] ))
let connections : (id_snake * (int * int), bool) H.t = H.create 0
let reset_cache () =
  H.clear cache ;
  H.clear connections ;
  for x = 0 to taille_terrain - 1 do
	for y = 0 to taille_terrain - 1 do
		snake_map.(x).(y) <- []
	done
  done ;
  let me = mon_equipe () in
  trainees_moto ()
  |> Array.to_list
  |> List.filter (fun m -> m.team = me)
  |> List.iter (fun m ->
	Array.iter (fun (x, y) ->
		snake_map.(x).(y) <- m.id :: snake_map.(x).(y)
	) m.emplacement)

let print_map () =
    for y = 0 to taille_terrain - 1 do
      for x = 0 to taille_terrain - 1 do
      Printf.printf "%c%!" (match regarder_type_case (x, y) with
	| Vide -> ' '
	| Obstacle -> '#'
	| Point_croisement -> '+'
	| Source -> 'o'
      )
    done ;
    Printf.printf "\n%!"
  done

module IdSet = Set.Make (struct type t = id_snake let compare = compare end)
module PosSet = Set.Make (struct type t = position let compare = compare end)

let sources_snakes_connected_with snake =
  let snakes = trainees_moto () |> Array.to_list |> List.filter (fun s -> s.team = snake.team) in
(*
  let ss = sources_energie () in
*)
  let visited = ref IdSet.empty in
  let sources = ref PosSet.empty in
  let stack = Queue.create () in
  visited := IdSet.add snake.id !visited ;
  Queue.push snake.id stack ;
  while not (Queue.is_empty stack) do
(*
	Printf.printf "queue is running\n%!" ;
*)
	let snake_id = Queue.pop stack in
	let sna = moto_by_id snake_id in
	Array.iter (fun sna_pos ->
		List.iter (fun ((x, y) as p) ->
			match regarder_type_case p with
			| Source -> sources := PosSet.add p !sources
			| Vide | Point_croisement ->
			  let print_list l =
			    Printf.printf "[";
			    let rec sub = function
			      | [] -> Printf.printf "]\n"
			      | head::tail -> Printf.printf "%d, " head
			    in
			    sub l
			  in
			  print_list snake_map.(x).(y);
			  List.iter (fun sid ->
			    if not (IdSet.mem sid !visited)
			    then begin
			      visited := IdSet.add sid !visited ;
			      Queue.push sid stack
			    end)
					snake_map.(x).(y) 
			| _ -> ()
			)
			(dneighbours sna_pos)
		)
		sna.emplacement
  done ;
(*
  PosSet.iter (fun (x, y) -> Printf.printf " - sources at %i %i\n%!" x y) !sources ;
  Printf.printf "nb sources = %i\n%!" (PosSet.cardinal !sources) ;
*)
  !sources, !visited

let sources_connected_with snake = fst (sources_snakes_connected_with snake)
					
  
let source_at_pos p =
	sources_energie ()
	|> Array.to_list
	|> List.filter (fun s -> s.pos = p)
	|> List.hd

let coeff_of_snake snake =
  if H.mem cache snake.id
  then H.find cache snake.id
  else begin 
    let result =
	sources_connected_with snake
        |> PosSet.elements
	|> List.map (fun p -> float (source_at_pos p).capacite)
	|> sum
    in
    H.add cache snake.id result ;
    result
  end

let friends_of : trainee_moto -> (int * (int * int)) list = fun snake ->
  let ignore_snakes = snd (sources_snakes_connected_with snake) in
  let snakes = trainees_moto () |> Array.to_list |> List.filter (fun s -> s.team = snake.team) in
  let snakes = List.fold_left (fun set s -> IdSet.add s.id set) IdSet.empty snakes in
  let good_snakes = IdSet.diff snakes ignore_snakes |> IdSet.elements in
  good_snakes
  |> List.map (fun sid ->
        let this_snake = moto_by_id sid in
        let this_score = int_of_float (coeff_of_snake this_snake) in
	List.map (fun p ->
		List.map (fun p -> (this_score, p))
			(dneighbours p))
	(this_snake.emplacement |> Array.to_list))
  |> List.flatten
  |> List.flatten
	

let select_sources snake =
   let truc = sources_energie ()
   |> Array.fold_left (fun set s -> PosSet.add s.pos set) PosSet.empty
   |> (fun set -> PosSet.diff set (sources_connected_with snake))
   |> PosSet.elements
(*
   |> List.map (fun p -> (source_at_pos p).capacite, p)
*)
   |> List.map (fun p ->
        let s = source_at_pos p in
        List.map (fun pos -> s.capacite, pos)
		(dneighbours s.pos)) 
   |> List.flatten
   |> List.append (friends_of snake)
   in
(*
	Printf.printf "select_sources # %i\n%!" (List.length truc) ;
*)
	truc

let print_sources ss =
  List.iter (fun (coef, p) -> Printf.printf "   %i at %i %i\n%!" coef (fst p) (snd p)) ss

let diffusion_at p sources =
(*
  Printf.printf "diffusion_at has # %i\n%!" (List.length sources) ;
  print_sources sources ;
*)
let (pos, neg) as result =
  List.fold_left (fun (pos, neg) (coef, p') ->
    let d = dist p p' in
    (* Printf.printf " d = %i \n%!" d ; *)
    if d = 0 && p <> p'
    then (pos, neg)
    else begin
      let kfd = (d + 1) in
      let cost = float coef /. float kfd in
      (* Printf.printf " cost = %f\n%!" cost ; *)
      if coef < 0
      then (pos, neg -. cost)
      else (pos +. cost, neg)
    end)
    (0., 0.)
    sources
in
(*
  Printf.printf "and the best is %f, %f\n%!" pos neg ;
*)
  result

(*
** Fonction appellée au début de la partie
*)
let init_game () =  (* Pose ton code ici *)
  flush stderr; flush stdout;; (* Pour que vos sorties s'affichent *)

let diffusing snake p sources =

 let pos, neg = diffusion_at p sources in  
 let coef = coeff_of_snake snake in  
(*
 Printf.printf "coef = %f\n%!" coef ;
*)
 let pas_cool = Coefs.pas_cool /. ( abs_float coef) in
 if coef < 0.0
 then pos +. neg *. pas_cool
 else if coef > 0.0
 then neg +. pos *. pas_cool
 else neg +. pos
(*  abs_float (pos +. neg +. coef) *)


let print_position f (x, y) = Printf.fprintf f "(%i, %i)" x y

let best_move snake p =
  let sources = select_sources snake in
  let possible_moves =
    neighbours p
    |> List.filter case_traversable
  in
  if possible_moves = []
  then None
  else begin
  	let Some best_p =
    		maximum_by (fun p ->
      			let value = diffusing snake p sources
(*
      let () = Printf.printf "%a : %f\n%!" print_position p value
*)
  		        in value
    		) possible_moves in
  	Some (snake.id, p, best_p, diffusing snake best_p sources)
  end

let best_split snake =
  let is = Array.mapi (fun i _ -> i) snake.emplacement |> Array.to_list in
  let is = List.tl is in
  let trucs =
	List.map (fun i -> match best_move snake snake.emplacement.(i) with None -> None | Some t -> Some (i, t)) is
	|> unsome_list in
  match maximum_by (fun (_, (_, _, _, score)) -> score) trucs with
  | None -> None
  | Some (best_i, (_, _, _, score)) ->
(*
	let Some (_, _, _, score) = best_move snake snake.emplacement.(best_i) in
*)
  	let emp = snake.emplacement in
  	Some (score, snake, emp.(best_i - 1), emp.(best_i)) 


let action () =  (* Pose ton code ici *)
  reset_cache () ;
(*
  ignore (read_line ()) ;
*)
  let mt = mon_equipe () in

  let ts = trainees_moto ()
  |> Array.to_list      
  |> List.filter (fun t -> t.team = mt) in
  let mes =
	ts
	|> List.map (fun me -> [ me, me.emplacement.(0) ;
                              me, me.emplacement.(Array.length me.emplacement - 1) ])
	|> List.flatten in
  Printf.printf "mes -> # %i\n%!" (List.length mes) ;
  let best_moves = unsome_list (List.map (fun (me, p) -> best_move me p) mes) in
  Printf.printf "best_moves # %i\n%!" (List.length best_moves) ;

  let score_actuel = diff_score () in
  let best_moves =
    best_moves
  |> List.filter (fun (id, p, p', _) ->
    (* afficher_trainee_moto (moto_by_id id); *)
      let erreur = deplacer id p p' in
      Printf.printf "simulation : %a -> %a %!" print_position p print_position p' ;
      (* afficher_trainee_moto (moto_by_id id); *)
      match erreur with
	| Ok ->
      let new_score = diff_score () in
(*
      assert (new_score = 0) ;
*)
      annuler () ;
(*      afficher_trainee_moto (moto_by_id id); *)
      Printf.printf " act: %i, new:%i\n%!" score_actuel new_score ;
      new_score >= score_actuel
	| _ ->
	  afficher_erreur erreur ;
	  assert false
) in
  let Some (id, p, p', score) = 
	maximum_by (fun (id, p, p', score) -> score) best_moves
  in

  match
  	ts
	|> List.map best_split
	|> unsome_list
	|> maximum_by (fun (score, _, _, _) -> score)
  with
  | Some (split_score, split_snake, split_p, split_p')
    when split_score > score ->
(*
  in
 
  if split_score > score
  then begin
*)
(*
		|> maximum_by (fun snake -> let score, _, _, _ = best_split snake in score)
		|> unsome
		|> best_split in
*)
     begin
	let erreur = couper_trainee_moto split_snake.id split_p split_p' in
	afficher_erreur erreur ;
	Printf.printf "SPLIT %f  %a \n%!" split_score print_position split_p ;
	ignore (read_line ()) ;
     end
  | _ -> begin
  	let erreur = deplacer id p p' in
    	Printf.printf "move %a -> %a\n%!" print_position p print_position p' ;
	afficher_erreur erreur ;
  end ;
(*
  affiche_diff () ;
*)
  flush stderr; flush stdout;; (* Pour que vos sorties s'affichent *)

let dump_infos () =
  if debug then
    begin
      Printf.printf "<dump_infod>\n%!";
      Array.iter
	(fun t -> begin
	  flush stdout;
	  afficher_trainee_moto t;
	  flush stdout;
	end)
	(trainees_moto ());
      Printf.printf "</dump_infod>\n%!" ;
      print_map ()
    end

let jouer () =
  begin
    dump_infos ();
    action ();
    Printf.printf "action 1\n%!" ;
    dump_infos ();
    action ();
    Printf.printf "action 2\n%!" ;
    dump_infos ();
    action ();
    Printf.printf "action 3\n%!" ;
    dump_infos ();
    Printf.printf "gain : %i\n%!" (diff_score ());
(*    read_line ();*)
    ();
  end


(*
** Fonction appellée à la fin de la partie
*)
let end_game () =  (* Pose ton code ici *)
  flush stderr; flush stdout;; (* Pour que vos sorties s'affichent *)

(* /!\ Ne touche pas a ce qui suit /!\ *)
Callback.register "ml_init_game" init_game;;Callback.register "ml_jouer" jouer;;Callback.register "ml_end_game" end_game;;
