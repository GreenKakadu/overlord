/* 
 * File:   TerraformAction.h
 * Author: alex
 *
 * Created on February 15, 2010, 9:12 PM
 */

#ifndef _TERRAFORMACTION_H
#define	_TERRAFORMACTION_H

#include "ActionRule.h"

/**
  *@author Alex Dribin
  */

class TerraformAction : public ActionRule{
public:
    TerraformAction( const string & keyword, GameData * parent)
                                    : ActionRule(keyword, parent){}
    TerraformAction( const  TerraformAction * prototype );
    virtual ~TerraformAction(){}
    GameData * createInstanceOfSelf();
    ACTION_RESULT carryOut(Entity * entity, AbstractData * parameter,   int value);
private:

};

#endif	/* _TERRAFORMACTION_H */

