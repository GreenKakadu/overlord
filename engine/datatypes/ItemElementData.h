/***************************************************************************
                          ItemElementData.h 
                             -------------------
    begin                : Wed Mar 5 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef ITEM_ELEMENT_DATA_H
#define ITEM_ELEMENT_DATA_H

#include <stdio.h>
#include "AbstractData.h"
class ItemRule;
/**This is a class, enveloping elements into GameData.
  It serves to provide possibility to pass elements to reporting routines,
  demanding GameData as input.

  *@author Alex Dribin
  */

class ItemElementData : public AbstractData  {
public: 
	  ItemElementData(ItemRule * item, int num) {item_ =  item; num_ = num;}
    string printName();
    void saveAsParameter (ostream &out);
    protected:
  ItemRule * item_;
  int num_;
  
};

#endif
