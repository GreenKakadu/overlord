/***************************************************************************
                          BasicProductionStrategy.cpp
                             -------------------
    begin                : Thu Sep 18 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "BasicProductionStrategy.h"
#include "ItemElement.h"
#include "SkillUseElement.h"
#include "ItemRule.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "RulesCollection.h"
#include "ToolUseElement.h"
#include "BinaryMessage.h"
extern ReportPattern * notEnoughResourcesReporter;
extern ReportPattern * startUseReporter;
extern ReportPattern * productionReporter;
extern GameData  targetTypeSelf;
//extern const int VERY_BIG_NUMBER;

BasicProductionStrategy::BasicProductionStrategy ( const BasicProductionStrategy * prototype ): BasicUsingStrategy(prototype)
{
  productNumber_ = 1;
  productionDays_ =1;
	mana_ = 0;
}



GameData * BasicProductionStrategy::createInstanceOfSelf()
{
  return CREATE_INSTANCE<BasicProductionStrategy> (this);
}



STATUS
BasicProductionStrategy::initialize        ( Parser *parser )
{
	BasicUsingStrategy::initialize (parser);
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

void BasicProductionStrategy::save(ostream &out)
{
  for(vector <ItemElement *>::iterator iter = resources_.begin(); iter != resources_.end(); ++iter)
    {
        out<<"CONSUME"<<" ";
        (*iter)->save(out);
    }
    if(productNumber_) out<<"MULTIPLE"<<" "<<productNumber_<<endl;
   for(vector <ToolUseElement *>::iterator iter = tools_.begin();
          iter != tools_.end(); ++iter)
    {
        out<<"TOOL"<<" ";
        (*iter)->save(out);
    }

}




void    BasicProductionStrategy::extractKnowledge (Entity * recipient, int parameter)
{
}



void BasicProductionStrategy::reportUse(USING_RESULT result, TokenEntity * unit)
{
}

USING_RESULT BasicProductionStrategy::unitMayUse(UnitEntity * unit, SkillRule * skill)
{
    /** We may be in the middle of production. Then we anyway may continue.*/
    /** Otherwise we'll need resources */

    if (!unit->isCurrentlyUsingSkill(skill)) // beginning of production cycle
    {
        if (unit->isTraced())
        {
            cout << "== TRACING " << unit->print() << " starting new production cycle.\n";

        }
        if (checkManaAvailability(unit) == 0) {
            return NO_MANA;
        }

        // if (unit->isTraced())
        // {
        //       cout <<"== TRACING " <<unit->print()<<" unitMayUse calls for resourse check for "<<skill->printTag() <<"\n";
        //       for(vector <ItemElement *>::iterator iter = resources_.begin(); iter != resources_.end(); ++iter)
        //       {
        // 	cout << (*iter)->getItemNumber()<<" "<<(*iter)->getItemType()->printTag()<<" | ";
        //       }
        //       cout << endl;
        // }
        if (checkResourcesAvailability(unit) == 0)
        {
            return NO_RESOURCES;
        }
        else
        {
            consumeResources(unit, 1);
            consumeMana(unit, 1);
            if (getEffectiveProductionRate(unit, skill) < productionDays_)// Production will not be finished today
            {
                unit->addReport(new BinaryMessage(startUseReporter, unit, skill));
            }
            return USING_OK;
        }
    }
    return USING_OK;
}

int BasicProductionStrategy::checkResourcesAvailability(TokenEntity * unit)
{
    int currentProductionCycles;
    int maxProductionCycles = VERY_BIG_NUMBER; // very big number
    if (unit->isTraced())
    {
        cout << "== TRACING " << unit->print() << " resourse check.\n";

    }
    for (vector <ItemElement *>::iterator iter = resources_.begin(); iter != resources_.end(); ++iter)
    {
        currentProductionCycles = unit->hasItem((*iter)->getItemType()) / (*iter)->getItemNumber();
        if (unit->isTraced())
        {
            cout << "== TRACING " << unit->print() << " has " << unit->hasItem((*iter)->getItemType()) << " of " << (*iter)->getItemType()->printTag() << " ==> required for production " << (*iter)->getItemNumber() << "\n";

        }
        if (currentProductionCycles == 0)
            return 0;
        if (iter == resources_.begin())
            maxProductionCycles = currentProductionCycles;
        else
        {
            if (currentProductionCycles < maxProductionCycles)
                maxProductionCycles = currentProductionCycles;
        }
    }
    return maxProductionCycles;
}



