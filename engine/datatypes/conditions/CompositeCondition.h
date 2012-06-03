/* 
 * File:   CompositeCondition.h
 * Author: alex
 * Composite condition combines simple conditions in a form
 * ( A1 OR A2 OR A3) AND (A4 OR NOT A5) AND...)
 * where Ai - are simple conditions
 *
 * Currently not implemented
 * 
 * Created on April 6, 2011, 11:08 AM
 */

#ifndef COMPOSITECONDITION_H
#define	COMPOSITECONDITION_H
#include "BasicCondition.h"


class CompositeCondition : public BasicCondition{
public:
  CompositeCondition(const string & keyword, GameData * parent ): BasicCondition(keyword, parent){}
  CompositeCondition(const CompositeCondition * prototype): BasicCondition(prototype){}
  CompositeCondition(const CompositeCondition& orig);
    virtual ~CompositeCondition(){}
  GameData * createInstanceOfSelf();

  virtual void save(ostream &out);
  virtual bool isSatisfied(TokenEntity * agent , Entity * target=0);
//  virtual void conditionHandler(Entity * );
  virtual void setSubject(Rule * );
  virtual Rule * getSubject()const;
  virtual void extractKnowledge (Entity * recipient, int parameter = 0);

private:

};

#endif	/* COMPOSITECONDITION_H */

