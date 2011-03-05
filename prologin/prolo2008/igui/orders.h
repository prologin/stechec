#ifndef DEF_ORDERS_H
#define DEF_ORDERS_H


struct Order {
  int robot_id;
  int order_id; //see constant.h
  int dir;
};

struct AwaitingOrders {

  int next_order;
  struct Order orders[MAX_TOTAL_ORDERS];

};

// Use that to send all the orders to the api, like a real champion
// the orders given (of index i, 0 <= i < orders->next_order) will be sent and 
// must be valid.
int SendOrders(struct AwaitingOrders* orders);

void DisplayCurrentOrders(struct AwaitingOrders *aw);

// internal use
int SendOneOrder(struct Order* ord);

#endif
