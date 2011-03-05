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
          ( i, ~{player; password}, acc -> (player, i, password != none) +> acc),
          games, [] )
          |> List.sort
    Map.fold( (id, (l, _), _ -> do jlog("(send list to user {id})"); Session.send(l, {games=list}: msg_client)), listeners, void)
  valid(id, client_key, listeners) =
    match Map.get(id, listeners) with
    | {none} -> do jlog("the client id {id} is not found in the user map ! "); false
    | {some=(_, k)} ->
      out = k == client_key
      do if out then jlog("the client {id} is valid !")
      else jlog("the client {id} is invalid ! bad key !")
      out
  gc( games, listeners ) = // garbage collection...
    do jlog("start GC")
    out = Map.fold(
      (id, (session, key), (games, listeners) ->
        if Session.is_open(session) then
          (games, listeners)
        else
          do jlog("removing user {id}")
          (Map.remove(id, games), Map.remove(id, listeners))
      ), listeners, (games, listeners)
    )
    do jlog("end")
    out
  Session.make_opt(
  (Map.empty, Map.empty),
  ( (games , listeners ), msg ->
    (games, listerners) = gc(games, listeners)
    match msg : msg_listgames with
    | ~{create; id; client_key} ->
      if valid(id, client_key, listeners) then
        games = Map.add(id, create, games)
        do send(games, listeners)
        (games, listeners) |> some
      else (games, listeners) |> some
    | ~{join; password; id; client_key} ->
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
    | {register_listener=l; ~id; ~client_key} ->
      do jlog("register {id} : {client_key}")
      listeners =  Map.add(id, (l, client_key), listeners)
      do send(games, listeners)
      (games, listeners) |> some
  )
)

create((client_id, client_key), b) =
  name = jQuery.getVal(#{"name"})
  if name != "" then
    do Session.send(listgames,
      {
        create={
          player=name
          password=if b then some(jQuery.getVal(#{"password"})) else none
        }
        id=client_id;
        client_key=client_key
      })
    [#games_creation <- <>attente d'un joueur</> ]
  else []
//

wait_games(idk) =
  rec pas_button() =
    <>
      <input type="text" id="password" />
      <input type="button" value="faire une partie sans password" onclick={
      _ ->
       [#password_button <- button(), #password -> value <- "" ] } />
      <input type="button" value="attendre un adversaire" onclick={ _ -> create(idk, true) } />
    </>
  //
  and button() =
    <>
      <input type="button" value="faire une partie protegee par password" onclick={
      _ ->
      [#password_button <- pas_button() ] } />
      <input type="button" value="attendre un adversaire" onclick={ _ -> create(idk, false) } />
    </>
  //
  <h1>Create a game</h1>
  <input type="text" id="name" />
  <div id="password_button">{ button() }</div>
//


a_join((client_id, client_key), p, n) =
  do Session.send(listgames, {join=n; password=p; id=client_id; client_key=client_key})
  [#main <- <>La partie va bientot commencer</>]
//

join_np(idk, n) = a_join(idk, none, n)
join_p(idk, n, p) = a_join(idk, some(p), n)


onload() =
  client_id = Random.int(max_int)
  client_key = Random.int(max_int)
  idk = (client_id, client_key)
  do exec_actions([ #games_creation <- wait_games(idk) ])
  
  listener = Session.make_opt(
    void,
    (s, (msg:msg_client) ->
      match msg with
        | ~{games} -> do [#games_list <-
          xhtml =
            List.foldl(
              ((s, i, p), acc ->
                if i == client_id
                then <>{acc}<li>{s}<span class="info">(Ma partie)</span></li></>
                else if p
                then <>{acc}<li><a onclick={ _ -> join_p(idk, i, jQuery.getVal(#{"password_{i}"}))} href="javascript:void(0)">{s}</a>
                  <input type="password" id={"password_{i}"} /></li></>
                else <>{acc}<li><a onclick={_ -> join_np(idk, i)} href="javascript:void(0)">{s}</a></li></>
              ),
              games, <></>)
            <p>tables :</p><ul>{xhtml}</ul>
          ]
          some(void)
        | ~{session} ->
          do start(idk, session)
          none
        | ~{bad_password} ->
          do [#games_creation <- <><p>Mauvais Password...</p>{ wait_games(idk) }</> ]
          some(void)
    )
  )
  Session.send(listgames, {register_listener=listener; id=client_id; client_key=client_key} : msg_listgames)
//

page =
  <div onload={_ -> onload()}>
    <div id="head"><h1><img src="logo_cube.png" title="prologin"/>PROLOGIN {year} : {title} </h1></div>
    <div id="main">
      <div id="games_list"></div>
      <div id="games_creation"></div>
    </div>
    <div class="hidden">
      <img src="/chat.png" />
      <img src="/chat_mini.png" />
      <img src="/singe.png" />
      <img src="/singe_mini.png" />
      <img src="/kangourou.png" />
      <img src="/kangourou_mini.png" />
      <img src="/perroquet_mini.png" />
      <img src="/perroquet.png" />
    </div>
    <div id="footer">
      <div class="links">
        <ul>
          <li><a href="http://www.prologin.org">Prologin</a></li>
          <li>(c)<a href="http://www.mlstate.com"><img src="/logo_mlstate.png" title="mlstate"/></a></li>
          <li><a href="/howtoplay">How to play ?</a></li>
        </ul>
      </div>
    </div>
  </div>

//