bool BasicProductionStrategy::consumeResources(TokenEntity * unit, int numCycles)
{

  for(vector <ItemElement *>::iterator iter = resources_.begin(); iter != resources_.end(); ++iter)
    {
if (unit->isTraced())
{
      cout <<"== TRACING " <<unit->print()<<" consuming "<< (*iter)->getItemNumber() * numCycles<<" of " << (*iter)->getItemType()->printTag();
}
      if((*iter)->getItemNumber() * numCycles != unit->takeFromInventory ((*iter)->getItemType(),(*iter)->getItemNumber() * numCycles ))
      {
if (unit->isTraced())
{
      cout << "numCycles: "<<numCycles<<"  FAILURE"<<endl;
}
      return false;
      }
if (unit->isTraced())
{
      cout << " SUCCESS"<<endl;
}
    }
      return true;
}



void BasicProductionStrategy::printSkillDescription(ostream & out)
{
 if(resources_.empty())
    return;
 else
 out << " Use requires: " ;
 for(vector <ItemElement *>::iterator iter = resources_.begin();
     iter != resources_.end(); ++iter )
     {
       if(iter != resources_.begin())
        out<<", ";
       out<< (*iter)->print();
     }
  out<<".";
}



USING_RESULT  BasicProductionStrategy::checkTarget(UnitEntity * unit, GameData * targetType)
{
   AbstractData * target = unit->getTarget();
// check target type
  // if targetType = SELF any target will work
   if(targetType == &targetTypeSelf)
      return USING_OK;


   if(target==0)             // we may try to find default target
       return NO_TARGET;    //  such as containning construction

  NewEntityPlaceholder * placeholder = dynamic_cast<NewEntityPlaceholder *>(target);
  if(placeholder != 0)  // this is  placeholder.
    {
      GameData* realEntity = placeholder->getRealEntity();
      if(realEntity) // We can get real entity id from placeholder
        {
            if(realEntity->isDescendantFrom(targetType))
              return USING_OK;
            else
              return WRONG_TARGET;
        }
      else   // placeholder is still unresolved. Skill can't be used
              return TARGET_NOT_EXIST;
    }

  //  or some GameData (Entity or rule)  check
  GameData * dataTarget = dynamic_cast<GameData *>(target);
  if(dataTarget ==0)
  {
     // cout<<"Error for "<<unit->print()<<" targeting "<<targetType->print()<<endl;
    return WRONG_TARGET;
  }
  assert(dataTarget);
  if(dataTarget->isDescendantFrom(targetType))
    return USING_OK;
  else
    return WRONG_TARGET;

}


RationalNumber BasicProductionStrategy::getEffectiveProductionRate(UnitEntity * unit, SkillRule * skill)
{
  vector <ToolUseElement *>::iterator iter;
	int bonus = calculateProductionBonus(unit,skill);
  RationalNumber effectiveProductionRate (unit->getFiguresNumber() *(100 + bonus),100);


// Tools accelerate production
  for(iter = tools_.begin(); iter != tools_.end();iter++)
  {
    RationalNumber toolBonus((*iter)->getBonus() * unit->hasEquiped( (*iter)->getItemType()),100);
    effectiveProductionRate =  effectiveProductionRate + toolBonus;//(*iter)->getBonus() * unit->hasEquiped( (*iter)->getItemType())/100;
  }
	return effectiveProductionRate;
}




