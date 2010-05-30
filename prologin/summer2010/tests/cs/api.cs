// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_cs.rb

using System.Runtime.CompilerServices;

namespace Prologin {
// Énumération représentant une erreur renvoyée par une des fonctions d'action.
	public enum Erreur {
		OK, // <- aucune erreur n'est survenue
		POSITION_INVALIDE, // <- la position spécifiée est invalide
		UTILISATION_IMPOSSIBLE, // <- vous ne pouvez pas utiliser cet objet
		PLUS_D_ARGENT, // <- vous n'avez pas assez d'argent pour acheter l'objet en question
	}

// Le type d'un objet
	public enum TypeObjet {
		FILET, // <- permet d'attraper des pieces ou d'immobiliser des unités
		MARTEAU, // <- permet de frapper des unités.
		RIEN, // <- l'unite ne porte aucun objet
	}

// Représente une position sur le terrain du jeu.
	class Position {
		public Position() {
		}
		public int X; // <- coordonnée X
		public int Y; // <- coordonnée Y
	}

// les caracteristiques d'un objet
	class CaracteristiquesObjet {
		public CaracteristiquesObjet() {
		}
		public int Coute; // <- ce que coute l'objet
		public int Porte; // <- la porte de l'objet
	}

// Représente une unité sur le terrain.
	class Unite {
		public Unite() {
			PosUnite = new Position();
		}
		public Position PosUnite; // <- la position de l'unité
		public int Team; // <- donne l'equipe du joueur
		public int Ko; // <- renvoie le temps que l'unite passera sans pouvoir bouger.
		public int Pa; // <- le nombre de PA restants à l'unité
		public TypeObjet Objet; // <- renvoie l'objet que porte l'unite.
		public int Id; // <- l'identifiant unique de l'unité
	}

// Représente le type d'une piece.
	class Piece {
		public Piece() {
			PosPiece = new Position();
		}
		public int Valeur; // <- la valeur de la piece.
		public Position PosPiece; // <- renvoie la position de la piece
		public int TourApparition; // <- renvoie le tour ou la piece apparaitra ou est apparu.
	}

	class Api {
// Taille du terrain.
		public const int TAILLE_TERRAIN = 21;

// le nombre de points d'actions des unites au debut du tour.
		public const int MAX_PA = 3;

// Le nombre d'unités par équipe.
		public const int NBR_UNITES = 9;

// Le nombre maximum de tours.
		public const int MAX_TURN = 100;

// Renvoie le numero de votre equipe.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern int MonEquipe();

// Renvoie le score d'une equipe.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern int Score(int team);

// Renvoie le nombre d'equipes sur la map
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern int NombreEquipes();

// Renvoie le numéro du tour actuel.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern int TourActuel();

// Renvoie les pieces qui sont sur la map
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Piece[] PiecesEnJeu();

// Renvoie le 10 prochaines pieces a vennir
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Piece[] PiecesAVennir();

// Retourne la liste des unités actuellement en jeu.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Unite[] Unites();

// Déplace une unité vers une position à portée.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur Deplacer(Position cible, Position pos);

// Achete un objet
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur AcheterObjet(Position cible, TypeObjet objet);

// utilise un objet
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur Utiliser(Position attaquant, Position cible);

// coupe la partie, cette action coute 100 d'or et met fin a la partie.
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern Erreur AcheverLaPartie();

// Affiche le contenu d'une valeur de type erreur
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern void AfficherErreur(Erreur v);

// Affiche le contenu d'une valeur de type type_objet
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern void AfficherTypeObjet(TypeObjet v);

// Affiche le contenu d'une valeur de type position
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern void AfficherPosition(Position v);

// Affiche le contenu d'une valeur de type caracteristiques_objet
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern void AfficherCaracteristiquesObjet(CaracteristiquesObjet v);

// Affiche le contenu d'une valeur de type unite
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern void AfficherUnite(Unite v);

// Affiche le contenu d'une valeur de type piece
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern void AfficherPiece(Piece v);

	}
}
