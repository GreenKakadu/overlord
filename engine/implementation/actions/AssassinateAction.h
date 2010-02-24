/* 
 * File:   AssassinateAction.h
 * Author: alex
 *
 * Created on February 15, 2010, 9:14 PM
 */

#ifndef _ASSASSINATEACTION_H
#define	_ASSASSINATEACTION_H

#include "ActionRule.h"

/**
  *@author Alex Dribin
  */

class AssassinateAction : public ActionRule{
public:
    AssassinateAction( const string & keyword, GameData * parent)
                                    : ActionRule(keyword, parent){}
    AssassinateAction( const  AssassinateAction * prototype );
    virtual ~AssassinateAction(){}
    GameData * createInstanceOfSelf();
    ACTION_RESULT carryOut(Entity * entity,  AbstractData * parameter,  int value);
private:

};

#endif	/* _ASSASSINATEACTION_H */

