/* 
 * File:   TransformAction.cpp
 * Author: alex
 * 
 * Created on February 15, 2010, 9:13 PM
 */

#include "TransformAction.h"
#include "UnitEntity.h"
#include "RaceRule.h"
#include "BinaryMessage.h"

extern ReportPattern * transformReport;
TransformAction     sampleTransformAction =     TransformAction("TRANSFORM_ACTION", &sampleAction);

TransformAction::TransformAction(const TransformAction * prototype )
                    : ActionRule(prototype)
{
    name_ = "Transform Action";
}


GameData * TransformAction::createInstanceOfSelf()
{
  return CREATE_INSTANCE<TransformAction> (this);
}




ACTION_RESULT TransformAction::carryOut(Entity * entity,  AbstractData * parameter,  int value)
{
 UnitEntity * mage = dynamic_cast<UnitEntity *>(entity);
  assert(mage);

  RaceRule * race = dynamic_cast<RaceRule *>(parameter);
  if(race==0)
  {
      cout<<"Error: TransformAction parameter "<< parameter->print()<<" is wrong"<<endl;
  }
   UnitEntity * target= dynamic_cast<UnitEntity *>(mage->getTarget());
  if(target == 0)
  {
      target = mage;
  }

   target->changeRace(race);
   mage->addReport(new BinaryMessage(transformReport,mage,race));

    return     ACTION_SUCCESS;
}


