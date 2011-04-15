// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_java.rb

// Représente une position sur le terrain du jeu
class Position {
  public int x; // coordonnée en X
  public int y; // coordonnée en Y
}

// Caracteristiques d'une source d'énergie
class Source_energie {
  public int id; // identifiant de la source d'énergie
  public Position pos = new Position(); // position de la source d'énergie
  public int coef; // coefficient representant les points d'énergie que la source va vous apporter
}

// Représente une traînée de moto sur le terrain
class Trainee_moto {
  public int id; // identifiant de la traînee
  public Position[] emplacement; // position de chaque composant de la traînée de moto
  public int team; // identifiant de l'équipe qui possède cette traînée de moto
}

// Énumération représentant une erreur renvoyée par une des fonctions d'action
enum Erreur{
  Ok, // <- aucune erreur n'est survenue
  Id_invalide, // <- identifiant invalide
  Position_invalide, // <- la position spécifiée est invalide
  Plus_de_pa, // <- vous n'avez pas assez de points d'action
  Bonus_invalide, // <- vous n'avez pas ce bonus
  Pas_a_toi, // <- l'unité n'est pas a vous
}

// Énumération représentant les différents types de case
enum Type_case{
  Vide, // <- rien n'est présent sur la case
  Obstacle, // <- cette case est inaccessible
  Bonus, // <- cette case cotient un bonus
  Point_croisement, // <- point de croisement de traînées
}

// Énumération représentant les différents types de bonii
enum Type_bonus{
  Pas_bonus, // <- ceci n'est pas un bonus :-)
  Bonus_croisement, // <- bonus permettant de croiser deux traînées de moto sur une case
  Plus_long, // <- bonus permettant d'agrandir une traînée de moto
  Plus_pa, // <- bonus permettant d'avoir plus de points d'action
  Bonus_regeneration, // <- bonus permettant de regenerer une source d'energie
}

public class Interface
{
// Taille du terrain
  public static final int TAILLE_TERRAIN = 50;

// Nombre de tours par partie
  public static final int FIN_PARTIE = 1500;

// Nombre de points d'action par tour
  public static final int MAX_PA = 3;

// Taille des traînées de moto
  public static final int TAILLE_TRAINEE = 900;

// Longueur maximale de l'allongement
  public static final int MAX_ALLONGEMENT = 5;

  // Retourne le numéro de votre équipe
  public static native int mon_equipe();

  // Retourne les scores de chaque équipe
  public static native Integer[] scores();

  // Retourne le nombre d'équipes sur le terrain
  public static native int nombre_equipes();

  // Retourne le numéro du tour actuel
  public static native int tour_actuel();

  // Retourne la liste des sources d'énergie
  public static native Source_energie[] sources_energie();

  // Retourne la liste des traînées de moto
  public static native Trainee_moto[] trainees_moto();

  // Retourne le type d'une case
  public static native Type_case regarder_type_case(Position pos);

  // Retourne le type de bonus d'une case
  public static native Type_bonus regarder_type_bonus(Position pos);

  // Retourne la liste des bonus d'une équipe
  public static native Type_bonus[] regarder_bonus(int equipe);

  // Déplace une moto
  public static native Erreur deplacer(int id, Position de, Position vers);

  // Coupe une traînée de moto en deux nouvelles traînées. « entre » et « et » doivent être deux positions adjacentes occupées par une même traînée de moto.
  public static native Erreur couper_trainee_moto(int id, Position entre, Position et);

  // Annuler l'action précédente
  public static native Erreur cancel();

  // Enrouler la traînée de moto en un point
  public static native Erreur enrouler(int id, Position point);

  // Régénère une source d'énergie à son maximal
  public static native Erreur regenerer_source_energie(int id);

  // Allonge le tour en rajoutant des points d'action
  public static native Erreur allonger_pa();

  // Allonge une traînée de moto. L'allongement se fera aux prochains déplacements. La longueur du prolongement doit être comprise entre 0 et MAX_ALLONGEMENT (inclus).
  public static native Erreur agrandir_trainee_moto(int id, int longueur);

  // Pose un point de croisement sur une case du terrain. La case doit ne pas déjà être un point de croisement.
  public static native Erreur poser_point_croisement(Position point);

  // Fusionner deux traînées de moto. Les deux doivent appartenir à la même équipe, mais doivent être deux traînées distinctes. « pos1 » et « pos2 » doivent être adjacentes et occupées respectivement par « id1 » et « id2 ».
  public static native Erreur fusionner(int id1, Position pos1, int id2, Position pos2);

  // Affiche le contenu d'une valeur de type erreur
  public static native void afficher_erreur(Erreur v);

  // Affiche le contenu d'une valeur de type type_case
  public static native void afficher_type_case(Type_case v);

  // Affiche le contenu d'une valeur de type type_bonus
  public static native void afficher_type_bonus(Type_bonus v);

  // Affiche le contenu d'une valeur de type position
  public static native void afficher_position(Position v);

  // Affiche le contenu d'une valeur de type source_energie
  public static native void afficher_source_energie(Source_energie v);

  // Affiche le contenu d'une valeur de type trainee_moto
  public static native void afficher_trainee_moto(Trainee_moto v);

}
