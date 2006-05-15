open Api
open Misc
open Perso
open Data

let conv n = mon_equipe () * _MAX_GOODMEN + n

let persos = List.map (fun n -> perso n) [conv 0; conv 1;conv 2];;


(*   let x, y = Random.int mapx, Random.int mapy in *)
(*     if visible x y = 1 then *)
(*       (50, Move(x, y)) *)
(*     else *)
(*       explore () *)

let get_other_actions () =
  [], [], []

let get_actions n =
    explore n ::
    [];;

let action_list () =
  let l0, l1, l2 = get_other_actions () in
    (0, l0@(get_actions 0))::
    (1, l1@(get_actions 1))::
    (2, l2@(get_actions 2))::[]

let init_explore () =
  let cmp (a,_,_) (b,_,_) = compare b a in
  let l = List.sort cmp (get_hidden ()) in
  let filt (a, b) l =
    List.filter (fun (_,x,y) -> visible_depuis a b x y = 0) l
  in
  let rec assign = function
    | _,[] -> ()
    | ((_,a,b)::li), (e::p) ->
	e#add_order (60, Move(a, b));
	assign (filt (a,b) li, p)
    | _ -> ()
  in
    assign (l, persos)

let play () =
  if (current_date ()) = 1 then
    List.iter (fun n -> n#add_orders (get_actions n#id)) persos;
  List.iter (fun n -> n#execute) persos

(*
** Fonction appele au debut de la partie
*)
let init_game () =  (* Pose ton code ici *)
  Random.self_init ();
  print_string "--------------------- La carte ---\n";
  print_map ();
  init_explore ();
  flush stderr; flush stdout;; (* Pour que vos sorties s'affichent *)

(*
** Fonction appele a chaque tour
*)
let play_turn () =  (* Pose ton code ici *)
  print_info ();
  print_string "--------------------- Champ de vision ---\n";
  print_visible () ;
  play ();
  flush stderr; flush stdout;; (* Pour que vos sorties s'affichent *)

(* /!\ Ne touche pas a ce qui suit /!\ *)
Callback.register "ml_init_game" init_game;;

Callback.register "ml_play_turn" play_turn;;
