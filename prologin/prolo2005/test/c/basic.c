/*!
** This file has been generated, if you wish to
** modify it in a permanent way, please refer
** to the script file : make_player/make_c.rb
*/

#include "prologin.h"
#include <stdio.h>
#include <stdlib.h>

/* struct info */
/* { */
/*   int destx, desty; */
/* }; */

/* info infos[MAX_COLEOPTERE]; */

void print_map()
{
  int i, j;
  for (j = 0; j < world_size_y(); j++)
    {
      for (i = 0; i < world_size_x(); i++)
	{
	  if (building_type(i, j) >= 0)
	    {
	      if (building_type(i, j) == 1)
		{
		  make_coleoptera(factory_id(i, j), 1);
		  printf("$");
		}
	      else
		printf("H");
	    }
	  else if (coleoptera_id(i, j) >= 0)
	    printf("X");
	  else switch (land_type(i, j))
	    {
	    case -1: printf("?"); break;
	    case 0: printf("."); break;
	    case 1: printf("~"); break;
	    case 2: printf("M"); break;
	    case 3: printf("P"); break;
	    }
	}
      printf("\n");
    }
}

void print_valeur()
{
  int i, j;
  for (j = 0; j < world_size_y(); j++)
    {
      for (i = 0; i < world_size_x(); i++)
	{
	  if (-1 == land_value(i, j))
	    printf("~");
	  else
	    {
	      if (PLASMA == land_type(i, j))
		printf("@");
	      else
		if (ORE == land_type(i, j))
		  printf("*");
		else
		  printf("%c", '0' + (land_value(i, j) / 10));
	    }
	}
      printf("\n");
    }
}


/*!
** Fonction de début
*/
void init_game(void)
{
  printf("init_game\n");
  printf("-----------------------------------------------------\n");
  print_map();
  print_valeur();
  printf("-----------------------------------------------------\n");
}

/*!
** Fonction appelée a chaque tour
*/
void play_turn(void)
{
  int i;
/* \tfonction a completer */
  //print_map();

    for (i = get_first_coleoptera(); i != -1; i = get_next_coleoptera(i))  
      {  
        printf("** moving %d **\n", i);  
        printf("position: %d, %d\n", coleoptera_x(i), coleoptera_y(i));  
        printf("state: %d\n", coleoptera_state(i));  
        printf("damage: %d\n", coleoptera_damage(i));  
        printf("energie: %d\n", coleoptera_plasma(i));  
        if (i == 0)  
  	throw_plasma(i, 4, INC_Y);  
        if (i % 10 == 1 && rand() % 4 == 0)  
        	make_building(i, 1);  
        //      else if (i % 5 == 2 && rand() % 4 == 0)  
        //      	make_building(i, 3);  
        else  
  	{  
  	  move_coleoptera(i, rand() % 4);  
  	}  
      }  
}
