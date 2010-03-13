// the rules are in this file.
year = 2010
title = "la guerre des toons"

type seq_state = {must_remove_ko} / {can_play_card} / {can_just_move}

type apply_move_acc =
  { st:seq_state; n_attack: int }

Rules = {{
  get_max_move(t:toon) = match t with
  | {titi} -> 1
  | {elmer} -> 2
  | {bipbip} -> 5
  | {coyote} -> 3
  get_max_dist_attack(t:toon) = match t with
  | {titi} -> 1
  | {elmer} -> 3
  | {bipbip} -> 1
  | {coyote} -> 2
  get_ko(toon) = match toon with
  | {titi} -> 1
  | {elmer} -> 5
  | {bipbip} -> 3
  | {coyote} -> error("coyote n'a pas de ko constant")
  in_board( (x, y) ) = x >= 0 && x < sizeX && y >= 0 && y < sizeY
  sizeX = 20
  sizeY = 20
  spawn =
  | {white} -> Coords.pos(7,  2)
  | {black} -> Coords.pos(7, 17)
  spawn_toon(toon, team, board : board) =
    { board with board=Map.add(
        spawn(team),
        ({
          team=team;
          sick={ready_to_move};
          original_toon=toon;
          real_toon=toon;
          pa=0;
          already_attack=true;
          already_moved=true;
        } : toon_state),
        board.board
      )
    }
  //
  nbr_toons(team, state:board) =
    Map.fold(
    (_, toon, acc ->
      acc + (if team == toon.team then 1 else 0)),
    state.board, 0)
  //
  has_people_ko(team, state:board) =
    Map.fold(
    (_, toon, acc ->
      acc || (toon.team == team && match toon.sick with
        | {ko} -> true
        | _ -> false
      )),
    state.board, false) : bool
  //
  fake_move_acc =
    {st = {must_remove_ko} ; n_attack = 0 } : apply_move_acc
  initial_acc_verif(state:board) =
    {st = if Rules.has_people_ko( Team.other_team(state.toplay), state)
       then {must_remove_ko}
       else {can_play_card}
     n_attack = 0;
    }: apply_move_acc
  //
  update_toon(board, pos, update) =
    Option.map(
      (toon -> {board with board =
        Map.add(pos, update(toon), Map.remove(pos, board.board)) }),
      get_toon(pos, board)
    ) ? board
  //
  get_toon(pos, board : board) = Map.get(pos, board.board)
  update_ko(toon) =
    {toon with sick = match toon.sick with
      | {ko} -> if ko > 0 then {ko = ko - 1} else error("modif d'un ko <= 0")
      | _ -> error("tentative de modif sur un toon non malade")
    }
  //
  remove_toon(board : board, pos) = {board with board = Map.remove(pos, board.board)}
  add_toon(board : board, pos, toon) = {board with board = Map.add(pos, toon, board.board)}
  contains_toon(board : board, pos) = Map.exists(pos, board.board)
  toon_map(f : ( coords -> toon_state -> option( (coords, toon_state) ) ), board:board) =
    newmap =
      Map.fold(
        ( pos, toon, acc ->
          Option.map(
            ((pos, toon) -> Map.add(pos, toon, acc)),
            f(pos, toon)
          ) ? acc
        ), board.board, Map.empty
      )
    {board with board = newmap}
  //
  toon_fold(f, acc0, board) =
    Map.fold( f, board.board, acc0 )
  find_toon(prop, board) =
    toon_fold(
    (_, toon, acc ->
      Option.map(some, acc) ? if prop(toon) then some(toon) else none
    ), none, board)
  end_of_game(board) =
    // do jlog("end of game ?")
    Option.map(
      ( toon -> // do jlog("titi state : {sick_to_string(toon.sick)}")
        match toon.sick with {ko} -> true _ -> false ),
      find_toon( (t -> t.original_toon == titi), board )) ? false
  //
  n_units_moved(board)=
    Map.fold(
      ( pos, toon, acc -> acc + (if toon.already_moved then 1 else 0)
      ), board.board, 0)
  apply_move(session, acc:apply_move_acc, state:board, move) =
    err(s) = do Session.send(session, {INVALID_PARAM=s}:msg_game_client) none
    err_ko() = err("vous ne pouvez pas jouer maintenant : vous devez supprimer un point KO d'une unite ennemie avant.")
    (match move:diff_board with
    | {toon=from;attack=to} ->
      if acc.st == {must_remove_ko} then err_ko()
      else if acc.n_attack >= 1 then err("vous ne pouvez plus attaquer ce tour la.")
      else
      Option.map(
      (attaquant_toon ->
      if attaquant_toon.pa < 1 then err("vous n'avez pas assez de points d'actions : {attaquant_toon.pa}") else
      match attaquant_toon.real_toon with
      | {coyote} -> // todo checker les regles
        liko =
          [
            (Coords.pos(0, 1), 2),
            (Coords.pos(1, 0), 2),
            (Coords.pos(1, 1), 2),
            (Coords.pos(0, 2), 1),
            (Coords.pos(1, 2), 1),
            (Coords.pos(2, 2), 1)
          ]
        liko = liko ++ List.map(( (coords, p) -> (Coords.mult_e(coords, (-1, 1)) , p) ), liko)
        liko = liko ++ List.map(( (coords, p) -> (Coords.mult_e(coords, (1, -1)), p) ) , liko)
        liko = (Coords.pos(0, 0), 3) +> liko // todo additionner la position do coyote
        List.foldl(
          ((coords, ko), state ->
            update_toon(state, to, (victime -> if victime.sick == {ready_to_move} then {victime with sick = {ko = ko}} else victime))
          ),
          liko,
          state
        ) |> some
      | attaquant -> // todo check the move number
        Option.map(
          (victime ->
            if victime.team == attaquant_toon.team then err("vous ne pouvez pas attaquer une unite de votre team !") else
            if victime.sick != {ready_to_move} then err("vous ne pouvez pas attaquer une unite deja KO") else
            porte = get_max_dist_attack(attaquant)
            cout = Coords.distance(from, to)
            if porte >= cout then
            nstate =
              update_toon(state, from,
                (attaquant ->
                  {attaquant with already_attack = true; already_moved = true; pa = attaquant.pa - 1}))
              |> update_toon(_, to,
                (victime -> {victime with sick = {ko = Rules.get_ko(attaquant)}}))
            some(nstate)
            else err("cible hors de portee : {porte} <= {cout}.")
          ), get_toon(to, state)
        ) ? err("vous n'attaquez aucun toon.")
      ), get_toon(from, state)) ? err("aucun toon a cette position")
      |> Option.map(state -> ({acc with st = {can_just_move} ; n_attack = acc.n_attack + 1 }, state) )
    | {from; to} ->
    if acc.st == {must_remove_ko} then err_ko()
    else if not(Rules.in_board(to)) then err("deplacez vous sur le plateau !")
    else match get_toon(from, state) with
    | {none} -> err("aucun toon a cette position ({from.f1}, {from.f2})")
    | {some = toon} ->
      if toon.team != state.toplay then err("deplacez votre toon !")
      else if toon.sick != {ready_to_move} then err("votre toon est KO ou dort !")
      else if n_units_moved(state) >= 3 then err("vous ne pouvez actioner que trois unites.")
      else if contains_toon(state, to) then err("vous ne pouvez pas vous deplacer sur un autre toon")
      else
        cout = Coords.distance(from, to)
        if toon.pa < cout
        then err("vous n'avez pas assez de points d'actions ({toon.pa} < {cout}).")
        else
        some( (
          {acc with st = {can_just_move}},
          add_toon(remove_toon(state, from), to, {toon with pa = toon.pa - cout ; already_moved=true})
          ))
    end
    | {born=toon} ->
    if {must_remove_ko} == acc.st then err_ko()
    else if toon == titi then err("vous ne pouvez pas avoir deux titis sur la map.")
    else if TwoTeams.get(state.toplay, state.nbr_toons_to_spawn) <= 0 then err("vous ne pouvez pas avoir plus de 8 unites sur la map.")
    else
      spawn = spawn(state.toplay)
      match get_toon(spawn, state) with
      | {none} ->
        s = spawn_toon(toon, state.toplay, state)
        new_state = {s with nbr_toons_to_spawn = TwoTeams.apply(s.toplay, (_ - 1), s.nbr_toons_to_spawn)}
        some( ( {acc with st={can_just_move} } , new_state ) )
      | {some = toon} -> err("vous avez deja une unite sur le spawn ({toon_to_string(toon.original_toon)})")
      end
    | {remove_ko = pos} ->
    if acc.st == {must_remove_ko}
    then
      match get_toon(pos, state) with
      | {none} -> err("aucun toon a cette position")
      | {some=toon} ->
        if toon.team == state.toplay
        then err("vous devez enlever un point KO a une unite ennemie.")
        else (match toon.sick with
        | {ko} ->
          if ko > 0
          then
            nstate = update_toon(state, pos, update_ko)
            some(  ( {acc with st = {can_play_card}} , nstate) )
          else err("unite deja a 0 points KO")
        | _ -> err("l'unite n'est pas KO")
        )
    else do err("Vous avez deja supprime un KO") none
    ) : option( (apply_move_acc, board)  )
  //
}}


