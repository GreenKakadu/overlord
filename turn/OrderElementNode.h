/***************************************************************************
                          OrderElementsNode.h
                    .
                             -------------------
    begin                : Thu Nov  7 11:46:00 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef ORDER_ELEMENT_NODE_H
#define ORDER_ELEMENT_NODE_H     
class OrderElement;
#include "OrderElement.h"


class OrderElementNode
{
    public:
  OrderElementNode(OrderElement * order);
  ~OrderElementNode();
  OrderElement * getOrderElement();
  void setNext(OrderElementNode * next);
  void setLast(OrderElementNode * last);
  OrderElementNode * next();
  OrderElementNode * last();
    protected:
  OrderElementNode * next_;
  OrderElementNode * last_;
  OrderElement * order_;
    private:
  
};
#endif






