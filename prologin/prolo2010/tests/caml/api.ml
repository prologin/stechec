(*
** This file has been generated, if you wish to
** modify it in a permanent way, please refer
** to the script file : gen/generator_caml.rb
*)

(*
** Taille de d�part du terrain.
*)
let taille_depart = 19

(*
** Le nombre maximal d'unit�s pouvant appartenir � une �quipe.
*)
let nbr_max_unites = 10

(*
** Le temps, en nombre de tours, entre deux r�tr�cissement du terrain.
*)
let temps_retrecissement = 5

(*
** Le nombre maximum en jeu de chaque carte.
*)
let max_cartes = 2

(*
** �num�ration repr�sentant une erreur renvoy�e par une des fonctions d'action.
*)
type erreur =
| Ok (* <- aucune erreur n'est survenue *)
| Position_invalide (* <- la position sp�cifi�e est invalide *)
| Case_occupee (* <- la case sur laquelle vous tentez de vous d�placer est occup�e *)
| Pas_a_portee (* <- la cible n'est pas � port�e *)
| Unite_ko (* <- l'unit� que vous essayez de faire agir ou sur laquelle vous essayez d'agir  est KO *)
| Unite_debout (* <- l'unit� que vous essayez de relever est d�j� debout *)
| Quota_depasse (* <- nombre maximal d'unites, de spawn ou de relevages par tour d�pass� *)
| Plus_de_pa (* <- cette unit� a r�alis� toutes ses actions *)
| Deja_attaque (* <- cette unit� a d�j� attaqu� *)
| Unite_interdite (* <- cette unit� ne peut pas �tre amen�e en renfort *)
| Renfort_impossible (* <- une unit� est d�j� pr�sente sur le spawn *)
| Pas_a_moi (* <- l'unit� cibl�e n'est pas � vous *)
| Plus_de_cartes (* <- il n'y a plus de cartes du type sp�cifi� dans votre main *)
| Phase_cartes_terminee (* <- vous ne pouvez plus poser de cartes car vous avez fait une action *)


(*
** Le type d'une unit� sur le terrain
*)
type type_unite =
| Perroquet (* <- 1 PA et port�e de 1 *)
| Singe (* <- 2 PA et port�e de 3 *)
| Chat (* <- 5 PA et port�e de 1 *)
| Kangourou (* <- 3 PA et port�e de 2 (explose !) *)


(*
** Repr�sente une position sur le terrain du jeu.
*)
type position = (int * int)

(*
** Contient les informations sur la taille du terrain du jeu.
*)
type taille_terrain = {
  taille : int ; (* <- taille actuelle du terrain *)
  min_coord : int ; (* <- coordonn�e minimale en X ou en Y *)
  max_coord : int ; (* <- coordonn�e maximale en X ou en Y *)
}

(*
** Donne les caract�ristiques d'un type d'unit�.
*)
type caracs = {
  pa_init : int ; (* <- nombre de points d'actions par tour *)
  portee : int ; (* <- port�e maximale de l'unit� *)
}

(*
** Repr�sente une unit� sur le terrain.
*)
type unite = {
  pos : position ; (* <- la position de l'unit� *)
  ennemi : bool ; (* <- vrai si l'unit� appartient � l'ennemi *)
  type_unite_actuel : type_unite ; (* <- le type de l'unit�, qui change si l'unit� est d�guis�e *)
  vrai_type_unite : type_unite ; (* <- le vrai type de l'unit� (qui ne change pas lors du d�guisement) *)
  ko : int ; (* <- une valeur n�gative si l'unit� n'est pas KO, sinon le nombre de marqueurs KO sur l'unit� *)
  pa : int ; (* <- le nombre de PA restants � l'unit� *)
  attaques : int ; (* <- le nombre d'attaques restants � l'unit� *)
  attaques_gratuites : int ; (* <- le nombre d'attaques gratuites (voir la partie banzai) *)
  id : int ; (* <- l'identifiant unique de l'unit� *)
}

(*
** Repr�sente l'ensemble des cartes que vous pouvez utiliser.
*)
type cartes = {
  potion : int ; (* <- le nombre de cartes � Potion magique � *)
  deguisement : int ; (* <- le nombre de cartes � D�guisement � *)
  banzai : int ; (* <- le nombre de cartes � Banza� � *)
  pacifisme : int ; (* <- le nombre de cartes � Pacifisme � *)
}

(*
** Renvoie le nombre de points de commandements.
*)
external nombre_pc : unit -> int = "ml_nombre_pc"
(*
** Renvoie le nombre d'unit�s en jeu.
*)
external nombre_unites : bool -> int = "ml_nombre_unites"
(*
** Renvoie le num�ro du tour actuel.
*)
external tour_actuel : unit -> int = "ml_tour_actuel"
(*
** Renvoie la position du spawn (ennemi ou non).
*)
external pos_renfort : bool -> position = "ml_pos_renfort"
(*
** Renvoie les caract�ristiques d'un type d'unit�.
*)
external caracteristiques : type_unite -> caracs = "ml_caracteristiques"
(*
** Retourne une structure "cartes" contenant les informations sur les cartes que vous avez en main.
*)
external mes_cartes : unit -> cartes = "ml_mes_cartes"
(*
** Retourne la liste des unit�s actuellement en jeu.
*)
external unites : unit -> unite array = "ml_unites"
(*
** Retourne la taille actuelle du terrain et les coordonn�es min/max dans une structure "taille_terrain".
*)
external taille_terrain_actuelle : unit -> taille_terrain = "ml_taille_terrain_actuelle"
(*
** Utilise une carte � Potion magique � que vous avez dans votre main.
*)
external potion : position -> erreur = "ml_potion"
(*
** Utilise une carte � D�guisement � que vous avez dans votre main.
*)
external deguisement : position -> type_unite -> erreur = "ml_deguisement"
(*
** Utilise une carte � Banza� � que vous avez dans votre main.
*)
external banzai : position -> erreur = "ml_banzai"
(*
** Utilise une carte � Pacifisme � que vous avez dans votre main.
*)
external pacifisme : unit -> erreur = "ml_pacifisme"
(*
** D�place une unit� vers une position � port�e.
*)
external deplacer : position -> position -> erreur = "ml_deplacer"
(*
** Rel�ve une unit� n'ayant plus de marqueurs de KO.
*)
external relever : position -> erreur = "ml_relever"
(*
** Attaque une autre unit�.
*)
external attaquer : position -> position -> erreur = "ml_attaquer"
(*
** Fait apparaitre une unit� sur la case de spawn.
*)
external renfort : type_unite -> erreur = "ml_renfort"
(*
** Annule l'effet de la derni�re action et remet le jeu dans l'�tat pr�c�dent. Renvoie false s'il n'y a rien � annuler, true sinon.
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
