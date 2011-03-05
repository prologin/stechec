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


server =
  do jlog("server !")
  html(title, body) = ( _, _, _ ->
    custom_css = [ _ ->
      some({
         custom_body   = none
         custom_headers= none
         custom_css    = ["/style.css"]
         custom_js = []
         })
      ]
      Resource.full_page(
      title,
      body,
      <></>,
      {success} : web_response,
      custom_css
      ))
  urls = parser
  | "/howtoplay" -> html("how to play ?", page_help)
  | "/style.css" -> (_, _, _ -> Resource.css(file_content("style.css")))
  | "/damier.png" -> (_, _, _ -> Resource.dyn_image({png=file_content(
    if Random.int(2) == 1
    then "damier.png"
    else "damier_2.png"
    )}))
  | "/" ([a-z_]*) ".png" -> (_, _, _ -> Resource.image({png=file_content("{__2}.png")}))
  | "/" Rule.eos -> html("One Prologin Application (powered by OPA from MLState)", page )
  Server.full_server(void, (_, _, _ -> void), urls)
//
