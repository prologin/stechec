(*
** This file has been generated, if you wish to
** modify it in a permanent way, please refer
** to the script file : gen/generator_caml.rb
*)

open Api;;
open Array;;

(*
** Fonction appellée au début de la partie.
*)
let init_game () =
  flush stderr; flush stdout;; (* Pour que vos sorties s'affichent *)


(* Fonction appellée pour la phase de retrait de KO. *)
let retirer_ko () =
  let get_ennemis = fold_left (fun a u -> if u.ennemi then a else u::a) []
  and mycomp u1 u2 = 
    if u1.ko = u2.ko then 0
    else if u1.ko > u2.ko then -1
    else 1
  in let ue_list = get_ennemis (unites ())
  in (List.hd (List.sort mycomp ue_list)).pos;;

(* seq :: int -> int -> int list *)
let seq start stop =
  let f = (if start > stop then (+) else (-)) in
  let rec helper f acc i =
    if i = start then (i::acc) else helper f (i::acc) (f i 1)
  in helper f [] stop;;

(* cart_prod :: 'a list -> 'b list -> ('a * 'b) list *)
let cart_prod l1 l2 =
  let rec build_tuple_list l elem = 
    match l with
    | [] -> []
    | x::xs -> (elem, x)::build_tuple_list xs elem
  in List.concat (List.map (build_tuple_list l2) l1);;

(* get_range :: position -> int -> position list *)
let get_range (x,y) pa =
  let tt = taille_terrain_actuelle () in
  let fstx = (let r = x - pa in if r < tt.min_coord then tt.min_coord else r)
  and fsty = (let r = y - pa in if r < tt.min_coord then tt.min_coord else r)
  and lstx = (let r = x + pa in if r > tt.max_coord then tt.max_coord else r)
  and lsty = (let r = y + pa in if r > tt.max_coord then tt.max_coord else r)
  in cart_prod (seq fstx lstx) (seq fsty lsty)

exception Cant_move;;

let (--) (x,y) (a,b) = (x-a, y-b)

(* get_titi :: unite array -> bool -> unite *)
let get_titi units bool =
  let a = ref units.(0) in
    iter
      (fun u -> 
        if u.vrai_type_unite = Perroquet && u.ennemi = bool then
          a := u) units;
    !a

let best_pos bro target =
  let caracs = caracteristiques bro.type_unite_actuel in
  let (distx, disty) = target.pos -- bro.pos in
  let aux dist pos_init =
    let op = (if dist > 0 then (+) else (-)) in
      if (abs dist) < caracs.pa_init then
        op (op pos_init dist) (-1)
      else
        op dist (caracs.pa_init - 1)
  in (aux distx (fst bro.pos), aux disty (snd bro.pos))

(* count :: type_unite -> bool -> unite array -> int *)
let count tu team =
  fold_left (fun a u -> a + (if u.ennemi = team && u.vrai_type_unite = tu then 1 else 0)) 0

(* choose_u2s :: unite array -> type_unite *)
let choose_u2s unites =
  let nb_bb_adv = count Chat true unites
  and nb_my_elm = count Singe false unites
  in if nb_bb_adv > (2 * nb_my_elm) then
    Singe
(*  else if (count Chat false unites) > (nbr_max_unites / 2) then
    Kangourou *)
  else
    Chat

(* -------------------------------------------------------------------------- *)
(* try_move :: unite -> erreur *)
let try_move u units =
  if u.ko >= 0 then Unite_ko
  else if u.ennemi then Pas_a_moi
  else
    let (x, y) = best_pos u (get_titi units true) in
    let comp (x1, y1) (x2, y2) =
      let d1 = abs (x - x1 + y - y1)
      and d2 = abs (x - x2 + y - y2) in
        if d1 < d2 then (-1)
        else if d1 = d2 then 0
        else 1
    in let rec lst = get_range u.pos u.pa
    and pick_first = function
      | [] -> Case_occupee
      | pos::xs ->
          match deplacer u.pos pos with
            | Ok -> Ok
            | a -> afficher_erreur a; pick_first xs
    in pick_first (List.sort comp lst)

(* free_spawn :: unites array -> erreur *)
let free_spawn unites map =
  let (sx, sy) = pos_renfort false in
  try match map.(sx).(sy) with
    | None -> Ok
    | Some u2move ->
        match try_move u2move unites with
          | Case_occupee -> Renfort_impossible
          | otherwise -> otherwise
  with _ -> print_endline "le spawn est hors de la map";
    print_int sx; print_string " - "; print_int sy;
    Renfort_impossible
(* -------------------------------------------------------------------------- *)

(* build_map :: unite array -> unite option array array *)
let build_map unites =
  let tta = taille_depart in
(*  print_string "------------------ TAILLE TERRAIN --------------------"; *)
(*  print_newline (); *)
(*  print_int tta.taille; print_newline (); *)
(*  print_int tta.min_coord; print_newline (); *)
(*  print_int tta.max_coord; print_newline (); *)
  let a = make_matrix tta tta None in
    iter (fun u -> let (x,y) = u.pos in
            try a.(x).(y) <- Some u with _ -> print_endline "WAT") unites;
    a

let rec neighbours acc map = function
  | [] -> acc
  | (x,y)::tail ->
      match map.(x).(y) with
        | None -> neighbours acc map tail
        | Some u -> neighbours (u::acc) map tail

(* is_in_range :: pos -> unite -> bool *)
let is_in_range pos u =
  if (u.ko >= 0) then
    false
  else
    let bonus_porte = (if u.type_unite_actuel = Singe then 2 else 0) in
    let range = get_range u.pos (u.pa + bonus_porte) in
      List.mem pos range

(* get_status :: unite array -> unite option array array -> (unite * unite list) list
 *    renvoit une stack contenant des couples (unites ami, [unites ennemies])
 *    ces couples signifiant qu'« unite ami » est en position d'attaquer ou
 *    d'être attaquée par les autres.
 *)
let get_status unites map =
  let helper u1 u2 =
    (u1.ennemi <> u2.ennemi) && (is_in_range u1.pos u2)
  and s = ref [] in
    iter
      (fun u ->
        if not u.ennemi then
          let range = get_range u.pos 6 in
          let neighbours = neighbours [] map range in
            match List.filter (fun x -> helper x u) neighbours with
              | [] -> ()
              | lst -> s := (u, lst)::!s) unites;
    !s



let rec do_the_job = function
  | [] -> Ok
  | (bro, target)::tail ->
      let new_pos = best_pos bro target in
        match deplacer bro.pos new_pos with
          | Ok -> (match attaquer new_pos target.pos with 
                     | Ok -> do_the_job tail
                     | a -> a)
          | a -> afficher_erreur a; a

(* choose -> ('a * 'b) list -> ('a * 'c) list -> 'a option *)
let rec choose already_chosen lst =
  let rec kmem x = function
    | [] -> false
    | (k,_)::_ when x = k -> true
    | _::xs -> kmem x xs
  in match lst with
    | [] -> None
    | (x, _)::xs when kmem x already_chosen -> choose already_chosen xs
    | (x, _)::_ -> Some x
  
(* focus_titi :: unite array -> unite -> (unite * pos) list *)
let focus_titi units titi =
  let comp (d1, u1) (d2, u2) = d1 - d2 + u1.pa - u2.pa in
  let bros = List.filter (fun u -> not u.ennemi) (to_list units) in
  let u_with_d =
    List.map
      (fun u -> ((fst u.pos) - (fst titi.pos) + (snd u.pos) - (snd titi.pos), u)) 
      bros 
  in let sorted = List.sort comp u_with_d in
    List.map (fun (_, u) -> (u, best_pos u titi)) sorted

(* try_escape :: unite -> unite list -> pos option *)
let try_escape unite ennemis = 
  let rec area = get_range unite.pos unite.pa
  and take_fst = function
    | [] -> None
    | x::xs ->
        if List.exists (fun e -> is_in_range x e) ennemis then
          take_fst xs
        else
          Some x
  in take_fst area

(* send_bipbips :: unites array -> unit *)
let send_bipbips units =
  let titi_ennemi = get_titi units true in
  let lst = focus_titi units titi_ennemi in
  let rec helper = function
    | [] -> ()
    | (u, pos)::xs ->
        match deplacer u.pos pos with
          | Ok | Case_occupee -> 
              ignore (attaquer pos titi_ennemi.pos);
              helper xs
          | erreur -> afficher_erreur erreur
  in helper lst

let rec abort_and_escape u ennemis = function
  | true -> abort_and_escape u ennemis (annuler ())
  | false ->
      match try_escape u ennemis with
        | None -> ()
        | Some new_pos -> 
            match deplacer u.pos new_pos with
              | Ok -> ()
              | a -> afficher_erreur a

(* protect_titi :: (unite * unite list) list -> unite list -> unite -> unit *)
let protect_titi stack ennemis titi =
  let def_target =
    List.fold_left
      (fun acc e ->
        match List.filter (fun (_,l) -> List.mem e l) stack with
          | [] -> acc
          | lst -> 
              (match choose acc lst with
                 | None -> acc
                 | Some x -> (x, e)::acc))
      [] ennemis
  in if do_the_job def_target <> Ok then
    abort_and_escape titi ennemis true

let organize_defense elmers_and_cie titi =
  let dist u1 u2 =
    let (distx, disty) = u1.pos -- u2.pos in
    let bd, sd = (max distx disty), (min distx disty) in
       2*bd - sd
  in let my_comp (_, e1, d2titi1) (_, e2, d2titi2) =
    if e1.type_unite_actuel = e2.type_unite_actuel then
      if d2titi1 = d2titi2 then 0
      else if d2titi1 > d2titi2 then 1
      else (-1)
    else
      if e1.type_unite_actuel = Kangourou then 1
      else if e2.type_unite_actuel = Kangourou then (-1)
      else if e1.type_unite_actuel = Singe then 1
      else if e2.type_unite_actuel = Singe then (-1)
      else if d2titi1 = d2titi2 then 0
      else if d2titi1 > d2titi2 then 1
      else (-1)
  in let pairs = List.concat (List.map
    (fun (elm, ennemis) -> List.map (fun e -> (elm, e, dist e titi)) ennemis)
    elmers_and_cie) in
  let sorted = List.sort my_comp pairs in
    afficher_erreur (do_the_job (List.map (fun (a,b,_) -> (a,b)) sorted))

(* Fonction appellée pour la phase de jeu. *)
let jouer () =
  let is_empty = function [] -> true | _ -> false in
  let units = unites () in
  let map = build_map units in
  let my_titi = get_titi units false in
  let stack = get_status units map in
  let nb_bros = fold_left (fun a u -> if u.ennemi then a else a + 1) 0 units in
    (if (is_empty stack && nb_bros > 3) then
       if (mes_cartes ()).pacifisme > 0 then
         ignore (pacifisme ()));

    if nb_bros < nbr_max_unites then
      (match free_spawn units map with
        | Ok -> ignore (renfort (choose_u2s units))
        | otherwise -> afficher_erreur otherwise;
        );

    (* Si personne n'est en danger et qu'on ne peut attaquer personne, on
     * envoit les Bipbips en direction du titi adverse *)
    if is_empty stack then
      send_bipbips units

    else
      (* Si titi est en danger *)
      let is_titi u = u.vrai_type_unite = Perroquet in
      let maybe_titi = List.filter (fun (u,_) -> is_titi u) stack in
        match maybe_titi with
          | (titi, ennemis)::_ -> protect_titi stack ennemis titi
          | [] ->
            (match List.partition (fun (u,_) -> u.vrai_type_unite = Singe) stack with
              (* Cas où la défense est safe, on s'occupe d'attaquer *)
              | ([], rest) -> () (* FIXME *)
              (* Il y'a des ennemis dans la zone défensive *)
              | (elmers, rest) -> organize_defense elmers my_titi); (* FIXME *)
  flush stderr; flush stdout;;

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
