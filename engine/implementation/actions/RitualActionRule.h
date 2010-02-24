/* 
 * File:   RitualActionRule.h
 * Author: alex
 *
 * Created on December 27, 2009, 5:30 PM
 */

#ifndef _RITUALACTIONRULE_H
#define	_RITUALACTIONRULE_H
#include "ActionRule.h"

/**
  *@author Alex Dribin
  */

class RitualActionRule : public ActionRule{
public:
    RitualActionRule( const string & keyword, GameData * parent)
                                    : ActionRule(keyword, parent){}
    RitualActionRule( const  RitualActionRule * prototype );
    virtual ~RitualActionRule(){}
    GameData * createInstanceOfSelf();
    ACTION_RESULT carryOut(Entity * entity, AbstractData * parameter,   int value);
private:

};

#endif	/* _RITUALACTIONRULE_H */

