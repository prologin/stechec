List.filter( (s -> String.length(s) > 3),  li)
li |> List.filter <| (~ String.leng

List.filter( (~ String.length(_) > 3) , li)
