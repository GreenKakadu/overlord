/***************************************************************************
                          ItemCondition.h  -  description
                             -------------------
    begin                : Aug 09 2006
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef ITEM_CONDITION_H
#define ITEM_CONDITION_H
#include "BasicCondition.h"
#include "ItemElement.h"
class UnitEntity;
class ItemRule;
class Rule;



class ItemCondition : public BasicCondition  {
public: 
  ItemCondition(const string & keyword, GameData * parent ): BasicCondition(keyword, parent){}
  ItemCondition(const ItemCondition * prototype);
	~ItemCondition(){delete itemRequirement_;}
  STATUS      initialize ( Parser *parser );
  GameData * createInstanceOfSelf();
  bool isSatisfied(TokenEntity * unit);
  void conditionHandler(Entity * entity);
  void setSubject(Rule * subject);
  void extractKnowledge (Entity * recipient, int parameter = 0);
  ItemElement * getItemElement() const {return itemRequirement_;}
  Rule * getSubject() const ;
  ItemRule * getItem() const ;
  int  getNumber() const ;
//  ostream & reportCondition(ostream & out);
  string print(){return string("condition: ") + itemRequirement_->print();}
protected:
	ItemElement * itemRequirement_;
  ItemRule * subject_;
};
extern ItemCondition  sampleItemCondition;

#endif
