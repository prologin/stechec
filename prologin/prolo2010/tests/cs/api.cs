// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_cs.rb

using System.Runtime.CompilerServices;

namespace Prologin {
// �num�ration repr�sentant une erreur renvoy�e par une des fonctions d'action.
	public enum Erreur {
		OK, // <- aucune erreur n'est survenue
		POSITION_INVALIDE, // <- la position sp�cifi�e est invalide
		CASE_OCCUPEE, // <- la case sur laquelle vous tentez de vous d�placer est occup�e
		PAS_A_PORTEE, // <- la cible n'est pas � port�e
		UNITE_KO, // <- l'unit� que vous essayez de faire agir ou sur laquelle vous essayez d'agir  est KO
		UNITE_DEBOUT, // <- l'unit� que vous essayez de relever est d�j� debout
		QUOTA_DEPASSE, // <- nombre maximal d'unites, de spawn ou de relevages par tour d�pass�
		PLUS_DE_PA, // <- cette unit� a r�alis� toutes ses actions
		DEJA_ATTAQUE, // <- cette unit� a d�j� attaqu�
		UNITE_INTERDITE, // <- cette unit� ne peut pas �tre amen�e en renfort
		RENFORT_IMPOSSIBLE, // <- une unit� est d�j� pr�sente sur le spawn
		PAS_A_MOI, // <- l'unit� cibl�e n'est pas � vous
		PLUS_DE_CARTES, // <- il n'y a plus de cartes du type sp�cifi� dans votre main
		PHASE_CARTES_TERMINEE, // <- vous ne pouvez plus poser de cartes car vous avez fait une action
	}

// Le type d'une unit� sur le terrain
	public enum TypeUnite {
		PERROQUET, // <- 1 PA et port�e de 1
		SINGE, // <- 2 PA et port�e de 3
		CHAT, // <- 5 PA et port�e de 1
		KANGOUROU, // <- 3 PA et port�e de 2 (explose !)
	}

// Repr�sente une position sur le terrain du jeu.
	class Position {
		public Position() {
		}
		public int X; // <- coordonn�e X
		public int Y; // <- coordonn�e Y
	}

// Contient les informations sur la taille du terrain du jeu.
	class TailleTerrain {
		public TailleTerrain() {
		}
		public int Taille; // <- taille actuelle du terrain
		public int MinCoord; // <- coordonn�e minimale en X ou en Y
		public int MaxCoord; // <- coordonn�e maximale en X ou en Y
	}

// Donne les caract�ristiques d'un type d'unit�.
	class Caracs {
		public Caracs() {
		}
		public int PaInit; // <- nombre de points d'actions par tour
		public int Portee; // <- port�e maximale de l'unit�
	}

// Repr�sente une unit� sur le terrain.
	class Unite {
		public Unite() {
			Pos = new Position();
		}
		public Position Pos; // <- la position de l'unit�
		public bool Ennemi; // <- vrai si l'unit� appartient � l'ennemi
		public TypeUnite TypeUniteActuel; // <- le type de l'unit�, qui change si l'unit� est d�guis�e
		public TypeUnite VraiTypeUnite; // <- le vrai type de l'unit� (qui ne change pas lors du d�guisement)
		public int Ko; // <- une valeur n�gative si l'unit� n'est pas KO, sinon le nombre de marqueurs KO sur l'unit�
		public int Pa; // <- le nombre de PA restants � l'unit�
		public int Attaques; // <- le nombre d'attaques restants � l'unit�
		public int AttaquesGratuites; // <- le nombre d'attaques gratuites (voir la partie banzai)
		public int Id; // <- l'identifiant unique de l'unit�
	}

// Repr�sente l'ensemble des cartes que vous pouvez utiliser.
	class Cartes {
		public Cartes() {
		}
		public int Potion; // <- le nombre de cartes � Potion magique �
		public int Deguisement; // <- le nombre de cartes � D�guisement �
		public int Banzai; // <- le nombre de cartes � Banza� �
		public int Pacifisme; // <- le nombre de cartes � Pacifisme �
	}

	class Api {
// Taille de d�part du terrain.
		public const int TAILLE_DEPART = 19;

// Le nombre maximal d'unit�s pouvant appartenir � une �quipe.
		public const int NBR_MAX_UNITES = 10;

// Le temps, en nombre de tours, entre deux r�tr�cissement du terrain.
		public const int TEMPS_RETRECISSEMENT = 5;

// Le nombre maximum en jeu de chaque carte.
		public const int MAX_CARTES = 2;

// Renvoie le nombre de points de commandements.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern int NombrePc();

// Renvoie le nombre d'unit�s en jeu.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern int NombreUnites(bool ennemi);

// Renvoie le num�ro du tour actuel.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern int TourActuel();

// Renvoie la position du spawn (ennemi ou non).
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Position PosRenfort(bool ennemi);

// Renvoie les caract�ristiques d'un type d'unit�.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Caracs Caracteristiques(TypeUnite tu);

// Retourne une structure "cartes" contenant les informations sur les cartes que vous avez en main.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Cartes MesCartes();

// Retourne la liste des unit�s actuellement en jeu.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Unite[] Unites();

// Retourne la taille actuelle du terrain et les coordonn�es min/max dans une structure "taille_terrain".
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern TailleTerrain TailleTerrainActuelle();

// Utilise une carte � Potion magique � que vous avez dans votre main.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur Potion(Position cible);

// Utilise une carte � D�guisement � que vous avez dans votre main.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur Deguisement(Position cible, TypeUnite nouveau_type);

// Utilise une carte � Banza� � que vous avez dans votre main.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur Banzai(Position cible);

// Utilise une carte � Pacifisme � que vous avez dans votre main.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur Pacifisme();

// D�place une unit� vers une position � port�e.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur Deplacer(Position cible, Position pos);

// Rel�ve une unit� n'ayant plus de marqueurs de KO.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur Relever(Position cible);

// Attaque une autre unit�.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur Attaquer(Position attaquant, Position cible);

// Fait apparaitre une unit� sur la case de spawn.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur Renfort(TypeUnite quoi);

// Annule l'effet de la derni�re action et remet le jeu dans l'�tat pr�c�dent. Renvoie false s'il n'y a rien � annuler, true sinon.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern bool Annuler();

// Affiche le contenu d'une valeur de type erreur
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern void AfficherErreur(Erreur v);

// Affiche le contenu d'une valeur de type type_unite
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern void AfficherTypeUnite(TypeUnite v);

// Affiche le contenu d'une valeur de type position
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern void AfficherPosition(Position v);

// Affiche le contenu d'une valeur de type taille_terrain
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern void AfficherTailleTerrain(TailleTerrain v);

// Affiche le contenu d'une valeur de type caracs
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern void AfficherCaracs(Caracs v);

// Affiche le contenu d'une valeur de type unite
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern void AfficherUnite(Unite v);

// Affiche le contenu d'une valeur de type cartes
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern void AfficherCartes(Cartes v);

	}
}
