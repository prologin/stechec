// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_java.rb

// Repr�sente une position sur le terrain du jeu.
class Position {
  public int x; // coordonn�e X
  public int y; // coordonn�e Y
}

// Contient les informations sur la taille du terrain du jeu.
class Taille_terrain {
  public int taille; // taille actuelle du terrain
  public int min_coord; // coordonn�e minimale en X ou en Y
  public int max_coord; // coordonn�e maximale en X ou en Y
}

// Donne les caract�ristiques d'un type d'unit�.
class Caracs {
  public int pa_init; // nombre de points d'actions par tour
  public int portee; // port�e maximale de l'unit�
}

// Repr�sente une unit� sur le terrain.
class Unite {
  public Position pos = new Position(); // la position de l'unit�
  public boolean ennemi; // vrai si l'unit� appartient � l'ennemi
  public Type_unite type_unite_actuel = Type_unite.Perroquet; // le type de l'unit�, qui change si l'unit� est d�guis�e
  public Type_unite vrai_type_unite = Type_unite.Perroquet; // le vrai type de l'unit� (qui ne change pas lors du d�guisement)
  public int ko; // une valeur n�gative si l'unit� n'est pas KO, sinon le nombre de marqueurs KO sur l'unit�
  public int pa; // le nombre de PA restants � l'unit�
  public int attaques; // le nombre d'attaques restants � l'unit�
  public int attaques_gratuites; // le nombre d'attaques gratuites (voir la partie banzai)
  public int id; // l'identifiant unique de l'unit�
}

// Repr�sente l'ensemble des cartes que vous pouvez utiliser.
class Cartes {
  public int potion; // le nombre de cartes � Potion magique �
  public int deguisement; // le nombre de cartes � D�guisement �
  public int banzai; // le nombre de cartes � Banza� �
  public int pacifisme; // le nombre de cartes � Pacifisme �
}

// �num�ration repr�sentant une erreur renvoy�e par une des fonctions d'action.
enum Erreur{
  Ok, // <- aucune erreur n'est survenue
  Position_invalide, // <- la position sp�cifi�e est invalide
  Case_occupee, // <- la case sur laquelle vous tentez de vous d�placer est occup�e
  Pas_a_portee, // <- la cible n'est pas � port�e
  Unite_ko, // <- l'unit� que vous essayez de faire agir ou sur laquelle vous essayez d'agir  est KO
  Unite_debout, // <- l'unit� que vous essayez de relever est d�j� debout
  Quota_depasse, // <- nombre maximal d'unites, de spawn ou de relevages par tour d�pass�
  Plus_de_pa, // <- cette unit� a r�alis� toutes ses actions
  Deja_attaque, // <- cette unit� a d�j� attaqu�
  Unite_interdite, // <- cette unit� ne peut pas �tre amen�e en renfort
  Renfort_impossible, // <- une unit� est d�j� pr�sente sur le spawn
  Pas_a_moi, // <- l'unit� cibl�e n'est pas � vous
  Plus_de_cartes, // <- il n'y a plus de cartes du type sp�cifi� dans votre main
  Phase_cartes_terminee, // <- vous ne pouvez plus poser de cartes car vous avez fait une action
}

// Le type d'une unit� sur le terrain
enum Type_unite{
  Perroquet, // <- 1 PA et port�e de 1
  Singe, // <- 2 PA et port�e de 3
  Chat, // <- 5 PA et port�e de 1
  Kangourou, // <- 3 PA et port�e de 2 (explose !)
}

public class Interface
{
// Taille de d�part du terrain.
  public static final int TAILLE_DEPART = 25;

// Le nombre maximal d'unit�s pouvant appartenir � une �quipe.
  public static final int NBR_MAX_UNITES = 10;

// Le temps, en nombre de tours, entre deux r�tr�cissement du terrain.
  public static final int TEMPS_RETRECISSEMENT = 5;

// Le nombre maximum en jeu de chaque carte.
  public static final int MAX_CARTES = 2;

  // Renvoie le nombre de points de commandements.
  public static native int nombre_pc();

  // Renvoie le nombre d'unit�s en jeu.
  public static native int nombre_unites(boolean ennemi);

  // Renvoie le num�ro du tour actuel.
  public static native int tour_actuel();

  // Renvoie la position du spawn (ennemi ou non).
  public static native Position pos_renfort(boolean ennemi);

  // Renvoie les caract�ristiques d'un type d'unit�.
  public static native Caracs caracteristiques(Type_unite tu);

  // Retourne une structure "cartes" contenant les informations sur les cartes que vous avez en main.
  public static native Cartes mes_cartes();

  // Retourne la liste des unit�s actuellement en jeu.
  public static native Unite[] unites();

  // Retourne la taille actuelle du terrain et les coordonn�es min/max dans une structure "taille_terrain".
  public static native Taille_terrain taille_terrain_actuelle();

  // Utilise une carte � Potion magique � que vous avez dans votre main.
  public static native Erreur potion(Position cible);

  // Utilise une carte � D�guisement � que vous avez dans votre main.
  public static native Erreur deguisement(Position cible, Type_unite nouveau_type);

  // Utilise une carte � Banza� � que vous avez dans votre main.
  public static native Erreur banzai(Position cible);

  // Utilise une carte � Pacifisme � que vous avez dans votre main.
  public static native Erreur pacifisme();

  // D�place une unit� vers une position � port�e.
  public static native Erreur deplacer(Position cible, Position pos);

  // Rel�ve une unit� n'ayant plus de marqueurs de KO.
  public static native Erreur relever(Position cible);

  // Attaque une autre unit�.
  public static native Erreur attaquer(Position attaquant, Position cible);

  // Fait apparaitre une unit� sur la case de spawn.
  public static native Erreur renfort(Type_unite quoi);

  // Annule l'effet de la derni�re action et remet le jeu dans l'�tat pr�c�dent. Renvoie false s'il n'y a rien � annuler, true sinon.
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
