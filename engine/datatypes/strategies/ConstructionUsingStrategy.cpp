/***************************************************************************
                          ConstructionUsingStrategy.cpp 
                             -------------------
    begin                : Wed Sep 3 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "ConstructionUsingStrategy.h"
#include "ConstructionRule.h"
#include "RulesCollection.h"
#include "ItemRule.h"
#include "UnitEntity.h"
#include "TokenEntity.h"
#include "SkillUseElement.h"
#include "BasicCondition.h"
#include "BinaryPattern.h"
#include "UnaryPattern.h"
#include "LocationEntity.h"
#include "ConstructionEntity.h"
#include "EntitiesCollection.h"
#include "GameInfo.h"
#include "StringData.h"

extern EntitiesCollection <ConstructionEntity>  buildingsAndShips;
extern RulesCollection <ConstructionRule>      constructions;
extern RulesCollection    <ItemRule>     items;
extern GameInfo game;
extern Reporter * newBuidingStartedReporter;
extern Reporter * buidingFinishedReporter;
extern Reporter * constructionStartedReporter;


ConstructionUsingStrategy::ConstructionUsingStrategy ( const ConstructionUsingStrategy * prototype ): BasicUsingStrategy(prototype)
{
  multiplicationFactor_ = 1;
  construction_ = 0;
  productionDays_ = 1;
  resourceType_ = 0;
  resourceNumber_ =1;
  constructionToUpgrade_ = 0;
}

GameData * ConstructionUsingStrategy::createInstanceOfSelf()
{
  return CREATE_INSTANCE<ConstructionUsingStrategy> (this);
}



STATUS
ConstructionUsingStrategy::initialize        ( Parser *parser )
{
  if (parser->matchKeyword ("PRODUCES") )
    {
      string tag = parser->getWord();
      construction_ = constructions[tag];
      if(construction_ == 0 )
      {
        cout << "Unknown construction type to produce: "<< tag<<" \n";
        return IO_ERROR;
      }
      productionDays_ =  parser->getInteger();
      if(productionDays_ == 0)
             productionDays_ = 1; // default 1 day to start construction
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

  if (parser->matchKeyword ("UPGRADE") )
    {
      string tag = parser->getWord();
      constructionToUpgrade_ = constructions[tag];
      if(constructionToUpgrade_ == 0 )
      {
        cout << "Unknown construction type to upgrade: "<< tag<<" \n";
        return IO_ERROR;
      }
      return OK;
    }
      return OK;
}


USING_RESULT ConstructionUsingStrategy::unitUse(UnitEntity * unit, SkillRule * skill, int & useCounter)
{
  SkillUseElement * dailyUse = new SkillUseElement(skill,unit->getFiguresNumber(),productionDays_);
  ConstructionEntity * newBuilding;
  if(unit->addSkillUse(dailyUse) >= 1)
  {
    newBuilding = construction_->startConstruction(unit);
    if(!newBuilding)
          return UNUSABLE;
    if(newBuilding->isCompleted())
      {
        newBuilding->buildingCompleted();
        unit->addReport( new BinaryPattern
                          (buidingFinishedReporter, construction_,
                              new StringData(newBuilding->printTag())));
        unit->getLocation()->addReport( new BinaryPattern
                          (buidingFinishedReporter, construction_,
                              new StringData(newBuilding->printTag())));
      }
    else
      {
        unit->addReport( new BinaryPattern
                          (newBuidingStartedReporter, construction_,
                              new StringData(newBuilding->printTag())));
        unit->getLocation()->addReport( new BinaryPattern
                          (newBuidingStartedReporter, construction_,
                              new StringData(newBuilding->printTag())));
      }
      
    // If we used placeholder as target now it's time to set it to real construction
    AbstractData * target = unit->getTarget();
    NewEntityPlaceholder * placeholder = dynamic_cast<NewEntityPlaceholder *>(target);
    if(placeholder)
      {
        Entity * newEntity = placeholder->getRealEntity();
        if (!newEntity)
          {
            placeholder->setRealEntity(newBuilding);
            //unit->setTarget(newBuilding);
          }
        else
          {
                //report placeholder duplication ?
          }
        }
    
    unit->getCurrentOrder()->setCompletionFlag(true);
    return USING_COMPLETED;
  }
  return USING_IN_PROGRESS;
}



USING_RESULT ConstructionUsingStrategy::unitMayUse(UnitEntity * unit, SkillRule * skill)
{
/** We may be in the middle of construction. Then we anyway may continue.*/
/** Otherwise we'll need resources */
   BasicCondition * buildCondition = construction_->getBuildCondition();
  if(buildCondition)
  { 
    if(buildCondition->isSatisfied(unit))
      {
//           report condition
          cout << "CONDITION FAILED: "<<construction_->getBuildCondition()->print() <<endl;
          return CONDITION_FAILURE;
      }
  }

 if (unit->isCurrentlyUsingSkill(skill))
    return  USING_OK;

 if( !unit->getLocation()->getTerrain()->mayBuild())
      {
          return CANNOT_USE;
      }
 if(unit->hasItem(resourceType_) < resourceNumber_)
      {
          return NO_RESOURCES;
      }
 if (!unit->mayBuild(construction_))
      {
          return CANNOT_USE;
      }
 if(unit->getLocation()->getLandPrice())
      {
        unit->pay(unit->getLocation()->getLandPrice() * construction_->getLandUse());
        // transfer money to owner.  - to title-residence building.
        }
 unit->getLocation()->useLand(construction_->getLandUse());

 unit->takeFromInventory(resourceType_, resourceNumber_);
 
 unit->addReport( new UnaryPattern (constructionStartedReporter, construction_));

  return  USING_OK;
}



void ConstructionUsingStrategy::printSkillDescription(ostream & out)
{
  if(!resourceType_)
    {
      out << " Use requires: " << resourceNumber_;
      if(resourceNumber_ > 1)
        out << resourceType_->getPluralName()<< " " << resourceType_->printTag();
      else
      out << resourceType_->print();
 
      out<<".";
    }

 if(construction_)
    return;
 out << " Use produces: " << construction_->print()<<" in "
    << productionDays_ <<" days.";
}



void    ConstructionUsingStrategy::extractKnowledge (Entity * recipient, int parameter)
{
  if(construction_)
  {
    if(recipient->addKnowledge(construction_))
      construction_->extractKnowledge(recipient);
  }

  if(constructionToUpgrade_)
  {
    if(recipient->addKnowledge(constructionToUpgrade_))
      constructionToUpgrade_->extractKnowledge(recipient);
  }

}