type game_state =
{
  sessions : teams_info( (key, option(channel(msg_game_client))))
  board : board
}

game_start_state(keys) =
({
  sessions = TwoTeams.combine(keys, TwoTeams.make(none));
  board = Rules.spawn_toon(titi, white, Rules.spawn_toon(titi, black, empty_board))
} |> next_player ) : game_state


update(state : game_state) =
  msg(b) = {
    board=state.board;
    me = b
  } : msg_game_client
  do TwoTeams.iter( (color, (_, s) -> Option.iter(Session.send(_, msg(color)), s) ), state.sessions )
  state
//

next_player(state : game_state) =
  {state with board =
    Rules.toon_map(
      ( (x, y), toon ->
        // todo supprimer les unites mortes, gerer les cartes, etc...
        toon = { toon with
          real_toon = toon.original_toon; // carte deguisement
          pa = Rules.get_max_move(toon.original_toon);
          already_attack = false;
          already_moved = false;
        }
        some( ( (x, y), toon ))
      ),
      {state.board with toplay = Team.other_team(state.board.toplay)} : board)
  } : game_state
//

start_game(state) =
  _ = update(state)
  void
//

everybody_here(state:game_state) =
  TwoTeams.for_all((| (_, {none}) -> false | _ -> true), state.sessions)

