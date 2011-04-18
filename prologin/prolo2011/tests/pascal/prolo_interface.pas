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
	TAILLE_TERRAIN            =  50;
(*
  Nombre de tours par partie
*)
	FIN_PARTIE                =  1500;
(*
  Nombre de points d'action par tour
*)
	MAX_PA                    =  3;
(*
  Taille des tra�n�es de moto
*)
	TAILLE_TRAINEE            =  900;
(*
  Longueur maximale de l'allongement
*)
	MAX_ALLONGEMENT           =  5;
(*
  �num�ration repr�sentant une erreur renvoy�e par une des fonctions d'action
*)
type erreur =
  (
    ok { <- aucune erreur n'est survenue },
    id_invalide { <- identifiant invalide },
    position_invalide { <- la position sp�cifi�e est invalide },
    plus_de_pa { <- vous n'avez pas assez de points d'action },
    bonus_invalide { <- vous n'avez pas ce bonus },
    pas_a_toi { <- l'unit� n'est pas a vous }
  );

type array_of_erreur = array of erreur;


(*
  �num�ration repr�sentant les diff�rents types de case
*)
type type_case =
  (
    vide { <- rien n'est pr�sent sur la case },
    obstacle { <- cette case est inaccessible },
    bonus { <- cette case cotient un bonus },
    point_croisement { <- point de croisement de tra�n�es }
  );

type array_of_type_case = array of type_case;


(*
  �num�ration repr�sentant les diff�rents types de bonii
*)
type type_bonus =
  (
    pas_bonus { <- ceci n'est pas un bonus :-) },
    bonus_croisement { <- bonus permettant de croiser deux tra�n�es de moto sur une case },
    plus_long { <- bonus permettant d'agrandir une tra�n�e de moto },
    plus_pa { <- bonus permettant d'avoir plus de points d'action },
    bonus_regeneration { <- bonus permettant de regenerer une source d'energie }
  );

type array_of_type_bonus = array of type_bonus;


(*
  Repr�sente une position sur le terrain du jeu
*)
type position =
  record
    x : cint; (*  <- coordonn�e en X *)
    y : cint; (*  <- coordonn�e en Y *)
  end;

type array_of_position = array of position;


(*
  Caracteristiques d'une source d'�nergie
*)
type source_energie =
  record
    id : cint; (*  <- identifiant de la source d'�nergie *)
    pos : position; (*  <- position de la source d'�nergie *)
    coef : cint; (*  <- coefficient representant les points d'�nergie que la source va vous apporter *)
  end;

type array_of_source_energie = array of source_energie;


(*
  Repr�sente une tra�n�e de moto sur le terrain
*)
type trainee_moto =
  record
    id : cint; (*  <- identifiant de la tra�nee *)
    emplacement : array of position; (*  <- position de chaque composant de la tra�n�e de moto *)
    team : cint; (*  <- identifiant de l'�quipe qui poss�de cette tra�n�e de moto *)
  end;

type array_of_trainee_moto = array of trainee_moto;


(*
  Retourne le num�ro de votre �quipe
*)
function mon_equipe() : cint; cdecl; external;

(*
  Retourne les scores de chaque �quipe
*)
function scores() : array_of_cint; cdecl; external;

(*
  Retourne le nombre d'�quipes sur le terrain
*)
function nombre_equipes() : cint; cdecl; external;

(*
  Retourne le num�ro du tour actuel
*)
function tour_actuel() : cint; cdecl; external;

(*
  Retourne la liste des sources d'�nergie
*)
function sources_energie() : array_of_source_energie; cdecl; external;

(*
  Retourne la liste des tra�n�es de moto
*)
function trainees_moto() : array_of_trainee_moto; cdecl; external;

(*
  Retourne le type d'une case
*)
function regarder_type_case(pos : position) : type_case; cdecl; external;

(*
  Retourne le type de bonus d'une case
*)
function regarder_type_bonus(pos : position) : type_bonus; cdecl; external;

(*
  Retourne la liste des bonus d'une �quipe
*)
function regarder_bonus(equipe : cint) : array_of_type_bonus; cdecl; external;

(*
  D�place une moto
*)
function deplacer(id : cint; de : position; vers : position) : erreur; cdecl; external;

(*
  Coupe une tra�n�e de moto en deux nouvelles tra�n�es. � entre � et � et � doivent �tre deux positions adjacentes occup�es par une m�me tra�n�e de moto.
*)
function couper_trainee_moto(id : cint; entre : position; et : position) : erreur; cdecl; external;

(*
  Annuler l'action pr�c�dente
*)
function cancel() : erreur; cdecl; external;

(*
  Enrouler la tra�n�e de moto en un point
*)
function enrouler(id : cint; point : position) : erreur; cdecl; external;

(*
  R�g�n�re une source d'�nergie � son maximal
*)
function regenerer_source_energie(id : cint) : erreur; cdecl; external;

(*
  Allonge le tour en rajoutant des points d'action
*)
function allonger_pa() : erreur; cdecl; external;

(*
  Allonge une tra�n�e de moto. L'allongement se fera aux prochains d�placements. La longueur du prolongement doit �tre comprise entre 0 et MAX_ALLONGEMENT (inclus).
*)
function agrandir_trainee_moto(id : cint; longueur : cint) : erreur; cdecl; external;

(*
  Pose un point de croisement sur une case du terrain. La case doit ne pas d�j� �tre un point de croisement.
*)
function poser_point_croisement(point : position) : erreur; cdecl; external;

(*
  Fusionner deux tra�n�es de moto. Les deux doivent appartenir � la m�me �quipe, mais doivent �tre deux tra�n�es distinctes. � pos1 � et � pos2 � doivent �tre adjacentes et occup�es respectivement par � id1 � et � id2 �.
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
  Affiche le contenu d'une valeur de type type_bonus
*)
procedure afficher_type_bonus(v : type_bonus); cdecl; external;

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
