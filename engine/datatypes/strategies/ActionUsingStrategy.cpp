/***************************************************************************
                          ActionUsingStrategy.cpp 
                             -------------------
    begin                : Sun Jan 18 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "ActionUsingStrategy.h"
#include "SkillUseElement.h"
#include "ToolUseElement.h"
#include "ItemElement.h"
#include "ActionRule.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "RulesCollection.h"
#include "BinaryPattern.h"

extern Reporter * notEnoughResourcesReporter;
extern Reporter * privateActionReporter;
extern Reporter * publicActionReporter;
extern RulesCollection    <ActionRule>     fx_actions;


GameData * ActionUsingStrategy::createInstanceOfSelf()
{
  return CREATE_INSTANCE<ActionUsingStrategy> (this);
}



STATUS
ActionUsingStrategy::initialize        ( Parser *parser )
{
  if (parser->matchKeyword ("PRODUCES") )
    {
      productType_ = fx_actions[parser->getWord()];
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


  if (parser->matchKeyword ("TOOL") )
  {
      ToolUseElement * tool = ToolUseElement::readElement (parser);
      if( tool)
        tools_.push_back(tool);
      return OK;
    }
      return OK;
}




/*
 *
 */



USING_RESULT ActionUsingStrategy::unitUse(UnitEntity * unit, SkillRule * skill, int &useCounter)
{
// Production modifiers:

// Tools accelerate production
  vector <ToolUseElement *>::iterator iter;
  int effectiveProductionRate = unit->getFiguresNumber();
  for(iter = tools_.begin(); iter != tools_.end();iter++)
  {
    effectiveProductionRate =  effectiveProductionRate + (*iter)->getBonus() * unit->hasEquiped( (*iter)->getItemType())/100;
  }


  SkillUseElement * dailyUse = new SkillUseElement(skill,effectiveProductionRate,productionDays_);

  int cycleCounter  = unit->addSkillUse(dailyUse);
  if(cycleCounter == 0) // In the middle of the production cycle
  {
     return USING_IN_PROGRESS;
  }


  else // The   production cycle is finished. 
  {
//cout<<"<===ACTION== "<< *productType_<<endl;
     productType_->carryOut(unit);

  }

   return USING_COMPLETED;
}


USING_RESULT ActionUsingStrategy::unitMayUse(UnitEntity * unit, SkillRule * skill)
{
  // check target type
    USING_RESULT result = checkTarget(unit,productType_->getTargetType());
    if( result != USING_OK)
      return  result;
    else
  return BasicProductionStrategy::unitMayUse(unit,skill);
}



void    ActionUsingStrategy::extractKnowledge (Entity * recipient, int parameter)
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



void ActionUsingStrategy::reportUse(USING_RESULT result, TokenEntity * tokenEntity)
{
  for(vector <ItemElement *>::iterator iter = resources_.begin(); iter != resources_.end(); ++iter)
    {
      if (tokenEntity->hasItem((*iter)->getItemType()) < (*iter)->getItemNumber())
        tokenEntity->addReport(new BinaryPattern(notEnoughResourcesReporter, (*iter)->getItemType(),productType_));
    }
}



void ActionUsingStrategy::printSkillDescription(ostream & out)
{
  BasicProductionStrategy::printSkillDescription(out);

 if(!productType_)
    return;
 else

 out << " Use results in " <<" ";
  out << productType_->print();

  out<<" in "<< productionDays_ <<" days.";
}

