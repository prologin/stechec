
client_id = PageGlobal.page_index
client_key = Random.int(max_int)
_ = @client(client_key)

type key = int


// gestion de la liste des parties
type game_entry =
  {player:string; password : option(string) }
type msg_client =
  {games:list((string, int, bool))} /
  {session:channel(msg_game)} /
  {bad_password}
type msg_listgames =
  {create:game_entry; id:int; client_key:key} /
  {join : int; password : option(string); id:int; client_key:int} /
  {register_listener : channel(msg_client); id:int; client_key:key}


listgames = // session server
  send(games:map, listeners:map) =
    list = Map.fold(
          ( i, {player; password}, acc -> (player, i, password != none) +> acc),
          games, [] )
    Map.fold( (_, (l, _), _ -> Session.send(l, {games=list}: msg_client)), listeners, void)
  valid(id, client_key, listeners) =
    match Map.get(id, listeners) with
    | {none} -> false
    | {some=(_, k)} -> k == client_key
  Session.make_opt(
  (Map.empty, Map.empty),
  ( (games , listeners ), msg ->
    match msg : msg_listgames with
    | {create; id; client_key} ->
      if valid(id, client_key, listeners) then
        games = Map.add(id, create, games)
        do send(games, listeners)
        (games, listeners) |> some
      else (games, listeners) |> some
    | {join; password; id; client_key} ->
      (games, listeners) = match Map.get(join, games) with
      | {none} -> (games, listeners)
      | {some=game} ->
        match Map.get(join, listeners) with
        | {none} -> (games, listeners)
        | {some=(listener1, ck1)} ->
        match Map.get(id, listeners) with
        | {none} -> (games, listeners)
        | {some=(listener2, ck2)} ->
          if ck2 == client_key then
            if password == game.password then
              msg={session=session_factory(TwoTeams.set2(ck1, ck2))}
              do Session.send(listener1, msg)
              do Session.send(listener2, msg)
              ( games |> Map.remove(id, _) |> Map.remove(join, _),
                listeners |> Map.remove(id, _) |> Map.remove(join, _))
            else
              do Session.send(listener2, {bad_password})
              (games, listeners)
          else (games, listeners)
      do send(games, listeners)
      (games, listeners) |> some
    | {register_listener=l; id; client_key} ->
      listeners =  Map.add(id, (l, client_key), listeners)
      do send(games, listeners)
      (games, listeners) |> some
  )
)

create(b) =
  name = jQuery.getVal(#("name"))
  if name != "" then
    do Session.send(listgames,
      {
        create={
          player=name
          password=if b then some(jQuery.getVal(#("password"))) else none
        }
        id=client_id;
        client_key=client_key
      })
    [#games_creation <- <>attente d'un joueur</> ]
  else []
//

wait_games() =
  rec pas_button() =
    <>
      <input type="text" id="password" />
      <input type="button" value="faire une partie sans password" onclick={
       [#password_button <- button(), #password -> value <- "" ] } />
      <input type="button" value="attendre un adversaire" onclick={ create(true) } />
    </>
  //
  and button() =
    <>
      <input type="button" value="faire une partie protegee par password" onclick={
      [#password_button <- pas_button() ] } />
      <input type="button" value="attendre un adversaire" onclick={ create(false) } />
    </>
  //
  <h1>Create a game</h1>
  <input type="text" id="name" />
  <div id="password_button">{ button() }</div>
//


a_join(p, n) =
  do Session.send(listgames, {join=n; password=p; id=client_id; client_key=client_key})
  [#main <- <>La partie va bientot commencer</>]
//

join_np(n) = a_join(none, n)
join_p(n, p) = a_join(some(p), n)


onload() =
  listener = Session.make_opt(
    void,
    (s, (msg:msg_client) ->
      match msg with
        | {games} -> do [#games_list <-
          xhtml =
            List.foldl(
              ((s, i, p), acc ->
                if i == client_id
                then <>{acc}<li>{s}<span class="info">(Ma partie)</span></li></>
                else if p
                then <>{acc}<li><a onclick={join_p(i, jQuery.getVal(#("password_{i}")))} href="javascript:void(0)">{s}</a>
                  <input type="password" id={"password_{i}"} /></li></>
                else <>{acc}<li><a onclick={join_np(i)} href="javascript:void(0)">{s}</a></li></>
              ),
              games, <></>)
            <p>tables :</p><ul>{xhtml}</ul>
          ]
          some(void)
        | {session} ->
          do start(session)
          none
        | {bad_password} ->
          do [#games_creation <- <><p>Mauvais Password...</p>{ wait_games() }</> ]
          some(void)
    )
  )
  Session.send(listgames, {register_listener=listener; id=client_id; client_key=client_key} : msg_listgames)
//

page =
  <div onload={onload()}>
    <div id="head"><h1>PROLOGIN {year} : {title} </h1></div>
    <div id="main">
      <div id="games_list"></div>
      <div id="games_creation">{ wait_games() }</div>
    </div>
    <div id="footer">j'ai plus faim</div>
    <div class="links">
      <ul>
        <li><a href="http://www.prologin.org">Prologin</a></li>
        <li><a href="http://www.mlstate.com">MLState</a></li>
        <li><a href="http://www.codes-sources.fr">CodeS-SourceS</a></li>
        <li><a href="">UPMC</a></li>
        <li><a href="">Epita</a></li>
        <li><a href="">Epitech</a></li>
      </ul>
    </div>
  </div>
//
