(*
** This file has been generated, if you wish to
** modify it in a permanent way, please refer
** to the script file : gen/generator_caml.rb
*)

open Stdlib
open Hapi
open Cache

let debug = false

module Coefs = struct
  let pas_cool = 0.02
  let pupute = 10
  let amoi = 100
end

let print_map () =
  let snake_map = Cache.snake_map () in
    for y = 0 to taille_terrain - 1 do
      let end_line = ref [] in
      for x = 0 to taille_terrain - 1 do
      Printf.printf "%c%!" (match regarder_type_case (x, y) with
	| Obstacle -> '#'
	| Point_croisement -> '+'
	| Unite -> begin
	  let source = unites_energie () |> Array.to_list |> List.filter (fun s -> s.pos = (x, y)) |> List.hd in
	end_line := source :: ( ! end_line);
	  'o'
	end
	| Vide ->
	  match regarder_type_bonus (x, y) with
	    | Pas_bonus ->
	      if case_traversable (x, y) then ' '
	      else
		begin match snake_map.(x).(y) with
		  | [] -> '?'
		  | [x] -> (string_of_int x).[0]
		  | _ -> 'S'
		end
	    | _ -> '*'
      )
    done ;
      List.iter (fun s ->
	Printf.printf " %i/%i" s.valeur s.valeur_max
      ) (!end_line);
    Printf.printf "\n%!"
  done

module IdSet = Set.Make (struct type t = id_snake let compare = compare end)
module PosSet = Set.Make (struct type t = position let compare = compare end)

let sources_snakes_connected_with snake = time "sources_snakes_connected_with" @$ fun () ->
  let snake_map = Cache.snake_map () in
(*  let snakes = trainees_moto () |> Array.to_list |> List.filter (fun s -> s.team = snake.team) in *)
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
			| Unite -> sources := PosSet.add p !sources
			| Vide ->
			  if not (case_traversable (x, y)) then (* trainee *)
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
	Cache.sources ()
	|> List.filter (fun s -> s.pos = p)
	|> List.hd

let coeff_of_snake snake = time "coeff_of_snake" @$ fun () ->
  let cache = Cache.snake_cache () in
  if H.mem cache snake.id
  then H.find cache snake.id
  else begin 
    let result =
	sources_connected_with snake
        |> PosSet.elements
	|> List.map (fun p -> float (source_at_pos p).valeur)
	|> sum
    in
    H.add cache snake.id result ;
    result
  end

let friends_of : trainee_moto -> (int * (int * int)) list = fun snake -> time "friends_of" @$ fun () ->
  let ignore_snakes = snd (sources_snakes_connected_with snake) in
  let snakes = Cache.trainees_moto () |> List.filter (fun s -> s.team = snake.team) in
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
	

let targets_ennemy : unit -> (int * Hapi.position) list = fun () -> time "targets_ennemy" @$ fun () ->
try
	let pss = Cache.trainees_moto ()
		|> List.filter (fun s -> s.team <> mon_equipe ())
		|> List.map (fun s -> 
			if Array.length s.emplacement < 2
			then []
			else
			List.map (fun pos -> [(Coefs.pupute, pos); (-Coefs.pupute, pos)])
			((neighbours s.emplacement.(0))
                        @(neighbours s.emplacement.(Array.length s.emplacement - 1))))
		|> List.flatten
		|> List.flatten
		|> List.filter (fun (coef, pos) -> case_traversable pos)
	in pss
with Invalid_argument _ -> failwith "FUUUUUUUUUUU"

let select_sources snake = time "select_sources" @$ fun () -> 
   let already_connected = sources_connected_with snake in
   let snakes_ennemys =
	Cache.trainees_moto ()
	|> List.filter (fun s -> s.team <> mon_equipe ())
	|> List.map (fun s ->
		List.map neighbours [s.emplacement.(0);s.emplacement.(Array.length s.emplacement - 1)])
	|> List.flatten
	|> List.flatten in
   let truc = Cache.sources ()
(*
   |> List.fold_left (fun set s -> PosSet.add s.pos set) PosSet.empty
   |> (fun set -> PosSet.diff set (sources_connected_with snake))
   |> PosSet.elements
*)
(*
   |> List.map (fun p -> (source_at_pos p).valeur, p)
*)
   |> List.map (fun s ->
(*
        let s = source_at_pos p in
*)
        let valeur = if PosSet.mem s.pos already_connected then 0 else s.valeur in
        List.map (fun pos ->
		let di = try minimum (List.map (fun p -> if not (case_traversable p && case_traversable pos && p <> pos)
							then max_int
							else let d = dist p pos in if d = 0 then max_int else d) snakes_ennemys)
			with _ -> max_int in
(*
		Printf.printf "min dist = %i vers %a\n%!" di print_position pos ;
*)
		let d = Coefs.amoi * s.valeur / di in
		[valeur + d, pos; -d, pos])
		(dneighbours s.pos)) 
   |> List.flatten
   |> List.flatten
   |> List.append (friends_of snake)
   |> List.append (targets_ennemy ())
   in
(*
	Printf.printf "select_sources # %i\n%!" (List.length truc) ;
*)
	truc

module TrucSet = Set.Make (struct type t = int * Hapi.position let compare = compare end)
let no_dups =
	List.fold_left (fun set p -> TrucSet.add p set) TrucSet.empty