// process production, consume resources if nescessary
// 1 portion of resources is consumed during a call of BasicProductionStrategy::unitMayUse
USING_RESULT BasicProductionStrategy::produce(UnitEntity * unit, SkillRule * skill, int & useRestrictionCounter, int & effectiveProduction, OrderLine * order, bool & newCycle)
{
  if(useRestrictionCounter < 0)  // former restriction counter already expired 
    return CANNOT_USE;
        
  USING_RESULT result;
// Production modifiers:
 RationalNumber effectiveProductionRate = getEffectiveProductionRate(unit,skill);


  SkillUseElement * dailyUse = new SkillUseElement(skill,effectiveProductionRate,productionDays_);

  int cycleCounter  = unit->addSkillUse(dailyUse);

 if(cycleCounter == 0) // In the middle of the production cycle
  {
		 return USING_IN_PROGRESS;
  }

  else // The old  production cycle is finished. Do we want to start new?
  {
    if (unit->isTraced())
  {
      cout <<"== TRACING " <<unit->print()<<" The old  production cycle is finished. Do we want to start new?\n";
  }
    int resourcesAvailable = checkResourcesAvailability(unit);
    int manaAvailable = checkManaAvailability(unit);

  if (unit->isTraced())
  {
      cout <<"== TRACING " <<unit->print()<<" cycleCounter= "<<cycleCounter<<" produce calls for resourse check. "<<resourcesAvailable<<" available\n";
  }
    if(resourcesAvailable == 0)
    {
            if (unit->isTraced())
    {
      cout <<"== TRACING " <<unit->print()<<" The old  production cycle is finished. No Resources\n";
    }
      effectiveProduction = cycleCounter * productNumber_;
      return NO_RESOURCES;
    }
   if(manaAvailable == 0)
    {
      effectiveProduction = cycleCounter * productNumber_;
      result = NO_MANA;
            if (unit->isTraced())
        {
            cout <<"== TRACING " <<unit->print()<<" The old  production cycle is finished. No Mana\n";
        }
      return NO_MANA;//?
    }
    
    if( resourcesAvailable  < cycleCounter) 
        cycleCounter = resourcesAvailable;

    if( manaAvailable < cycleCounter)
        cycleCounter = manaAvailable;
    


    effectiveProduction = cycleCounter * productNumber_;
  if (unit->isTraced())
  {
      cout <<"== TRACING " <<unit->print()<< " ==> producing "<< productNumber_<<" ->"<<effectiveProduction <<"   resourses: "<<resourcesAvailable <<" restr= "<< useRestrictionCounter<<"\n";
  }

    if( (useRestrictionCounter > 0) && (effectiveProduction  >= useRestrictionCounter) ) // limited number of new cycles
      {
        effectiveProduction = useRestrictionCounter;
        cycleCounter = (effectiveProduction + productNumber_ -1)/ productNumber_;// ??? why?
        if(cycleCounter >1)
		{
		  consumeMana(unit,cycleCounter);
		  consumeResources(unit,cycleCounter);
		}
             if (unit->isTraced())
  {
      cout <<"== TRACING " <<unit->print()<<" Number of items to produce is limited to "
        << effectiveProduction<< " cycleCounter= " << cycleCounter<<" no new cycle"<<endl;
  }
           result = USING_COMPLETED;
        useRestrictionCounter = -1;
        order->setCompletionFlag(true);
      }

    else
    {
             if (unit->isTraced())
  {
      cout <<"== TRACING " <<unit->print()<<" Starting new cycle "
        << " cycleCounter= " << cycleCounter<<endl;
  }
        if(dailyUse->getDaysUsed() > 0)
        {
	consumeMana(unit,cycleCounter);
        consumeResources(unit,cycleCounter);
        newCycle = true;
        unit->addSkillUse(dailyUse);
        }
        if( useRestrictionCounter != 0)
          useRestrictionCounter = useRestrictionCounter - effectiveProduction;
        result = USING_IN_PROGRESS;
    }

}
return result;
}



int BasicProductionStrategy::checkManaAvailability(UnitEntity * unit)
{
  if(mana_)
		return unit->hasMana()/mana_;
	else
          return VERY_BIG_NUMBER;
}




bool BasicProductionStrategy::consumeMana(UnitEntity * unit, int numCycles)
{
 if(mana_ != 0)
	unit->setMana(unit->hasMana() - mana_ * numCycles);
 return true;
}

//void BasicProductionStrategy::operator = (BasicProductionStrategy& data)
//{

//   productionDays_ = data.productionDays_;
//   resources_ = data.resources_;
//   productNumber_ = data.productNumber_;
//   tools_ = data.tools_;
//	 mana_ = data.mana_;
//}

BasicUsingStrategy * BasicProductionStrategy::cloneSelf()
{
 BasicProductionStrategy * copyOfSelf = new BasicProductionStrategy(keyword_,parent_);
 *copyOfSelf = *this;

 return copyOfSelf;
}
