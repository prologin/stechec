/*
 * Copyright (c) 2010 MLState
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * All advertising materials mentioning features or use of this software
 *       must display the following acknowledgement:
 *       This product includes software developed by MLState,
 *       and its contributors.
 *     * Neither the name of the MLState nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * @author Maxime Audouin <maxime.audouin@mlstate.com>
 */



// the rules are in this file.
year = 2010
title = "la guerre des toons"

type seq_state = {must_remove_ko} / {can_play_card} / {can_just_move}

type apply_move_acc =
  { st:seq_state; n_move_allowed : int;}

Rules = {{
  max_nbr_toons = 10
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
  | {titi} -> 3
  | {elmer} -> 3
  | {bipbip} -> 3
  | {coyote} -> error("coyote n'a pas de ko constant")

  in_board( (x, y), board : board ) =
    x >= 0 && x < board.size.f1 && y >= 0 && y < board.size.f2

  
  sizeX = 21
  sizeY = 21
  time = 15
  delay = 15
  spawn(t, pos) =
    (x, y) = pos
    decX = (sizeX - x ) / 2
    decY = (sizeY - y ) / 2
    match t with
    | {white} -> Coords.pos(10 - decX,  3 - decY)
    | {black} -> Coords.pos(10 - decX, 17 - decY)
  //
  spawn_toon(toon, team, board : board) =
    { board with board=Map.add(
        spawn(team, board.size),
        ({
          team=team;
          sick={ready_to_move};
          original_toon=toon;
          real_toon=toon;
          pa=0;
          n_attack=0;
          already_moved=true;
          attaques_gratuites = 0;
        } : toon_state),
        board.board      )
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
        | ~{ko} -> ko > 0
        | _ -> false
      )),
    state.board, false) : bool
  //
  fake_move_acc =
    {st = {must_remove_ko} ; n_move_allowed = 3;} : apply_move_acc
  initial_acc_verif(state:board) =
    {st = if Rules.has_people_ko( Team.other_team(state.toplay), state)
       then {must_remove_ko}
       else {can_play_card}
     n_move_allowed = 3;
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
      | {ko=1} -> {sleep}
      | ~{ko} -> if ko > 0 then {ko = ko - 1} else error("modif d'un ko <= 0")
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
    do jlog("end of game ?")
    lost(team) =
      Option.map(
        ( toon -> do jlog("titi state : {sick_to_string(toon.sick)}")
          match toon.sick with ~{ko} -> true _ -> false ),
        find_toon( (t -> t.original_toon == titi && t.team == team), board )) ? true // si on a plus de titi, alors on a perdu (ca arrive quand on degage des cases sur les cotes.)
    lw = lost(white)
    lb = lost(black)
    (lw, lb)
  //
  n_units_moved(board) =
    Map.fold(
      ( pos, toon, acc -> acc + (if toon.already_moved then 1 else 0)
      ), board.board, 0)
  never_move(toon:toon_state) = not(toon.already_moved)
  cards_match(a: card, b:card) =
    match (a, b) with
    | ({banzaii=_}, ~{banzaii}) -> true
    | ({doctor=_}, ~{doctor}) -> true
    | ({deguisement=_}, ~{deguisement}) -> true
    | ({pacifisme=_}, ~{pacifisme}) -> true
    | _ -> false
  remove_card(card, cards) =
    List.foldl(
    (card, (optcard, newli) ->
      Option.bind(
      (card2 ->
        if cards_match(card, card2)
        then some( (none, newli) )
        else none
      ),
      optcard) ? (optcard, List.cons(card, newli))
    ),
    cards, (some(card), [])
    ) |> snd
  switch_cards(cards, team, card) =
    TwoTeams.apply(team, remove_card(card, _), cards)
    |> TwoTeams.apply(Team.other_team(team), List.cons(card, _))
  contains_card(cards, team, card) =
    TwoTeams.get(team, cards) |>
      List.exists( cards_match(card, _), _)
  apply_move(session, acc:apply_move_acc, state:board, move) =
    err(s) = do Session.send(session, {INVALID_PARAM=s}:msg_game_client) none
    err_ko() = err("vous ne pouvez pas jouer maintenant : vous devez supprimer un point KO d'une unite ennemie avant.")
    (match move:diff_board with
    | {play_card=(card, coords)} ->
      pacifisme_apply(state) =
        state = Rules.toon_map(
          ( p, toon ->
            some( (p, {toon with n_attack = toon.n_attack - 1} ))
          ), state)
        acc = {acc with
          n_move_allowed = acc.n_move_allowed + 1}
        some( (acc, state) )
      if acc.st == {must_remove_ko} then err_ko()
      else if acc.st == {can_just_move} then err("vous ne pouvez plus jouer de cartes car vous avez deja bouge.")
      else (if contains_card(state.cards, state.toplay, card) then
        Option.map(
          (target_toon ->
            state = {state with cards = switch_cards(state.cards, state.toplay, card) } : board
            match card with
            | {banzaii} ->
               state =
                update_toon(state, coords,
                  (toon ->
                    {toon with
                      attaques_gratuites = toon.attaques_gratuites + 1;
                    } : toon_state
                  )
                )
              some( (acc, state) )
            | {doctor} ->
              state =
                update_toon(state, coords,
                  (toon ->
                    {toon with
                      sick ={ready_to_move};
                      pa = 0;
                      already_moved = true;
                      n_attack = 0;
                    } : toon_state
                  )
                )
              some( (acc, state) )
            | ~{deguisement} ->
              match deguisement with
              | {titi} -> err("Tu ne peux pas te deguiser en titi !")
              | _ ->
                state = update_toon(state, coords,
                  (toon ->
                    {toon with
                      real_toon = deguisement // je suis cache !
                      pa = get_max_move(deguisement)
                    }
                  ))
              some( (acc, state) )
              end
            | {pacifisme} -> pacifisme_apply(state)
            end
          ), get_toon(coords, state)
        )
      else err("vous ne possedez pas la carte.") )
        ?
        ( match card with
          | {pacifisme} ->
            state = {state with cards = switch_cards(state.cards, state.toplay, card) } : board
            pacifisme_apply(state)
          | _ -> err("aucun toon a cette position") )
    | {relever_un_toon=pos} ->
      if acc.st == {must_remove_ko} then err_ko()
      else
      Option.map( (toon ->
        if toon.team != state.toplay then err("relevez un toon de votre equipe.")
        else match toon.sick with
        | {ko = 0}
        | {sleep} ->
          update_toon(state, pos,
            (t ->
              {t with
                sick = {ready_to_move};
                pa=0;
                already_moved=true;
              }) : toon_state -> toon_state) |> some
        | _ -> err("votre toon n'est pas ko ou n'est pas a 0 points ko")
      ), get_toon(pos, state)) ? err("aucun toon a cette position")
      |> Option.map(state -> ({acc with st = {can_just_move}  }, state) )
    | {toon=from;attack=to} ->
      if acc.st == {must_remove_ko} then err_ko()
      else
      Option.map(
      (attaquant_toon ->
      if n_units_moved(state) >= acc.n_move_allowed && never_move(attaquant_toon) then err("vous ne pouvez plus actionner d'unites.")
      else if attaquant_toon.n_attack == 0 && attaquant_toon.attaques_gratuites == 0 then err("ce toon ne peut plus attaquer ce tour.")
      else if attaquant_toon.pa < 1 then err("vous n'avez pas assez de points d'actions : {attaquant_toon.pa}")
      else if attaquant_toon.team != state.toplay then err("attaquez avec votre toon !")
      else if attaquant_toon.sick != {ready_to_move} then err("vous ne pouvez pas attaquer : unite KO")
      else
      match attaquant_toon.real_toon with
      | {coyote} ->
        rec coyotes_explosions_positions(from, map) = // TODO DOES NOT WORK :(
          liko =
            [
              (Coords.pos(0, 1), 2),
              (Coords.pos(1, 0), 2),
              (Coords.pos(1, 1), 2),
              (Coords.pos(0, 2), 1),
              (Coords.pos(1, 2), 1),
              (Coords.pos(2, 2), 1),
              (Coords.pos(2, 1), 1),
              (Coords.pos(2, 0), 1)
            ]
          liko = liko ++ List.map(( (coords, p) -> (Coords.mult_e(coords, (-1, 1)) , p) ), liko)
          liko = liko ++ List.map(( (coords, p) -> (Coords.mult_e(coords, (1, -1)), p) ) , liko)
          liko = (Coords.pos(0, 0), 3) +> liko
          liko = List.map( ( (pos, force) -> (Coords.add(pos, from), force)), liko)
          List.foldl(
            ( (pos, force), map ->
              f2 = Map.get(pos, map) ? force
              newmap = Map.add(pos, max(f2, force), map)
              if Map.exists(pos, map) then newmap else
              match Map.get(pos, state.board) with
              | {some = toon } ->
                do jlog("victime");
                if toon.real_toon == {coyote} then
                  do jlog("explosions en series");
                  coyotes_explosions_positions(pos, newmap)
                else newmap
              | _ -> newmap
            ),
            liko,
            map
          )
        //
        liko = coyotes_explosions_positions(from, Map.empty) |> Map.To.assoc_list
        do List.iter( ( ((a, b), c) -> jlog("toto {a}, {b} X {c}")), liko)
        List.foldl(
          ((coords, ko), state ->
            update_toon(state, coords, (victime -> if victime.sick == {ready_to_move} then {victime with sick = {ko = ko}} else victime))
          ),
          liko,
          state
        ) |> some // on update pas n_attack parce-que l'attaquant est KO donc ce n'est pas important
      | attaquant ->
        Option.map(
          (victime ->
            // if victime.team == attaquant_toon.team then err("vous ne pouvez pas attaquer une unite de votre team !") else
            if victime.sick != {ready_to_move} then err("vous ne pouvez pas attaquer une unite deja KO") else
            porte = get_max_dist_attack(attaquant)
            cout = Coords.distance(from, to)
            if porte >= cout then
            nstate =
              update_toon(state, from,
                (attaquant ->
                  if attaquant.attaques_gratuites > 0
                  then {attaquant with attaques_gratuites = attaquant.attaques_gratuites - 1; already_moved = true }
                  else {attaquant with n_attack = attaquant.n_attack - 1; already_moved = true; pa = attaquant.pa - 1}))
              |> update_toon(_, to,
                (victime -> {victime with sick = {ko = Rules.get_ko(attaquant)}}))
            some(nstate)
            else err("cible hors de portee : {porte} <= {cout}.")
          ), get_toon(to, state)
        ) ? err("vous n'attaquez aucun toon.")
      ), get_toon(from, state)) ? err("aucun toon a cette position")
      |> Option.map(state -> ({acc with st = {can_just_move}  }, state) )
    | ~{from; to} ->
    if acc.st == {must_remove_ko} then err_ko()
    else if not(Rules.in_board(to, state)) then err("deplacez vous sur le plateau !")
    else match get_toon(from, state) with
    | {none} -> err("aucun toon a cette position ({from.f1}, {from.f2})")
    | {some = toon} ->
      if toon.team != state.toplay then err("deplacez votre toon !")
      else if toon.sick != {ready_to_move} then err("votre toon est KO ou dort !")
      else if n_units_moved(state) >= acc.n_move_allowed && never_move(toon) then err("vous ne pouvez plus actioner d'unites.")
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
    spawn = spawn(state.toplay, state.size)
    if {must_remove_ko} == acc.st then err_ko()
    else if not(Rules.in_board(spawn, state)) then err("le spawn n'est plus sur le plateau.")
    else if toon == titi then err("vous ne pouvez pas avoir deux titis sur la map.")
    else if n_units_moved(state) >= acc.n_move_allowed then err("vous ne pouvez plus actioner d'unites.")
    else if TwoTeams.get(state.toplay, state.nbr_toons_to_spawn) <= 0 then err("vous ne pouvez pas avoir plus de {Rules.max_nbr_toons} unites sur la map.")
    else
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
        | ~{ko} ->
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
  do TwoTeams.iter( (color, (_, s : option(channel(msg_game_client))) -> Option.iter(Session.send(_, msg(color)), s) ), state.sessions )
  state
//

next_player_board(board : board) =
  danger( pos ) =
    max = board.size
    pos.f1 == 0 || pos.f1 == max.f1 - 1 ||
    pos.f2 == 0 || pos.f2 == max.f2 - 1
  //
  (resize, new_size, tbr) =
    tmp = board.time_before_resizing - 1
    if tmp == -1 then
      (x, y) = board.size
      (true, (x - 2, y - 2), Rules.delay)
    else (false, board.size, tmp)
    board = {board with time_before_resizing = tbr;
      size = new_size }
    Rules.toon_map(
      ( (x, y), toon ->
        toon = { toon with
          real_toon = toon.original_toon; // carte deguisement
          pa = Rules.get_max_move(toon.original_toon);
          n_attack = 1;
          already_moved = false;
          attaques_gratuites = 0;
        }
        if resize then
          if danger( (x, y))
          then
            if toon.real_toon == titi
            then some( ( (x - 1, y - 1), {toon with sick={ko=1000}} ) ) // tomber dans le vide = 1000 pts ko
            else none
          else some( ( (x - 1, y - 1), toon ))
        else some( ( (x, y), toon ))
      ),
      {board with toplay = Team.other_team(board.toplay)} : board)
//

next_player(state) = { state with board = next_player_board(state.board) }

start_game(state) =
  _ = update(state)
  void
//

everybody_here(state:game_state) =
  TwoTeams.for_all((| (_, {none}) -> false | _ -> true), state.sessions)

kill_game(sessions, reason) =
  msg={end = reason} : msg_game_client
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

apply_diff_list(session, board, acc, moves) =
  List.foldl(
    (move, (acc_opt: option( (apply_move_acc, board) )) ->
      Option.bind(
        ( (acc, board : board) ->
          Option.map(
            ( (acc, board : board) -> (acc, board)),
            Rules.apply_move(session, acc, board, move)
          )
        ),
        acc_opt
      )
    ),
    moves,
    some( (acc, board) )
   )
   |> Option.bind( ((acc, s) ->
   if acc.st == {must_remove_ko} then none else some((acc, s))
   ) , _)


session_move(state : game_state, moves : list(diff_board), key, keys) =
  team = state.board.toplay
  sessions = state.sessions
  then_try(color, key) =
    ((key, session) = TwoTeams.get(color, sessions)
    if team == color && key == key then session
    else none):option(channel(msg_game_client))
  try(a, b) = Option.map(some, then_try(a, b))
  k = try(white, keys.white) ? then_try(black, keys.black)
  new_state = match k with
  | {some = session } ->
     Option.map( ( (a, b) -> (a, {state with board = b})),
       apply_diff_list(session, state.board, Rules.initial_acc_verif(state.board), moves)
     )
     |> Option.bind(  ((_, s) ->
       do jlog("regles valides")
       ns = next_player(s)
       do TwoTeams.iter( (color, (_, s) -> Option.iter(Session.send(_, {app_diffs = moves; me = color} ), s) ), state.sessions )
       do jlog("next player !")
       (lw, lb) = Rules.end_of_game(ns.board)
       if lw || lb then do jlog("[stats] FIN d'une partie") kill_game(state.sessions, TwoTeams.set2(lw, lb)) else some(ns)
       ), _)
  | {none} -> some(state)
  match new_state with // ne jamais tuer la session serveur.
  | {some = state} -> some(state)
  | {none} -> some(state)
//

session_factory(keys) =
  do jlog("[stats] LANCEMENT d'une partie")
  Session.make_opt( // server session
    game_start_state(keys), // session state
    ( state, msg ->
    do jlog("MOVE");
    match msg with
    | ~{register; key} -> session_register(state, some(register), key, keys)
    | ~{moves; key} -> session_move(state, moves, key, keys)
    )
  )
//


