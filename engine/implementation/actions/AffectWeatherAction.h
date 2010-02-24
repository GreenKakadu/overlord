/* 
 * File:   AffectWeatherAction.h
 * Author: alex
 *
 * Created on February 15, 2010, 9:16 PM
 */

#ifndef _AFFECTWEATHERACTION_H
#define	_AFFECTWEATHERACTION_H

#include "ActionRule.h"

/**
  *@author Alex Dribin
  */

class AffectWeatherAction : public ActionRule{
public:
    AffectWeatherAction( const string & keyword, GameData * parent)
                                    : ActionRule(keyword, parent){}
    AffectWeatherAction( const  AffectWeatherAction * prototype );
    virtual ~AffectWeatherAction(){}
    GameData * createInstanceOfSelf();
    ACTION_RESULT carryOut(Entity * entity,  AbstractData * parameter,  int value);
private:

};

#endif	/* _AFFECTWEATHERACTION_H */

