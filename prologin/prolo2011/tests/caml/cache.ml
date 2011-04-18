open Stdlib;;
open Api;;

type id_snake = int
let case_indice x y = x + y * taille_terrain

module Cache : sig
  type snake_cache = (id_snake, float ) H.t
  type connections = (id_snake * (int * int), bool) H.t
  type motos_id_cache = trainee_moto option array
  type snake_map = id_snake list array array

  type distances = {
    values : int option array array;
    params : (position list * int ) array
  }

  val reset : unit -> unit
  val snake_map : unit -> snake_map
  val connections : unit -> connections
  val snake_cache : unit -> snake_cache
  val motos_id : unit -> motos_id_cache
  val distances : unit -> distances

end = struct

  let ncases = taille_terrain * taille_terrain

  type snake_cache = (id_snake, float ) H.t
  type connections = (id_snake * (int * int), bool) H.t
  type motos_id_cache = trainee_moto option array
  type snake_map = id_snake list array array

  type distances = {
    values : int option array array;
    params : ( position list * int ) array
  }

  type cache = {
    snake_coefs : snake_cache;
    motos_id : motos_id_cache ref;
    connections : connections;
    snake_map : snake_map;
    distances : distances;
  }

  let mk_cache_snake : unit -> snake_cache = fun () -> H.create 2
  let mk_cache_motos : unit -> motos_id_cache = fun () ->
    let li = trainees_moto () |> Array.to_list in
    let ids = List.map (fun s -> s.id) li in
    let max = 1 + List.fold1 max ids in
    let arr = Array.make max None in
    let () = List.iter
      (fun s ->
	arr.(s.id) <- Some s
      ) li
    in arr
  let mk_cache_connections : unit -> connections = fun () -> H.create 0

  let mk_snake_map : unit -> snake_map = fun () ->
    Array.init taille_terrain (fun _ ->
      Array.init taille_terrain (fun _ -> [] ))

  let mk_distances () =
    let d = Array.init
      ncases ( fun i -> Array.make ncases None)
    in
    let li : position list = [] in
    let params = Array.make ncases (li, 0) in
    {values = d; params = params}

  let mk_cache : unit -> cache = fun () ->
    {
      snake_coefs = mk_cache_snake (); 
      motos_id = ref ( mk_cache_motos () );
      connections = mk_cache_connections ();
      snake_map = mk_snake_map ();
      distances = mk_distances ();
    }
  let cache : cache = mk_cache ()

  let motos_id () = ! ( cache.motos_id )
  let snake_cache () = cache.snake_coefs
  let connections () = cache.connections
  let snake_map () = cache.snake_map
  let distances () = cache.distances
  let reset_snake_cache () =
    let cache = cache.snake_coefs and
	connections = cache.connections and
	distances = cache.distances and
	snake_map = cache.snake_map in
    for x = 0 to taille_terrain - 1 do
      for y = 0 to taille_terrain - 1 do

	let indice1 = case_indice x y in
	distances.params.(indice1) <- ([], 0);
	let v0 = if case_traversable (x, y)
	  then None
	  else Some (-1) in
	for x2 = 0 to taille_terrain - 1 do
	  for y2 = 0 to taille_terrain - 1 do
	    let value =
	      if x = x2 && y = y2
	      then Some 0
	      else
		match v0 with
		  | Some _ -> v0
		  | None ->
		    if case_traversable (x2, y2) then
		      if x = x2 && y = y2
		      then Some 0
		      else None
		    else Some (-1)
	    in
	    distances.values.(indice1).(case_indice x2 y2) <- value;
	  done;
	done ;
      done;
    done;
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

  let reset () =
    reset_snake_cache ();
    cache.motos_id := mk_cache_motos ()
end
