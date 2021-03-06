include Api;;
open Stdlib;;
open Cache;;

let in_board x y =
  x >= 0 && x < taille_terrain && y >= 0 && y < taille_terrain 

let moto_by_id id =
try
  match (Cache.motos_id () ).(id) with
    | None -> begin
      Array.iter afficher_trainee_moto (trainees_moto ()) ;
      Printf.printf "ID RECHERCHE = %i\n%!" id ;
      failwith "pas de motos" end
    | Some x -> x
with Invalid_argument _ -> failwith "moto_by_id"

let dist_process distances dia min_distance to_process ia ib =
  begin
    while dia.(ib) = None && !to_process != [] do
      let min_d = !min_distance in
      let new_min_d = min_d + 1 in
      let some_new_min_d = Some new_min_d in
      let to_process2 = !to_process in
      to_process := [];
      List.iter (fun (x, y) ->
	let i = case_indice x y in
	if dia.(i) = Some min_d then
	  neighbours (x, y) |>
	      List.iter (fun (x3, y3) ->
		if in_board x3 y3 then
		  let i3 = case_indice x3 y3 in
		  let old_d = dia.(i3) in
		  match old_d with
		    | None ->
		      begin
			to_process := (x3, y3) :: (! to_process);
			dia.(i3) <- some_new_min_d;
		      end
		    (*				  | Some -1 ->
						  if i = ib then
						  dia.(i3) <- some_new_min_d
		    *)
		    | Some x -> ()
	      )
      )
	to_process2;
      min_distance := !min_distance + 1;
    done;
    distances.Cache.params.(ia) <- (!to_process, !min_distance);
  end

  let process_distance d =
    match d with
      | None -> 0
      | Some (-1) -> 0
      | Some s ->
	(*
	  let other = chemin a b |> Array.length in
	  assert (other = s);
	*)
	s

let dist0 distances dia a ia ib =
   let () =  if dia.(ib) = None then
       let min_distance = ref 0 in
       let to_process = ref [a] in
       let () =
	 match distances.Cache.params.(ia) with
	   | ([], 0) -> ();
	   | (toproc, mind) -> begin
	     min_distance := mind;
	     to_process := toproc;
	   end
       in
       dist_process distances dia min_distance to_process ia ib
   in
   process_distance dia.(ib)

let dist =
  (* let ndist = ref 0 in *)
  fun a b -> (* time "dist" @$ fun () -> *)
  let (x1, y1) = a and (x2, y2) = b in
  if not (in_board x1 y1) || not (in_board x2 y2) then 0 else
    let distances = Cache.distances () in
  let ia = case_indice x1 y1 in
  let ib = case_indice x2 y2 in
  let dia = distances.Cache.values.(ia) in
  let dib = distances.Cache.values.(ib) in
  let fromb = distances.Cache.values.(ib).(ia) in
  match fromb with
      Some s -> process_distance fromb
    | None ->
	match distances.Cache.params.(ib) with
	  | (hd::tl, _) -> dist0 distances dib b ib ia
	  | ([], 0) ->
	    dist0 distances dia a ia ib
	  |   ([], _) -> 0
(*
let dist a b =
  try
    dist a b
  with _ ->
    chemin a b |> Array.length
*)

let print_sources ss =
  List.iter (fun (coef, p) -> Printf.printf "   %i at %i %i\n%!" coef (fst p) (snd p)) ss


let print_position f (x, y) = Printf.fprintf f "(%i, %i)" x y
