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



type coords = (int, int)

Coords = {{
  distance((x1, y1), (x2, y2)) =
    max(Math.abs_i(y2 - y1), Math.abs_i(x2 - x1)) // norme infinie : deplacements en diagonale + horizontaux
  //
  add( (a, b), (c, d) ) = (a + c, b + d)
  rem( (a, b), (c, d) ) = (a - c, b - d)
  
  mult_e( (a, b), (c, d)) = (a  * c, b * d)
  pos(a, b) = (a, b) : coords

  e_min((x1, y1), (x2, y2)) =
    (min(x1, x2), min(y1, y2))
  e_max((x1, y1), (x2, y2)) =
    (max(x1, x2), max(y1, y2))
}}
