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
  Taille du terrain.
*)
	TAILLE_TERRAIN            =  21;
(*
  le nombre de points d'actions des unites au debut du tour.
*)
	MAX_PA                    =  3;
(*
  Le nombre d'unités par équipe.
*)
	NBR_UNITES                =  9;
(*
  Le nombre maximum de tours.
*)
	MAX_TURN                  =  100;
(*
  Énumération représentant une erreur renvoyée par une des fonctions d'action.
*)
type erreur =
  (
    ok { <- aucune erreur n'est survenue },
    position_invalide { <- la position spécifiée est invalide },
    utilisation_impossible { <- vous ne pouvez pas utiliser cet objet },
    plus_d_argent { <- vous n'avez pas assez d'argent pour acheter l'objet en question }
  );

type array_of_erreur = array of erreur;


(*
  Le type d'un objet
*)
type type_objet =
  (
    filet { <- permet d'attraper des pieces ou d'immobiliser des unités },
    marteau { <- permet de frapper des unités. },
    rien { <- l'unite ne porte aucun objet }
  );

type array_of_type_objet = array of type_objet;


(*
  Représente une position sur le terrain du jeu.
*)
type position =
  record
    x : cint; (*  <- coordonnée X *)
    y : cint; (*  <- coordonnée Y *)
  end;

type array_of_position = array of position;


(*
  les caracteristiques d'un objet
*)
type caracteristiques_objet =
  record
    coute : cint; (*  <- ce que coute l'objet *)
    porte : cint; (*  <- la porte de l'objet *)
  end;

type array_of_caracteristiques_objet = array of caracteristiques_objet;


(*
  Représente une unité sur le terrain.
*)
type unite =
  record
    pos_unite : position; (*  <- la position de l'unité *)
    team : cint; (*  <- donne l'equipe du joueur *)
    ko : cint; (*  <- renvoie le temps que l'unite passera sans pouvoir bouger. *)
    pa : cint; (*  <- le nombre de PA restants à l'unité *)
    objet : type_objet; (*  <- renvoie l'objet que porte l'unite. *)
    id : cint; (*  <- l'identifiant unique de l'unité *)
  end;

type array_of_unite = array of unite;


(*
  Représente le type d'une piece.
*)
type piece =
  record
    valeur : cint; (*  <- la valeur de la piece. *)
    pos_piece : position; (*  <- renvoie la position de la piece *)
    tour_apparition : cint; (*  <- renvoie le tour ou la piece apparaitra ou est apparu. *)
  end;

type array_of_piece = array of piece;


(*
  Renvoie le numero de votre equipe.
*)
function mon_equipe() : cint; cdecl; external;

(*
  Renvoie le score d'une equipe.
*)
function score(team : cint) : cint; cdecl; external;

(*
  Renvoie le nombre d'equipes sur la map
*)
function nombre_equipes() : cint; cdecl; external;

(*
  Renvoie le numéro du tour actuel.
*)
function tour_actuel() : cint; cdecl; external;

(*
  Renvoie les pieces qui sont sur la map
*)
function pieces_en_jeu() : array_of_piece; cdecl; external;

(*
  Renvoie le 10 prochaines pieces a vennir
*)
function pieces_a_vennir() : array_of_piece; cdecl; external;

(*
  Retourne la liste des unités actuellement en jeu.
*)
function unites() : array_of_unite; cdecl; external;

(*
  Déplace une unité vers une position à portée.
*)
function deplacer(cible : position; pos : position) : erreur; cdecl; external;

(*
  Achete un objet
*)
function acheter_objet(cible : position; objet : type_objet) : erreur; cdecl; external;

(*
  utilise un objet
*)
function utiliser(attaquant : position; cible : position) : erreur; cdecl; external;

(*
  coupe la partie, cette action coute 100 d'or et met fin a la partie.
*)
function achever_la_partie() : erreur; cdecl; external;

(*
  Affiche le contenu d'une valeur de type erreur
*)
procedure afficher_erreur(v : erreur); cdecl; external;

(*
  Affiche le contenu d'une valeur de type type_objet
*)
procedure afficher_type_objet(v : type_objet); cdecl; external;

(*
  Affiche le contenu d'une valeur de type position
*)
procedure afficher_position(v : position); cdecl; external;

(*
  Affiche le contenu d'une valeur de type caracteristiques_objet
*)
procedure afficher_caracteristiques_objet(v : caracteristiques_objet); cdecl; external;

(*
  Affiche le contenu d'une valeur de type unite
*)
procedure afficher_unite(v : unite); cdecl; external;

(*
  Affiche le contenu d'une valeur de type piece
*)
procedure afficher_piece(v : piece); cdecl; external;

implementation

begin
end.
