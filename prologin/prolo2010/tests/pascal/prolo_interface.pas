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
  Taille de départ du terrain.
*)
	TAILLE_DEPART             =  19;
(*
  Le nombre maximal d'unités pouvant appartenir à une équipe.
*)
	NBR_MAX_UNITES            =  10;
(*
  Le temps, en nombre de tours, entre deux rétrécissement du terrain.
*)
	TEMPS_RETRECISSEMENT      =  5;
(*
  Le nombre maximum en jeu de chaque carte.
*)
	MAX_CARTES                =  2;
(*
  Énumération représentant une erreur renvoyée par une des fonctions d'action.
*)
type erreur =
  (
    ok { <- aucune erreur n'est survenue },
    position_invalide { <- la position spécifiée est invalide },
    case_occupee { <- la case sur laquelle vous tentez de vous déplacer est occupée },
    pas_a_portee { <- la cible n'est pas à portée },
    unite_ko { <- l'unité que vous essayez de faire agir ou sur laquelle vous essayez d'agir  est KO },
    unite_debout { <- l'unité que vous essayez de relever est déjà debout },
    quota_depasse { <- nombre maximal d'unites, de spawn ou de relevages par tour dépassé },
    plus_de_pa { <- cette unité a réalisé toutes ses actions },
    deja_attaque { <- cette unité a déjà attaqué },
    unite_interdite { <- cette unité ne peut pas être amenée en renfort },
    renfort_impossible { <- une unité est déjà présente sur le spawn },
    pas_a_moi { <- l'unité ciblée n'est pas à vous },
    plus_de_cartes { <- il n'y a plus de cartes du type spécifié dans votre main },
    phase_cartes_terminee { <- vous ne pouvez plus poser de cartes car vous avez fait une action }
  );

type array_of_erreur = array of erreur;


(*
  Le type d'une unité sur le terrain
*)
type type_unite =
  (
    perroquet { <- 1 PA et portée de 1 },
    singe { <- 2 PA et portée de 3 },
    chat { <- 5 PA et portée de 1 },
    kangourou { <- 3 PA et portée de 2 (explose !) }
  );

type array_of_type_unite = array of type_unite;


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
  Contient les informations sur la taille du terrain du jeu.
*)
type taille_terrain =
  record
    taille : cint; (*  <- taille actuelle du terrain *)
    min_coord : cint; (*  <- coordonnée minimale en X ou en Y *)
    max_coord : cint; (*  <- coordonnée maximale en X ou en Y *)
  end;

type array_of_taille_terrain = array of taille_terrain;


(*
  Donne les caractéristiques d'un type d'unité.
*)
type caracs =
  record
    pa_init : cint; (*  <- nombre de points d'actions par tour *)
    portee : cint; (*  <- portée maximale de l'unité *)
  end;

type array_of_caracs = array of caracs;


(*
  Représente une unité sur le terrain.
*)
type unite =
  record
    pos : position; (*  <- la position de l'unité *)
    ennemi : boolean; (*  <- vrai si l'unité appartient à l'ennemi *)
    type_unite_actuel : type_unite; (*  <- le type de l'unité, qui change si l'unité est déguisée *)
    vrai_type_unite : type_unite; (*  <- le vrai type de l'unité (qui ne change pas lors du déguisement) *)
    ko : cint; (*  <- une valeur négative si l'unité n'est pas KO, sinon le nombre de marqueurs KO sur l'unité *)
    pa : cint; (*  <- le nombre de PA restants à l'unité *)
    attaques : cint; (*  <- le nombre d'attaques restants à l'unité *)
    attaques_gratuites : cint; (*  <- le nombre d'attaques gratuites (voir la partie banzai) *)
    id : cint; (*  <- l'identifiant unique de l'unité *)
  end;

type array_of_unite = array of unite;


(*
  Représente l'ensemble des cartes que vous pouvez utiliser.
*)
type cartes =
  record
    potion : cint; (*  <- le nombre de cartes « Potion magique » *)
    deguisement : cint; (*  <- le nombre de cartes « Déguisement » *)
    banzai : cint; (*  <- le nombre de cartes « Banzaï » *)
    pacifisme : cint; (*  <- le nombre de cartes « Pacifisme » *)
  end;

type array_of_cartes = array of cartes;


(*
  Renvoie le nombre de points de commandements.
*)
function nombre_pc() : cint; cdecl; external;

(*
  Renvoie le nombre d'unités en jeu.
*)
function nombre_unites(ennemi : boolean) : cint; cdecl; external;

(*
  Renvoie le numéro du tour actuel.
*)
function tour_actuel() : cint; cdecl; external;

(*
  Renvoie la position du spawn (ennemi ou non).
*)
function pos_renfort(ennemi : boolean) : position; cdecl; external;

(*
  Renvoie les caractéristiques d'un type d'unité.
*)
function caracteristiques(tu : type_unite) : caracs; cdecl; external;

(*
  Retourne une structure "cartes" contenant les informations sur les cartes que vous avez en main.
*)
function mes_cartes() : cartes; cdecl; external;

(*
  Retourne la liste des unités actuellement en jeu.
*)
function unites() : array_of_unite; cdecl; external;

(*
  Retourne la taille actuelle du terrain et les coordonnées min/max dans une structure "taille_terrain".
*)
function taille_terrain_actuelle() : taille_terrain; cdecl; external;

(*
  Utilise une carte « Potion magique » que vous avez dans votre main.
*)
function potion(cible : position) : erreur; cdecl; external;

(*
  Utilise une carte « Déguisement » que vous avez dans votre main.
*)
function deguisement(cible : position; nouveau_type : type_unite) : erreur; cdecl; external;

(*
  Utilise une carte « Banzaï » que vous avez dans votre main.
*)
function banzai(cible : position) : erreur; cdecl; external;

(*
  Utilise une carte « Pacifisme » que vous avez dans votre main.
*)
function pacifisme() : erreur; cdecl; external;

(*
  Déplace une unité vers une position à portée.
*)
function deplacer(cible : position; pos : position) : erreur; cdecl; external;

(*
  Relève une unité n'ayant plus de marqueurs de KO.
*)
function relever(cible : position) : erreur; cdecl; external;

(*
  Attaque une autre unité.
*)
function attaquer(attaquant : position; cible : position) : erreur; cdecl; external;

(*
  Fait apparaitre une unité sur la case de spawn.
*)
function renfort(quoi : type_unite) : erreur; cdecl; external;

(*
  Annule l'effet de la dernière action et remet le jeu dans l'état précédent. Renvoie false s'il n'y a rien à annuler, true sinon.
*)
function annuler() : boolean; cdecl; external;

(*
  Affiche le contenu d'une valeur de type erreur
*)
procedure afficher_erreur(v : erreur); cdecl; external;

(*
  Affiche le contenu d'une valeur de type type_unite
*)
procedure afficher_type_unite(v : type_unite); cdecl; external;

(*
  Affiche le contenu d'une valeur de type position
*)
procedure afficher_position(v : position); cdecl; external;

(*
  Affiche le contenu d'une valeur de type taille_terrain
*)
procedure afficher_taille_terrain(v : taille_terrain); cdecl; external;

(*
  Affiche le contenu d'une valeur de type caracs
*)
procedure afficher_caracs(v : caracs); cdecl; external;

(*
  Affiche le contenu d'une valeur de type unite
*)
procedure afficher_unite(v : unite); cdecl; external;

(*
  Affiche le contenu d'une valeur de type cartes
*)
procedure afficher_cartes(v : cartes); cdecl; external;

implementation

begin
end.
