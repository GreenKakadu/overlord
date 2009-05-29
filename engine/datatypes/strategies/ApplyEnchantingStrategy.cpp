/***************************************************************************
                          ApplyEnchantingStrategy.cpp
                             -------------------
    begin                : Thu Mar 03 2006
    copyright            : (C) 2006 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "ApplyEnchantingStrategy.h"
#include "ItemRule.h"
#include "LocationEntity.h"
#include "FactionEntity.h"
#include "UnitEntity.h"
#include "ResourceCompetitiveRequest.h"
#include "RulesCollection.h"
#include "ToolUseElement.h"
#include "BinaryMessage.h"
#include "ConstructionEntity.h"

 extern ReportPattern * notAvailableReporter;
 ApplyEnchantingStrategy  sampleApplyEnchanting ("COMBAT_ENCHANT", &sampleApply);
//
//
// ReportPattern *	 harvestingNotPermittedReporter  = new ReportPattern("harvestingNotPermittedReporter");



ApplyEnchantingStrategy::ApplyEnchantingStrategy ( const ApplyEnchantingStrategy * prototype )
        : BasicApplyStrategy(prototype)
{
   productNumber_ = 1;
   productType_ =0;
}




GameData * ApplyEnchantingStrategy::createInstanceOfSelf()
{
  return CREATE_INSTANCE<ApplyEnchantingStrategy> (this);
}



STATUS
ApplyEnchantingStrategy::initialize        ( Parser *parser )
{
  if (parser->matchKeyword ("GRANTS") )
    {
      productType_ = enchantments[parser->getWord()];
      productionDays_ =  parser->getInteger();
      if( (productType_ == 0) ||  (productionDays_ == 0) )
      {
        cout << "Error while reading GRANTS \n";
        return IO_ERROR;
      }
      return OK;
    }
  if (parser->matchKeyword ("CONSUME") )
    {
			if(parser->matchElement())
			  resources_.push_back(new ItemElement(parser));
      return OK;
    }

  if (parser->matchKeyword ("MULTIPLE") )
    {
      productNumber_ =  parser->getInteger();
      if(productNumber_ == 0)
        productNumber_ = 1;
      return OK;
    }


  if (parser->matchKeyword ("TOOL") )
  {
      ToolUseElement * tool = ToolUseElement::readElement (parser);
      if( tool)
        tools_.push_back(tool);
      return OK;
    }
      return OK;

}



RationalNumber ApplyEnchantingStrategy::getDailyProduction()
{
  return  RationalNumber(harvest_ , days_);
}



USING_RESULT ApplyEnchantingStrategy::unitUse(UnitEntity * unit, SkillRule * skill, int & useCounter)
{
  RationalNumber request = getDailyProduction() * unit->getFiguresNumber();
  if( request.isEmpty())
  {
//    unit->addReport(new BinaryMessage(notAvailableReporter,resourceConsumed_ ,unit->getLocation()));
    return CANNOT_USE;
  }
	int bonus = calculateProductionBonus(unit,skill);


	vector <ToolUseElement *>::iterator iter;
  for(iter = tools_.begin(); iter != tools_.end();iter++)
  {
    bonus += (((*iter)->getBonus() * unit->hasEquiped( (*iter)->getItemType())) /
		unit->getFiguresNumber()) /100;
  }
   request = request * (100 + bonus)/100;



  ConstructionEntity * containingConstruction = unit->getContainingConstruction();
  if(containingConstruction)
  {
    containingConstruction->setProductionBonus(getHarvestedResource(), skill,unit->getSkillLevel(skill));
  }

  unit->getLocation()->addDailyConflictRequest(new ResourceCompetitiveRequest(unit,unit->getCurrentOrder(),getHarvestedResource(), request));
  return USING_COMPLETED;
}



void    ApplyEnchantingStrategy::extractKnowledge (Entity * recipient, int parameter)
{
  if(resourceConsumed_)
  {
    if(recipient->addKnowledge(resourceConsumed_))
      resourceConsumed_->extractKnowledge(recipient);
  }

  if(resourceHarvested_)
  {
  if(recipient->addKnowledge(resourceHarvested_))
    resourceHarvested_->extractKnowledge(recipient);
  }

  for(vector <ToolUseElement *>::iterator iter = tools_.begin(); iter != tools_.end(); ++iter)
    {
      if((*iter)->getItemType())
      {
        if(recipient->addKnowledge((*iter)->getItemType()))
          (*iter)->getItemType()->extractKnowledge(recipient);
       }
     }
}



USING_RESULT ApplyEnchantingStrategy::unitMayUse(UnitEntity * unit, SkillRule * skill)
{
  LocationEntity * location = unit->getLocation();
	if(location == 0)
	{
	 return USING_RESULT_END;
	}
  RationalNumber request = getDailyProduction();
  if( !harvest_)
  {
     if(!location->getResource(resourceConsumed_))
    unit->addReport(new BinaryMessage(notAvailableReporter,resourceConsumed_ ,unit->getLocation()));
    return NO_RESOURCES;
  }
  FactionEntity * owner = location->getRealOwner();

    if(owner)
    {
      if(!owner->stanceAtLeast(unit,
				location->getOwnershipPolicy().getHarvestPermission(resourceConsumed_)))
      {
//        unit->addReport(new BinaryMessage (harvestingNotPermittedReporter,
//				 resourceConsumed_, location),0,0);
        return USING_NOT_ALLOWED;
      }
    }
  return  USING_OK;
}



void ApplyEnchantingStrategy::reportUse(USING_RESULT result, TokenEntity * tokenEntity)
{
    tokenEntity->addReport(new BinaryMessage(notAvailableReporter,resourceConsumed_ ,tokenEntity->getLocation()));

}



void ApplyEnchantingStrategy::printSkillDescription(ostream & out)
{

 if(!resourceHarvested_)
    return;
 else

 out << " Use produces: " << harvest_<<" ";
 if(harvest_ > 1)
  out << resourceHarvested_->getPluralName()<< " " << resourceHarvested_->printTag();
 else
  out << resourceHarvested_->print();

  out<<" in "<< days_ <<" days.";
}
