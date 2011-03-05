/*!
** This file has been generated, if you wish to
** modify it in a permanent way, please refer
** to the script file : gen/generator_c.rb
*/

#include <stdio.h>
#include "prologin.h"

/*!
** Fonction appele au debut de la partie
*/
void init_game(void)
{
  /* fonction a completer */
   printf("HELLLLLLLLLLLLLLOOOOO\n");
   printf("Trying to define competence : %d\n", defini_competence(0, 0, 2, 2));
   printf("HELLLLLLLLLLLLLLOOOOO\n");
   printf("Trying to define competence : %d\n", defini_competence(0, 0, 0, 0));
}

/*!
** Fonction appele a chaque tour
*/
void play_turn(void)
{
   transmettre_message(mon_identifiant (), 0, 1, 0);
   transmettre_message(mon_identifiant (), 0, 2, 0);
   transmettre_message(mon_identifiant (), 0, 3, 0);
   int msg_id;
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
   printf("Equipe : %d", mon_equipe ());
   int i;
   int j;
   printf ("\n");
   for (i = 0; i < taille_corps_y (); ++i)
   {
      for (j = 0; j < taille_corps_x (); ++j)
	 if (visible(j, i))
	    printf(".");
	 else
	    printf("X");
      printf("\n");
   }
   deplace_leucocyte(position_leucocyte_x(mon_identifiant ()),
					  position_leucocyte_y(mon_identifiant ()) - 1);
   printf("Id : %d, x : %d, y : %d\n", mon_identifiant (),
	  position_leucocyte_x(mon_identifiant ()),
	  position_leucocyte_y(mon_identifiant ()));
  /* fonction a completer */
}

