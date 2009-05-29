/***************************************************************************
                          BasicCondition.h 
                             -------------------
    begin                : Sun Jan 26 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
/** Defines interface for condition objects */
#ifndef BASIC_CONDITION_H
#define BASIC_CONDITION_H
#include "GameData.h"
class Entity;
class UnitEntity;
class Rule;
class TokenEntity;

class BasicCondition : public GameData{
public: 
  BasicCondition(const string & keyword, GameData * parent ): GameData(keyword, parent){}
  BasicCondition(const BasicCondition * prototype): GameData(prototype){}
  
	virtual     ~BasicCondition(){}
  virtual bool isSatisfied(TokenEntity * ){return true;}
  virtual void conditionHandler(Entity * ){}
  virtual void setSubject(Rule * ){}
  virtual Rule * getSubject()const{return 0;}
  virtual void extractKnowledge (Entity * recipient, int parameter = 0);
//  virtual ostream & reportCondition(ostream & out) {return out;}
          GameData * createInstanceOfSelf();
};
extern BasicCondition  sampleBasicCondition; 

#endif
