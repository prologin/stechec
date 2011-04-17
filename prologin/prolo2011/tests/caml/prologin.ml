(*
** This file has been generated, if you wish to
** modify it in a permanent way, please refer
** to the script file : gen/generator_caml.rb
*)

open Api;;

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

let already_connected snake source =
  snake.emplacement
  |> Array.to_list
  |> List.exists (fun pos ->
       List.exists (fun p -> p = source)
	 (dneighbours pos)
    )

let coeff_of_snake snake =
    sources_energie ()
  |> Array.to_list
  |> List.map (fun s ->
      if already_connected snake s.pos
      then float s.coef
      else 0.0
    )
  |> sum

let select_sources snake =
   sources_energie ()
   |> Array.to_list
   |> List.filter (fun s -> not (already_connected snake s.pos))
   |> List.map (fun s ->
        List.map (fun pos -> s.coef, pos) (dneighbours s.pos)) 
   |> List.flatten

let diffusion_at p sources =
(*
  let p = snake.emplacement.(0) in
  let sources =
    sources_energies ()
    |> List.filter (fun s -> not (already_connected snake s.pos))
    |> Array.to_list
    |> List.map (fun s ->
         List.map (fun pos -> s.coef, pos)
	  (dneighbours s.pos)) 
    |> List.flatten in
*)
  List.fold_left (fun (pos, neg) (coef, p') ->
    let d = dist p p' in
    if d = 0 && p <> p'
    then (pos, neg)
    else begin
      let cost = float coef /. float (d + 1) in
      if coef < 0
      then (pos, neg -. cost)
      else (pos +. cost, neg)
    end)
    (0., 0.)
    sources

(*
let affiche_diff p =
  for y = 0 to 40 do
    for x = 0 to 20 do
      Printf.printf "%3.0f " (fst (diffusion_at (x, y)))
    done ;
    Printf.printf "\n%!" ;
  done
*)


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


(*
** Fonction appellée pour la phase de jeu
*)
let jouer () =  (* Pose ton code ici *)
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
    	afficher_erreur erreur ;
  end ;

(*
  affiche_diff () ;
*)
  flush stderr; flush stdout;; (* Pour que vos sorties s'affichent *)

(*
** Fonction appellée à la fin de la partie
*)
let end_game () =  (* Pose ton code ici *)
  flush stderr; flush stdout;; (* Pour que vos sorties s'affichent *)

(* /!\ Ne touche pas a ce qui suit /!\ *)
Callback.register "ml_init_game" init_game;;Callback.register "ml_jouer" jouer;;Callback.register "ml_end_game" end_game;;
