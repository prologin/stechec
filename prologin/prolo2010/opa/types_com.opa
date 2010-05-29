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


type toon = {titi} / {elmer} / {bipbip} / {coyote}
(titi, elmer, bipbip, coyote) = ({titi}, {elmer}, {bipbip}, {coyote})
types_toons = [titi, elmer, bipbip, coyote]


toon_to_string =
  | {titi} -> "perroquet"
  | {elmer} -> "singe"
  | {bipbip} -> "chat"
  | {coyote} -> "kangourou"
//

toon_to_xhtml(long, toon, team) =
  strtoon=toon_to_string(toon)
  title=strtoon
  <div class={["mini_toon", Team.team_to_class(team)]}>
   <img src="{strtoon}{
   if long then ""
   else "_mini"
   }.png" title={title}/></div>
//


type toon_is_sick = {ko:int} / {sleep} / {ready_to_move}

sick_to_string(sick : toon_is_sick) =
  match sick with
  | ~{ko} -> "ko : {ko}"
  | {sleep} -> "endormie"
  | {ready_to_move} -> "au combat !"

type card = {banzaii} / {doctor} / {deguisement: toon} / {pacifisme}
xhtml_of_card(c:card) =
  <>{match c with
  | {banzaii} -> "banzai"
  | {doctor} -> "doctor"
  | ~{deguisement} -> "déguisement"
  | {pacifisme} -> "pacifisme"
  }</>
//


default_cards = [{banzaii}, {doctor}, {deguisement={elmer}}, {pacifisme}]

type toon_state =
  {
    team:team;
    sick:toon_is_sick;
    original_toon:toon; // carte deguisement
    real_toon:toon;
    pa:int;
    n_attack:int;
    already_moved:bool;
    attaques_gratuites:int;
  }
//

toon_state_to_xhtml(toon) =
  img = "{toon_to_string(toon.real_toon)}.png"
  <div class="toon">
    <div>team : {Team.team_to_string(toon.team)}</div>
    <div class="toon_picture"><img src={img} /></div>
    <div>état de sante : { sick_to_string(toon.sick) }</div>
    <div>pa: {toon.pa}</div>
    <div>peut attaquer : {toon.n_attack} fois</div>
    <div>{if toon.already_moved then "a déjà bougée" else "n'a pas encore bougée" }</div>
    <div>a fait {toon.attaques_gratuites} banzai</div>
  </div>


type coordsmap('a) = map(coords, 'a)

type board =
{
  toplay:team;
  board:coordsmap( toon_state );
  nbr_toons_to_spawn : teams_info(int);
  cards:teams_info(list(card));
  size: (int, int);
  time_before_resizing : int
}

empty_board = {
  toplay=black;
  board=Map.empty;
  nbr_toons_to_spawn = TwoTeams.make(Rules.max_nbr_toons - 1);
  cards = TwoTeams.make( default_cards );
  size = (Rules.sizeX, Rules.sizeY);
  time_before_resizing = Rules.time;
  } : board


//les messages envoyes au client
type msg_game_client =
  {board : board; me : team} /
  {app_diffs : list(diff_board); me : team} /
  {set_session : channel(msg_game_client)} /
  {INVALID_PARAM:string} /
  {spawn:toon} /
  {click:(int, int)} /
  {ok} /
  {cancel} /
  {attaque} /
  {`end` : teams_info(bool)} /
  {remove_ko} /
  {play_card : card} /
  {relever_un_toon} /
  {set_state: client_state}

type diff_board =
  {from:coords; to:coords} /
  {born:toon} /
  {remove_ko:coords} /
  {toon:coords; attack:coords} /
  {play_card : (card, coords)} /
  {relever_un_toon:coords}

// les messages envoyes au serveur
type msg_game =
  {register:channel(msg_game_client); key:key} /
  {moves:list(diff_board); key:key}
