/***************************************************************************
                          CraftUsingStrategy.cpp 
                             -------------------
    begin                : Thu Feb 20 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "CraftUsingStrategy.h"
#include "ItemElementData.h"
#include "ItemRule.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "RulesCollection.h"
#include "ToolUseElement.h"
#include "BinaryPattern.h"
extern Reporter * notEnoughResourcesReporter;
extern Reporter * productionReporter;

extern RulesCollection    <ItemRule>     items;

CraftUsingStrategy::CraftUsingStrategy ( const CraftUsingStrategy * prototype ): BasicUsingStrategy(prototype)
{
  productNumber_ = 1; 
}



GameData * CraftUsingStrategy::createInstanceOfSelf()
{
  return CREATE_INSTANCE<CraftUsingStrategy> (this);
}



STATUS
CraftUsingStrategy::initialize        ( Parser *parser )
{
  if (parser->matchKeyword ("PRODUCES") )
    {
      productType_ = items[parser->getWord()];
      productionDays_ =  parser->getInteger();
      if( (productType_ == 0) ||  (productionDays_ == 0) )
      {
        cout << "Error while reading PRODUCES \n";
        return IO_ERROR;
      }
      return OK;
    }
  if (parser->matchKeyword ("CONSUME") )
    {
      resourceType_ = items[parser->getWord()];
      resourceNumber_ =  parser->getInteger();
      if( (resourceNumber_ == 0) ||  (resourceType_ == 0) )
        {
        cout << "Error while reading CONSUME \n";
        return IO_ERROR;
        }
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



bool CraftUsingStrategy::use(UnitEntity * unit, Order * OrderId)
{
  vector <ToolUseElement *>::iterator iter;
  Rational dailyProduction(productNumber_ * unit->getFiguresNumber(), productionDays_);
  for(iter = tools_.begin(); iter != tools_.end();iter++)
  {
    dailyProduction = dailyProduction + dailyProduction * (*iter)->getBonus() * unit->hasEquiped( (*iter)->getItemType())/100;
    }

  Rational hadBefore = unit->getItemAmount(productType_);
  unit->addToInventory(productType_, dailyProduction );
  Rational nowHas = hadBefore + dailyProduction;
  int added = nowHas.getValue() - hadBefore.getValue();
  if(added !=0)
  {  
    if(!unit->isSilent())
      {
        unit->addReport(
        new BinaryPattern(productionReporter, unit,
        new ItemElementData(productType_,added))
                  );
      }
    unit->getLocation()->addReport(
    new BinaryPattern(productionReporter, unit,
    new ItemElementData(productType_,added))
        /*, 0, observation condition*/);
  }
  return true;
}



void    CraftUsingStrategy::extractKnowledge (Entity * recipient, int parameter)
{
  if(productType_)
  {
    if(recipient->addKnowledge(productType_))
      productType_->extractKnowledge(recipient);
  }   

  if(resourceType_)
  {
  if(recipient->addKnowledge(resourceType_))
    resourceType_->extractKnowledge(recipient);
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

USING_RESULT CraftUsingStrategy::mayUse(UnitEntity * unit, SkillRule * skill)
{
  vector <ToolUseElement *>::iterator iter;
  Rational dailyProduction(productNumber_ * unit->getFiguresNumber(), productionDays_);
  for(iter = tools_.begin(); iter != tools_.end();iter++)
  {
    dailyProduction = dailyProduction + dailyProduction * (*iter)->getBonus() * unit->hasEquiped( (*iter)->getItemType())/100;
    }
  Rational  currentAmount = unit->getItemAmount(productType_);

  if( currentAmount.isInteger() )
  {
      if(!unit->takeFromInventoryExactly(resourceType_, resourceNumber_))
      {
//        unit->addReport(new BinaryPattern(notEnoughResourcesReporter,resourceType_ ,productType_));
        return NO_RESOURCES;
        }

   }
  else if( (currentAmount + dailyProduction).getValue() > currentAmount.getValue() )
    {
      if(!unit->takeFromInventoryExactly(resourceType_, resourceNumber_))
      {
        // no resources but old production in progress
        dailyProduction =  currentAmount + 1 - currentAmount.getValue();
        unit->addReport(new BinaryPattern(notEnoughResourcesReporter,resourceType_ ,productType_));
        }
    }
    return  USING_OK;
}



void CraftUsingStrategy::reportUse(USING_RESULT result, UnitEntity * unit, Order * OrderId)
{
        unit->addReport(new BinaryPattern(notEnoughResourcesReporter,resourceType_ ,productType_));
}
