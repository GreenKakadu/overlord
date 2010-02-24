/* 
 * File:   InfectionActionRule.h
 * Author: alex
 *
 * Created on January 18, 2010, 12:51 PM
 */

#ifndef _INFECTIONACTIONRULE_H
#define	_INFECTIONACTIONRULE_H
#include "ActionRule.h"
class PlagueEffectRule;

class InfectionActionRule : public ActionRule{
public:
    InfectionActionRule( const string & keyword, GameData * parent)
                                    : ActionRule(keyword, parent){}
    InfectionActionRule( const  InfectionActionRule * prototype );
    STATUS     initialize      ( Parser *parser);
    virtual ~InfectionActionRule(){}
    GameData * createInstanceOfSelf();
    ACTION_RESULT carryOut(Entity * entity,  AbstractData * parameter,  int value);
private:
    PlagueEffectRule * effectType_;
};

#endif	/* _INFECTIONACTIONRULE_H */

