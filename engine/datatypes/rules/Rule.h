/***************************************************************************
                          Rule.h  -  description
                             -------------------
    begin                : Thu Jan 23 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef RULE_H
#define RULE_H
#include <vector>
#include "GameData.h"
class BasicCondition;
class Entity;

class Rule : public GameData  {
public: 
      Rule ( const Rule * prototype ): GameData(prototype){}
      Rule ( const string & keyword, GameData * parent): GameData(keyword, parent){}
      void bindCondition(BasicCondition * condition);
      void checkConditions(Entity * entity);
  virtual void     extractKnowledge (Entity * recipient, int parameter = 0);
protected:
	virtual ~Rule(){}
  inline  void     setTag          ( const string  &tag) {  tag_ = tag;}
  inline  void     setName         ( const string  &name){ name_ = name;}
  inline  void     setDescription  ( const string  &description){ description_ = description;}
  vector <BasicCondition *> conditions_;
};

#endif
