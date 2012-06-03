/***************************************************************************
                          BasicCondition.h 
                             -------------------
    begin                : Sun Jan 26 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
/** Defines interface for condition objects */
/* Includes mechanism for  throwing "interrupts" when check of condition
 * reveals that it's state was changed. In this case conditionHandler function
 * may be called by the side that performs check. */
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
static  BasicCondition * readCondition(Parser * parser);
  virtual void save(ostream &out);
  virtual bool isSatisfied(TokenEntity * agent, Entity * target=0 ){return false;} // may be also used as never-true condition
//  virtual void conditionHandler(Entity * ){}
  virtual void extractKnowledge (Entity * recipient, int parameter = 0);
//  virtual ostream & reportCondition(ostream & out) {return out;}
          GameData * createInstanceOfSelf();
};
extern BasicCondition  sampleBasicCondition; 

#endif
