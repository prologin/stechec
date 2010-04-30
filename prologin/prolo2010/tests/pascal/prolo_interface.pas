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
  Taille de d�part du terrain.
*)
	TAILLE_DEPART             =  25;
(*
  Le nombre maximal d'unit�s pouvant appartenir � une �quipe.
*)
	NBR_MAX_UNITES            =  10;
(*
  Le temps, en nombre de tours, entre deux r�tr�cissement du terrain.
*)
	TEMPS_RETRECISSEMENT      =  5;
(*
  Le nombre maximum en jeu de chaque carte.
*)
	MAX_CARTES                =  2;
(*
  �num�ration repr�sentant une erreur renvoy�e par une des fonctions d'action.
*)
type erreur =
  (
    ok { <- aucune erreur n'est survenue },
    position_invalide { <- la position sp�cifi�e est invalide },
    case_occupee { <- la case sur laquelle vous tentez de vous d�placer est occup�e },
    pas_a_portee { <- la cible n'est pas � port�e },
    unite_ko { <- l'unit� que vous essayez de faire agir ou sur laquelle vous essayez d'agir  est KO },
    unite_debout { <- l'unit� que vous essayez de relever est d�j� debout },
    quota_depasse { <- nombre maximal d'unites, de spawn ou de relevages par tour d�pass� },
    plus_de_pa { <- cette unit� a r�alis� toutes ses actions },
    deja_attaque { <- cette unit� a d�j� attaqu� },
    unite_interdite { <- cette unit� ne peut pas �tre amen�e en renfort },
    renfort_impossible { <- une unit� est d�j� pr�sente sur le spawn },
    pas_a_moi { <- l'unit� cibl�e n'est pas � vous },
    plus_de_cartes { <- il n'y a plus de cartes du type sp�cifi� dans votre main },
    phase_cartes_terminee { <- vous ne pouvez plus poser de cartes car vous avez fait une action }
  );

type array_of_erreur = array of erreur;


(*
  Le type d'une unit� sur le terrain
*)
type type_unite =
  (
    perroquet { <- 1 PA et port�e de 1 },
    singe { <- 2 PA et port�e de 3 },
    chat { <- 5 PA et port�e de 1 },
    kangourou { <- 3 PA et port�e de 2 (explose !) }
  );

type array_of_type_unite = array of type_unite;


(*
  Repr�sente une position sur le terrain du jeu.
*)
type position =
  record
    x : cint; (*  <- coordonn�e X *)
    y : cint; (*  <- coordonn�e Y *)
  end;

type array_of_position = array of position;


(*
  Contient les informations sur la taille du terrain du jeu.
*)
type taille_terrain =
  record
    taille : cint; (*  <- taille actuelle du terrain *)
    min_coord : cint; (*  <- coordonn�e minimale en X ou en Y *)
    max_coord : cint; (*  <- coordonn�e maximale en X ou en Y *)
  end;

type array_of_taille_terrain = array of taille_terrain;


(*
  Donne les caract�ristiques d'un type d'unit�.
*)
type caracs =
  record
    pa_init : cint; (*  <- nombre de points d'actions par tour *)
    portee : cint; (*  <- port�e maximale de l'unit� *)
  end;

type array_of_caracs = array of caracs;


(*
  Repr�sente une unit� sur le terrain.
*)
type unite =
  record
    pos : position; (*  <- la position de l'unit� *)
    ennemi : boolean; (*  <- vrai si l'unit� appartient � l'ennemi *)
    type_unite_actuel : type_unite; (*  <- le type de l'unit�, qui change si l'unit� est d�guis�e *)
    vrai_type_unite : type_unite; (*  <- le vrai type de l'unit� (qui ne change pas lors du d�guisement) *)
    ko : cint; (*  <- une valeur n�gative si l'unit� n'est pas KO, sinon le nombre de marqueurs KO sur l'unit� *)
    pa : cint; (*  <- le nombre de PA restants � l'unit� *)
    attaques : cint; (*  <- le nombre d'attaques restants � l'unit� *)
    attaques_gratuites : cint; (*  <- le nombre d'attaques gratuites (voir la partie banzai) *)
    id : cint; (*  <- l'identifiant unique de l'unit� *)
  end;

type array_of_unite = array of unite;


(*
  Repr�sente l'ensemble des cartes que vous pouvez utiliser.
*)
type cartes =
  record
    potion : cint; (*  <- le nombre de cartes � Potion magique � *)
    deguisement : cint; (*  <- le nombre de cartes � D�guisement � *)
    banzai : cint; (*  <- le nombre de cartes � Banza� � *)
    pacifisme : cint; (*  <- le nombre de cartes � Pacifisme � *)
  end;

type array_of_cartes = array of cartes;


(*
  Renvoie le nombre de points de commandements.
*)
function nombre_pc() : cint; cdecl; external;

(*
  Renvoie le nombre d'unit�s en jeu.
*)
function nombre_unites(ennemi : boolean) : cint; cdecl; external;

(*
  Renvoie le num�ro du tour actuel.
*)
function tour_actuel() : cint; cdecl; external;

(*
  Renvoie la position du spawn (ennemi ou non).
*)
function pos_renfort(ennemi : boolean) : position; cdecl; external;

(*
  Renvoie les caract�ristiques d'un type d'unit�.
*)
function caracteristiques(tu : type_unite) : caracs; cdecl; external;

(*
  Retourne une structure "cartes" contenant les informations sur les cartes que vous avez en main.
*)
function mes_cartes() : cartes; cdecl; external;

(*
  Retourne la liste des unit�s actuellement en jeu.
*)
function unites() : array_of_unite; cdecl; external;

(*
  Retourne la taille actuelle du terrain et les coordonn�es min/max dans une structure "taille_terrain".
*)
function taille_terrain_actuelle() : taille_terrain; cdecl; external;

(*
  Utilise une carte � Potion magique � que vous avez dans votre main.
*)
function potion(cible : position) : erreur; cdecl; external;

(*
  Utilise une carte � D�guisement � que vous avez dans votre main.
*)
function deguisement(cible : position; nouveau_type : type_unite) : erreur; cdecl; external;

(*
  Utilise une carte � Banza� � que vous avez dans votre main.
*)
function banzai(cible : position) : erreur; cdecl; external;

(*
  Utilise une carte � Pacifisme � que vous avez dans votre main.
*)
function pacifisme() : erreur; cdecl; external;

(*
  D�place une unit� vers une position � port�e.
*)
function deplacer(cible : position; pos : position) : erreur; cdecl; external;

(*
  Rel�ve une unit� n'ayant plus de marqueurs de KO.
*)
function relever(cible : position) : erreur; cdecl; external;

(*
  Attaque une autre unit�.
*)
function attaquer(attaquant : position; cible : position) : erreur; cdecl; external;

(*
  Fait apparaitre une unit� sur la case de spawn.
*)
function renfort(quoi : type_unite) : erreur; cdecl; external;

(*
  Annule l'effet de la derni�re action et remet le jeu dans l'�tat pr�c�dent. Renvoie false s'il n'y a rien � annuler, true sinon.
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
