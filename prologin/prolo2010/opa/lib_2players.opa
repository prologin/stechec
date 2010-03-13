type team = {white} / {black}
white = {white}
black = {black}

Team = {{
  other_team =
  | {black} -> white
  | {white} -> black
  team_to_string =
    | {white} -> "white"
    | {black} -> "black"
  //
  team_to_class = team_to_string
}}

type teams_info('a) =
  {white:'a; black:'a}
//

TwoTeams = {{
  get(team, infos) =
    {white; black} = infos
    match team with
    | {white=_} -> white
    | {black=_} -> black
  set(team, info, infos) =
    match team with
    | {white} -> {infos with white = info}
    | {black} -> {infos with black = info}
  iter( (f:(team -> 'a -> void)), infos) =
    do f(white, infos.white)
    do f(black, infos.black)
    void
  //
  set2(iw, ib) = {white = iw; black=ib}
  for_all(f, infos) = f(infos.white) && f(infos.black)
  exists(f, infos) = not( for_all( ( not @ f ),  infos) )
  init(f) = {white=f(white); black=f(black)}
  make(v) = {white=v; black=v}
  apply(team, finfo, infos) =
    match team with
    | {white} -> {infos with white = finfo(infos.white)}
    | {black} -> {infos with black = finfo(infos.black)}
  apply2(f, infos) = {white = f(infos.white); black=f(infos.black)}
  apply_each_colors(f, infos) = {white = f(white, infos.white); black=f(black, infos.black)}
  combine(infos1, infos2) =
    { white = (infos1.white, infos2.white) ;
      black = (infos1.black, infos2.black) }
  split(infos) = (apply2(fst, infos), apply2(snd, infos))
}}
