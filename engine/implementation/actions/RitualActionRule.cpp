/* 
 * File:   RitualActionRule.cpp
 * Author: alex
 * 
 * Created on December 27, 2009, 5:30 PM
 */

#include "RitualActionRule.h"
#include "TokenEntity.h"
#include "TertiaryMessage.h"
extern ReportPattern * ritualSourceReporter;
extern ReportPattern * ritualTargetReporter;
extern GameData  targetTypeSelf;


RitualActionRule     sampleRitualActionRule =     RitualActionRule("RITUAL_ACTION", &sampleAction);

RitualActionRule::RitualActionRule(const RitualActionRule * prototype )
                    : ActionRule(prototype)
{
    name_ = "Ritual Action";
}


GameData * RitualActionRule::createInstanceOfSelf()
{
  return CREATE_INSTANCE<RitualActionRule> (this);
}




ACTION_RESULT RitualActionRule::carryOut(Entity * entity,  AbstractData * parameter,  int value)
{
  TokenEntity * mage = dynamic_cast<TokenEntity *>(entity);
  assert(mage);
  TokenEntity * target = mage;
  if(targetType_ != &targetTypeSelf)
  {
    target= dynamic_cast<TokenEntity *>(mage->getTarget());
    assert(target);
  }

  if(mage->isTraced()||target->isTraced())
  {
    cout << mage<< "performing ritual targeting  " <<target <<endl;
  }
  target->getStats()->modifyMana(value);
  mage->addReport(new TertiaryMessage(ritualSourceReporter, mage,new IntegerData(value), target));
  target->addReport(new TertiaryMessage(ritualTargetReporter, mage,new IntegerData(value), target));

    return     ACTION_SUCCESS;
}


