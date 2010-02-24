/* 
 * File:   BuryActionRule.h
 * Author: alex
 *
 * Created on January 7, 2010, 10:38 PM
 */

#ifndef _BURYACTIONRULE_H
#define	_BURYACTIONRULE_H
#include "ActionRule.h"

class BuryActionRule : public ActionRule{
public:
    BuryActionRule( const string & keyword, GameData * parent)
                                    : ActionRule(keyword, parent){}
    BuryActionRule( const  BuryActionRule * prototype );
    virtual ~BuryActionRule(){}
    GameData * createInstanceOfSelf();
    ACTION_RESULT carryOut(Entity * entity, AbstractData * parameter,   int value);
private:

};

#endif	/* _BURYACTIONRULE_H */

