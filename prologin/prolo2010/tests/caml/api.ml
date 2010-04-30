(*
** This file has been generated, if you wish to
** modify it in a permanent way, please refer
** to the script file : gen/generator_caml.rb
*)

(*
** Taille de départ du terrain.
*)
let taille_depart = 19

(*
** Le nombre maximal d'unités pouvant appartenir à une équipe.
*)
let nbr_max_unites = 10

(*
** Le temps, en nombre de tours, entre deux rétrécissement du terrain.
*)
let temps_retrecissement = 5

(*
** Le nombre maximum en jeu de chaque carte.
*)
let max_cartes = 2

(*
** Énumération représentant une erreur renvoyée par une des fonctions d'action.
*)
type erreur =
| Ok (* <- aucune erreur n'est survenue *)
| Position_invalide (* <- la position spécifiée est invalide *)
| Case_occupee (* <- la case sur laquelle vous tentez de vous déplacer est occupée *)
| Pas_a_portee (* <- la cible n'est pas à portée *)
| Unite_ko (* <- l'unité que vous essayez de faire agir ou sur laquelle vous essayez d'agir  est KO *)
| Unite_debout (* <- l'unité que vous essayez de relever est déjà debout *)
| Quota_depasse (* <- nombre maximal d'unites, de spawn ou de relevages par tour dépassé *)
| Plus_de_pa (* <- cette unité a réalisé toutes ses actions *)
| Deja_attaque (* <- cette unité a déjà attaqué *)
| Unite_interdite (* <- cette unité ne peut pas être amenée en renfort *)
| Renfort_impossible (* <- une unité est déjà présente sur le spawn *)
| Pas_a_moi (* <- l'unité ciblée n'est pas à vous *)
| Plus_de_cartes (* <- il n'y a plus de cartes du type spécifié dans votre main *)
| Phase_cartes_terminee (* <- vous ne pouvez plus poser de cartes car vous avez fait une action *)


(*
** Le type d'une unité sur le terrain
*)
type type_unite =
| Perroquet (* <- 1 PA et portée de 1 *)
| Singe (* <- 2 PA et portée de 3 *)
| Chat (* <- 5 PA et portée de 1 *)
| Kangourou (* <- 3 PA et portée de 2 (explose !) *)


(*
** Représente une position sur le terrain du jeu.
*)
type position = (int * int)

(*
** Contient les informations sur la taille du terrain du jeu.
*)
type taille_terrain = {
  taille : int ; (* <- taille actuelle du terrain *)
  min_coord : int ; (* <- coordonnée minimale en X ou en Y *)
  max_coord : int ; (* <- coordonnée maximale en X ou en Y *)
}

(*
** Donne les caractéristiques d'un type d'unité.
*)
type caracs = {
  pa_init : int ; (* <- nombre de points d'actions par tour *)
  portee : int ; (* <- portée maximale de l'unité *)
}

(*
** Représente une unité sur le terrain.
*)
type unite = {
  pos : position ; (* <- la position de l'unité *)
  ennemi : bool ; (* <- vrai si l'unité appartient à l'ennemi *)
  type_unite_actuel : type_unite ; (* <- le type de l'unité, qui change si l'unité est déguisée *)
  vrai_type_unite : type_unite ; (* <- le vrai type de l'unité (qui ne change pas lors du déguisement) *)
  ko : int ; (* <- une valeur négative si l'unité n'est pas KO, sinon le nombre de marqueurs KO sur l'unité *)
  pa : int ; (* <- le nombre de PA restants à l'unité *)
  attaques : int ; (* <- le nombre d'attaques restants à l'unité *)
  attaques_gratuites : int ; (* <- le nombre d'attaques gratuites (voir la partie banzai) *)
  id : int ; (* <- l'identifiant unique de l'unité *)
}

(*
** Représente l'ensemble des cartes que vous pouvez utiliser.
*)
type cartes = {
  potion : int ; (* <- le nombre de cartes « Potion magique » *)
  deguisement : int ; (* <- le nombre de cartes « Déguisement » *)
  banzai : int ; (* <- le nombre de cartes « Banzaï » *)
  pacifisme : int ; (* <- le nombre de cartes « Pacifisme » *)
}

(*
** Renvoie le nombre de points de commandements.
*)
external nombre_pc : unit -> int = "ml_nombre_pc"
(*
** Renvoie le nombre d'unités en jeu.
*)
external nombre_unites : bool -> int = "ml_nombre_unites"
(*
** Renvoie le numéro du tour actuel.
*)
external tour_actuel : unit -> int = "ml_tour_actuel"
(*
** Renvoie la position du spawn (ennemi ou non).
*)
external pos_renfort : bool -> position = "ml_pos_renfort"
(*
** Renvoie les caractéristiques d'un type d'unité.
*)
external caracteristiques : type_unite -> caracs = "ml_caracteristiques"
(*
** Retourne une structure "cartes" contenant les informations sur les cartes que vous avez en main.
*)
external mes_cartes : unit -> cartes = "ml_mes_cartes"
(*
** Retourne la liste des unités actuellement en jeu.
*)
external unites : unit -> unite array = "ml_unites"
(*
** Retourne la taille actuelle du terrain et les coordonnées min/max dans une structure "taille_terrain".
*)
external taille_terrain_actuelle : unit -> taille_terrain = "ml_taille_terrain_actuelle"
(*
** Utilise une carte « Potion magique » que vous avez dans votre main.
*)
external potion : position -> erreur = "ml_potion"
(*
** Utilise une carte « Déguisement » que vous avez dans votre main.
*)
external deguisement : position -> type_unite -> erreur = "ml_deguisement"
(*
** Utilise une carte « Banzaï » que vous avez dans votre main.
*)
external banzai : position -> erreur = "ml_banzai"
(*
** Utilise une carte « Pacifisme » que vous avez dans votre main.
*)
external pacifisme : unit -> erreur = "ml_pacifisme"
(*
** Déplace une unité vers une position à portée.
*)
external deplacer : position -> position -> erreur = "ml_deplacer"
(*
** Relève une unité n'ayant plus de marqueurs de KO.
*)
external relever : position -> erreur = "ml_relever"
(*
** Attaque une autre unité.
*)
external attaquer : position -> position -> erreur = "ml_attaquer"
(*
** Fait apparaitre une unité sur la case de spawn.
*)
external renfort : type_unite -> erreur = "ml_renfort"
(*
** Annule l'effet de la dernière action et remet le jeu dans l'état précédent. Renvoie false s'il n'y a rien à annuler, true sinon.
*)
external annuler : unit -> bool = "ml_annuler"
(*
** Affiche le contenu d'une valeur de type erreur
*)
external afficher_erreur : erreur -> unit = "ml_afficher_erreur"
(*
** Affiche le contenu d'une valeur de type type_unite
*)
external afficher_type_unite : type_unite -> unit = "ml_afficher_type_unite"
(*
** Affiche le contenu d'une valeur de type position
*)
external afficher_position : position -> unit = "ml_afficher_position"
(*
** Affiche le contenu d'une valeur de type taille_terrain
*)
external afficher_taille_terrain : taille_terrain -> unit = "ml_afficher_taille_terrain"
(*
** Affiche le contenu d'une valeur de type caracs
*)
external afficher_caracs : caracs -> unit = "ml_afficher_caracs"
(*
** Affiche le contenu d'une valeur de type unite
*)
external afficher_unite : unite -> unit = "ml_afficher_unite"
(*
** Affiche le contenu d'une valeur de type cartes
*)
external afficher_cartes : cartes -> unit = "ml_afficher_cartes"
