// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_java.rb

// Repr�sente une position sur le terrain du jeu
class Position {
  public int x; // coordonn�e en X
  public int y; // coordonn�e en Y
}

// Caracteristiques d'une source d'�nergie
class Source_energie {
  public int id; // identifiant de la source d'�nergie
  public Position pos = new Position(); // position de la source d'�nergie
  public int coef; // coefficient representant les points d'�nergie que la source va vous apporter
}

// Repr�sente une tra�n�e de moto sur le terrain
class Trainee_moto {
  public int id; // identifiant de la tra�nee
  public Position[] emplacement; // position de chaque composant de la tra�n�e de moto
  public int team; // identifiant de l'�quipe qui poss�de cette tra�n�e de moto
}

// �num�ration repr�sentant une erreur renvoy�e par une des fonctions d'action
enum Erreur{
  Ok, // <- aucune erreur n'est survenue
  Id_invalide, // <- identifiant invalide
  Position_invalide, // <- la position sp�cifi�e est invalide
  Plus_de_pa, // <- vous n'avez pas assez de points d'action
  Bonus_invalide, // <- vous n'avez pas ce bonus
  Pas_a_toi, // <- l'unit� n'est pas a vous
}

// �num�ration repr�sentant les diff�rents types de case
enum Type_case{
  Vide, // <- rien n'est pr�sent sur la case
  Obstacle, // <- cette case est inaccessible
  Bonus, // <- cette case cotient un bonus
  Point_croisement, // <- point de croisement de tra�n�es
}

// �num�ration repr�sentant les diff�rents types de bonii
enum Bonus{
  Vide, // <- ceci n'est pas un bonus :-)
  Bonus_croisement, // <- bonus permettant de croiser deux tra�n�es de moto sur une case
  Plus_long, // <- bonus permettant d'agrandir une tra�n�e de moto
  Plus_pa, // <- bonus permettant d'avoir plus de points d'action
  Bonus_regeneration, // <- bonus permettant de regenerer une source d'energie
}

public class Interface
{
// Taille du terrain
  public static final int TAILLE_TERRAIN = 100;

// Nombre de tours avant la fin de la partie
  public static final int FIN_PARTIE = 1500;

// Nombre de points d'action par tour
  public static final int MAX_PA = 3;

// Taille des tra�n�es de moto
  public static final int TAILLE_TRAINEE = 900;

// Longueur maximale de l'allongement
  public static final int MAX_ALLONGEMENT = 5;

  // Retourne le num�ro de votre �quipe
  public static native int mon_equipe();

  // Retourne les scores de chaque �quipe
  public static native Integer[] scores();

  // Retourne le nombre d'�quipes sur le terrain
  public static native int nombre_equipes();

  // Retourne le num�ro du tour actuel
  public static native int tour_actuel();

  // Retourne la liste des sources d'�nergie
  public static native Source_energie[] sources_energie();

  // Retourne la liste des tra�n�es de moto
  public static native Trainee_moto[] trainees_moto();

  // Retourne le type d'une case
  public static native Type_case regarder_type_case(Position pos);

  // Retourne le type de bonus d'une case
  public static native Bonus regarder_type_bonus(Position pos);

  // Retourne la liste des bonus d'une �quipe
  public static native Bonus[] regarder_bonus(int equipe);

  // D�place une moto
  public static native Erreur deplacer(int id, Position de, Position vers);

  // Coupe une tra�n�e de moto en deux nouvelles tra�n�es. � entre � et � et � doivent �tre deux positions adjacentes occup�es par une m�me tra�n�e de moto.
  public static native Erreur couper_trainee_moto(int id, Position entre, Position et);

  // Annuler l'action pr�c�dente
  public static native Erreur cancel();

  // Enrouler la tra�n�e de moto en un point
  public static native Erreur enrouler(int id, Position point);

  // R�g�n�re une source d'�nergie � son maximal
  public static native Erreur regenerer_source_energie(int id);

  // Allonge le tour en rajoutant des points d'action
  public static native Erreur allonger_pa();

  // Allonge une tra�n�e de moto. L'allongement se fera aux prochains d�placements. La longueur du prolongement doit �tre comprise entre 0 et MAX_ALLONGEMENT (inclus).
  public static native Erreur agrandir_trainee_moto(int id, int longueur);

  // Pose un point de croisement sur une case du terrain. La case doit ne pas d�j� �tre un point de croisement.
  public static native Erreur poser_point_croisement(Position point);

  // Fusionner deux tra�n�es de moto. Les deux doivent appartenir � la m�me �quipe, mais doivent �tre deux tra�n�es distinctes. � pos1 � et � pos2 � doivent �tre adjacentes et occup�es respectivement par � id1 � et � id2 �.
  public static native Erreur fusionner(int id1, Position pos1, int id2, Position pos2);

  // Affiche le contenu d'une valeur de type erreur
  public static native void afficher_erreur(Erreur v);

  // Affiche le contenu d'une valeur de type type_case
  public static native void afficher_type_case(Type_case v);

  // Affiche le contenu d'une valeur de type bonus
  public static native void afficher_bonus(Bonus v);

  // Affiche le contenu d'une valeur de type position
  public static native void afficher_position(Position v);

  // Affiche le contenu d'une valeur de type source_energie
  public static native void afficher_source_energie(Source_energie v);

  // Affiche le contenu d'une valeur de type trainee_moto
  public static native void afficher_trainee_moto(Trainee_moto v);

}