kill_game(sessions) =
  msg={end} : msg_game_client
  send(x) = Option.iter((Session.send(_, msg)), x)
  do TwoTeams.iter( (_, s -> send(snd(s))), sessions)
  none
//
session_register(state:game_state, register, key, keys) =
  state = {state with sessions = TwoTeams.apply_each_colors(
  (color, (key2, old_register) -> if key == key2
    then (key2, register)
    else (key2, old_register)
  ), state.sessions) }
  do if everybody_here(state) then start_game(state)
  some(state)
//
session_move(state : game_state, moves : list(diff_board), key, keys) =
  team = state.board.toplay
  sessions = state.sessions
  then_try(color, key) =
    ((key, session) = TwoTeams.get(color, sessions)
    if team == color && key == key then session
    else none):option(channel(msg_game_client))
  try(a, b) = Option.map(some, then_try(a, b))
  k = try(white, keys.white) ? then_try(black, keys.black)
  match k with
  | {some = session } ->
    List.foldl(
      (move, (acc_opt: option( (apply_move_acc, game_state) )) ->
        Option.bind(
          ( (acc, state : game_state) ->
            Option.map(
              ( (acc, board : board) -> (acc, {state with board = board } : game_state)),
              Rules.apply_move(session, acc, state.board, move)
            )
          ),
          acc_opt
        )
      ),
      moves,
      some( (Rules.initial_acc_verif(state.board), state))
     )
     |> Option.bind( ((acc, s) ->
     if acc.st == {must_remove_ko} then none else some((acc, s))
     ) , _)
     |> Option.bind(  ((_, s) ->
       ns = update(next_player(s))
       if Rules.end_of_game(ns.board) then do jlog("[stats] FIN d'une partie") kill_game(state.sessions) else some(ns)
       ), _)
     // affiche tout les coups d'un coup, on pourrait peut-etre ajouter un defilement.
  | {none} -> some(state)
//

session_factory(keys) =
  do jlog("[stats] LANCEMENT d'une partie")
  Session.make_opt( // server session
    game_start_state(keys), // session state
    ( state ->
    | {register; key} -> session_register(state, some(register), key, keys)
    | {moves; key} -> session_move(state, moves, key, keys)
    )
  )
