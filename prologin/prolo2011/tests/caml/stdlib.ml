
let ( |> ) f x = x f

let unsome = function None -> assert false | Some x -> x
let unsome_list xs = List.map (function None -> [] | Some x -> [x]) xs |> List.flatten
let sum = List.fold_left ( +. ) 0.0
let max_by f a b = if f a > f b then a else b
let maximum_by f = function
  | [] -> None
  | x::xs -> Some (List.fold_left (max_by f) x xs)


let ( ++ ) (x, y) (x', y') = (x + x', y + y')

let near xs p = List.map (( ++ ) p) xs
let dxys = [ 1, 0 ; -1, 0 ; 0, 1 ; 0, -1 ]
let neighbours = near dxys
let dneighbours = near (dxys @ [ -1,-1 ; -1,1 ; 1,-1; 1,1 ])


module List = struct
  include List
  let fold1 f li =
    match li with
      | hd::tl ->
	fold_left f hd tl
      | [] -> assert false
end

module H = Hashtbl

module type PQueueItem = sig
  type t
  val compare : t -> t -> int
end

module PQueue (Item : PQueueItem) : sig
  type t
  val insert : Item.t -> t -> unit
  val pop : t -> Item.t option
  val empty : unit -> t
end = struct

  type t = Item.t list ref

  let insert item t =
    let rec f li acc =
      match li with
	| hd::tl ->
	  let cmp = Item.compare item hd in
	  if cmp > 0 then f tl ( hd :: acc )
	  else List.rev_append acc ( item :: li )
	| [] -> List.rev (item :: acc)
    in let li = f !t [] in
       t := li

  let empty () = ref []

  let pop (t:t) = match !t with
    | hd::tl ->
      begin
	t := tl;
	Some hd
      end
    | [] -> None
end
