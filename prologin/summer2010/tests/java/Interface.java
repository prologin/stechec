// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_java.rb

// Représente une position sur le terrain du jeu.
class Position {
  public int x; // coordonnée X
  public int y; // coordonnée Y
}

// les caracteristiques d'un objet
class Caracteristiques_objet {
  public int cout; // ce que coute l'objet
  public int porte; // la porte de l'objet
}

// Représente une unité sur le terrain.
class Unite {
  public Position pos_unite = new Position(); // la position de l'unité
  public int team; // donne l'equipe du joueur
  public int ko; // renvoie le temps que l'unite passera sans pouvoir bouger.
  public int pa; // le nombre de PA restants à l'unité
  public Type_objet objet = Type_objet.Filet; // renvoie l'objet que porte l'unite.
  public int id; // l'identifiant unique de l'unité
}

// Représente le type d'une piece.
class Piece {
  public int valeur; // la valeur de la piece.
  public Position pos_piece = new Position(); // renvoie la position de la piece
  public int tour_apparition; // renvoie le tour ou la piece apparaitra ou est apparu.
}

// Énumération représentant une erreur renvoyée par une des fonctions d'action.
enum Erreur{
  Ok, // <- aucune erreur n'est survenue
  Position_invalide, // <- la position spécifiée est invalide
  Plus_de_pa, // <- vous n'avez pas assez de points d'actions
  Pas_a_porte, // <- vous ne pouvez pas utiliser cet objet la cible n'est pas a porte
  Unite_ko, // <- votre unite est ko
  Pas_a_toi, // <- l'unite n'est pas a toi.
  Utilisation_impossible, // <- vous ne pouvez pas utiliser cet objet
  Plus_d_argent, // <- vous n'avez pas assez d'argent pour acheter l'objet en question
}

// Le type d'un objet
enum Type_objet{
  Filet, // <- permet d'attraper des pieces ou d'immobiliser des unités
  Marteau, // <- permet de frapper des unités.
  Rien, // <- l'unite ne porte aucun objet
}

public class Interface
{
// Taille du terrain.
  public static final int TAILLE_TERRAIN = 21;

// le prix de la fin d'une partie
  public static final int PRIX_FIN_PARTIE = 25;

// le nombre de points d'actions des unites au debut du tour.
  public static final int MAX_PA = 3;

// Le nombre d'unités par équipe.
  public static final int NBR_UNITES = 9;

// Le nombre maximum de tours.
  public static final int MAX_TURN = 100;

// Le nombre points KO infliges par un coup de marteau
  public static final int MARTEAU_KO = 10;

// Le nombre points KO qu'une unite subbit losqu'elle se prend un filet.
  public static final int FILET_KO = 4;

  // Renvoie le numero de votre equipe.
  public static native int mon_equipe();

  // Renvoie le score d'une equipe.
  public static native int score(int team);

  // Renvoie le nombre d'equipes sur la map
  public static native int nombre_equipes();

  // Renvoie le numéro du tour actuel.
  public static native int tour_actuel();

  // Renvoie les pieces qui sont sur la map
  public static native Piece[] pieces_en_jeu();

  // Renvoie le 10 prochaines pieces a vennir
  public static native Piece[] pieces_a_vennir();

  // Retourne la liste des unités actuellement en jeu.
  public static native Unite[] unites();

  // Retourne les caracteristiques de l'objet.
  public static native Caracteristiques_objet proprietes_objet(Type_objet to);

  // Déplace une unité vers une position à portée.
  public static native Erreur deplacer(Position cible, Position pos);

  // Achete un objet
  public static native Erreur acheter_objet(Position cible, Type_objet objet);

  // utilise un objet
  public static native Erreur utiliser(Position attaquant, Position cible);

  // coupe la partie, cette action coute 100 d'or et met fin a la partie.
  public static native Erreur achever_la_partie();

  // Affiche le contenu d'une valeur de type erreur
  public static native void afficher_erreur(Erreur v);

  // Affiche le contenu d'une valeur de type type_objet
  public static native void afficher_type_objet(Type_objet v);

  // Affiche le contenu d'une valeur de type position
  public static native void afficher_position(Position v);

  // Affiche le contenu d'une valeur de type caracteristiques_objet
  public static native void afficher_caracteristiques_objet(Caracteristiques_objet v);

  // Affiche le contenu d'une valeur de type unite
  public static native void afficher_unite(Unite v);

  // Affiche le contenu d'une valeur de type piece
  public static native void afficher_piece(Piece v);

}
