(*
** This file has been generated, if you wish to
** modify it in a permanent way, please refer
** to the script file : gen/generator_caml.rb
*)

(*
** Taille du terrain
*)
let taille_terrain = 20

(*
** Nombre de tours par partie
*)
let fin_partie = 1500

(*
** Nombre de points d'action par tour
*)
let max_pa = 3

(*
** Taille des traînées de moto
*)
let taille_trainee = 80

(*
** Longueur maximale de l'allongement
*)
let max_allongement = 5

(*
** Nombre de points d'action à rajouter avec bonus
*)
let ajout_pa = 5

(*
** Énumération représentant une erreur renvoyée par une des fonctions d'action
*)
type erreur =
| Ok (* <- aucune erreur n'est survenue *)
| Id_invalide (* <- identifiant invalide *)
| Position_invalide (* <- la position spécifiée est invalide *)
| Plus_de_pa (* <- vous n'avez pas assez de points d'action *)
| Bonus_invalide (* <- vous n'avez pas ce bonus *)
| Pas_a_toi (* <- l'unité n'est pas a vous *)


(*
** Énumération représentant les différents types de case
*)
type type_case =
| Vide (* <- rien n'est présent sur la case *)
| Obstacle (* <- cette case est inaccessible *)
| Bonus (* <- cette case cotient un bonus *)
| Point_croisement (* <- point de croisement de traînées *)
| Source (* <- source ou consommateur d'energie *)
| Trainee (* <- une trainée de moto *)


(*
** Énumération représentant les différents types de bonii
*)
type type_bonus =
| Pas_bonus (* <- ceci n'est pas un bonus :-) *)
| Bonus_croisement (* <- bonus permettant de croiser deux traînées de moto sur une case *)
| Plus_long (* <- bonus permettant d'agrandir une traînée de moto *)
| Plus_pa (* <- bonus permettant d'avoir plus de points d'action *)
| Bonus_regeneration (* <- bonus permettant de regenerer une source d'energie *)


(*
** Représente une position sur le terrain du jeu
*)
type position = (int * int)

(*
** Caracteristiques d'une source d'énergie
*)
type source_energie = {
  id : int ; (* <- identifiant de la source d'énergie *)
  pos : position ; (* <- position de la source d'énergie *)
  coef : int ; (* <- coefficient representant les points d'énergie que la source va vous apporter *)
}

(*
** Représente une traînée de moto sur le terrain
*)
type trainee_moto = {
  id : int ; (* <- identifiant de la traînee *)
  emplacement : position array ; (* <- position de chaque composant de la traînée de moto *)
  team : int ; (* <- identifiant de l'équipe qui possède cette traînée de moto *)
}

(*
** Retourne le numéro de votre équipe
*)
external mon_equipe : unit -> int = "ml_mon_equipe"
(*
** Retourne les scores de chaque équipe
*)
external scores : unit -> int array = "ml_scores"
(*
** Retourne le nombre d'équipes sur le terrain
*)
external nombre_equipes : unit -> int = "ml_nombre_equipes"
(*
** Retourne le numéro du tour actuel
*)
external tour_actuel : unit -> int = "ml_tour_actuel"
(*
** Retourne la liste des sources d'énergie
*)
external sources_energie : unit -> source_energie array = "ml_sources_energie"
(*
** Retourne la liste des traînées de moto
*)
external trainees_moto : unit -> trainee_moto array = "ml_trainees_moto"
(*
** Retourne le type d'une case
*)
external regarder_type_case : position -> type_case = "ml_regarder_type_case"
(*
** Retourne le type de bonus d'une case
*)
external regarder_type_bonus : position -> type_bonus = "ml_regarder_type_bonus"
(*
** Retourne la liste des bonus d'une équipe
*)
external regarder_bonus : int -> type_bonus array = "ml_regarder_bonus"
(*
** Renvoie le chemin le plus court entre deux points (fonction lente)
*)
external chemin : position -> position -> position array = "ml_chemin"
(*
** Déplace une moto
*)
external deplacer : int -> position -> position -> erreur = "ml_deplacer"
(*
** Coupe une traînée de moto en deux nouvelles traînées. « entre » et « et » doivent être deux positions adjacentes occupées par une même traînée de moto.
*)
external couper_trainee_moto : int -> position -> position -> erreur = "ml_couper_trainee_moto"
(*
** Annuler l'action précédente
*)
external cancel : unit -> erreur = "ml_cancel"
(*
** Enrouler la traînée de moto en un point
*)
external enrouler : int -> position -> erreur = "ml_enrouler"
(*
** Régénère une source d'énergie à son maximal
*)
external regenerer_source_energie : int -> erreur = "ml_regenerer_source_energie"
(*
** Allonge le tour en rajoutant des points d'action
*)
external allonger_pa : unit -> erreur = "ml_allonger_pa"
(*
** Allonge une traînée de moto. L'allongement se fera aux prochains déplacements. La longueur du prolongement doit être comprise entre 0 et MAX_ALLONGEMENT (inclus).
*)
external agrandir_trainee_moto : int -> int -> erreur = "ml_agrandir_trainee_moto"
(*
** Pose un point de croisement sur une case du terrain. La case doit ne pas déjà être un point de croisement.
*)
external poser_point_croisement : position -> erreur = "ml_poser_point_croisement"
(*
** Fusionner deux traînées de moto. Les deux doivent appartenir à la même équipe, mais doivent être deux traînées distinctes. « pos1 » et « pos2 » doivent être adjacentes et occupées respectivement par « id1 » et « id2 ».
*)
external fusionner : int -> position -> int -> position -> erreur = "ml_fusionner"
(*
** Affiche le contenu d'une valeur de type erreur
*)
external afficher_erreur : erreur -> unit = "ml_afficher_erreur"
(*
** Affiche le contenu d'une valeur de type type_case
*)
external afficher_type_case : type_case -> unit = "ml_afficher_type_case"
(*
** Affiche le contenu d'une valeur de type type_bonus
*)
external afficher_type_bonus : type_bonus -> unit = "ml_afficher_type_bonus"
(*
** Affiche le contenu d'une valeur de type position
*)
external afficher_position : position -> unit = "ml_afficher_position"
(*
** Affiche le contenu d'une valeur de type source_energie
*)
external afficher_source_energie : source_energie -> unit = "ml_afficher_source_energie"
(*
** Affiche le contenu d'une valeur de type trainee_moto
*)
external afficher_trainee_moto : trainee_moto -> unit = "ml_afficher_trainee_moto"
