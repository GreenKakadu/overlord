/***************************************************************************
                          BasicCondition.h 
                             -------------------
    begin                : Sun Jan 26 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
/** Defines interface for condition objects */
#ifndef BASIC_CONDITION_H
#define BASIC_CONDITION_H
#include "GameData.h"
class Entity;
class UnitEntity;
class Rule;

class BasicCondition : public GameData{
public: 
  BasicCondition(const string & keyword, GameData * parent ): GameData(keyword, parent){}
  BasicCondition(const BasicCondition * prototype): GameData(prototype){}
  
	virtual     ~BasicCondition(){}
  virtual bool isSatisfied(UnitEntity * ){return true;}
  virtual void conditionHandler(Entity * entity){}
  virtual void setSubject(Rule * subject){}
  virtual void extractKnowledge (Entity * recipient, int parameter = 0);
          GameData * createInstanceOfSelf();
};

#endif
