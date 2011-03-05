open Api

type thing =
  | Car
  | Book

type actions =
  | Move of (int * int)
  | Get of thing
  | Wait
      (*
	| memorize
	| use_car
      *)

let mapx = taille_ville_x ()
let mapy = taille_ville_y ()
let turn = ref 1

let map_iter f =
  for i = 0 to taille_ville_x () - 1 do
    for j = 0 to taille_ville_y () - 1 do
      f i j
    done
  done

let map_iter2 f g =
  for i = 0 to taille_ville_x () - 1 do
    for j = 0 to taille_ville_y () - 1 do
      f i j
    done;
    g i
  done

let print_map () =
  map_iter2
    (fun i -> fun j ->
       print_char
       ( match regarde i j with
	   | a when a = _WALL || a = _BET_PLACE -> 'X'
	   | _ -> '.'
       )
    )
    (fun a -> print_newline ())

let print_visible () =
  map_iter2
    (fun i -> fun j ->
       print_char
       (
	 if (visible i j = 0) then '#'
	 else
	   match regarde i j with
	     | a when a = _WALL -> 'X'
	     | a when a = _STANDARD -> ' '
	     | a when a = _BET_PLACE -> 'P'
	     | a when a = _GOODMAN -> 'O'
	     | _ -> failwith "Bad value for 'regarde'"
       )
    )
    (fun a -> print_newline ())

let print_info () =
  Printf.printf "taille_equipe: %d\n" (taille_equipe ());
  Printf.printf "score: %d\n" (score 0);
  Printf.printf "current_date: %d\n" (current_date ());
  Printf.printf "almanach_visible: %d\n" (almanach_visible ());
  Printf.printf "delorean_visible: %d\n" (delorean_visible ());
  Printf.printf "autonomie_delorean: %d\n" (autonomie_delorean ())

let print_action = function
  | Move (x, y) -> Printf.printf "move: %d %d\n" x y
  | Get t -> Printf.printf "get\n"
  | Wait -> Printf.printf "wait\n"

let exec n a =
  let my_exec = function
    | Move (x, y) -> ignore (deplace_joueur n x y)
    | _ -> failwith "not implemented"
  in
    Printf.printf "%d: " n; print_action a;
    my_exec a

let rm a = List.filter (fun x -> x <> a)
