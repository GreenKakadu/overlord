/* 
 * File:   TransformAction.h
 * Author: alex
 *
 * Created on February 15, 2010, 9:13 PM
 */

#ifndef _TRANSFORMACTION_H
#define	_TRANSFORMACTION_H

#include "ActionRule.h"

/**
  *@author Alex Dribin
  */

class TransformAction : public ActionRule{
public:
    TransformAction( const string & keyword, GameData * parent)
                                    : ActionRule(keyword, parent){}
    TransformAction( const  TransformAction * prototype );
    virtual ~TransformAction(){}
    GameData * createInstanceOfSelf();
    ACTION_RESULT carryOut(Entity * entity, AbstractData * parameter,   int value);
private:

};

#endif	/* _TRANSFORMACTION_H */

