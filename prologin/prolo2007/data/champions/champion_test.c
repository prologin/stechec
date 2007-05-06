/*!
** This file has been generated, if you wish to
** modify it in a permanent way, please refer
** to the script file : gen/generator_c.rb
*/

#include "prologin.h"

/*!
** Fonction appele au debut de la partie
*/
void init_game(void)
{
  /* fonction a completer */
  printf("HELLLLLLLLLLLLLLOOOOO\n");
  printf("Trying to define competence : %d\n", defini_competence(0, 0, 2, 8));
  printf("HELLLLLLLLLLLLLLOOOOO\n");
  // printf("Trying to define competence : %d\n", defini_competence(0, 0, 0, 0));
}

/*!
** Fonction appele a chaque tour
*/
void play_turn(void)
{
//  emet_anticorps ();
  transmettre_message(mon_identifiant (), 0, 1, 0);
  transmettre_message(mon_identifiant (), 0, 2, 0);
  transmettre_message(mon_identifiant (), 0, 3, 0);
  int msg_id;
  int i;
  while ((msg_id = recevoir_prochain_message()) != UNKNOWN)
    {
      printf("View message : %d, %d, %d, %d\n", corps_message(msg_id, 0),
	     corps_message(msg_id, 1),
	     corps_message(msg_id, 2),
	     corps_message(msg_id, 3));
    }
  printf("Trying to define competence : %d\n", defini_competence(0, 0, 0, 0));
  printf("Another turn<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
  printf("Nb Equipe : %d", equipes ());
  printf("Equipe : %d\n", mon_equipe ());
  for (i = 0; i < equipes (); ++i)
    if (i == mon_equipe ())
      printf("Taille de mon équipe : %d\n", taille_equipe(i));
    else
      printf("Taille de l'equipe : %d :  %d\n", i, taille_equipe(i));
  int j;
  printf ("\n");
  for (i = 0; i < taille_corps_y (); ++i)
    {
      for (j = 0; j < taille_corps_x (); ++j)
	if (visible(j, i))
	  {
	    switch(regarde(j, i))
	      {
	      case VIRUS:
		{
		  printf("V");
		  break;
		}
	      case WHITE_CELL:
		{
		  printf("O");
		  break;
		}
	      case CELL:
		{
		  printf("C");
		  break;
		}
	      default :
		{
		  if (bacteries_presentes(j, i))
		    printf ("B");
		  else
		    printf(".");
		}
	      }
	  }
	else
	  printf("X");
      printf("\n");
    }
  int etat;
  for (i = 0; i < taille_corps_y (); ++i)
    {
      for (j = 0; j < taille_corps_x (); ++j)
	if ((etat = etat_cellule (j, i)) >= 0)
	  {
	    switch(etat)
	      {
	      case CELL_STATE_INFECTED:
		{
		  printf ("cellule [%d, %d] : infected\n", i, j);
		  break;
		}
	      default:
		{
		  printf ("cellule [%d, %d] : no problem :)\n", i, j);
		  break;
		}
	      }
	  }
    }
  deplace_leucocyte(position_leucocyte_x(mon_identifiant ()),
		    position_leucocyte_y(mon_identifiant ()) - 1);
  printf("Id : %d, x : %d, y : %d\n", mon_identifiant (),
	 position_leucocyte_x(mon_identifiant ()),
	 position_leucocyte_y(mon_identifiant ()));
  /* fonction a completer */
}

