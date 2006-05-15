open Api
open Misc
open Data

let perso id_ = object(self)
  val mutable orders = []
  val id = id_

  method id = id
  method add_order o = orders <- o::orders
  method add_orders o = orders <- o@orders
  method rm_order o = rm o orders
  method pop_order = match orders with
    | e::l -> orders <- l; e
    | _ -> failwith "Internal error. No order to pop."

  method get_action =
    let cmp (n1,_) (n2,_) = compare n2 n1 in
      orders <- List.sort cmp orders;
      (function ((_,a)::_) -> a | _ -> Wait) orders

  method execute =
    Printf.printf "[%d] pos: %d %d\n" id (position_joueur_x id) (position_joueur_y id);
    let o = self#get_action in
      (
	match o with
	  | Move(x,y) when x = position_joueur_x id && y = position_joueur_y id
							   -> self#pop_order;
							     self#add_order (nexplore x y)
	  | _ -> ()
      );
      exec id o;
end
