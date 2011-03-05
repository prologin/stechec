unit prolo_interface;
interface
uses CTypes;

(*
  This file has been generated, if you wish to
  modify it in a permanent way, please refer
  to the script file : gen/generator_pascal.rb
*)


type array_of_cint = array of cint;
type array_of_boolean = array of boolean;
const
(*
  Taille du terrain
*)
	TAILLE_TERRAIN            =  100;
(*
  Nombre de tours avant la fin de la partie
*)
	FIN_PARTIE                =  1500;
(*
  Nombre de points d'action par tour
*)
	MAX_PA                    =  3;
(*
  Taille des traînées de moto
*)
	TAILLE_TRAINEE            =  900;
(*
  Longueur maximale de l'allongement
*)
	MAX_ALLONGEMENT           =  5;
(*
  Énumération représentant une erreur renvoyée par une des fonctions d'action
*)
type erreur =
  (
    ok { <- aucune erreur n'est survenue },
    id_invalide { <- identifiant invalide },
    position_invalide { <- la position spécifiée est invalide },
    plus_de_pa { <- vous n'avez pas assez de points d'action },
    bonus_invalide { <- vous n'avez pas ce bonus },
    pas_a_toi { <- l'unité n'est pas a vous }
  );

type array_of_erreur = array of erreur;


(*
  Énumération représentant les différents types de case
*)
type type_case =
  (
    vide { <- rien n'est présent sur la case },
    obstacle { <- cette case est inaccessible },
    bonus { <- cette case cotient un bonus },
    point_croisement { <- point de croisement de traînées }
  );

type array_of_type_case = array of type_case;


(*
  Énumération représentant les différents types de bonii
*)
type bonus =
  (
    vide { <- ceci n'est pas un bonus :-) },
    bonus_croisement { <- bonus permettant de croiser deux traînées de moto sur une case },
    plus_long { <- bonus permettant d'agrandir une traînée de moto },
    plus_pa { <- bonus permettant d'avoir plus de points d'action },
    bonus_regeneration { <- bonus permettant de regenerer une source d'energie }
  );

type array_of_bonus = array of bonus;


(*
  Représente une position sur le terrain du jeu
*)
type position =
  record
    x : cint; (*  <- coordonnée en X *)
    y : cint; (*  <- coordonnée en Y *)
  end;

type array_of_position = array of position;


(*
  Caracteristiques d'une source d'énergie
*)
type source_energie =
  record
    id : cint; (*  <- identifiant de la source d'énergie *)
    pos : position; (*  <- position de la source d'énergie *)
    coef : cint; (*  <- coefficient representant les points d'énergie que la source va vous apporter *)
  end;

type array_of_source_energie = array of source_energie;


(*
  Représente une traînée de moto sur le terrain
*)
type trainee_moto =
  record
    id : cint; (*  <- identifiant de la traînee *)
    emplacement : array of position; (*  <- position de chaque composant de la traînée de moto *)
    team : cint; (*  <- identifiant de l'équipe qui possède cette traînée de moto *)
  end;

type array_of_trainee_moto = array of trainee_moto;


(*
  Retourne le numéro de votre équipe
*)
function mon_equipe() : cint; cdecl; external;

(*
  Retourne les scores de chaque équipe
*)
function scores() : array_of_cint; cdecl; external;

(*
  Retourne le nombre d'équipes sur le terrain
*)
function nombre_equipes() : cint; cdecl; external;

(*
  Retourne le numéro du tour actuel
*)
function tour_actuel() : cint; cdecl; external;

(*
  Retourne la liste des sources d'énergie
*)
function sources_energie() : array_of_source_energie; cdecl; external;

(*
  Retourne la liste des traînées de moto
*)
function trainees_moto() : array_of_trainee_moto; cdecl; external;

(*
  Retourne le type d'une case
*)
function regarder_type_case(pos : position) : type_case; cdecl; external;

(*
  Retourne le type de bonus d'une case
*)
function regarder_type_bonus(pos : position) : bonus; cdecl; external;

(*
  Retourne la liste des bonus d'une équipe
*)
function regarder_bonus(equipe : cint) : array_of_bonus; cdecl; external;

(*
  Déplace une moto
*)
function deplacer(id : cint; de : position; vers : position) : erreur; cdecl; external;

(*
  Coupe une traînée de moto en deux nouvelles traînées. « entre » et « et » doivent être deux positions adjacentes occupées par une même traînée de moto.
*)
function couper_trainee_moto(id : cint; entre : position; et : position) : erreur; cdecl; external;

(*
  Annuler l'action précédente
*)
function cancel() : erreur; cdecl; external;

(*
  Enrouler la traînée de moto en un point
*)
function enrouler(id : cint; point : position) : erreur; cdecl; external;

(*
  Régénère une source d'énergie à son maximal
*)
function regenerer_source_energie(id : cint) : erreur; cdecl; external;

(*
  Allonge le tour en rajoutant des points d'action
*)
function allonger_pa() : erreur; cdecl; external;

(*
  Allonge une traînée de moto. L'allongement se fera aux prochains déplacements. La longueur du prolongement doit être comprise entre 0 et MAX_ALLONGEMENT (inclus).
*)
function agrandir_trainee_moto(id : cint; longueur : cint) : erreur; cdecl; external;

(*
  Pose un point de croisement sur une case du terrain. La case doit ne pas déjà être un point de croisement.
*)
function poser_point_croisement(point : position) : erreur; cdecl; external;

(*
  Fusionner deux traînées de moto. Les deux doivent appartenir à la même équipe, mais doivent être deux traînées distinctes. « pos1 » et « pos2 » doivent être adjacentes et occupées respectivement par « id1 » et « id2 ».
*)
function fusionner(id1 : cint; pos1 : position; id2 : cint; pos2 : position) : erreur; cdecl; external;

(*
  Affiche le contenu d'une valeur de type erreur
*)
procedure afficher_erreur(v : erreur); cdecl; external;

(*
  Affiche le contenu d'une valeur de type type_case
*)
procedure afficher_type_case(v : type_case); cdecl; external;

(*
  Affiche le contenu d'une valeur de type bonus
*)
procedure afficher_bonus(v : bonus); cdecl; external;

(*
  Affiche le contenu d'une valeur de type position
*)
procedure afficher_position(v : position); cdecl; external;

(*
  Affiche le contenu d'une valeur de type source_energie
*)
procedure afficher_source_energie(v : source_energie); cdecl; external;

(*
  Affiche le contenu d'une valeur de type trainee_moto
*)
procedure afficher_trainee_moto(v : trainee_moto); cdecl; external;

implementation

begin
end.
