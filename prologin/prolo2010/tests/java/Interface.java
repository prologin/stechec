// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_java.rb

// Représente une position sur le terrain du jeu.
class Position {
  public int x; // coordonnée X
  public int y; // coordonnée Y
}

// Contient les informations sur la taille du terrain du jeu.
class Taille_terrain {
  public int taille; // taille actuelle du terrain
  public int min_coord; // coordonnée minimale en X ou en Y
  public int max_coord; // coordonnée maximale en X ou en Y
}

// Donne les caractéristiques d'un type d'unité.
class Caracs {
  public int pa_init; // nombre de points d'actions par tour
  public int portee; // portée maximale de l'unité
}

// Représente une unité sur le terrain.
class Unite {
  public Position pos = new Position(); // la position de l'unité
  public boolean ennemi; // vrai si l'unité appartient à l'ennemi
  public Type_unite type_unite_actuel = Type_unite.Perroquet; // le type de l'unité, qui change si l'unité est déguisée
  public Type_unite vrai_type_unite = Type_unite.Perroquet; // le vrai type de l'unité (qui ne change pas lors du déguisement)
  public int ko; // une valeur négative si l'unité n'est pas KO, sinon le nombre de marqueurs KO sur l'unité
  public int pa; // le nombre de PA restants à l'unité
  public int attaques; // le nombre d'attaques restants à l'unité
  public int attaques_gratuites; // le nombre d'attaques gratuites (voir la partie banzai)
  public int id; // l'identifiant unique de l'unité
}

// Représente l'ensemble des cartes que vous pouvez utiliser.
class Cartes {
  public int potion; // le nombre de cartes « Potion magique »
  public int deguisement; // le nombre de cartes « Déguisement »
  public int banzai; // le nombre de cartes « Banzaï »
  public int pacifisme; // le nombre de cartes « Pacifisme »
}

// Énumération représentant une erreur renvoyée par une des fonctions d'action.
enum Erreur{
  Ok, // <- aucune erreur n'est survenue
  Position_invalide, // <- la position spécifiée est invalide
  Case_occupee, // <- la case sur laquelle vous tentez de vous déplacer est occupée
  Pas_a_portee, // <- la cible n'est pas à portée
  Unite_ko, // <- l'unité que vous essayez de faire agir ou sur laquelle vous essayez d'agir  est KO
  Unite_debout, // <- l'unité que vous essayez de relever est déjà debout
  Quota_depasse, // <- nombre maximal d'unites, de spawn ou de relevages par tour dépassé
  Plus_de_pa, // <- cette unité a réalisé toutes ses actions
  Deja_attaque, // <- cette unité a déjà attaqué
  Unite_interdite, // <- cette unité ne peut pas être amenée en renfort
  Renfort_impossible, // <- une unité est déjà présente sur le spawn
  Pas_a_moi, // <- l'unité ciblée n'est pas à vous
  Plus_de_cartes, // <- il n'y a plus de cartes du type spécifié dans votre main
  Phase_cartes_terminee, // <- vous ne pouvez plus poser de cartes car vous avez fait une action
}

// Le type d'une unité sur le terrain
enum Type_unite{
  Perroquet, // <- 1 PA et portée de 1
  Singe, // <- 2 PA et portée de 3
  Chat, // <- 5 PA et portée de 1
  Kangourou, // <- 3 PA et portée de 2 (explose !)
}

public class Interface
{
// Taille de départ du terrain.
  public static final int TAILLE_DEPART = 25;

// Le nombre maximal d'unités pouvant appartenir à une équipe.
  public static final int NBR_MAX_UNITES = 10;

// Le temps, en nombre de tours, entre deux rétrécissement du terrain.
  public static final int TEMPS_RETRECISSEMENT = 5;

// Le nombre maximum en jeu de chaque carte.
  public static final int MAX_CARTES = 2;

  // Renvoie le nombre de points de commandements.
  public static native int nombre_pc();

  // Renvoie le nombre d'unités en jeu.
  public static native int nombre_unites(boolean ennemi);

  // Renvoie le numéro du tour actuel.
  public static native int tour_actuel();

  // Renvoie la position du spawn (ennemi ou non).
  public static native Position pos_renfort(boolean ennemi);

  // Renvoie les caractéristiques d'un type d'unité.
  public static native Caracs caracteristiques(Type_unite tu);

  // Retourne une structure "cartes" contenant les informations sur les cartes que vous avez en main.
  public static native Cartes mes_cartes();

  // Retourne la liste des unités actuellement en jeu.
  public static native Unite[] unites();

  // Retourne la taille actuelle du terrain et les coordonnées min/max dans une structure "taille_terrain".
  public static native Taille_terrain taille_terrain_actuelle();

  // Utilise une carte « Potion magique » que vous avez dans votre main.
  public static native Erreur potion(Position cible);

  // Utilise une carte « Déguisement » que vous avez dans votre main.
  public static native Erreur deguisement(Position cible, Type_unite nouveau_type);

  // Utilise une carte « Banzaï » que vous avez dans votre main.
  public static native Erreur banzai(Position cible);

  // Utilise une carte « Pacifisme » que vous avez dans votre main.
  public static native Erreur pacifisme();

  // Déplace une unité vers une position à portée.
  public static native Erreur deplacer(Position cible, Position pos);

  // Relève une unité n'ayant plus de marqueurs de KO.
  public static native Erreur relever(Position cible);

  // Attaque une autre unité.
  public static native Erreur attaquer(Position attaquant, Position cible);

  // Fait apparaitre une unité sur la case de spawn.
  public static native Erreur renfort(Type_unite quoi);

  // Annule l'effet de la dernière action et remet le jeu dans l'état précédent. Renvoie false s'il n'y a rien à annuler, true sinon.
  public static native boolean annuler();

  // Affiche le contenu d'une valeur de type erreur
  public static native void afficher_erreur(Erreur v);

  // Affiche le contenu d'une valeur de type type_unite
  public static native void afficher_type_unite(Type_unite v);

  // Affiche le contenu d'une valeur de type position
  public static native void afficher_position(Position v);

  // Affiche le contenu d'une valeur de type taille_terrain
  public static native void afficher_taille_terrain(Taille_terrain v);

  // Affiche le contenu d'une valeur de type caracs
  public static native void afficher_caracs(Caracs v);

  // Affiche le contenu d'une valeur de type unite
  public static native void afficher_unite(Unite v);

  // Affiche le contenu d'une valeur de type cartes
  public static native void afficher_cartes(Cartes v);

}
