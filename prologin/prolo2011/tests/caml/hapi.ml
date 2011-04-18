include Api;;
open Stdlib;;
open Cache;;


let annuler () = cancel ()

let moto_by_id id =
  match (Cache.motos_id () ).(id) with
    | None -> begin
      Array.iter afficher_trainee_moto (trainees_moto ()) ;
      Printf.printf "ID RECHERCHE = %i\n%!" id ;
      failwith "pas de motos" end
    | Some x -> x

let dist a b =
  let (x1, y1) = a and (x2, y2) = b in
  let distances = Cache.distances () in
  let ia = case_indice x1 y1 in
  let ib = case_indice x2 y2 in
  let dia = distances.(ia) in
  let () =  if dia.(ib) = None then
      let min_distance = ref 0 in
      let to_process = ref [a] in
      while !to_process != [] do
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
		  if x3 >= 0 && x3 < taille_terrain &&
		    y3 >= 0 && y3 < taille_terrain then
		    let i3 = case_indice x3 y3 in
		    let old_d = dia.(i3) in
		    match old_d with
		      | None ->
			begin
			  to_process := (x3, y3) :: (! to_process);
			  dia.(i3) <- some_new_min_d;
			end
		      | Some -1 -> ()
		      | Some x -> if x > new_min_d
			then assert false
		)
	)
	  to_process2;
	min_distance := !min_distance + 1;
      done;
  in
  match dia.(ib) with
    | None -> 0
    | Some (-1) -> 0
    | Some s ->
      let other = chemin a b |> Array.length in
      assert (other = s);
      s
(* chemin a b |> Array.length *)

let print_sources ss =
  List.iter (fun (coef, p) -> Printf.printf "   %i at %i %i\n%!" coef (fst p) (snd p)) ss


let print_position f (x, y) = Printf.fprintf f "(%i, %i)" x y
