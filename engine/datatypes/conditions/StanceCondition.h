/* 
 * File:   StanceCondition.h
 * Author: alex
 *
 * Created on April 11, 2011, 12:30 PM
 */

#ifndef STANCECONDITION_H
#define	STANCECONDITION_H
#include "BasicCondition.h"
class StanceVariety;

class StanceCondition : public BasicCondition{
public:
  StanceCondition(const string & keyword, GameData * parent ): BasicCondition(keyword, parent){}
  StanceCondition(const StanceCondition * prototype);
	~StanceCondition(){}
  static StanceCondition * createStanceCondition(StanceVariety * stance);
  STATUS      initialize ( Parser *parser );
  virtual void save(ostream &out);
  GameData * createInstanceOfSelf();
  bool isSatisfied(TokenEntity * unit, Entity * target =0);
private:
  StanceVariety * stanceRequired_;
};

#endif	/* STANCECONDITION_H */

