/* 
 * File:   TerraformAction.cpp
 * Author: alex
 * 
 * Created on February 15, 2010, 9:12 PM
 */

#include "TerraformAction.h"
#include "Entity.h"
#include "LocationEntity.h"
#include "BinaryMessage.h"
extern ReportPattern * terraformReport;
extern ReportPattern * terraformReport2;

TerraformAction     sampleTerraformAction =     TerraformAction("TERRAFORM_ACTION", &sampleAction);

TerraformAction::TerraformAction(const TerraformAction * prototype )
                    : ActionRule(prototype)
{
    name_ = "Terraform Action";
}


GameData * TerraformAction::createInstanceOfSelf()
{
  return CREATE_INSTANCE<TerraformAction> (this);
}




ACTION_RESULT TerraformAction::carryOut(Entity * entity, AbstractData * parameter,   int value)
{
  TokenEntity * mage = dynamic_cast<TokenEntity *>(entity);
  assert(mage);
  LocationEntity * target= dynamic_cast<LocationEntity *>(mage->getTarget());
  if(target == 0)
  {
      target = mage->getLocation();
  }
  TerrainRule * terrain = dynamic_cast<TerrainRule *>(parameter);
  if(terrain==0)
  {
      cout<<"Error: TerraformAction parameter "<< parameter->print()<<" is wrong"<<endl;
  }
  // 1. Set new Terrain
    target->setTerrain(terrain);
    //replace resourses
    target->cleanResourses();
    target->generateResourses();
    // no need to reset exits: traveling time calculated dynamically

    target->addReport(new BinaryMessage(terraformReport,target,terrain));
    mage->addReport(new BinaryMessage(terraformReport2,target,terrain));
    return     ACTION_SUCCESS;
}


