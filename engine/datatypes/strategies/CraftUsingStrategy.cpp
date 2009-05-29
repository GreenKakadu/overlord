/***************************************************************************
                          CraftUsingStrategy.cpp
                             -------------------
    begin                : Thu Feb 20 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "CraftUsingStrategy.h"
#include "ItemElement.h"
#include "SkillUseElement.h"
#include "ItemRule.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "RulesCollection.h"
#include "ToolUseElement.h"
#include "BinaryMessage.h"
extern ReportPattern * notEnoughResourcesReporter;
extern ReportPattern * productionReporter;


CraftUsingStrategy        sampleCraftUsing        ("USING_CRAFT",        &sampleUsing);

CraftUsingStrategy::CraftUsingStrategy ( const CraftUsingStrategy * prototype ): BasicProductionStrategy(prototype)
{
  productNumber_ = 1;
  productType_ =0;
	mana_ = 0;
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

	if (parser->matchKeyword ("USE_MANA") )
    {
      mana_ =  parser->getInteger();
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



USING_RESULT CraftUsingStrategy::unitUse(UnitEntity * unit, SkillRule * skill, 
					int & useRestrictionCounter,OrderLine * order)
{
 //cout << unit->print()<<" "<<skill->print()<<" level " <<endl;
    int effectiveProduction = 0;
		USING_RESULT result =
			produce(unit, skill, useRestrictionCounter, effectiveProduction,order);
		if (effectiveProduction == 0)
		{
			return result;
		}
//  if (unit->isTraced())
// {
//       cout <<"== TRACING " <<unit->print()<< " ==> produces "<< effectiveProduction<< " of "<< productType_->print() <<"\n";
// 
// }
    unit->addToInventory(productType_, effectiveProduction);
      if(!unit->isSilent())
        {
//QQQ
          unit->addReport(
          new BinaryMessage(productionReporter, unit,
          new ItemElement(productType_,effectiveProduction))
                  );
      unit->getLocation()->addReport(
      new BinaryMessage(productionReporter, unit,
      new ItemElement(productType_,effectiveProduction))
        /*, 0, observation condition*/);
    }
    return result;
}


//USING_RESULT CraftUsingStrategy::unitMayUse(UnitEntity * unit, SkillRule * skill)
//{
///** We may be in the middle of production. Then we anyway may continue.*/
///** Otherwise we'll need resources */
//
// if (!unit->isCurrentlyUsingSkill(skill)) // beginning of production cycle
//  {
//    if(checkResourcesAvailability(unit) == 0)
//      {
//        return NO_RESOURCES;
//      }
//     else
//     {
//        consumeResources(unit,1);
//        return  USING_OK;
//     }
//  }
//    return  USING_OK;
//
//
//
//
////  vector <ToolUseElement *>::iterator iter;
////  RationalNumber dailyProduction(productNumber_ * unit->getFiguresNumber(), productionDays_);
////  for(iter = tools_.begin(); iter != tools_.end();iter++)
////  {
////    dailyProduction = dailyProduction + dailyProduction * (*iter)->getBonus() * unit->hasEquiped( (*iter)->getItemType())/100;
////  }
//
////  RationalNumber  currentAmount = unit->getItemAmount(productType_);
//// Check for new production cycle + check for production counter
////  if( currentAmount.isInteger() )
////  {
////      if(resourceNumber_ != unit->takeFromInventory(resourceType_, resourceNumber_))
////      {
////        return NO_RESOURCES;
////       }
////   }
////  else if( (currentAmount + dailyProduction).getValue() > currentAmount.getValue() )
////    {
////      if(resourceNumber_ != unit->takeFromInventory(resourceType_, resourceNumber_))
////      {
////        // no resources but old production in progress
////        dailyProduction =  currentAmount + 1 - currentAmount.getValue();
////        unit->addReport(new BinaryMessage(notEnoughResourcesReporter,resourceType_ ,productType_));
////        }
////    }
////    return  USING_OK;
//}



void    CraftUsingStrategy::extractKnowledge (Entity * recipient, int parameter)
{
  if(productType_)
  {
    if(recipient->addKnowledge(productType_))
      productType_->extractKnowledge(recipient);
  }

  for(vector <ItemElement *>::iterator iter = resources_.begin(); iter != resources_.end(); ++iter)
    {
      if(recipient->addKnowledge((*iter)->getItemType()))
        (*iter)->getItemType()->extractKnowledge(recipient);
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



void CraftUsingStrategy::reportUse(USING_RESULT result, TokenEntity * tokenEntity)
{
  for(vector <ItemElement *>::iterator iter = resources_.begin(); iter != resources_.end(); ++iter)
    {
      if (tokenEntity->hasItem((*iter)->getItemType()) < (*iter)->getItemNumber())
        tokenEntity->addReport(new BinaryMessage(notEnoughResourcesReporter, (*iter)->getItemType(),productType_));
    }
}



void CraftUsingStrategy::printSkillDescription(ostream & out)
{
  BasicProductionStrategy::printSkillDescription(out);

 if(!productType_)
    return;
 else

 out << " Use produces: " << productNumber_<<" ";
 if(productNumber_ > 1)
  out << productType_->getPluralName()<< " " << productType_->printTag();
 else
  out << productType_->print();

  out<<" in "<< productionDays_ <<" days.";
}



BasicUsingStrategy * CraftUsingStrategy::cloneSelf()
{
 CraftUsingStrategy * copyOfSelf = new CraftUsingStrategy(this);
 *copyOfSelf = *this;
 return copyOfSelf;
}
