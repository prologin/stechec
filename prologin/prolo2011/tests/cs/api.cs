// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_cs.rb

using System.Runtime.CompilerServices;

namespace Prologin {
// Énumération représentant une erreur renvoyée par une des fonctions d'action
	public enum Erreur {
		OK, // <- aucune erreur n'est survenue
		ID_INVALIDE, // <- identifiant invalide
		POSITION_INVALIDE, // <- la position spécifiée est invalide
		PLUS_DE_PA, // <- vous n'avez pas assez de points d'action
		BONUS_INVALIDE, // <- vous n'avez pas ce bonus
		PAS_A_TOI, // <- l'unité n'est pas a vous
	}

// Énumération représentant les différents types de case
	public enum TypeCase {
		VIDE, // <- rien n'est présent sur la case
		OBSTACLE, // <- cette case est inaccessible
		BONUS, // <- cette case cotient un bonus
		POINT_CROISEMENT, // <- point de croisement de traînées
	}

// Énumération représentant les différents types de bonii
	public enum TypeBonus {
		PAS_BONUS, // <- ceci n'est pas un bonus :-)
		BONUS_CROISEMENT, // <- bonus permettant de croiser deux traînées de moto sur une case
		PLUS_LONG, // <- bonus permettant d'agrandir une traînée de moto
		PLUS_PA, // <- bonus permettant d'avoir plus de points d'action
		BONUS_REGENERATION, // <- bonus permettant de regenerer une source d'energie
	}

// Représente une position sur le terrain du jeu
	class Position {
		public Position() {
		}
		public int X; // <- coordonnée en X
		public int Y; // <- coordonnée en Y
	}

// Caracteristiques d'une source d'énergie
	class SourceEnergie {
		public SourceEnergie() {
			Pos = new Position();
		}
		public int Id; // <- identifiant de la source d'énergie
		public Position Pos; // <- position de la source d'énergie
		public int Coef; // <- coefficient representant les points d'énergie que la source va vous apporter
	}

// Représente une traînée de moto sur le terrain
	class TraineeMoto {
		public TraineeMoto() {
			Emplacement = new Position array();
		}
		public int Id; // <- identifiant de la traînee
		public Position[] Emplacement; // <- position de chaque composant de la traînée de moto
		public int Team; // <- identifiant de l'équipe qui possède cette traînée de moto
	}

	class Api {
// Taille du terrain
		public const int TAILLE_TERRAIN = 100;

// Nombre de tours par partie
		public const int FIN_PARTIE = 1500;

// Nombre de points d'action par tour
		public const int MAX_PA = 3;

// Taille des traînées de moto
		public const int TAILLE_TRAINEE = 900;

// Longueur maximale de l'allongement
		public const int MAX_ALLONGEMENT = 5;

// Retourne le numéro de votre équipe
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern int MonEquipe();

// Retourne les scores de chaque équipe
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern int[] Scores();

// Retourne le nombre d'équipes sur le terrain
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern int NombreEquipes();

// Retourne le numéro du tour actuel
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern int TourActuel();

// Retourne la liste des sources d'énergie
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern SourceEnergie[] SourcesEnergie();

// Retourne la liste des traînées de moto
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern TraineeMoto[] TraineesMoto();

// Retourne le type d'une case
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern TypeCase RegarderTypeCase(Position pos);

// Retourne le type de bonus d'une case
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern TypeBonus RegarderTypeBonus(Position pos);

// Retourne la liste des bonus d'une équipe
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern TypeBonus[] RegarderBonus(int equipe);

// Déplace une moto
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur Deplacer(int id, Position de, Position vers);

// Coupe une traînée de moto en deux nouvelles traînées. « entre » et « et » doivent être deux positions adjacentes occupées par une même traînée de moto.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur CouperTraineeMoto(int id, Position entre, Position et);

// Annuler l'action précédente
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur Cancel();

// Enrouler la traînée de moto en un point
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur Enrouler(int id, Position point);

// Régénère une source d'énergie à son maximal
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur RegenererSourceEnergie(int id);

// Allonge le tour en rajoutant des points d'action
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur AllongerPa();

// Allonge une traînée de moto. L'allongement se fera aux prochains déplacements. La longueur du prolongement doit être comprise entre 0 et MAX_ALLONGEMENT (inclus).
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur AgrandirTraineeMoto(int id, int longueur);

// Pose un point de croisement sur une case du terrain. La case doit ne pas déjà être un point de croisement.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur PoserPointCroisement(Position point);

// Fusionner deux traînées de moto. Les deux doivent appartenir à la même équipe, mais doivent être deux traînées distinctes. « pos1 » et « pos2 » doivent être adjacentes et occupées respectivement par « id1 » et « id2 ».
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur Fusionner(int id1, Position pos1, int id2, Position pos2);

// Affiche le contenu d'une valeur de type erreur
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern void AfficherErreur(Erreur v);

// Affiche le contenu d'une valeur de type type_case
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern void AfficherTypeCase(TypeCase v);

// Affiche le contenu d'une valeur de type type_bonus
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern void AfficherTypeBonus(TypeBonus v);

// Affiche le contenu d'une valeur de type position
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern void AfficherPosition(Position v);

// Affiche le contenu d'une valeur de type source_energie
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern void AfficherSourceEnergie(SourceEnergie v);

// Affiche le contenu d'une valeur de type trainee_moto
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern void AfficherTraineeMoto(TraineeMoto v);

	}
}
