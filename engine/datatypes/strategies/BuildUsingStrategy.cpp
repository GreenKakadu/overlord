/***************************************************************************
                          BuildUsingStrategy.cpp
                             -------------------
    begin                : Thu Feb 20 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "BuildUsingStrategy.h"
#include "ItemRule.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "ConstructionEntity.h"
#include "ConstructionRule.h"
#include "ConstructionWorksVariety.h"
#include "ConstructionWorksElement.h"
#include "ItemElement.h"
#include "SkillUseElement.h"
#include "ToolUseElement.h"
#include "EntitiesCollection.h"
#include "RulesCollection.h"
#include "VarietiesCollection.h"
#include "OrderPrototype.h"
#include "TargetOrder.h"
#include "BinaryMessage.h"
#include "StringData.h"

extern VarietiesCollection   <ConstructionWorksVariety>  construction_works;
extern EntitiesCollection <ConstructionEntity>  buildingsAndShips;
extern RulesCollection <ConstructionRule>      constructions;
extern ReportPattern * newBuidingStartedReporter;
extern ReportPattern * buidingFinishedReporter;

GameData * BuildUsingStrategy::createInstanceOfSelf()
{
  return CREATE_INSTANCE<BuildUsingStrategy> (this);
}



BuildUsingStrategy::BuildUsingStrategy ( const BuildUsingStrategy * prototype ): BasicProductionStrategy(prototype)
{
  constructionWorkProduced_ =0;
}



STATUS
BuildUsingStrategy::initialize        ( Parser *parser )
{
  if (parser->matchKeyword ("PRODUCES") )
    {
      constructionWorkProduced_ = construction_works[parser->getWord()];
      productionDays_ =  parser->getInteger();
      if( (constructionWorkProduced_ == 0) ||  (productionDays_ == 0) )
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






USING_RESULT BuildUsingStrategy::unitUse(UnitEntity * unit, SkillRule * skill, int &useCounter)
{
  ConstructionEntity * construction =  dynamic_cast<ConstructionEntity *>(unit->getTarget());
//  buildingsAndShips[unit->getTarget()->getTag()];
  assert(construction);

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

  else // The old  production cycle is finished. Do we want to start new?
  {
    int resourcesAvailable = checkResourcesAvailability(unit);
    int workToDo = construction->workToDo(constructionWorkProduced_);
    if( resourcesAvailable < cycleCounter)
        cycleCounter = resourcesAvailable;
    int effectiveProduction = cycleCounter * productNumber_;
    if( effectiveProduction  >= workToDo) // building will be finished today
      {
        effectiveProduction = workToDo;
        cycleCounter = (effectiveProduction + productNumber_ -1)/ productNumber_;
        if(cycleCounter >1)
          consumeResources(unit,cycleCounter-1);

        if (construction->addBuildingWork(
                new ConstructionWorksElement(constructionWorkProduced_,
                                                effectiveProduction)))
            {
              construction->buildingCompleted();
              unit->getLocation()->addReport(
                  new BinaryMessage(buidingFinishedReporter, construction->getConstructionType(),
                              new StringData(construction->printTag())));
              // private
              unit->addReport(
                  new BinaryMessage(buidingFinishedReporter, construction->getConstructionType(),
                              new StringData(construction->printTag())));
            }
        return USING_COMPLETED;
      }
    else
      {
        consumeResources(unit,cycleCounter-1);
        if(dailyUse->getDaysUsed() > 0)
          {
            unit->addSkillUse(dailyUse);
            consumeResources(unit,1); // new cycle started
            }
        construction->addBuildingWork(
                new ConstructionWorksElement(constructionWorkProduced_,
                                                effectiveProduction));
        return USING_IN_PROGRESS;
      }
    }
}




USING_RESULT BuildUsingStrategy::unitMayUse(UnitEntity * unit, SkillRule * skill)
{
/** We may be in the middle of construction. Then we anyway may continue.*/
/** Otherwise we'll need resources */
   AbstractData * target = unit->getTarget();
   // target may be:
   // existing construction
   // construction type to build
   // existing placeholder
   // new placeholder
   // 0
   ConstructionEntity * construction =  dynamic_cast<ConstructionEntity *>(unit->getTarget());
   if(!construction)  // No such entity. May be This is a rule?
    {
      ConstructionRule * constructionType =  dynamic_cast<ConstructionRule *>(unit->getTarget());

      if(constructionType)
        {
              // check that building need this work
               if(!constructionType->getResourceRequirement(constructionWorkProduced_))
                  return  WRONG_TARGET;
              // create new entity
              if(checkResourcesAvailability(unit) == 0)
                {
                  return NO_RESOURCES;
                }
              if( !unit->getLocation()->getTerrain()->mayBuild())
                {
                  return CANNOT_USE;
                }
              else
                {
                  ConstructionEntity * newBuilding = constructionType->startConstruction(unit);
                  if(!newBuilding)
                      return UNUSABLE;
                  if (!unit->mayBuild(constructionType))
                      {
                        return NO_RESOURCES;
                      }
                  if(unit->getLocation()->getLandPrice())
                      {
                        unit->pay(unit->getLocation()->getLandPrice() *
                                          constructionType->getLandUse());
                        // transfer money to owner - to title-residence building.
                      }
                  unit->getLocation()->useLand(constructionType->getLandUse());

                  unit->setTarget(newBuilding);
                  consumeResources(unit,1);
                  unit->getLocation()->addReport(
                      new BinaryMessage(newBuidingStartedReporter, constructionType,
                                        new StringData(newBuilding->printTag())));
                  //  private report
                  unit->addReport(
                      new BinaryMessage(newBuidingStartedReporter, constructionType,
                                        new StringData(newBuilding->printTag())));
                  return  USING_OK;
                }
        }
       else // this is not a rule.May be it is NewEntityPlaceholder?
       {
          NewEntityPlaceholder * placeholder = dynamic_cast<NewEntityPlaceholder *>(target);
          if(placeholder)
              {
                 Entity * newEntity = placeholder->getRealEntity();
                 if (newEntity)
                  {
                    construction = dynamic_cast<ConstructionEntity *>(newEntity);
                    assert(construction);
                    unit->setTarget(construction);
                  }

              }
       }

    }

  // still no target identified. Try to use  ContainingConstruction as a target

  if(!construction)
    {
      construction = unit->getContainingConstruction();
      if(!construction)
        {
          return  TARGET_NOT_EXIST;
        }
      if(construction->isCompleted())
        return  TARGET_NOT_EXIST;
      else
        unit->setTarget(construction);
    }
 //=========================================================
    if(construction->isCompleted())
        return  WRONG_TARGET;

    if(!construction->getConstructionType()-> getResourceRequirement(constructionWorkProduced_))
        return  WRONG_TARGET;


 if (!unit->isCurrentlyUsingSkill(skill)) // beginning of production cycle
  {
    if(checkResourcesAvailability(unit) == 0)
      {
        return NO_RESOURCES;
      }
     else
     {
        consumeResources(unit,1);
        return  USING_OK;
     }
  }
    return  USING_OK;

}



void BuildUsingStrategy::printSkillDescription(ostream & out)
{
  BasicProductionStrategy::printSkillDescription(out);

 if(!constructionWorkProduced_)
    return;
 else

 out << " Use produces: " << productNumber_<<" ";
 if(productNumber_ > 1)
  out << constructionWorkProduced_->getPluralName()<< " " << constructionWorkProduced_->printTag();
 else
  out << constructionWorkProduced_->print();

  out<<" in "<< productionDays_ <<" days.";
}



void    BuildUsingStrategy::extractKnowledge (Entity * recipient, int parameter)
{

}

