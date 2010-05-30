(*
** This file has been generated, if you wish to
** modify it in a permanent way, please refer
** to the script file : gen/generator_caml.rb
*)

(*
** Taille du terrain.
*)
let taille_terrain = 21

(*
** le nombre de points d'actions des unites au debut du tour.
*)
let max_pa = 3

(*
** Le nombre d'unités par équipe.
*)
let nbr_unites = 9

(*
** Le nombre maximum de tours.
*)
let max_turn = 100

(*
** Énumération représentant une erreur renvoyée par une des fonctions d'action.
*)
type erreur =
| Ok (* <- aucune erreur n'est survenue *)
| Position_invalide (* <- la position spécifiée est invalide *)
| Utilisation_impossible (* <- vous ne pouvez pas utiliser cet objet *)
| Plus_d_argent (* <- vous n'avez pas assez d'argent pour acheter l'objet en question *)


(*
** Le type d'un objet
*)
type type_objet =
| Filet (* <- permet d'attraper des pieces ou d'immobiliser des unités *)
| Marteau (* <- permet de frapper des unités. *)
| Rien (* <- l'unite ne porte aucun objet *)


(*
** Représente une position sur le terrain du jeu.
*)
type position = (int * int)

(*
** les caracteristiques d'un objet
*)
type caracteristiques_objet = {
  coute : int ; (* <- ce que coute l'objet *)
  porte : int ; (* <- la porte de l'objet *)
}

(*
** Représente une unité sur le terrain.
*)
type unite = {
  pos_unite : position ; (* <- la position de l'unité *)
  team : int ; (* <- donne l'equipe du joueur *)
  ko : int ; (* <- renvoie le temps que l'unite passera sans pouvoir bouger. *)
  pa : int ; (* <- le nombre de PA restants à l'unité *)
  objet : type_objet ; (* <- renvoie l'objet que porte l'unite. *)
  id : int ; (* <- l'identifiant unique de l'unité *)
}

(*
** Représente le type d'une piece.
*)
type piece = {
  valeur : int ; (* <- la valeur de la piece. *)
  pos_piece : position ; (* <- renvoie la position de la piece *)
  tour_apparition : int ; (* <- renvoie le tour ou la piece apparaitra ou est apparu. *)
}

(*
** Renvoie le numero de votre equipe.
*)
external mon_equipe : unit -> int = "ml_mon_equipe"
(*
** Renvoie le score d'une equipe.
*)
external score : int -> int = "ml_score"
(*
** Renvoie le nombre d'equipes sur la map
*)
external nombre_equipes : unit -> int = "ml_nombre_equipes"
(*
** Renvoie le numéro du tour actuel.
*)
external tour_actuel : unit -> int = "ml_tour_actuel"
(*
** Renvoie les pieces qui sont sur la map
*)
external pieces_en_jeu : unit -> piece array = "ml_pieces_en_jeu"
(*
** Renvoie le 10 prochaines pieces a vennir
*)
external pieces_a_vennir : unit -> piece array = "ml_pieces_a_vennir"
(*
** Retourne la liste des unités actuellement en jeu.
*)
external unites : unit -> unite array = "ml_unites"
(*
** Déplace une unité vers une position à portée.
*)
external deplacer : position -> position -> erreur = "ml_deplacer"
(*
** Achete un objet
*)
external acheter_objet : position -> type_objet -> erreur = "ml_acheter_objet"
(*
** utilise un objet
*)
external utiliser : position -> position -> erreur = "ml_utiliser"
(*
** coupe la partie, cette action coute 100 d'or et met fin a la partie.
*)
external achever_la_partie : unit -> erreur = "ml_achever_la_partie"
(*
** Affiche le contenu d'une valeur de type erreur
*)
external afficher_erreur : erreur -> unit = "ml_afficher_erreur"
(*
** Affiche le contenu d'une valeur de type type_objet
*)
external afficher_type_objet : type_objet -> unit = "ml_afficher_type_objet"
(*
** Affiche le contenu d'une valeur de type position
*)
external afficher_position : position -> unit = "ml_afficher_position"
(*
** Affiche le contenu d'une valeur de type caracteristiques_objet
*)
external afficher_caracteristiques_objet : caracteristiques_objet -> unit = "ml_afficher_caracteristiques_objet"
(*
** Affiche le contenu d'une valeur de type unite
*)
external afficher_unite : unite -> unit = "ml_afficher_unite"
(*
** Affiche le contenu d'une valeur de type piece
*)
external afficher_piece : piece -> unit = "ml_afficher_piece"
