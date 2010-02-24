/* 
 * File:   CreatePortalAction.h
 * Author: alex
 *
 * Created on February 15, 2010, 9:15 PM
 */

#ifndef _CREATEPORTALACTION_H
#define	_CREATEPORTALACTION_H

#include "ActionRule.h"
#include "LocationEntity.h"

/**
  *@author Alex Dribin
  */

class CreatePortalAction : public ActionRule{
public:
    CreatePortalAction( const string & keyword, GameData * parent)
                                    : ActionRule(keyword, parent){}
    CreatePortalAction( const  CreatePortalAction * prototype );
    virtual ~CreatePortalAction(){}
    GameData * createInstanceOfSelf();
    STATUS  initialize        ( Parser *parser );
    ACTION_RESULT carryOut(Entity * entity,  AbstractData * parameter,  int value);
private:
    vector <LocationEntity *> destinations_;

};

#endif	/* _CREATEPORTALACTION_H */

