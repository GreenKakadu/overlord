/***************************************************************************
                          OrderElementsNode.cpp
                    .
                             -------------------
    begin                : Thu Nov  7 11:46:00 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "OrderElementNode.h"
OrderElementNode::OrderElementNode(OrderElement * order)
{
  //  cout << "New OrderElementNode created" << endl;  
 order_ = order;
 next_ = 0;
 prev_ = 0;
 order_ -> load();
    cout << "New OrderElementNode created and loaded" << endl;  
}

OrderElementNode::~OrderElementNode()
{
  
 delete order_; // ?
 if(prev_)
   prev_ -> setNext(next_);
 next_ -> setPrev(prev_);
}

OrderElement * OrderElementNode::getOrderElement()
{
  return order_;
}

void OrderElementNode::setNext(OrderElementNode * next)
{
  next_ = next;
}

void OrderElementNode::setPrev(OrderElementNode * prev)
{
  prev_ = prev;
}

OrderElementNode * OrderElementNode::next()
{
  return next_;
}

OrderElementNode * OrderElementNode::prev()
{
  return prev_;
}

