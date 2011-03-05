#include <stdio.h>
#include <SDL.h>
#include "graphics.h"
#include "camera.h"
#include "constants.h"
#include "prologin.h"
#include "orders.h"
#include "assert.h"


static const char *directions_str[] = {
  "BAS",
  "GAUCHE",
  "HAUT",
  "DROITE",
  "ICI" };

static const char *orders_str[] =
  { "attendre",
    "lacher_pomme",
    "deplacement",
    "ramasser_pomme",
    "grappin",
    "",
    "turbo",
    "trognon" };


int SendOrders(struct AwaitingOrders* orders) {
  assert(orders != NULL);
  int no = orders->next_order;
  assert( no >= 0 && no <= MAX_TOTAL_ORDERS);
  int ok = 1;
  int i;
  for (i = 0 ; i < no ; i++) {
    int r = SendOneOrder(&orders->orders[i]);
    if (r < 0) ok = r;
  }
  return ok;
}


int SendOneOrder(struct Order* ord) {
  assert(ord != NULL);
  printf("The following order is being sent to the server:\n");
  DisplayOrder(ord);
  int ret = 0;
  int type = ord->order_id;
  assert(type >= ACTION_WAIT &&
	 type <= ACTION_LAUNCH_BULLET);
  int dir = ord->dir;
  int id = ord->robot_id;
  assert(id >= 0 && id < MAX_ROBOTS/2);

  int types_with_directions[] = {ACTION_DROP_BALL,
				 ACTION_MOVE,
				 ACTION_HOOK_ROBOT,
				 ACTION_LAUNCH_BULLET};
  int sz = sizeof(types_with_directions) / sizeof(int);
  int k;
  for (k=0 ; k < sz ; ++k) {
    if (type == types_with_directions[k]) {
      assert(dir >= HAUT && dir <= ICI);
      break;
    }
  }

  switch(type) {
  case ACTION_WAIT:
    ret=attendre(id);
    break;
  case ACTION_DROP_BALL:
    ret=lacher_pomme(id, dir);
    break;
  case ACTION_MOVE:
    ret=deplacer(id, dir);
    break;
  case ACTION_PICK_UP_BALL:
    ret=ramasser_pomme(id);
    break;
  case ACTION_HOOK_ROBOT:
    ret=grappin(id, dir);
    break;
  case ACTION_RELEASE_HOOK:
    assert(0);
    break;
  case ACTION_BOOST_TURBO:
    ret=turbo(id);
    break;
  case ACTION_LAUNCH_BULLET:
    ret=trognon(id, dir);
    break;
  }
  return ret;
}

void DisplayCurrentOrders(struct AwaitingOrders *aw) {
  assert(aw != NULL);
  printf("\n************ Ordres en cours de l'interface: ************\n");
  printf("Rq: cette liste ne presupose pas de l'ordes d'execution des ordres sur le serveur\n"
	 "Voir le sujet ou le wiki pour cela\n");
  int i;
  for (i = 0 ; i < aw ->next_order ; i++) {
    struct Order *o = &aw->orders[i];
    printf("Ordre #%d: ", i+1);
    printf("hamster_id = %d, ordre = %s",
	   o->robot_id + MAX_ROBOTS / 2, orders_str[o->order_id]);
    int orders_with_dir[] = {0,1,1,0,1,0,0,0,0,0};
    if (orders_with_dir[o->order_id])
      printf(", direction: %s\n", directions_str[o->dir]);
    else printf("\n");
  }
  printf("\n");
  printf("\n");
}

void DisplayOrder(struct Order *o) {
  assert(o != NULL);
  printf("hamster_id = %d, ordre = %s, direction = %s\n",
	 o->robot_id + MAX_ROBOTS / 2, orders_str[o->order_id], directions_str[o->dir]);
}
