type coords = (int, int)

Coords = {{
  distance((x1, y1), (x2, y2)) =
    max(Math.abs_i(y2 - y1), Math.abs_i(x2 - x1)) // norme infinie : deplacements en diagonale + horizontaux
  //
  add( (a, b), (c, d) ) = (a + c, b + d) 
  mult_e( (a, b), (c, d)) = (a  * c, b * d)
  pos(a, b) = (a, b) : coords
}}
