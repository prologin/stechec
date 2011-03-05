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





/*
 todo :
   - grep sur les todos
   - explosion en series
*/


// renderer constants
dx = 30
dy = 30

type client_state =
{
  session:option(channel(msg_game_client));
  board:board;
  original_board:board;
  click:option(coords);
  attaque:bool;
  me:team;
  difflist : list(diff_board);
  rules_acc : apply_move_acc;
}

client_state_init =
{
  click=none;
  difflist=[];
  me=white;
  board=empty_board
  original_board=empty_board;
  attaque = false;
  session=none;
  rules_acc=Rules.fake_move_acc
}:client_state

start((client_id, client_key), session : channel(msg_game) ) =
  tbl_jq = {id="tbl"}:jQs
  spawn(s : channel(msg_game_client), toon) =
    Session.send(s, {spawn=toon}:msg_game_client)
  show_deguisement() =
    _ = jQuery.removeClass("hidden", #pieces_deguisement)
    void
  hide_deguisement() =
    _ = jQuery.addClass("hidden", #pieces_deguisement)
    void
  pieces(onclick, team) =
      List.foldl(
        ( toon, acc ->
          <>{acc}<div onclick={_ -> onclick(toon)}>{
            toon_to_xhtml(false, toon, team)
          }</div></>
        ),
        types_toons, <></>
      )
  play_card(team, s, card) =
    match card with
    | ~{deguisement} ->
      p = pieces(
      (toon ->
         do Session.send(s,
           {play_card = {deguisement=toon}}:msg_game_client)
         do hide_deguisement()
         void), team)
      do exec_actions(
        [#pieces_deguisement <- <>{p}<div class="bouton" onclick={_ -> hide_deguisement()}>cancel</div></> ]
      )
      do show_deguisement()
      void
    | card -> Session.send(s, {play_card = card} : msg_game_client)
  show(me, s, board:board) =
    (board_xml, style, _) = Map.fold(
    (  (x, y),
       toon,
       (xhtml, style, n) ->
      // {x=x0; y=y0} = jQuery.offseti(jQs(tbl_jq))
        id = "toon_{n}"
        style=List.cons(
          ([{position={absolute}}, {left={px=x * dx /* + x0 */}}, {top={px=y * dy /*+ y0 */ }}], id),
          style
          )
        xhtml = <><div id={id}>{
          toon_to_xhtml(false, toon.real_toon, toon.team)
        }</div>{xhtml}</>
        (xhtml, style, n+1)
      ),
      board.board,
      (<div id="select"></div>,
      [ ([{position={absolute}}, {left={px=-1000}}, {top={px=-1000}}], "select") ],
      0)
      )
    size = css { width: {board.size.f1 * dx}px; height: {board.size.f2 * dy}px}
    _ = jQuery.setTypedCss(size, jQs(tbl_jq))
    do exec_actions([ {tbl_jq} <- board_xml ])
    do List.iter( ((style, id) -> _ = jQuery.setTypedCss(style, #{id}) void), style)

    cards = List.foldl(
      (card, acc ->
        <><div class={["mycard", "card"]} onclick={ _ -> play_card(me, s, card)}>{xhtml_of_card(card)}</div>{acc}</>
      ),
      TwoTeams.get(me, board.cards),
      List.foldl(
      (card, acc ->
        <><div class={["notmycard", "card"]}>{xhtml_of_card(card)}</div>{acc}</>
      ),
      TwoTeams.get(Team.other_team(me), board.cards),<></>
      )
    )

    do exec_actions( [#cards <- cards] )
    
    void
  //
  attaque_msg = <>cliquez ici pour annuler l'attaque.</>
  no_attaque_msg = <>cliquez ici pour attaquer</>
  get_diff(attaque, pos0, pos1) =
    (if attaque
    then {toon=pos0; attack=pos1}
    else {from=pos0; to=pos1}) : diff_board
  log(s:string) = do [ #log <- <span class="log">{s}</span>] void
  attaque(b) =
    do [#attaque <- if b then attaque_msg else no_attaque_msg ]
    void
  try_add_to_difflist(s, acc, diff, list, board) =
    match Rules.apply_move(s, acc, board, diff) with
    | {none} -> (acc, list, board, false)
    | {some = (acc2, b)} -> (acc2, List.cons(diff, list), b, true)
  hide_infos() =
    do exec_actions([#infos <- <></> ])
    void
  show_infos(toon) =
    i = toon_state_to_xhtml(toon)
    do exec_actions([#infos <- <div onclick={_ -> do hide_infos() void}>{i}</div> ])
    void
  get_diff_ko(pos) = {remove_ko=pos} : diff_board
  click_toon((pos: coords), toon:toon_state, size) =
    pa = (toon.pa, toon.pa)
    (x, y) = Coords.rem(pos, pa)
    (x, y) = Coords.e_max((x, y), (0, 0)) // ne pas depasser
    
    (w, h) = Coords.add(
      Coords.add( (1, 1), pa),
      Coords.rem( pos, (x, y))
      )
    (w, h) = Coords.e_min((w, h), (size.f1 - x, size.f2 - y)) // ne pas depasser
        
    jQuery.setTypedCss([{left={px= x * dx}}, {top={px = y * dx}}, {width={px = w * dx}}, {height={px = h * dy}}], `$`("select"))
  click(optpos : option(coords)) =
    (x, y) = optpos ? (-1000, -1000)
    jQuery.setTypedCss([{left={px= x * dx}}, {top={px = y * dx}}, {width={px = dx}}, {height={px = dy}}], `$`("select"))
  s = Session.make_opt( // session client
    client_state_init,
    (state:client_state, msg ->
      board = state.board
      match msg : msg_game_client with
      | ~{end} ->
        action_none(event) = void
        do if TwoTeams.get(state.me, end)
           then
             if TwoTeams.get(Team.other_team(state.me), end)
             then log("Match nul !")
             else log("You just lost the game !")
        else log("Felicitation ! tu as gagne !")
        do List.map( `$`, ["tbl", "attaque", "ok", "cancel"])
             |> List.iter( (x -> do jQuery.click(action_none, x) void), _)
        do [#pieces <- <></>, #info <- <>Fin du jeu</> ]
        none
      | {set_session = session} -> some({state with session = some(session)})
      | {attaque = _} ->
        default(a) =
          do attaque(a)
          do hide_infos()
          some( {state with attaque = a} )
        
        if board.toplay != state.me then some(state) else
        a = not(state.attaque)
        if a then
          match state.click with
          | {some=pos} ->
            match Rules.get_toon(pos, state.board) with
            | {none} -> default(a)
            | {some=toon} ->
              match toon.real_toon with
              | {coyote} ->
                (acc, difflist, board, b) = try_add_to_difflist(Option.get(state.session), state.rules_acc, get_diff(true, pos, pos), state.difflist, state.board)
                do show(state.me, Option.get(state.session), board)
                do hide_infos()
                some( {state with difflist = difflist; click=none; attaque=false; board=board; rules_acc=acc})
              | _ -> default(a)
              end
            end
          | {none} ->
            default(a)
        else default(a)
      | {INVALID_PARAM=s} ->
        do jQuery.removeClass("hidden", #right)
        do [#log <- <span class="error">{s}</span>]
        some(state)
      | {spawn=toon} ->
        if board.toplay != state.me then some(state) else
        (acc, difflist, board, b) = try_add_to_difflist( Option.get(state.session), state.rules_acc, {born=toon}, state.difflist, state.board )
        do show(state.me, Option.get(state.session), board)
        do hide_infos()
        some( {state with difflist = difflist; rules_acc = acc; board = board ; attaque = if b then false else state.attaque})
      | ~{app_diffs; me} ->
        do jlog("app_diffs")
        if me != state.board.toplay then
        (rec f(diffs, state) = // fonction de defilement
        (match diffs with
          | [] ->
            (
            do jlog("end of diffs")
            do jQuery.removeClass("hidden", #right)
            do log( if state.rules_acc.st == {must_remove_ko} then "vous devez supprimer un point KO" else "vous pouvez jouer une carte ou bouger")
            board = next_player_board(state.board)
            do if jQuery.size(#toplay) == 1 then exec( [
              #toplay <- Team.team_to_string(board.toplay),
              #pieces <- pieces(spawn(Option.get(state.session), _), me),
              #team <- Team.team_to_string(me) ])
            acc = Rules.initial_acc_verif(board)
            do show(me, Option.get(state.session), board)
            Session.send(Option.get(state.session), {set_state={state with board = board; me = me; difflist=[]; click=none; original_board = board; rules_acc = acc}})
            )
          | [diff|tail] ->
            do jlog("apply a diff")
            (match apply_diff_list(Option.get(state.session), state.board, state.rules_acc, [diff]) with
            | {none} -> error("should never append")
            | {some=(acc, board) } ->
              do show(state.me, Option.get(state.session), board)
              do sleep(600, (-> f(tail, {state with rules_acc = acc ; board = board } )))
              void
            )
          )
        do hide_infos()
        do sleep(600, -> f(app_diffs, {state with board = state.original_board; rules_acc = Rules.initial_acc_verif(state.original_board)})) // le sleep est important ici pour ne pas se retrouver dans un etat incoherent et pour ne pas setter hidden apres avoir tente d'enlever hidden. ca arrive quand la liste est vide
        do jQuery.addClass("hidden", #right)
        some(state)
        )
        else
          do jlog("apply all diffs")
          (match apply_diff_list(Option.get(state.session), state.original_board, Rules.initial_acc_verif(state.original_board), app_diffs) with
            | {none} -> error("should never append")
            | {some=(_, board) } ->
              do jQuery.removeClass("hidden", #right)
              board = next_player_board(board)
              do if jQuery.size(#toplay) == 1 then
                exec([ #toplay <- Team.team_to_string(board.toplay),
                #pieces <- pieces(spawn(Option.get(state.session), _), me),
                #team <- Team.team_to_string(me) ])
              do show(me, Option.get(state.session), board)
              some(
              {state with
                original_board = board;
                board = board;
                rules_acc = Rules.initial_acc_verif(board);
                me = me; difflist=[]; click=none;
             } ))
      | ~{set_state} -> some(set_state)
      | ~{board; me} ->
        do if jQuery.size(#toplay) == 1 then exec([
          #toplay <- Team.team_to_string(board.toplay),
          #pieces <- pieces(spawn(Option.get(state.session), _), me),
          #team <- Team.team_to_string(me) ])
        do show(state.me, Option.get(state.session), board)
        acc = Rules.initial_acc_verif(board)
        do hide_infos()
        do log( if acc.st == {must_remove_ko} then "vous devez supprimer un point KO" else "vous pouvez jouer une carte ou bouger")
        some( {state with me = me; original_board = board; board = board ; difflist=[]; click=none; rules_acc = acc})
      | {cancel} ->
        if board.toplay != state.me then some(state) else
        do show(state.me, Option.get(state.session), state.original_board)
        do attaque(false)
        do hide_infos()
        some( {state with board = state.original_board; difflist = []; click = none; attaque = false; rules_acc = Rules.initial_acc_verif(state.original_board)})
      | {ok} ->
        if board.toplay != state.me then some(state) else
        do Session.send(session, {moves=List.rev(state.difflist); key=client_key}:msg_game)
        do attaque(false)
        do hide_infos()

        do if state.rules_acc.st != {must_remove_ko} then
           _ = jQuery.addClass("hidden", #right) void
        
        some( {state with click = none; difflist = []; attaque = false})
      | ~{remove_ko} ->
        if board.toplay != state.me then some(state) else
        match state.click with
        | {some = pos} ->
          (acc, difflist, board, b) = try_add_to_difflist(Option.get(state.session), state.rules_acc, get_diff_ko(pos), state.difflist, state.board)
          do show(state.me, Option.get(state.session), board)
          do click(none)
          do hide_infos()
          some( {state with difflist = difflist; click=none; attaque=if b then false else state.attaque; board=board; rules_acc=acc})
        | {none} -> do log("aucune unite selectionnee.") some(state)
        end
      | {play_card = card} ->
        apply_card(pos) =
          (acc, difflist, board, b) = try_add_to_difflist(Option.get(state.session), state.rules_acc, {play_card = (card, pos)}, state.difflist, state.board)
          do show(state.me, Option.get(state.session), board)
          do click(none)
          do hide_infos()
          some( {state with difflist = difflist; click=none; attaque=if b then false else state.attaque; board=board; rules_acc=acc})
        if board.toplay != state.me then some(state) else
          match state.click with
          | {some = pos} -> apply_card(pos)
          | {none} ->
            match card with
            | {pacifisme} -> apply_card( (0,0))
            | _ -> do log("aucune unite selectionnee.") some(state)
            end
          end
      | {relever_un_toon} ->
        if board.toplay != state.me then some(state) else
          match state.click with
          | {some = pos} ->
            (acc, difflist, board, b) = try_add_to_difflist(Option.get(state.session), state.rules_acc, {relever_un_toon=pos}, state.difflist, state.board)
            do show(state.me, Option.get(state.session), board)
            do click(none)
            do hide_infos()
            some( {state with difflist = difflist; click=none; attaque=if b then false else state.attaque; board=board; rules_acc=acc})
          | {none} -> some(state)
          end
      | {click=pos1} ->
        if board.toplay != state.me then some(state) else
        do log("click : ({pos1.f1}, {pos1.f2})")
        match state.click with
        | {some=pos0} ->
          do attaque(false)
          (acc, difflist, board, b) = try_add_to_difflist(Option.get(state.session), state.rules_acc, get_diff(state.attaque, pos0, pos1), state.difflist, state.board)
          do show(state.me, Option.get(state.session), board)
          do hide_infos()
          some( {state with difflist = difflist; click=none; attaque=if b then false else state.attaque; board=board; rules_acc=acc})
        | {none=_} ->
          match Rules.get_toon(pos1, state.board) : option(toon_state) with
          | {none} ->
            do click(none)
            do log("cliquez sur un toon !")
            some(state)
          | {some = toon} ->
            do click_toon((pos1:coords), toon, board.size)
            do show_infos(toon)
            some( {state with click = some(pos1) })
          end
    ) : (client_state -> msg_game_client -> option(client_state))
  ) : channel(msg_game_client)
  do Session.send(s, {set_session=s} : msg_game_client)
  click_on_board(e) =
    ~{x; y} = jQuery.offseti(jQs(tbl_jq))
    //do log("offseti(...).x = {x} ; offseti(...).y = {y}; dx = {dx} ; dy = {dy}")
    x = (e.pageX - x ) / dx
    y = (e.pageY - y ) / dy
    do Session.send(s, {click=(x, y)} : msg_game_client)
    []
  //
  do Session.send(session, {register=s; key=client_key} : msg_game )
  do [#main <-
    <>
    <div id="pieces_deguisement" class={["hidden", "front"]}></div>
    <div id="tbl" onclick={ click_on_board }></div>
    <div id="right">
      <div id="info">Vous jouez les <span id="team"></span>, c'est aux <span id="toplay"></span> de jouer</div>
      <div class="bouton" id="attaque" onclick={_ -> Session.send(s, {attaque})} >{no_attaque_msg}</div>
      <div class="bouton" id="ko" onclick={_ -> Session.send(s, {remove_ko})}>enlever un point KO</div>
      <div class="bouton" id="ok" onclick={_ -> Session.send(s, {ok}:msg_game_client )} >valider</div>
      <div class="bouton" id="cancel" onclick={_ -> Session.send(s, {cancel}:msg_game_client )} >annuler</div>
      <div class="bouton" id="relever" onclick={ _ -> Session.send(s, {relever_un_toon} : msg_game_client)}>relever un toon</div>
      <div id="pieces"></div>
      <div id="log" style="clear:both"></div>
      <div id="infos"></div>
      <div id="cards"></div>
    </div>
    </>
  ]
  void
