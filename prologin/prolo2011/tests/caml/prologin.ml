(*
** This file has been generated, if you wish to
** modify it in a permanent way, please refer
** to the script file : gen/generator_caml.rb
*)

open Api;;

let debug = false

let ( |> ) f x = x f
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

let is_free = function
| Trainee | Obstacle | Source -> false | _ -> true


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

module IdSet = Set.Make (struct type t = id_snake let compare = compare end)
module PosSet = Set.Make (struct type t = position let compare = compare end)

let sources_connected_with snake =
  let snakes = trainees_moto () in
(*
  let ss = sources_energie () in
*)
  let visited = ref IdSet.empty in
  let sources = ref PosSet.empty in
  let stack = Queue.create () in
  visited := IdSet.add snake.id !visited ;
  Queue.push snake.id stack ;
  while not (Queue.is_empty stack) do
	Printf.printf "queue is running\n%!" ;
	let snake_id = Queue.pop stack in
	let sna = snakes.(snake_id) in
	Array.iter (fun sna_pos ->
		List.iter (fun ((x, y) as p) ->
			match regarder_type_case p with
			| Source -> sources := PosSet.add p !sources
			| Trainee_et_croisement
			| Trainee ->
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
  PosSet.iter (fun (x, y) -> Printf.printf " - sources at %i %i\n%!" x y) !sources ;
  Printf.printf "nb sources = %i\n%!" (PosSet.cardinal !sources) ;
  !sources
					
  
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
	|> List.map (fun p -> float (source_at_pos p).coef)
	|> sum
    in
    H.add cache snake.id result ;
    result
  end

let select_sources snake =
   let truc = sources_energie ()
   |> Array.fold_left (fun set s -> PosSet.add s.pos set) PosSet.empty
   |> (fun set -> PosSet.diff set (sources_connected_with snake))
   |> PosSet.elements
(*
   |> List.map (fun p -> (source_at_pos p).coef, p)
*)
   |> List.map (fun p ->
        let s = source_at_pos p in
        List.map (fun pos -> s.coef, pos) (dneighbours s.pos)) 
   |> List.flatten
   in
	Printf.printf "select_sources # %i\n%!" (List.length truc) ;
	truc

let print_sources ss =
  List.iter (fun (coef, p) -> Printf.printf "   %i at %i %i\n%!" coef (fst p) (snd p)) ss

let diffusion_at p sources =
  Printf.printf "diffusion_at has # %i\n%!" (List.length sources) ;
  print_sources sources ;
let (pos, neg) as result =
  List.fold_left (fun (pos, neg) (coef, p') ->
    let d = dist p p' in
    Printf.printf " d = %i \n%!" d ;
    if d = 0 && p <> p'
    then (pos, neg)
    else begin
      Printf.printf "...\n%!" ;
      let cost = float coef /. float (d + 1) in
      Printf.printf " cost = %f\n%!" cost ;
      if coef < 0
      then (pos, neg -. cost)
      else (pos +. cost, neg)
    end)
    (0., 0.)
    sources
in
  Printf.printf "and the best is %f, %f\n%!" pos neg ;
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
 if coef < 0.0
 then pos
 else if coef > 0.0
 then neg
 else neg +. pos
(*  abs_float (pos +. neg +. coef) *)


let print_position f (x, y) = Printf.fprintf f "(%i, %i)" x y

let best_move snake p =
  let sources = select_sources snake in
  let possible_moves =
    neighbours p
    |> List.filter (fun p -> regarder_type_case p |>  is_free)
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
  let best_moves = unsome_list (List.map (fun (me, p) -> best_move me p) mes) in
  Printf.printf "best_moves # %i\n%!" (List.length best_moves) ;
  let Some (id, p, p', score) = 
	maximum_by (fun (id, p, p', score) -> score) best_moves
  in
 
  if score = 0.0
  then begin
	let Some (score, snake, p, p') =
		ts
		|> List.map best_split
		|> unsome_list
		|> maximum_by (fun (score, _, _, _) -> score)
	in
(*
		|> maximum_by (fun snake -> let score, _, _, _ = best_split snake in score)
		|> unsome
		|> best_split in
*)
	let erreur = couper_trainee_moto snake.id p p' in
	afficher_erreur erreur ;
	Printf.printf "SPLIT %f  %a \n%!" score print_position p ;
	Printf.printf "SPLIT\n%!" ;
	Printf.printf "SPLIT\n%!" ;
	Printf.printf "SPLIT\n%!" ;
	Printf.printf "SPLIT\n%!" ;
	Printf.printf "SPLIT\n%!" ;
	Printf.printf "SPLIT\n%!" ;
	Printf.printf "SPLIT\n%!" ;
	Printf.printf "SPLIT\n%!" ;
	Printf.printf "SPLIT\n%!" ;
	Printf.printf "SPLIT\n%!" ;
	Printf.printf "SPLIT\n%!" ;
	ignore (read_line ()) ;
  end else begin
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
      Printf.printf "</dump_infod>\n%!"
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
    Printf.printf "gain : %i\n%!" (diff_score ())
  end


(*
** Fonction appellée à la fin de la partie
*)
let end_game () =  (* Pose ton code ici *)
  flush stderr; flush stdout;; (* Pour que vos sorties s'affichent *)

(* /!\ Ne touche pas a ce qui suit /!\ *)
Callback.register "ml_init_game" init_game;;Callback.register "ml_jouer" jouer;;Callback.register "ml_end_game" end_game;;
