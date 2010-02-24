/* 
 * File:   BuryActionRule.cpp
 * Author: alex
 * 
 * Created on January 7, 2010, 10:38 PM
 */

#include "BuryActionRule.h"
#include "TokenEntity.h"
#include "TertiaryMessage.h"
#include "LocationEntity.h"
//extern ReportPattern * ritualSourceReporter;
//extern ReportPattern * ritualTargetReporter;
//extern GameData  targetTypeSelf;


BuryActionRule     sampleBuryActionRule =      BuryActionRule("BURY_ACTION", &sampleAction);

 BuryActionRule:: BuryActionRule(const  BuryActionRule * prototype )
                    : ActionRule(prototype)
{
    name_ = "Bury Action";
}


GameData *  BuryActionRule::createInstanceOfSelf()
{
  return CREATE_INSTANCE< BuryActionRule> (this);
}




ACTION_RESULT  BuryActionRule::carryOut(Entity * entity, AbstractData * parameter,   int value)
{
    if(entity->isTraced())
    {
        cout << entity->print() << "buries "<<value <<" dead bodies"<<endl;
    }

  TokenEntity * token = dynamic_cast<TokenEntity *>(entity);
  assert(token);
  LocationEntity * location = token->getLocation();
  assert(location);

  //Convert value dead to bones
    ItemRule * deads = items["dead"];
    ItemRule * bones = items["bone"];
    int wasBodies = location->getAvailableResource(deads).roundDown();
    int wasBones = location->getAvailableResource(bones).roundDown();
    RationalNumber burnBodies(value,1);
    RationalNumber haveBodies  = location->takeAvailableResource(deads,burnBodies);
    int burntBodies = haveBodies.getValue();
// modify max.
    //location->setResource(deads,wasBodies - burntBodies);
    location->setAvailableResource(deads,wasBodies - burntBodies);
    //location->setResource(bones,wasBones + burntBodies);
    location->setAvailableResource(bones,wasBones + burntBodies);
    if(entity->isTraced())
    {
        cout << location->print() << " now has "<< wasBodies - burntBodies<<" dead bodies and "<<wasBones + burntBodies<<" bones"<<endl;
    }
    return     ACTION_SUCCESS;
}





