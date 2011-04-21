(*
** This file has been generated, if you wish to
** modify it in a permanent way, please refer
** to the script file : gen/generator_caml.rb
*)

(*
** Taille du terrain
*)
let taille_terrain = 30

(*
** Nombre de tours par partie
*)
let fin_partie = 150

(*
** Nombre de points d'action par tour
*)
let max_pa = 3

(*
** Taille des tra�n�es de moto
*)
let intensite_trainee = 120

(*
** Longueur maximale de l'allongement
*)
let max_allongement = 5

(*
** Nombre de points d'action � rajouter avec bonus
*)
let ajout_pa = 5

(*
** �num�ration repr�sentant une erreur renvoy�e par une des fonctions d'action
*)
type erreur =
| Ok (* <- aucune erreur n'est survenue *)
| Id_invalide (* <- identifiant invalide *)
| Position_invalide (* <- la position sp�cifi�e est invalide *)
| Plus_de_pa (* <- vous n'avez pas assez de points d'action *)
| Bonus_invalide (* <- vous n'avez pas ce bonus *)
| Pas_a_toi (* <- l'unit� n'est pas a vous *)
| Intensite_invalide (* <- cette intensit� est invalide *)


(*
** �num�ration repr�sentant les diff�rents types de case
*)
type type_case =
| Vide (* <- rien n'est pr�sent sur la case *)
| Obstacle (* <- cette case est inaccessible *)
| Point_croisement (* <- point de croisement de tra�n�es *)
| Unite (* <- unit� d'�nergie *)


(*
** �num�ration repr�sentant les diff�rents types de bonii
*)
type type_bonus =
| Pas_bonus (* <- ceci n'est pas un bonus :-) *)
| Bonus_croisement (* <- bonus permettant de croiser deux tra�n�es de moto sur une case *)
| Plus_long (* <- bonus permettant d'agrandir une tra�n�e de moto *)
| Plus_pa (* <- bonus permettant d'avoir plus de points d'action *)
| Bonus_regeneration (* <- bonus permettant de regenerer une unit� d'�nergie *)


(*
** Repr�sente une position sur le terrain du jeu
*)
type position = (int * int)

(*
** Caracteristiques d'une unit� d'�nergie
*)
type unite_energie = {
  id : int ; (* <- identifiant de l'unit� d'�nergie *)
  pos : position ; (* <- position de l'unit� d'�nergie *)
  valeur : int ; (* <- coefficient repr�sentant les points d'�nergie que l'unit� va vous apporter *)
  valeur_max : int ; (* <- coefficient repr�sentant la capacit� de l'unit� lorsqu'elle est charg�e au maximum *)
}

(*
** Repr�sente une tra�n�e de moto sur le terrain
*)
type trainee_moto = {
  id : int ; (* <- identifiant de la tra�nee *)
  emplacement : position array ; (* <- position de chaque composant de la tra�n�e de moto *)
  team : int ; (* <- identifiant de l'�quipe qui poss�de cette tra�n�e de moto *)
  intensite : int ; (* <- taille maximale de la tra�n�e *)
}

(*
** Retourne le num�ro de votre �quipe
*)
external mon_equipe : unit -> int = "ml_mon_equipe"
(*
** Retourne les scores de chaque �quipe
*)
external scores : unit -> int array = "ml_scores"
(*
** Retourne le nombre d'�quipes sur le terrain
*)
external nombre_equipes : unit -> int = "ml_nombre_equipes"
(*
** Retourne le num�ro du tour actuel
*)
external tour_actuel : unit -> int = "ml_tour_actuel"
(*
** Retourne la liste des unit�s d'�nergie
*)
external unites_energie : unit -> unite_energie array = "ml_unites_energie"
(*
** Retourne la liste des tra�n�es de moto
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
** Retourne la liste des bonus d'une �quipe
*)
external regarder_bonus : int -> type_bonus array = "ml_regarder_bonus"
(*
** Retourne la liste des id des tra�n�es pr�sentes sur une case
*)
external regarder_trainee_case : position -> int array = "ml_regarder_trainee_case"
(*
** Retourne si une case peut �tre travers�e par une tra�n�e de plus
*)
external case_traversable : position -> bool = "ml_case_traversable"
(*
** Renvoie les points que vous allez gagner a la fin du tour
*)
external gain_tour_suivant : unit -> int = "ml_gain_tour_suivant"
(*
** Renvoie le chemin le plus court entre deux points (fonction lente)
*)
external chemin : position -> position -> position array = "ml_chemin"
(*
** D�place une moto
*)
external deplacer : int -> position -> position -> erreur = "ml_deplacer"
(*
** Coupe une tra�n�e de moto en deux nouvelles tra�n�es. � p1 � et � p2 � doivent �tre deux positions adjacentes occup�es par une m�me tra�n�e de moto.
*)
external couper_trainee_moto : int -> position -> position -> int -> erreur = "ml_couper_trainee_moto"
(*
** Annuler l'action pr�c�dente
*)
external cancel : unit -> erreur = "ml_cancel"
(*
** Enrouler la tra�n�e de moto en un point
*)
external enrouler : int -> position -> erreur = "ml_enrouler"
(*
** R�g�n�re une unit� d'�nergie � son maximal
*)
external regenerer_unite_energie : int -> erreur = "ml_regenerer_unite_energie"
(*
** Allonge le tour en rajoutant des points d'action
*)
external allonger_pa : unit -> erreur = "ml_allonger_pa"
(*
** Allonge une tra�n�e de moto. L'allongement se fera aux prochains d�placements. La longueur du prolongement doit �tre comprise entre 0 et MAX_ALLONGEMENT (inclus).
*)
external etendre_trainee_moto : int -> int -> erreur = "ml_etendre_trainee_moto"
(*
** Pose un point de croisement sur une case du terrain. La case doit ne pas d�j� �tre un point de croisement.
*)
external poser_point_croisement : position -> erreur = "ml_poser_point_croisement"
(*
** Fusionner deux tra�n�es de moto. Les deux doivent appartenir � la m�me �quipe, mais doivent �tre deux tra�n�es distinctes. � pos1 � et � pos2 � doivent �tre adjacentes et occup�es respectivement par � id1 � et � id2 �.
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
** Affiche le contenu d'une valeur de type unite_energie
*)
external afficher_unite_energie : unite_energie -> unit = "ml_afficher_unite_energie"
(*
** Affiche le contenu d'une valeur de type trainee_moto
*)
external afficher_trainee_moto : trainee_moto -> unit = "ml_afficher_trainee_moto"
