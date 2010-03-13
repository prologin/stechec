
do jlog("running server")

server =
  do jlog("server !")
  urls = parser
  | "/" -> html("One Prologin Application", page )
  | "/damier.png" -> Resource.image({png=file_content("damier.png")})
  | "/bipbip.png" -> Resource.image({png=file_content("bipbip.png")})
  | "/titi.png" -> Resource.image({png=file_content("titi.png")})
  | "/elmer.png" -> Resource.image({png=file_content("elmer.png")})
  | "/coyote.png" -> Resource.image({png=file_content("coyote.png")})
  
  | "/bipbip_mini.png" -> Resource.image({png=file_content("bipbip_mini.png")})
  | "/titi_mini.png" -> Resource.image({png=file_content("titi_mini.png")})
  | "/elmer_mini.png" -> Resource.image({png=file_content("elmer_mini.png")})
  | "/coyote_mini.png" -> Resource.image({png=file_content("coyote_mini.png")})
  simple_server(urls)
//

css = css

#{"__internal__log"}{
display:none;
}

.toon{
  border: 2px solid #000;
}

#games_list {
  width : 30%;
  float : left;
}
#games_creation{
  width : 69%;
  float : right;
}

#main, #footer {
  border-top : 1px solid rgb(0,0,0)  ;
}
#main{
  min-height:700px;
}
#right{
  margin-left:700px;
  width:300px;
}
#footer{
  font-size : x-smaller;
  text-align : center;
}

#select{
  position:absolute;
  left:-3000px;
  width:{px(float_of_int(dx))};
  height:{px(float_of_int(dy))};
  background-color:#DEF;
  opacity:0.5;
}

.bouton{
  display:inline;
  border:1px inset #DEF;
  background-color: #7AD;
}

.info{
  font-size : x-smaller;
  text-decoration : bold;
}
.links{
  border-top:1px solid #000;
}
.links ul, .links ul li{
  margin:0;
  padding:0;
}
.links ul li{
  width: 33%;
  list-style:none;
  float:left;
  font-size:xx-smaller;
  text-align:center;
}

#tbl {
  width: 600px;
  height: 600px;
  background-image: url('/damier.png');
  position:absolute;
}

.white{
  border: 3px solid #DDD;
  font-weight:bold;
}
.black{
  border: 3px solid #444;
  font-weight:bold;
}
.error{
  color: #f00
}
