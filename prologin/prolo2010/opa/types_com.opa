


type toon = {titi} / {elmer} / {bipbip} / {coyote}
(titi, elmer, bipbip, coyote) = ({titi}, {elmer}, {bipbip}, {coyote})
types_toons = [titi, elmer, bipbip, coyote]


toon_to_string =
  | {titi} -> "titi"
  | {elmer} -> "elmer"
  | {bipbip} -> "bipbip"
  | {coyote} -> "coyote"
//

toon_to_xhtml(long, toon, team) =
  strtoon=toon_to_string(toon)
  title=strtoon // todo
  <span class={[Team.team_to_class(team)]}>
   <img src="{strtoon}{
   if long then ""
   else "_mini"
   }.png" title={title}/></span>
//


type toon_is_sick = {ko:int} / {sleep} / {ready_to_move}

sick_to_string(sick : toon_is_sick) =
  match sick with
  | {ko} -> "ko : {ko}"
  | {sleep} -> "endormie"
  | {ready_to_move} -> "au combat !"

type card = {banzaii} / {doctor} / {deguisement} / {pacifisme}

type toon_state =
  {
    team:team;
    sick:toon_is_sick;
    original_toon:toon; // carte deguisement
    real_toon:toon;
    pa:int;
    already_attack:bool;
    already_moved:bool
  }
//

toon_state_to_xhtml(toon) =
  img = match toon.real_toon with
  | {titi} -> "titi.png"
  | {elmer} -> "elmer.png"
  | {bipbip} -> "bipbip.png"
  | {coyote} -> "coyote.png"
  <div class="toon">
    <div>team : {Team.team_to_string(toon.team)}</div>
    <div class="toon_picture"><img src={img} /></div>
    <div>etat de sante : { sick_to_string(toon.sick) }</div>
    <div>pa: {toon.pa}</div>
    <div>{if toon.already_attack then "a deja attaque" else "n'a pas encore attaque" }</div>
    <div>{if toon.already_moved then "a deja bouge" else "n'a pas encore bouge" }</div>
  </div>


type coordsmap('a) = map(coords, 'a)

type board =
{
  toplay:team;
  board:coordsmap( toon_state );
  nbr_toons_to_spawn : teams_info(int);
  cards:teams_info(list(card))
}

empty_board = {
  toplay=black;
  board=Map.empty;
  nbr_toons_to_spawn = TwoTeams.make(8);
  cards = TwoTeams.make( [] )
  } : board


//les messages envoyes au client
type msg_game_client =
  {board : board; me : team} /
  {set_session : channel(msg_game_client)} /
  {INVALID_PARAM:string} /
  {spawn:toon} /
  {click:(int, int)} /
  {ok} /
  {cancel} /
  {attaque} /
  {end} /
  {remove_ko}

type diff_board =
  {from:coords; to:coords} /
  {born:toon} /
  {remove_ko:coords} /
  {toon:coords; attack:coords}
  /*todo cards*/

// les messages envoyes au serveur
type msg_game =
  {register:channel(msg_game_client); key:key} /
  {moves:list(diff_board); key:key}
