// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_cs.rb

using System.Runtime.CompilerServices;

namespace Prologin {
// Énumération représentant une erreur renvoyée par une des fonctions d'action.
	public enum Erreur {
		OK, // <- aucune erreur n'est survenue
		POSITION_INVALIDE, // <- la position spécifiée est invalide
		CASE_OCCUPEE, // <- la case sur laquelle vous tentez de vous déplacer est occupée
		PAS_A_PORTEE, // <- la cible n'est pas à portée
		UNITE_KO, // <- l'unité que vous essayez de faire agir ou sur laquelle vous essayez d'agir  est KO
		UNITE_DEBOUT, // <- l'unité que vous essayez de relever est déjà debout
		QUOTA_DEPASSE, // <- nombre maximal d'unites, de spawn ou de relevages par tour dépassé
		PLUS_DE_PA, // <- cette unité a réalisé toutes ses actions
		DEJA_ATTAQUE, // <- cette unité a déjà attaqué
		UNITE_INTERDITE, // <- cette unité ne peut pas être amenée en renfort
		RENFORT_IMPOSSIBLE, // <- une unité est déjà présente sur le spawn
		PAS_A_MOI, // <- l'unité ciblée n'est pas à vous
		PLUS_DE_CARTES, // <- il n'y a plus de cartes du type spécifié dans votre main
		PHASE_CARTES_TERMINEE, // <- vous ne pouvez plus poser de cartes car vous avez fait une action
	}

// Le type d'une unité sur le terrain
	public enum TypeUnite {
		PERROQUET, // <- 1 PA et portée de 1
		SINGE, // <- 2 PA et portée de 3
		CHAT, // <- 5 PA et portée de 1
		KANGOUROU, // <- 3 PA et portée de 2 (explose !)
	}

// Représente une position sur le terrain du jeu.
	class Position {
		public Position() {
		}
		public int X; // <- coordonnée X
		public int Y; // <- coordonnée Y
	}

// Contient les informations sur la taille du terrain du jeu.
	class TailleTerrain {
		public TailleTerrain() {
		}
		public int Taille; // <- taille actuelle du terrain
		public int MinCoord; // <- coordonnée minimale en X ou en Y
		public int MaxCoord; // <- coordonnée maximale en X ou en Y
	}

// Donne les caractéristiques d'un type d'unité.
	class Caracs {
		public Caracs() {
		}
		public int PaInit; // <- nombre de points d'actions par tour
		public int Portee; // <- portée maximale de l'unité
	}

// Représente une unité sur le terrain.
	class Unite {
		public Unite() {
			Pos = new Position();
		}
		public Position Pos; // <- la position de l'unité
		public bool Ennemi; // <- vrai si l'unité appartient à l'ennemi
		public TypeUnite TypeUniteActuel; // <- le type de l'unité, qui change si l'unité est déguisée
		public TypeUnite VraiTypeUnite; // <- le vrai type de l'unité (qui ne change pas lors du déguisement)
		public int Ko; // <- une valeur négative si l'unité n'est pas KO, sinon le nombre de marqueurs KO sur l'unité
		public int Pa; // <- le nombre de PA restants à l'unité
		public int Attaques; // <- le nombre d'attaques restants à l'unité
		public int AttaquesGratuites; // <- le nombre d'attaques gratuites (voir la partie banzai)
		public int Id; // <- l'identifiant unique de l'unité
	}

// Représente l'ensemble des cartes que vous pouvez utiliser.
	class Cartes {
		public Cartes() {
		}
		public int Potion; // <- le nombre de cartes « Potion magique »
		public int Deguisement; // <- le nombre de cartes « Déguisement »
		public int Banzai; // <- le nombre de cartes « Banzaï »
		public int Pacifisme; // <- le nombre de cartes « Pacifisme »
	}

	class Api {
// Taille de départ du terrain.
		public const int TAILLE_DEPART = 19;

// Le nombre maximal d'unités pouvant appartenir à une équipe.
		public const int NBR_MAX_UNITES = 10;

// Le temps, en nombre de tours, entre deux rétrécissement du terrain.
		public const int TEMPS_RETRECISSEMENT = 5;

// Le nombre maximum en jeu de chaque carte.
		public const int MAX_CARTES = 2;

// Renvoie le nombre de points de commandements.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern int NombrePc();

// Renvoie le nombre d'unités en jeu.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern int NombreUnites(bool ennemi);

// Renvoie le numéro du tour actuel.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern int TourActuel();

// Renvoie la position du spawn (ennemi ou non).
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Position PosRenfort(bool ennemi);

// Renvoie les caractéristiques d'un type d'unité.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Caracs Caracteristiques(TypeUnite tu);

// Retourne une structure "cartes" contenant les informations sur les cartes que vous avez en main.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Cartes MesCartes();

// Retourne la liste des unités actuellement en jeu.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Unite[] Unites();

// Retourne la taille actuelle du terrain et les coordonnées min/max dans une structure "taille_terrain".
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern TailleTerrain TailleTerrainActuelle();

// Utilise une carte « Potion magique » que vous avez dans votre main.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur Potion(Position cible);

// Utilise une carte « Déguisement » que vous avez dans votre main.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur Deguisement(Position cible, TypeUnite nouveau_type);

// Utilise une carte « Banzaï » que vous avez dans votre main.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur Banzai(Position cible);

// Utilise une carte « Pacifisme » que vous avez dans votre main.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur Pacifisme();

// Déplace une unité vers une position à portée.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur Deplacer(Position cible, Position pos);

// Relève une unité n'ayant plus de marqueurs de KO.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur Relever(Position cible);

// Attaque une autre unité.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur Attaquer(Position attaquant, Position cible);

// Fait apparaitre une unité sur la case de spawn.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur Renfort(TypeUnite quoi);

// Annule l'effet de la dernière action et remet le jeu dans l'état précédent. Renvoie false s'il n'y a rien à annuler, true sinon.
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
