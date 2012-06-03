/***************************************************************************
                          ActionUsingStrategy.cpp
                             -------------------
    begin                : Sun Jan 18 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "ActionUsingStrategy.h"
#include "SkillUseElement.h"
#include "ToolUseElement.h"
#include "ItemElement.h"
#include "ActionRule.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "RulesCollection.h"
#include "BinaryMessage.h"
#include "DataManipulator.h"

//extern DataManipulator * dataManipulatorPtr;
extern ReportPattern * notEnoughResourcesReporter;
extern ReportPattern * privateActionReporter;
extern ReportPattern * publicActionReporter;

ActionUsingStrategy       sampleActionUsing       ("USING_ACTION",       &sampleUsing);


GameData * ActionUsingStrategy::createInstanceOfSelf()
{
  return CREATE_INSTANCE<ActionUsingStrategy> (this);
}

ActionUsingStrategy::ActionUsingStrategy(const ActionUsingStrategy * prototype)
        : BasicProductionStrategy(prototype)
{
    name_ = " Using Action";
    productType_ = 0;
    specificProduct_ = 0;
}


STATUS
ActionUsingStrategy::initialize        ( Parser *parser )
{
     BasicProductionStrategy::initialize(parser);

     if (parser->matchKeyword ("PRODUCES") )
    {
      productType_ = gameFacade->fx_actions[parser->getWord()];
      productionDays_ =  parser->getInteger();
      if( (productType_ == 0) ||  (productionDays_ == 0) )
      {
        cout << "Error while reading PRODUCES \n";
        return IO_ERROR;
      }
      return OK;
    }

  if (parser->matchKeyword ("PARAMETER") )
    {
      string tag =parser->getParameter();
      if(!tag.empty())
      {
        specificProduct_ = gameFacade->getDataManipulator()->findGameData(tag);
      }
      if(specificProduct_==0)
      {
         specificProduct_ = new StringData(tag);
      }
      return OK;
    }

      return OK;
}


void ActionUsingStrategy::save(ostream &out)
{
        BasicProductionStrategy::save(out);
    if(productType_) out<<"PRODUCES"<<" "<<productType_->getTag()
            <<" "<<productionDays_ << endl;

     if(specificProduct_)
     {
         out<< "PARAMETER"<<" ";
         specificProduct_->saveAsParameter(out);
         out<<endl;
     }

}

/*
 *
 */



USING_RESULT ActionUsingStrategy::unitUse(UnitEntity * unit, SkillRule * skill, 
					int &useCounter, OrderLine * )
{
    if(productType_==0)
    {
        cerr<<"Uninitialized ActionUsingStrategy for "<< getName()<<" resulting action undefined"<<endl;
        return UNUSABLE;
    }
// Production modifiers:
int effectiveProductionRate = getEffectiveProductionRate(unit,skill).getValue();
      if(unit->isTraced())
      {
        cout<<"<===ACTION== "<< getName()<<" "<<productType_->print()<< " "<<effectiveProductionRate <<endl;
      }


  SkillUseElement * dailyUse = new SkillUseElement(skill,effectiveProductionRate,productionDays_);

  int cycleCounter  = unit->addSkillUse(dailyUse);
  if(cycleCounter == 0) // In the middle of the production cycle
  {
     return USING_IN_PROGRESS;
  }


  else // The   production cycle is finished.
  {
      if(unit->isTraced())
      {
    cout<<"<===ACTION== "<< getName()<<" "<<productType_->print()<<endl;
      }
     productType_->carryOut(unit,specificProduct_,productNumber_);

  }

   return USING_COMPLETED;
}


USING_RESULT ActionUsingStrategy::unitMayUse(UnitEntity * unit, SkillRule * skill,OrderLine * order)
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
        tokenEntity->addReport(new BinaryMessage(notEnoughResourcesReporter, (*iter)->getItemType(),productType_));
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

BasicUsingStrategy * ActionUsingStrategy::cloneSelf()
{
 ActionUsingStrategy * copyOfSelf = new ActionUsingStrategy(keyword_,parent_);
 *copyOfSelf = *this;
 return copyOfSelf;
}
