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
    ~{white; black} = infos
    match team with
    | {white} -> white
    | {black} -> black
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
