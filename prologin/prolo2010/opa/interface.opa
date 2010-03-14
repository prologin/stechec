// todo KO
// todo cartes
// todo retrecissement de la map
// todo infos sur les unites
// todo meilleur gestion du click : ne pas pouvoir cliquer dans le vide, et afficher en jaune la case selectionnee devrait suffire


// todo annuller une seule action
// todo afficher la liste d'actions (pouvoir revennir)


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
  rules_acc : apply_move_acc
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

start(session : channel(msg_game) ) =
  tbl_jq = {id="tbl"}:jQs
  spawn(s : channel(msg_game_client), toon) =
    Session.send(s, {spawn=toon}:msg_game_client)
  pieces(s, team) =
      List.foldl(
        ( toon, acc -> <>{acc}<span onclick={spawn(s, toon)}>{toon_to_xhtml(false, toon, team)}</span></>),
        types_toons, <></>
      )
  show(board) =
    board = Map.fold(
    (  (x, y),
       toon,
       acc ->
      // {x=x0; y=y0} = jQuery.offseti(jQs(tbl_jq))
      style=[{position={absolute}}, {left={px=x * dx /* + x0 */}}, {top={px=y * dy /*+ y0 */ }}]
        <><div style={style}>{
          toon_to_xhtml(false, toon.real_toon, toon.team)
        }: {x}, {y}</div>{acc}</>
      ),
      board.board,
      <div id="select" style={[{position={absolute}}, {left={px=-1000}}, {top={px=-1000}}]}></div>)
    do [ {tbl_jq} <- board ]
    void
  //
  attaque_msg = <>cliquez ici pour annuller l'attaque.</>
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
  show_infos(toon) =
    do [#infos <- toon_state_to_xhtml(toon) ]
    void
  get_diff_ko(pos) = {remove_ko=pos} : diff_board
  click_toon((pos: coords), toon:toon_state) =
    pa = (toon.pa, toon.pa)
    (x, y) = Coords.add(pos, Coords.mult_e(pa, (-1, -1)) )
    (w, h) = Coords.add( (1, 1), Coords.mult_e(pa, (2,2) ))
    jQuery.setTypedCss([{left={px= x * dx}}, {top={px = y * dx}}, {width={px = w * dx}}, {height={px = h * dy}}], `$`("select"))
  click(optpos : option(coords)) =
    (x, y) = optpos ? (-1000, -1000)
    jQuery.setTypedCss([{left={px= x * dx}}, {top={px = y * dx}}, {width={px = dx}}, {height={px = dy}}], `$`("select"))
  s = Session.make_opt( // session client
    client_state_init,
    (state:client_state, msg ->
      board = state.board
      match msg : msg_game_client with
      | {end} ->
        action_none(event) = void
        do if board.toplay == state.me then log("You just lost the game !")
        else log("YEAH tay 1 roxor !")
        do List.map( `$`, ["tbl", "attaque", "ok", "cancel"])
             |> List.iter( (x -> do jQuery.click(action_none, x) void), _)
        do [#pieces <- <></>, #info <- <>Fin du jeu</> ]
        none
      | {set_session = session} -> some({state with session = some(session)})
      | {attaque = _} ->
        if board.toplay != state.me then some(state) else
        a = not(state.attaque)
        do attaque(a)
        some( {state with attaque = a} )
      | {INVALID_PARAM=s} ->
        do [#log <- <span class="error">{s}</span>]
        some(state)
      | {spawn=toon} ->
        if board.toplay != state.me then some(state) else
        (acc, difflist, board, b) = try_add_to_difflist( Option.get(state.session), state.rules_acc, {born=toon}, state.difflist, state.board )
        do show(board)
        some( {state with difflist = difflist; rules_acc = acc; board = board ; attaque = if b then false else state.attaque})
      | {board; me} ->
        do [
          #toplay <- Team.team_to_string(board.toplay),
          #pieces <- pieces(Option.get(state.session), me),
          #team <- Team.team_to_string(me) ]
        do show(board)
        acc = Rules.initial_acc_verif(board)
        do log( if acc.st == {must_remove_ko} then "vous devez supprimer un point KO" else "vous pouvez jouer une carte ou bouger")
        some( {state with me = me; original_board = board; board = board ; difflist=[]; click=none; rules_acc = acc})
      | {cancel} -> // todo clear the diff list
        if board.toplay != state.me then some(state) else
        do show(state.original_board)
        do attaque(false)
        some( {state with board = state.original_board; difflist = []; click = none; attaque = false; rules_acc = Rules.initial_acc_verif(board)})
      | {ok} ->
        if board.toplay != state.me then some(state) else
        do Session.send(session, {moves=List.rev(state.difflist); key=client_key}:msg_game)
        do attaque(false)
        some( {state with click = none; difflist = []; attaque = false})
      | {remove_ko} ->
        if board.toplay != state.me then some(state) else
        match state.click with
        | {some = pos} ->
          (acc, difflist, board, b) = try_add_to_difflist(Option.get(state.session), state.rules_acc, get_diff_ko(pos), state.difflist, state.board)
          do show(board)
          do click(none)
          some( {state with difflist = difflist; click=none; attaque=if b then false else state.attaque; board=board; rules_acc=acc})
        | {none} -> do log("aucune unite selectionnee.") some(state)
        end
      | {click=pos1} ->
        if board.toplay != state.me then some(state) else
        do log("click : ({pos1.f1}, {pos1.f2})")
        match state.click with
        | {some=pos0} ->
          do attaque(false)
          (acc, difflist, board, b) = try_add_to_difflist(Option.get(state.session), state.rules_acc, get_diff(state.attaque, pos0, pos1), state.difflist, state.board)
          do show(board)
          some( {state with difflist = difflist; click=none; attaque=if b then false else state.attaque; board=board; rules_acc=acc})
        | {none=_} ->
          match Rules.get_toon(pos1, state.board) : option(toon_state) with
          | {none=_} ->
            do click(none)
            do log("cliquez sur un toon !")
            some(state)
          | {some = toon} ->
            do click_toon((pos1:coords), toon)
            do show_infos(toon)
            some( {state with click = some(pos1) })
          end
    ) : (client_state -> msg_game_client -> option(client_state))
  )
  do Session.send(s, {set_session=s} : msg_game_client)
  click_on_board(e) =
    {x; y} = jQuery.offseti(jQs(tbl_jq))
    do log("offseti(...).x = {x} ; offseti(...).y = {y}; dx = {dx} ; dy = {dy}")
    x = (e.pageX - x ) / dx
    y = (e.pageY - y ) / dy
    do Session.send(s, {click=(x, y)}:msg_game_client)
    []
  //
  do Session.send(session, {register=s; key=client_key} : msg_game )
  do [#main <-
    <>
    <div id="tbl" onclick={ click_on_board(event) }></div>
    <div id="right">
      <div id="info">Vous jouez les <span id="team"></span>, c'est aux <span id="toplay"></span> de jouer</div>
      <div class="bouton" id="attaque" onclick={Session.send(s, {attaque})} >{no_attaque_msg}</div>
      <div class="bouton" id="ko" onclick={Session.send(s, {remove_ko})}>enlever un point KO</div>
      <div class="bouton" id="ok" onclick={Session.send(s, {ok}:msg_game_client )} >valider</div>
      <div class="bouton" id="cancel" onclick={Session.send(s, {cancel}:msg_game_client )} >annuler</div>
      <div id="pieces"></div>
      <div id="log"></div>
      <div id="infos"></div>
    </div>
    </>
  ]
  void
