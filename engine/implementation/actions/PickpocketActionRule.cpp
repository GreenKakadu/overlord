/***************************************************************************
                          PickpocketActionRule.cpp 
                             -------------------
    begin                : Tue Jan 20 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "TokenEntity.h"
#include "SkillRule.h"
#include "LocationEntity.h"
#include "PickpocketActionRule.h"
#include "UnaryPattern.h"
#include "BinaryPattern.h"

extern ActionRule    sampleAction;
extern Reporter * pickPocketNoTargetReporter;
extern Reporter * pickPocketEmptyPocketReporter;
extern Reporter * pickPocketStealingFailureReporter;
extern Reporter * pickPocketStealingPrivateReporter;
extern Reporter * pickPocketStealingTargetReporter;

PickpocketActionRule     samplePickpocketActionRule =     PickpocketActionRule("PICKPOCKET_ACTION", &sampleAction);
PickpocketActionRule::PickpocketActionRule ( const PickpocketActionRule * prototype ) : ActionRule(prototype)
{
}


GameData * PickpocketActionRule::createInstanceOfSelf()
{
  return CREATE_INSTANCE<PickpocketActionRule> (this);
}




ACTION_RESULT PickpocketActionRule::carryOut(Entity * entity)
{
  TokenEntity * thief = dynamic_cast<TokenEntity *>(entity);
  assert(thief);
  TokenEntity * target= dynamic_cast<TokenEntity *>(thief->getTarget());
  assert(target);
  
//  cout << "PickPocket Attempt: " << thief<< " stealing from " <<target <<endl;
  
  if(thief->getGlobalLocation() == 0 || thief->getGlobalLocation() != target->getGlobalLocation())
  {
//    cout << "==PP==> Failed to find target"<<endl;
      thief->addReport(new UnaryPattern(pickPocketNoTargetReporter, target));
    return ACTION_FAILURE;
  }
  vector < InventoryElement *> & inventory = target->getAllInventory();
  InventoryElementIterator currentPick = inventory.end();
  
  // select one small non-coin non-equipped item  with best (price - weight * 25 ) value
  for (InventoryElementIterator iterEquip = inventory.begin();
                    iterEquip != inventory.end(); iterEquip++)
    {
//      cout << "==PP==> Iterating through inventory. Current item is "<< *(*iterEquip)<<endl;
      if((*iterEquip)->getItemNumber() <= (*iterEquip)->getEquipedNumber())
        {
          continue;
        }
      if( (*iterEquip)->getItemType() == cash)
        {
          continue;
        }
       
      if( currentPick == inventory.end())
          currentPick = iterEquip;
      else
        {
               
//      cout << "==PP==> stealing weight of current item "<< (*iterEquip)->getItemType()->getFormalPrice() -
//              (*iterEquip)->getItemType()->getWeight() * 25<< " comparable with "<< (*currentPick)->getItemType() ->getFormalPrice() -
//              (*currentPick)->getItemType()->getWeight() * 25<<endl;
              
        if(
            (
              (*iterEquip)->getItemType()->getFormalPrice() -
              (*iterEquip)->getItemType()->getWeight() * 25
            )
              >
            (
              (*currentPick)->getItemType() ->getFormalPrice() -
              (*currentPick)->getItemType()->getWeight() * 25
            )
          )
       currentPick = iterEquip;
       }     
    }
  if(currentPick == inventory.end())
  {
//      cout << "==PP==> Failed to find anything in pockets" <<endl;
      thief->addReport(new UnaryPattern(pickPocketEmptyPocketReporter, target)
        /*, 0, observation condition*/);
    return ACTION_FAILURE;
  }  
    ItemRule * currentItem = (*currentPick)->getItemType();
//      cout << "==PP==> Selected Item is "<< *(currentItem)<<endl;
  //calculate chances
  int chance = 5 * thief->getStealth();
//      cout << "==PP==> Initial chance "<<chance <<endl;
  int skillLevel = thief->getSkillLevel(skills["ppoc"]);
  if(skillLevel > 1)
        chance += thief->getStealth();
//      cout << "==PP==> Chance after level correction"<<chance <<endl;
  chance -=  3 * target->getGlobalLocation()->getFactionalObservation( target->getFaction());     

//  cout << "==PP==> Factional observation is "<<target->getGlobalLocation()->getFactionalObservation( target->getFaction()) <<endl;
//  cout << "==PP==> Chance after Factional observation correction"<<chance <<endl;

  // really get alliance + friendly + neutral guards observation
  // guards should attack
  if(skillLevel < 5)
    chance -= (4 - skillLevel) * currentItem->getWeight();

//  cout << "==PP==> Chance after weight correction"<<chance <<endl;

   int random = rand() % 100;
//   cout << "==PP==> Chance "<<chance <<" vs. "<<random<<endl;
  if( chance <= 0 || chance < random)
//  if( chance <= 0 || chance < rand() % 100)
  {
//      cout << "==PP==> Failed to steal" <<endl;
      thief->addReport(new UnaryPattern(pickPocketStealingFailureReporter, target)
        /*, 0, observation condition*/);

    // calculate chance of attack
    return ACTION_FAILURE;
  }
  else
  {
//      cout << "==PP==> Stole "<< *(currentItem)<< " from "<<target<<endl;
    target->takeFromInventory( currentItem,1);
    thief->addToInventory(currentItem,1);
    thief->addReport(new BinaryPattern(pickPocketStealingPrivateReporter, currentItem, target));
    target->addReport(new BinaryPattern(pickPocketStealingTargetReporter, currentItem, target));
    return     ACTION_SUCCESS;
  }

}
