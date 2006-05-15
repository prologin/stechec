open Api
open Misc
open Printf

let visited =
  let res = Array.create _MAX_DATE (Array.create_matrix mapx mapy (-1)) in
    for i = 0 to _MAX_DATE -1 do
      res.(i) <- Array.create_matrix mapx mapy (-1)
    done;
    printf "lala\n";
    res

let copy m =
  let res = Array.create_matrix mapx mapy (-1) in
    for i = 0 to Array.length m - 1 do
      res.(i) <- Array.copy m.(i)
    done;
    res

let push_visit () =
  assert (!turn > 0);
  visited.(!turn) <- copy (visited.(!turn - 1))

let update_visited () =
  push_visit();
  map_iter (fun i -> fun j ->
	      if visible i j = 1 then
 		visited.(!turn).(i).(j) <- !turn
	   )

let fogn =
  let res = Array.create_matrix mapx mapy (-1) in
  let count x y =
    let res = ref 0 in
      map_iter
	(fun i -> fun j ->
	   if (visible_depuis x y i j) = 1 then
	     incr res;
	);
      !res
  in
    map_iter (fun i -> fun j -> res.(i).(j) <- count i j);
    res

let get_hidden () =
  let res = ref [] in
    map_iter (fun i -> fun j ->
 		if (regarde i j = _STANDARD) && (visible i j = 0) then
		  res := (fogn.(i).(j), i, j)::!res
	     );
    !res

let rec explore n =
  let l = get_hidden () in
  let cmp (a,_,_) (b,_,_) = compare b a in
  let (_,a,b) = List.nth (List.sort cmp l) n in
    (50, Move(a,b))

let rec nexplore x y =
  let dist a b = abs (x - a) + abs (y - a) in
  let l =  (get_hidden ()) in
  let cmp (a,x1,y1) (b,x2,y2) =
    let tmp = compare (fogn.(x2).(y2)) (fogn.(x1).(y1)) in
      if tmp <> 0 then tmp
      else compare b a in
  let (_,a,b) = List.hd (List.sort cmp l) in
    (50, Move(a,b))

let print_mat () =
  map_iter2 (fun i -> fun j -> printf "%d\t" fogn.(i).(j)) (fun _ -> printf "\n");;

print_mat ();;