let diffusion_at p sources = time "diffusion_at" ~size:(TrucSet.cardinal sources) @$ fun () ->
(*
  Printf.printf "diffusion_at has # %i\n%!" (List.length sources) ;
  print_sources sources ;
*)
(*
let (pos, neg) as result =
*)
  let pos = ref 0.0 in
  let neg = ref 0.0 in
  TrucSet.iter (fun (coef, p') ->
    let d = dist p' p in
    (* Printf.printf " d = %i \n%!" d ; *)
    if d = 0 && p <> p'
    then ()
    else begin
      let kfd = (d + 1) in
      let cost = float coef /. float kfd in
      (* Printf.printf " cost = %f\n%!" cost ; *)
      if coef < 0
      then neg := !neg -. cost
      else pos := !pos +. cost
    end)
    sources ;
  !pos, !neg
(*
  Printf.printf "and the best is %f, %f\n%!" pos neg ;
  result
*)

(*
** Fonction appellée au début de la partie
*)
let init_game () =  (* Pose ton code ici *)
(*
  Gc.set @$ { (Gc.get ()) with Gc.minor_heap_size = 2000000;
                             Gc.major_heap_increment = 2000000;
                             Gc.max_overhead = 1000000 } ;
*)
  flush stderr; flush stdout;; (* Pour que vos sorties s'affichent *)

let diffusing snake p sources = time "diffusing" @$ fun () ->

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

let best_move snake p = time "best_move" @$ fun () ->
  let sources = select_sources snake |> no_dups in
(*
  Printf.printf "#################################################\n%!" ;
  Printf.printf "######## NB SOURCES = %i   ######################\n%!" (TrucSet.cardinal sources) ;
  Printf.printf "#################################################\n%!" ;
*)
  let possible_moves =
    neighbours p |> List.filter case_traversable
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

let rec but_last = function
	| [] -> assert false
	| [_] -> []
	| x::xs -> x::but_last xs

let best_split snake = time "best_split" @$ fun () ->
try
  if Array.length snake.emplacement < 3
then None
else begin
  let emp = snake.emplacement in
  let is = Array.mapi (fun i _ -> i) emp
        |> Array.to_list
	|> List.tl
	|> but_last
(*
	|> List.filter (fun i ->
		let (x0, y0) = emp.(i - 1) in
		let (x1, y1) = emp.(i) in
		let (x2, y2) = emp.(i + 1) in
		not (  (x0 = x1 && x1 = x2)
                    || (y0 = y1 && y1 = y2) )
		)
*)
  in
  let trucs =
	List.map (fun i -> match best_move snake snake.emplacement.(i) with None -> None | Some t -> Some (i, t)) is
	|> unsome_list in
  match maximum_by (fun (_, (_, _, _, score)) -> score) trucs with
  | None -> None
  | Some (best_i, (_, _, _, score)) ->
(*
	let Some (_, _, _, score) = best_move snake snake.emplacement.(best_i) in
*)
  	Some (score, snake, emp.(best_i - 1), emp.(best_i)) 
end
with Invalid_argument _ -> failwith "best_split"


let action do_split =  (* Pose ton code ici *)
(*
  ignore (read_line ()) ;
*)
  let mt = mon_equipe () in

  let ts = Cache.trainees_moto ()
  |> List.filter (fun t -> t.team = mt) in
  let mes =
	ts
	|> List.map (fun me -> [ me, me.emplacement.(0) ;
                              me, me.emplacement.(Array.length me.emplacement - 1) ])
	|> List.flatten in
  let best_moves = unsome_list (List.map (fun (me, p) -> best_move me p) mes) in
(*
  Printf.printf "best_moves # %i\n%!" (List.length best_moves) ;
*)

(*
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
*)
  let Some (id, p, p', score) = 
    maximum_by (fun (id, p, p', score) -> score) best_moves
  in

  match
     if do_split
     then
  	ts
	|> List.map best_split
	|> unsome_list
	|> maximum_by (fun (score, _, _, _) -> score)
     else None
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
	let erreur = couper_trainee_moto split_snake.id split_p split_p' 0 in
	afficher_erreur erreur ;
(*	Printf.printf "SPLIT %f  %a \n%!" split_score print_position split_p ;
	ignore (read_line ()) ; *)
     end
  | _ -> begin
  	let erreur = deplacer id p p' in
(*
    	Printf.printf "move %a -> %a\n%!" print_position p print_position p' ;
*)
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
      List.iter
	(fun t -> begin
	  flush stdout;
	  afficher_trainee_moto t;
	  flush stdout;
	end)
	(Cache.trainees_moto ());
      Printf.printf "</dump_infod>\n%!" ;
    end ;
    print_map ()

let jouer () =
  try begin
    Cache.reset true ;
    dump_infos ();
    action true;
    Printf.printf "action 1\n%!" ;
    dump_infos ();
    Cache.reset false ;
    action true;
    Printf.printf "action 2\n%!" ;
    dump_infos ();
    Cache.reset false ;
    action true;
    Printf.printf "action 3\n%!" ;
    dump_infos ();
    Printf.printf "gain : %i\n%!" (diff_score ());
(*    read_line ();*)
    ();
  end with Invalid_argument _ -> failwith "jouer"


(*
** Fonction appellée à la fin de la partie
*)
let end_game () =  (* Pose ton code ici *)
  flush stderr; flush stdout;; (* Pour que vos sorties s'affichent *)

(* /!\ Ne touche pas a ce qui suit /!\ *)
Callback.register "ml_init_game" init_game;;Callback.register "ml_jouer" jouer;;Callback.register "ml_end_game" end_game;;
