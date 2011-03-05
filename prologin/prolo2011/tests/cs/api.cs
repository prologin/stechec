// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_cs.rb

using System.Runtime.CompilerServices;

namespace Prologin {
// �num�ration repr�sentant une erreur renvoy�e par une des fonctions d'action
	public enum Erreur {
		OK, // <- aucune erreur n'est survenue
		ID_INVALIDE, // <- identifiant invalide
		POSITION_INVALIDE, // <- la position sp�cifi�e est invalide
		PLUS_DE_PA, // <- vous n'avez pas assez de points d'action
		BONUS_INVALIDE, // <- vous n'avez pas ce bonus
		PAS_A_TOI, // <- l'unit� n'est pas a vous
	}

// �num�ration repr�sentant les diff�rents types de case
	public enum TypeCase {
		VIDE, // <- rien n'est pr�sent sur la case
		OBSTACLE, // <- cette case est inaccessible
		BONUS, // <- cette case cotient un bonus
		POINT_CROISEMENT, // <- point de croisement de tra�n�es
	}

// �num�ration repr�sentant les diff�rents types de bonii
	public enum Bonus {
		VIDE, // <- ceci n'est pas un bonus :-)
		BONUS_CROISEMENT, // <- bonus permettant de croiser deux tra�n�es de moto sur une case
		PLUS_LONG, // <- bonus permettant d'agrandir une tra�n�e de moto
		PLUS_PA, // <- bonus permettant d'avoir plus de points d'action
		BONUS_REGENERATION, // <- bonus permettant de regenerer une source d'energie
	}

// Repr�sente une position sur le terrain du jeu
	class Position {
		public Position() {
		}
		public int X; // <- coordonn�e en X
		public int Y; // <- coordonn�e en Y
	}

// Caracteristiques d'une source d'�nergie
	class SourceEnergie {
		public SourceEnergie() {
			Pos = new Position();
		}
		public int Id; // <- identifiant de la source d'�nergie
		public Position Pos; // <- position de la source d'�nergie
		public int Coef; // <- coefficient representant les points d'�nergie que la source va vous apporter
	}

// Repr�sente une tra�n�e de moto sur le terrain
	class TraineeMoto {
		public TraineeMoto() {
			Emplacement = new Position array();
		}
		public int Id; // <- identifiant de la tra�nee
		public Position[] Emplacement; // <- position de chaque composant de la tra�n�e de moto
		public int Team; // <- identifiant de l'�quipe qui poss�de cette tra�n�e de moto
	}

	class Api {
// Taille du terrain
		public const int TAILLE_TERRAIN = 100;

// Nombre de tours avant la fin de la partie
		public const int FIN_PARTIE = 1500;

// Nombre de points d'action par tour
		public const int MAX_PA = 3;

// Taille des tra�n�es de moto
		public const int TAILLE_TRAINEE = 900;

// Longueur maximale de l'allongement
		public const int MAX_ALLONGEMENT = 5;

// Retourne le num�ro de votre �quipe
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern int MonEquipe();

// Retourne les scores de chaque �quipe
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern int[] Scores();

// Retourne le nombre d'�quipes sur le terrain
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern int NombreEquipes();

// Retourne le num�ro du tour actuel
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern int TourActuel();

// Retourne la liste des sources d'�nergie
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern SourceEnergie[] SourcesEnergie();

// Retourne la liste des tra�n�es de moto
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern TraineeMoto[] TraineesMoto();

// Retourne le type d'une case
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern TypeCase RegarderTypeCase(Position pos);

// Retourne le type de bonus d'une case
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Bonus RegarderTypeBonus(Position pos);

// Retourne la liste des bonus d'une �quipe
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Bonus[] RegarderBonus(int equipe);

// D�place une moto
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur Deplacer(int id, Position de, Position vers);

// Coupe une tra�n�e de moto en deux nouvelles tra�n�es. � entre � et � et � doivent �tre deux positions adjacentes occup�es par une m�me tra�n�e de moto.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur CouperTraineeMoto(int id, Position entre, Position et);

// Annuler l'action pr�c�dente
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur Cancel();

// Enrouler la tra�n�e de moto en un point
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur Enrouler(int id, Position point);

// R�g�n�re une source d'�nergie � son maximal
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur RegenererSourceEnergie(int id);

// Allonge le tour en rajoutant des points d'action
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur AllongerPa();

// Allonge une tra�n�e de moto. L'allongement se fera aux prochains d�placements. La longueur du prolongement doit �tre comprise entre 0 et MAX_ALLONGEMENT (inclus).
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur AgrandirTraineeMoto(int id, int longueur);

// Pose un point de croisement sur une case du terrain. La case doit ne pas d�j� �tre un point de croisement.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur PoserPointCroisement(Position point);

// Fusionner deux tra�n�es de moto. Les deux doivent appartenir � la m�me �quipe, mais doivent �tre deux tra�n�es distinctes. � pos1 � et � pos2 � doivent �tre adjacentes et occup�es respectivement par � id1 � et � id2 �.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur Fusionner(int id1, Position pos1, int id2, Position pos2);

// Affiche le contenu d'une valeur de type erreur
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern void AfficherErreur(Erreur v);

// Affiche le contenu d'une valeur de type type_case
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern void AfficherTypeCase(TypeCase v);

// Affiche le contenu d'une valeur de type bonus
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern void AfficherBonus(Bonus v);

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
