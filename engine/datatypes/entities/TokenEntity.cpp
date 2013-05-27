/***************************************************************************
TokenEntity.cpp
-------------------
begin                : Fri Jul 25 2003
copyright            : (C) 2003 by Alex Dribin
email                : Alex.Dribin@gmail.com
***************************************************************************/

/***************************************************************************
*                                                                                            *
*   This program is free software; you can redistribute it and/or   *
*  modify it under the terms of the BSD License.                       *
*                                                                                            *
***************************************************************************/
#include <algorithm>
#include "TokenEntity.h"
#include "GameFacade.h"
#include "TeachingOffer.h"
#include "UnitEntity.h"
#include "ConstructionEntity.h"
#include "FactionEntity.h"
#include "LocationEntity.h"
#include "SimpleMessage.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "IntegerData.h"
#include "TerrainRule.h"
#include "WeatherRule.h"
#include "SeasonRule.h"
#include "SkillUseElement.h"
#include "TravelElement.h"
#include "OrderPrototype.h"
#include "StanceVariety.h"
#include "BasicOrderSynchronizationRequest.h"
#include "BasicLearningStrategy.h"
#include "ObservationCondition.h"
#include "DataManipulator.h"
#include "TargetOrder.h"
#include "BasicCombatManager.h"
#include "CombatStanceVariety.h"
#include "BattleInstance.h"
#include "CombatOrderLine.h"
#include "BattleField.h"
#include "CombatOrder.h"
#include "CombatMoveVariety.h"
//extern DataManipulator * dataManipulatorPtr;
extern ReportPattern * newLevelReporter;
extern ReportPattern * maxLevelReporter;
extern ReportPattern *	cannotOathYourselfReporter;
extern ReportPattern *	oathRejectedReporter;
extern ReportPattern *	oathReporter;
extern ReportPattern * enterPrivateReporter;
extern ReportPattern * enterPublicReporter;
extern ReportPattern * leavePrivateReporter;
extern ReportPattern * leavePublicReporter;
extern ReportPattern *	AttackedReporter;
ReportPattern * lootReporter = new ReportPattern("lootReporter");
extern SkillRule * combatSkill;
ReportPattern *	 movementBlockedReporter      = new ReportPattern("movementBlockedReporter");
TokenEntity sampleTokenEntity  ("PHYSICAL_ENTITY",  &sampleEntity);

TokenEntity::TokenEntity (const string & keyword, GameData * parent ) : Entity(keyword, parent)
{
  location_  = 0;
  combatTactics_.defaultInitialization();
  // cerr << "---TokenEntity constructor";
  // combatTactics_.report(cerr);
  // cerr <<endl;
  faction_ = 0;
  toOath_ = 0;
  traced_ = false;
  moving_ = 0;
  isMoving_ = false;
  guarding_ = false;
  passenger_ = false;
  advertising_ = false;
  announcing_ = false;
  fanatic_ = false;
  sharing_ = true;
  alive_ = true;
  withdrawingSupport_ = true;
  target_ = 0;
  defaultTarget_ = 0;
  battleInstance_ = 0;
  defaultCombatMovement_ = 0;
  inventory_ = InventoryAttribute(this);
  loyality_ = 100;
  isDisbanded_ = false;
  lastFaction_ =0;
}



TokenEntity::TokenEntity(const TokenEntity * prototype): Entity(prototype)
{
  location_  = 0;
  combatTactics_.defaultInitialization();
  // cerr << "---TokenEntity constructor";
  // combatTactics_.report(cerr);
  // cerr <<endl;
  faction_ = 0;
  lastFaction_ =0;
  toOath_ = 0;
  moving_ = 0;
  isMoving_ = false;
  guarding_ = false;
  passenger_ = false;
  advertising_ = false;
  announcing_ = false;
  fanatic_ = false;
  sharing_ = true;
  alive_ = true;
  withdrawingSupport_ = true;
  target_ = 0;
  defaultTarget_ = 0;
  battleInstance_ = 0;
  defaultCombatMovement_ = 0;
  inventory_ = InventoryAttribute(this);
  loyality_ = 100;
  isDisbanded_ = false;
}


// members that are not pointers may need explicit initialization
// for the cases when entities are created on-the-fly (like summoning new unit)
void TokenEntity::explicitInitialization()
{
  combatTactics_.defaultInitialization();
  inventory_.init(this);
  isPayingUpkeep_ = true;
  // cerr << "---TokenEntity explicit initialization of CombatTactics";
  // combatTactics_.report(cerr);
  // cerr <<endl;
}



bool      TokenEntity::defaultAction()
{
  if(isUnaccessible())
    return false;
  
  if(isFullDayOrderFlagSet())
    return false;
  else
    return false;
}



STATUS TokenEntity::initialize        ( Parser *parser )
{
  
  if (parser->matchKeyword("FACTION"))
  {
    faction_ = gameFacade->factions[ parser->getWord()];
    
    return OK;
  }
  
  
  if (parser->matchKeyword("TARGET"))
  {
    string tag = parser->getWord();
    if (tag.size() != 0)
    {
      target_ = TargetOrder::findTarget(tag);
    }
    return OK;
  }
  
  
  if (parser->matchKeyword("MOVING"))
  {
    moving_ = TravelElement::readElement(parser);
    isMoving_ = true;
    return OK;
  }
  
  if (parser->matchKeyword("SKILL_USE"))
  {
    SkillUseElement * newSkillUse = SkillUseElement::readElement(parser);
    if(newSkillUse)
      skillUse_.push_back(newSkillUse);
    return OK;
  }
  
  if (parser->matchKeyword("LOYALITY"))
  {
    loyality_ = parser->getInteger(); 
    return OK;
  }
  
  if (parser->matchKeyword("SILENT"))
  {
    silent_ = true;
    return OK;
  }
  
  if (parser->matchKeyword("NO_SHARING"))
  {
    sharing_ = false;
    return OK;
  }
  if (parser->matchKeyword("ANNOUNCE"))
  {
    announcing_ = true;
    return OK;
  }
  if (parser->matchKeyword("ADVERTISE"))
  {
    advertising_ = true;
    return OK;
  }
  if (parser->matchKeyword("NO_SUPPORT"))
  {
    withdrawingSupport_ = false;
    return OK;
  }
  if (parser->matchKeyword("COMBAT_ORDER"))
  {
    combatOrders_.push_back(new CombatOrderLine(parser->getText(),this));
  }
  if (parser->matchKeyword("COMBAT"))
  {
    addCombatSetting(parser->getText());
  }

  skills_.initialize(parser);
  inventory_.initialize(parser);
  combatTactics_.initialize(parser);
  return Entity::initialize(parser);
  
}



void TokenEntity::save(ostream &out)
{
  Entity::save(out);

  if(faction_) out << "FACTION " << faction_->getTag()<<endl;
  if(silent_) out << "SILENT"<<endl;
  if(announcing_) out << "ANNOUNCE"<<endl;
  if(advertising_) out << "ADVERTISE"<<endl;
  if(!withdrawingSupport_) out << "NO_SUPPORT"<<endl;
  if(!sharing_) out << "NO_SHARING"<<endl;
  //  TokenEntity * temp = this;
  combatTactics_.save(out);
  
  for (vector <string>::iterator iter = combatSettings_.begin();
  iter != combatSettings_.end(); iter++)
  {
    out << "COMBAT " <<(*iter)<<endl;
  }
  
  
  for (CombatOrderIterator iter = combatOrders_.begin();
  iter != combatOrders_.end(); iter++)
  {
    (*iter)->save(out);
  }
  
  inventory_.save(out);
  skills_.save(out);
  
  for (SkillUseIterator iter= skillUse_.begin(); iter != skillUse_.end(); iter++)
  {
    out << "SKILL_USE ";
    (*iter)->save(out);
  }
  
  out <<"LOYALITY "<<loyality_<<endl;
  
  if(target_)
  {
    out << "TARGET "; target_->saveAsParameter(out); out<< endl;
  }
}

void TokenEntity::save(ostream &out,string prefix)
{
  Entity::save(out,prefix);
  //  out << keyword_ << " " <<tag_ << endl;
  //  if(!name_.empty()) out << "NAME " <<name_ << endl;
  //  if(!description_.empty()) out << "DESCRIPTION " <<description_  << endl;
  if(faction_) out << prefix<< "FACTION " << faction_->getTag()<<endl;
  if(silent_) out << prefix<< "SILENT"<<endl;
  if(announcing_) out << prefix<< "ANNOUNCE"<<endl;
  if(advertising_) out << prefix<< "ADVERTISE"<<endl;
  if(!withdrawingSupport_) out << prefix<< "NO_SUPPORT"<<endl;
  if(!sharing_) out << prefix<< "NO_SHARING"<<endl;
  //  TokenEntity * temp = this;
  combatTactics_.save(out,prefix);

  for (vector <string>::iterator iter = combatSettings_.begin();
  iter != combatSettings_.end(); iter++)
  {
    out << prefix<< "COMBAT " <<(*iter)<<endl;
  }


  for (CombatOrderIterator iter = combatOrders_.begin();
  iter != combatOrders_.end(); iter++)
  {
    (*iter)->save(out,prefix);
  }

  inventory_.save(out,prefix);
  skills_.save(out,prefix);

  for (SkillUseIterator iter= skillUse_.begin(); iter != skillUse_.end(); iter++)
  {
    out << prefix<< "SKILL_USE ";
    (*iter)->save(out,prefix);
  }

  out << prefix<<"LOYALITY "<<loyality_<<endl;

  if(target_)
  {
    out << prefix<< "TARGET "; target_->saveAsParameter(out); out<< endl;
  }
}



// Create almost deep copy of game object

TokenEntity *  TokenEntity::makePrivateImage()
{
    TokenEntity * image = dynamic_cast<TokenEntity *> (this->createInstanceOfSelf());
    copyPrivateImage(image, this);
    return image;
}

void  TokenEntity::copyPrivateImage(TokenEntity * image, TokenEntity * source)
{
    image->setTag(source->getTag());
    image->setName(source->getName());
    image->setDescription(source->getDescription());
    if (moving_)
    {
        image->moving_ = source->moving_->clone();
    }
    else
    {
        image->moving_ = 0;
    }
    image->location_ = source->location_;



    image->setFaction(source->getFaction());
    image->setSilent(source->isSilent());
    image->setAnnouncing(source->getAnnouncing());
    image->setAdvertising(source->getAdvertising());
    image->setWithdrawingSupport(source->getWithdrawingSupport());
    image->setSharing(source->getSharing());

    image->combatTactics_ = source->combatTactics_;
    image->combatSettings_ = source->combatSettings_;
    image->combatOrders_ = source->combatOrders_;

    image->skills_ = source->skills_;
    image->skillUse_ = source->skillUse_;
    image->loyality_ = source->loyality_;
    image->target_ = source->target_;
    image->inventory_ = source->inventory_;
    image->enchantments_ = source->enchantments_;
    image->orders_ = source->orders_;
    image->imageLevel_ = PRIVATE_IMAGE;
    image->isImage_ = true;
    image->setTimeStamp(gameFacade->getGameTurn(), gameFacade->getCurrentDay());
    image->imageObservation_ =VERY_BIG_NUMBER;
}



void TokenEntity::makeAlliedImage(TokenEntity * source)
{
    setFaction(source->getFaction());
    inventory_ = source->inventory_;
    imageLevel_ = ALLIED_IMAGE;
}


void TokenEntity::makeObservedImage(TokenEntity * source)
{
    setFaction(source->getFaction());
    inventory_ = source->inventory_.getObservableImage();
    imageLevel_ = OBSERVABLE_IMAGE;
    //inventory_.combineInventories(&(token->inventory_));
}
long wcounter2 = 0;



// Create token object as it is seen by referent
// Take into account observation. (imageLevel = obse)
// for allied units effective observation may be higher

TokenEntity * TokenEntity::createTokenImage(FactionEntity * referent, int observation)
{
    TokenEntity * image;
    wcounter2++;
//    if (wcounter2 % 1000 == 0)
//        cerr << "Day " << gameFacade->getCurrentDay() << " createTokenImage  counter = " << wcounter2 / 1000 << ",000" << endl;
    if (referent == this->getFaction())
    {
       return makePrivateImage();
  //       image = dynamic_cast<TokenEntity *> (this->createInstanceOfSelf());
 //       makePrivateImage(image, this);
 //       return image;

    }

    // Not seen if stealth is higher than observation and not adverticind and not ally
    if (observation < this->getStealth() && !this->getAdvertising()
            && (!referent->stanceAtLeast(this->getFaction(), alliedStance)))
    {
        return 0;
    }


    image = dynamic_cast<TokenEntity *> (this->createInstanceOfSelf());



    image->imageObservation_ = observation;
    image->setTag(this->getTag());
    image->setName(this->getName());
    image->setDescription(this->getDescription());
    if (moving_)
    {
        image->moving_ = this->moving_->clone();
    }
    else
    {
        image->moving_ = 0;
    }
    image->location_ = this->location_;


    image->isImage_ = true;
    image->combatTactics_.undefine();
    image->setTimeStamp(gameFacade->getGameTurn(), gameFacade->getCurrentDay());
    //--------------------------------------------------------


    if (referent->stanceAtLeast(this->getFaction(), alliedStance))
    {
        image->makeAlliedImage(this);
    }
    else // Not allied
    {
        if (image->imageObservation_ > this->getStealth()
                || this->getAdvertising())
        {
            image->makeObservedImage(this);
        }
        else
        {
            image->inventory_ = this->inventory_.getObservableImage();
            this->imageLevel_ = UNKNOWN_IMAGE;
        }

    }

    return image;
}

// Update token image with the data from another image
void TokenEntity::updateImage(TokenEntity *tokenToAdd)
{
  Entity::updateImage(tokenToAdd);

  if(this->getTimeStamp() < tokenToAdd->getTimeStamp())      // Image is newer
    {
      if(tokenToAdd->getFaction())
        {
          setFaction(tokenToAdd->getFaction());
        }
       moving_ =  tokenToAdd->moving_;
       skillUse_ = tokenToAdd->skillUse_;
       this->loyality_ = tokenToAdd->loyality_;

      if(tokenToAdd->getImageLevel() == PRIVATE_IMAGE)
          {
            target_ = tokenToAdd->target_;
            this->silent_ = tokenToAdd->silent_;
            this->sharing_ = tokenToAdd->sharing_;
            this->announcing_ = tokenToAdd->announcing_;
            this->advertising_ = tokenToAdd->advertising_;
            this->withdrawingSupport_ = tokenToAdd->withdrawingSupport_;

          }


    }
  else if(this->getTimeStamp() == tokenToAdd->getTimeStamp())// Image is the same age
    {
      if(tokenToAdd->getFaction())
        {
          setFaction(tokenToAdd->getFaction());
        }

      if(this->getImageLevel() < tokenToAdd->getImageLevel())// new image has more reliable data
        {
//
          moving_ =  tokenToAdd->moving_;
          skillUse_ = tokenToAdd->skillUse_;
          this->loyality_ = tokenToAdd->loyality_;

          if(tokenToAdd->getImageLevel() == PRIVATE_IMAGE)
            {
//
              target_ = tokenToAdd->target_;
              this->silent_ = tokenToAdd->silent_;
              this->sharing_ = tokenToAdd->sharing_;
               this->announcing_ = tokenToAdd->announcing_;
               this->advertising_ = tokenToAdd->advertising_;
               this->withdrawingSupport_ = tokenToAdd->withdrawingSupport_;
            }

        }
//
    }
  else                                                       // Image is older
    {
//
      if(getFaction()== 0 && tokenToAdd->getFaction())
        {
          setLastKnownFaction(tokenToAdd->getFaction());
        }
    }


//====================
   if(this->imageLevel_ == PRIVATE_IMAGE)
     {
         return;
     }


    if(this->imageLevel_ == ALLIED_IMAGE)
     {
            this->makeAlliedImage(tokenToAdd);
            return;
     }

// OBSERVABLE_IMAGE: Faction info + visible inventory
    if(this->imageLevel_ == OBSERVABLE_IMAGE)
     {
        if(tokenToAdd->imageLevel_ ==  ALLIED_IMAGE)
        {
            this->makeAlliedImage(tokenToAdd);
            return;
        }
    }
// UNKNOWN_IMAGE: visible inventory

        if(tokenToAdd->imageLevel_ ==  ALLIED_IMAGE)
        {
        	this->makeAlliedImage(tokenToAdd);
            return;
        }

        if(tokenToAdd->imageLevel_ ==  OBSERVABLE_IMAGE)
        {
        	this->makeObservedImage(tokenToAdd);
        return;
        }
}


void TokenEntity::dailyUpdate()
{
  LocationEntity * location = getLocation();
  if(!location) // Disbanded
    return;
  Entity::dailyUpdate();
  clearTeachingOffers();
  moveAdvance();
  if(isTraced())
  {
     cout << "... Updating TokenEntity " << print()<<endl;
  }
  
  if(isGuarding())
    addSkill(combatSkill,BasicLearningStrategy::getPointsPerDay()/20);
  //1.5 days per month for guarding
}




void TokenEntity::postProcessData()
{
	if(isUnknownEntity())
	{
		return;
	}
}

void TokenEntity::postPostProcessData()
{
}



void TokenEntity::recalculateTravelTime()
{
  // Update movement times with new weather
  if(moving_)
  {
    int oldTime = moving_->getRemainingTime();
    MovementVariety * currentMode =moving_->getMode();
    int totalTime  = calculateTravelTime(moving_->getTravelTime(), currentMode);
    int time  = calculateTravelTime( moving_->getRemainingTime(),currentMode);
    moving_->resetRemainingTime(time);
    moving_->resetTravelTime(totalTime - (oldTime - time));
  }
  
}
// Reporting ==============================================
/** Retern reference to Entity which keeps reports from this  */
Entity * TokenEntity::getReportDestination()
{
  return location_;
}



void TokenEntity::privateReport(ReportPrinter &out)
{
}



void  TokenEntity::printOrderTemplate(ReportPrinter &out)
{
  if(isDisbanded())
    return; // no templates for dead units
    string str = ("#");
  out << getKeyword()<< " "<< getTag()<<endl;
  out.setPrefix(str);
  privateReport(out);
  out.setPrefix(0);
  for (vector<OrderLine *>::iterator iter = orders_.begin(); iter != orders_.end(); iter++)
  {
    (*iter)->printOrderLine(out);
  }
  out << endl;
}



/*
* Report token's specific states
*/
void    TokenEntity::reportFlags(ReportPrinter &out)
{
  //  Entity::reportFlags(out);
  if(guarding_) out << " Guarding.";
  if(passenger_) out << " Passenger.";
  if(advertising_) out << " Advertising.";
  if(announcing_) out << " Announcing.";
  if(!withdrawingSupport_) out << " Not withdrawing support.";
  if(!sharing_) out << " Not sharing.";
  combatTactics_.report(out);
  if(!combatSettings_.empty())
  {
    out << " Combat settings:";
    out<<reportCombatSettings();
    out << ".";
  }
}

/*
* Report Combat Settings
*/
string    TokenEntity::reportCombatSettings()
{
    string s;
    for (vector <string>::iterator iter = combatSettings_.begin();
    iter != combatSettings_.end(); iter++)
    {
      s.append(" ");
      s.append(*iter);
    }
    return s;
}

/*
* Report Combat Settings
*/
string    TokenEntity::reportCombatTactics()
{
    return combatTactics_.print();
}

void TokenEntity::reportSkills(FactionEntity * faction, ReportPrinter &out)
{
  skills_.report(out);
  //  out << "May learn: ";
  bool isFirst = true;
  int level;
  SkillRule * skill;
vector < SkillLevelElement *> mayLearnSkills = getMayLearnList();
for(vector < SkillLevelElement *>::iterator iter = mayLearnSkills.begin();
        iter != mayLearnSkills.end(); ++iter)
{
    skill = (*iter)->getSkill();
    level = getSkillLevel(skill);
      faction->addSkillKnowledge(skill, level + 1);
      if( isFirst)
      {
    if (isTraced())
      cout <<"== TRACING " << print()<< " May learn: " << endl;
      isFirst = false;
	out << "May learn: ";
      }
      else
      {
	out << ", ";
      }
      skill->printLevel(level + 1, out);
}
//  RulesIterator skillIter;
//  bool isFirst = true;
//  for (skillIter = skills.begin(); skillIter != skills.end(); skillIter++)
//  {
//    SkillRule * skill = dynamic_cast<SkillRule*>(*skillIter);
//    if(skill == 0)
//      continue;
//    if((skill->getRequirement(1) == 0) && (getSkillLevel(skill) == 0))
//      continue;
//
//    if (isTraced())
//      cout <<"== TRACING " << print()<< " May learn [T] " << endl;
//    if( mayStudySkill(skill))
//    {
//      faction->addSkillKnowledge(skill, getSkillLevel(skill) + 1); // It may be better placed
//      // in addNewSkill
//      if( isFirst)
//      {
//	isFirst = false;
//	out << "May learn: ";
//      }
//      else
//      {
//	out << ", ";
//      }
//      skill->printLevel(getSkillLevel(skill) + 1, out);
//    }
//
//  }
  
  if( !isFirst)
    out << ". ";
}

vector < SkillLevelElement *> TokenEntity::getMayLearnList()
{
    vector <SkillLevelElement *> skillsMeyBeLearned;
  for (RulesIterator skillIter = gameFacade->skills.begin(); skillIter != gameFacade->skills.end(); skillIter++)
  {
    SkillRule * skill = dynamic_cast<SkillRule*>(*skillIter);
    if(skill == 0)
      continue;
    if((skill->getRequirement(1) == 0) && (getSkillLevel(skill) == 0))
      continue;


    if( mayStudySkill(skill))
    {
        skillsMeyBeLearned.push_back(new SkillLevelElement(skill, getSkillLevel(skill)));
    }

  }
   return  skillsMeyBeLearned;
}

void TokenEntity::addReport(ReportRecord report)
{
    if(getFaction())
    {
        if(getFaction()->isNPCFaction())
        {
            processAttitude(report);
        }
    }
    Entity::addReport(report);
}

void TokenEntity::addReport(ReportMessage * report,OrderLine *  orderId, BasicCondition * observationCriteria)
{
    addReport(ReportRecord(report, orderId, observationCriteria));
}

void TokenEntity::processAttitude(ReportRecord report)
{
   ReportPattern * type =  report.reportMessage->getPattern();
   if(type ==AttackedReporter)
   {
       //OK
       TokenEntity * attacker = dynamic_cast<TokenEntity *>(report.reportMessage->getParameter(2));
       if(attacker)
       {
           getFaction()->setStance(attacker,enemyStance);
           //Report.
       }

   }
}
// Data access methods ==============================================


/*
* TokenEntity may not be able to execute orders when it is moving
* (not as passenger)
*/
bool TokenEntity::isBusy()  const
{
  if(moving_ && ! passenger_)
    return true;
  return false;
}



/*
* TokenEntity may be unaccessible for orders when it is
* in process of destruction
*/
bool TokenEntity::isUnaccessible() const
{
  if(location_ == 0)
    return true;
  else
    return false;
}



/** Location where Entity is actually available (if any)*/
LocationEntity * TokenEntity::getGlobalLocation() const
{
  if(moving_)
    return 0;
  else
    return location_;
}




// Inventory methods ==============================================
InventoryElement * TokenEntity::findInInventory(ItemRule * item)
{
  return inventory_.findItem(item);
}


// returns list of all items equipped at given slot
vector < InventoryElement > TokenEntity::getSlotContent(EquipmentSlotVariety * slot)
{
  return inventory_.getSlotContent(slot);
}



vector < InventoryElement> & TokenEntity::getAllInventory()
{
  return inventory_.getAll();
}



void TokenEntity::giveAllInventory(TokenEntity * unit)
{
  inventory_.giveAll(unit);
}



vector <AbstractArray> TokenEntity::aPrintInventory()
{
  return inventory_.aPrint();
}


 vector <AbstractArray> TokenEntity::aPrintSkills()
 {
   return skills_.aPrint();
 }

vector <AbstractArray> TokenEntity::aPrintEnchant()
{
  return enchantments_.aPrint();
}

///*
// * Takes up to num items from inventory. Returns number of items taken
// */
//int TokenEntity::takeFromInventory(ItemRule * item, int num)
//{
  //  return inventory_.takeFromInventory(item, num);
  //}
  
  
  /*
  * Takes exactly num items from inventory if they are available
  * and nothing if number of items in posession is less than required.
  * Returns number of items TokenEntity actually has.
  */
  
int TokenEntity::takeFromInventory(ItemRule * item, int num)
  {
    return inventory_.take(item, num);
  }
  
  
  
  void TokenEntity::deleteFromInventory(InventoryElement * element)
  {
    inventory_.deleteElement(element);
  }
  
  /*
  * After  checks for equipability and skills do equip item
  */
  int TokenEntity::equipItem(ItemRule * item, int num)
  {
    return inventory_.equipItem(item, num);
  }
  
  
  
  /*
  * Add I tem to inventory if it was not there
  * Increase it's amount otherwise
  */
  void TokenEntity::addToInventory(ItemRule * item, RationalNumber& num)
  {
    inventory_.add(item, num);
  }
  
  
  
  /** No descriptions */
  int TokenEntity::hasItem(ItemRule * item)
  {
    // This is a hack.
    if(item == gameFacade->items["mana"])
    {
      UnitEntity * unit = dynamic_cast<UnitEntity *>(this);
      if(unit)
	return unit->hasMana();
      else
	return 0;
    }
    return inventory_.hasItem(item);
  }
  
  
  
  int TokenEntity::hasEquiped(ItemRule * item)
  {
    return inventory_.hasEquiped(item);
  }
  
  
  
  bool TokenEntity::isEquiped(InventoryElement * item)
  {
    return inventory_.isEquiped(item);
  }
  
  
  
  RationalNumber     TokenEntity::getItemAmount(ItemRule * item)
  {
    return inventory_.getItemAmount(item);
  }
  
  
  
  void TokenEntity::addToInventory(ItemRule * item, int num)
  {
    inventory_.add(item, num);
  }
  
  void  TokenEntity::checkEquipmentConditions()
  {
  	inventory_.checkEquipmentConditions(this);
  }

  
  
  // Stacking/Containment ========================================================
  bool TokenEntity::isAccepting(UnitEntity * unit)
  {
    return (find(accepting_.begin(),accepting_.end(),unit) != accepting_.end());
  }
  
  
  
  void TokenEntity::clearAccept(UnitEntity * unit)
  {
    vector<UnitEntity * >::iterator iter = find(accepting_.begin(),accepting_.end(),unit);
    if(iter != accepting_.end())
      accepting_.erase(iter);
  }
  
  
  
  void TokenEntity::accept(UnitEntity * unit)
  {
    vector<UnitEntity * >::iterator iter = find(accepting_.begin(),accepting_.end(),unit);
    if(iter == accepting_.end())
      accepting_.push_back(unit);
  }
  
  
  
  // Skills ========================================================
  vector < SkillElement>& TokenEntity::getAllSkills()
  {
    return skills_.getAll();
  }
  
  
  
  
  /*
  * returns true if current unit may further study this skill
  * // may be it should return level, that unit may study or 0 ?
  */
  bool TokenEntity::mayStudySkill(SkillRule * skill)
  {
    if( skill->mayBeStudied(this) == LEARNING_OK)
      return true;
    else
      return false;
  }
  bool TokenEntity::mayStudyWithTeacher(SkillRule * skill)
  {
    if( skill->mayBeStudied(this) == TEACHING_REQUIRED)
      return true;
    else
      return false;
  }
  
  
  
  int TokenEntity::addSkill(SkillElement  skill)
  {
    return addSkill(skill.getSkill(), skill.getExpPoints());
  }
  
  
  
  int TokenEntity::getSkillLevel(SkillRule  * const skill)
  {
    return skills_.getSkillLevel(skill);
  }
  
  
  
  /** Adding skill experience, checking for new level reached */
  int TokenEntity::addSkill(SkillRule  * skill, int expPoints)
  {
    int oldLevel = this->getSkillLevel(skill);
    int newLevel = skills_.addSkill(skill, expPoints);
    
    if (oldLevel != newLevel)    // Check for a new level being reached
      gainNewLevel(skill,newLevel);
    
    return newLevel;
  }
  
  //  TokenEntity::newLevelReporter_ = new ReportPattern(""," reached level "," in ","");
  
  void TokenEntity::gainNewLevel(SkillRule * skill, int newLevel)
{
    if (getFaction())
    {
        TertiaryMessage * Message = new TertiaryMessage(newLevelReporter, this, new IntegerData(newLevel), skill);
        addReport(Message, 0, 0);

        // Add knowledge to faction
        getFaction()->addSkillKnowledge(skill, newLevel);
        //    // Recalculate stats
        //    recalculateStats();
        // Is it maximum level?
        if (newLevel >= skill->getMaxLevel())
        {
            BinaryMessage * Message = new BinaryMessage(maxLevelReporter, this, skill);
            addReport(Message, 0, 0);
        }
    }
    // Recalculate stats
    recalculateStats();
}
  
  
  
  int TokenEntity::getSkillPoints(SkillRule  * const skill)
  {
    return skills_.getSkillPoints(skill);
  }
  
  
  
  SkillElement *  TokenEntity::getSkillElement(SkillRule  * const skill)
  {
    return skills_.getSkillElement(skill);
  }
  
  
  void TokenEntity::cancelTeachingOffer()
  {
    vector <TeachingOffer  *>::iterator iter;
    for(iter = teachingDonorOffers_.begin(); iter != teachingDonorOffers_.end(); ++iter)
    {
      if((*iter)->getTeacher() == this)
      {
	        // cout<< *this<<" deletes " << *(*iter) <<endl;
	//delete (*iter);
	teachingDonorOffers_.erase(iter);
	return;
      }
    }
    cerr << "ERROR. on cancelling "<< print() <<" Can't find his own teachingOffers\n";
    
  }
  
   bool TokenEntity::canProspect(ResourceElement * res)
   {
     if(res == 0)
     {
       return true;
     }
     ItemRule * item = res->getResource();
     if(item == 0)
      {
      return true;
     }
     SkillLevelElement * skillCondition = item->getProspectSkill();
     if(skillCondition==0)
       {
       return true;
     }
     if(hasSkill(skillCondition))
     {
      return true;
     }
     // if has X-Ray glasses return true
     return false;
   }
  
  /*
  * Checks that Entity has specified experience in given skill
  */
  bool TokenEntity::hasSkill(SkillRule  * skill, int experience)
  {   
    return skills_.hasSkill(skill, experience);
  }
  
  
  int TokenEntity::getLearningCapacity()
  {
    return 1;
  }
  
  
  
  bool TokenEntity::teacherRequired(SkillRule * skill)
  {
    return true;
  }
  
  // Skill Use ========================================================
  // finds amount of skill use that remained from the previous use,
  // adds current skill use and
  // returns number of production cycle completed
  // and truncates SkillUseElement
  int TokenEntity::addSkillUse(SkillUseElement * skillUse)
  {
    int numCycles=0;
    //  SkillUseIterator iter = find(skillUse_.begin(), skillUse_.end(), skillUse);
    SkillUseIterator iter;
    for( iter = skillUse_.begin(); iter != skillUse_.end();++iter)
    {
      if((*iter)->getSkillUsed() == skillUse->getSkillUsed())
	break;
    }
    if(iter == skillUse_.end()) // not used before
    {
      if(skillUse->getDaysUsed() < skillUse->getDuration())
      {
	skillUse_.push_back(skillUse);
	return 0;
      }
      numCycles = (skillUse->getDaysUsed() / skillUse->getDuration()).roundDown();
      skillUse->setDaysUsed(skillUse->getDaysUsed() - numCycles * skillUse->getDuration());
      return  numCycles;
    }
    
    RationalNumber effectiveDays;
    if ( (*iter)->getDuration() == skillUse->getDuration() )
    {
      effectiveDays = (*iter)->getDaysUsed() + skillUse->getDaysUsed();
    }
    else  // skill is used with a different duration now
    {
      effectiveDays = ( (*iter)->getDaysUsed() * skillUse->getDuration()) / (*iter)->getDuration() +  skillUse->getDaysUsed();
      (*iter)->setDuration(skillUse->getDuration());
    }
    
    if(effectiveDays < (*iter)->getDuration())   // skill use not completed
    {
      (*iter)->setDaysUsed(effectiveDays);
      return 0;
    }
    else  // skill use completed (and may be several times)
    {
      numCycles = (effectiveDays / (*iter)->getDuration()).roundDown();
      skillUse->setDaysUsed(effectiveDays - numCycles * (*iter)->getDuration());
      delete (*iter);
      skillUse_.erase (iter); // Unused days will not be automatically used on next working cycle
      return numCycles;
    }
  }
  
  
  
  // finds amount of skill use that remained from the previous use,
  // adds current skill use and
  // returns number of production cycle completed
  // no truncation
  int TokenEntity::addCumullativeSkillUse(SkillUseElement * skillUse, int accumulationLimit )
  {
    int numCycles=0;
    if  (accumulationLimit < 1)
      accumulationLimit = 1;
    //  SkillUseIterator iter = find(skillUse_.begin(), skillUse_.end(), skillUse);
    SkillUseIterator iter;
    for( iter = skillUse_.begin(); iter != skillUse_.end();++iter)
    {
      if((*iter)->getSkillUsed() == skillUse->getSkillUsed())
	break;
    }
    if(iter == skillUse_.end()) // not used before
    {
      numCycles = (skillUse->getDaysUsed() / skillUse->getDuration()).roundDown();
      if (numCycles >= accumulationLimit) // all done. nothing to store
	return  accumulationLimit;
      
      else
      {
	skillUse_.push_back(skillUse);
	return numCycles;
      }
    }
    
    RationalNumber effectiveDays;
    if ( (*iter)->getDuration() == skillUse->getDuration() )
    {
      effectiveDays = (*iter)->getDaysUsed() + skillUse->getDaysUsed();
    }
    else  // skill is used with a different duration now
    {
      effectiveDays = ( (*iter)->getDaysUsed() * skillUse->getDuration()) / (*iter)->getDuration() +  skillUse->getDaysUsed();
      (*iter)->setDuration(skillUse->getDuration());
    }
    
    numCycles = (effectiveDays / (*iter)->getDuration()).roundDown();
    
    if (numCycles < accumulationLimit) // skill use not completed
      return  numCycles;
    
    
    else  // skill use completed
    {
      //      skillUse->setDaysUsed(effectiveDays - numCycles * (*iter)->getDuration());
      delete (*iter);
      skillUse_.erase (iter); // Unused days will not be  used on next working cycle
      return accumulationLimit;
    }
  }
  
  
  
  
  bool TokenEntity::isCurrentlyUsingSkill(SkillRule * skill)
  {
    for (SkillUseIterator iter = skillUse_.begin(); iter != skillUse_.end(); ++iter)
    {
      if( (*iter)->getSkillUsed() == skill)
	return true;
    }
    return false;
  }
  
  
  
  
  /*
  *  Intristic ability of Entity to study  skill
  *  Level limitations also taken into considerations
  */
  LEARNING_RESULT TokenEntity::mayLearn(SkillRule * skill)
  {
    if(isTraced())
    {
      cerr << " Basic method called for  "<<skill->print()<<endl;
    }
    return CANNOT_STUDY_FAILURE;
  }
  
  int TokenEntity::countElementalMagicSkill()
  {
    int elementalSkillCounter = 0;
    vector < SkillElement> skills = skills_.getAll();
    for (SkillIterator iter= skills.begin(); iter != skills.end(); iter++)
    {
      if((*iter).getSkill()->isElementalMagicSkill())
      {
	elementalSkillCounter++;
      }
    }
    return elementalSkillCounter;
  }
  // Movement ========================================================
  
  
  
  void TokenEntity::setEntityMoving(TravelElement * moving)
  {
    // Clear Pillage requests 
    getLocation()->getCombatManager()->removePillageRequest(this,false); 
    if(isGuarding())
    {
      guarding_ = false;
      getLocation()->selectNewGuard();
    }
    moving_ = moving;
    if(isTraced())                                  //For Debugging only
    {                                                      //For Debugging only
    cout <<"--xxx--->  Start Moving"<< endl;  //For Debugging only
    }                                                     //For Debugging only
    if(moving)
      isMoving_ = true;
    else
      isMoving_ = false;
  }
  
 // Entity is already moving in a stack but now will move independently 
  void TokenEntity::setEntityIndependentlyMoving(TravelElement * moving)
  {

    moving_ = moving->clone();
    if(isTraced())                                  //For Debugging only
    {                                                      //For Debugging only
    cout <<"--xxx--->  Start Moving Independenly"<< endl;  //For Debugging only
    }                                                     //For Debugging only
    if(moving)
      isMoving_ = true;
    else
      isMoving_ = false;
  }
    
  
  bool TokenEntity::moveAdvance()
  {
    if(moving_ == 0)
      return false;
    
    moving_->advance();
    
    
    if(moving_->isCrossingBorder())
    {
      tryEnterLocation();
    }
    
    
    if(moving_->isArrived())
    {
      movingGroupArrived();
      delete moving_;
      moving_ = 0;
      if(isTraced())                                  //For Debugging only
      {                                                      //For Debugging only
      cout <<"--xxx--->  Token Arrived"<< endl;  //For Debugging only
      }                                                     //For Debugging only
      isMoving_ = false;
      return true;
    }
    return false;
    
  }
  
  void TokenEntity::tryEnterLocation()
  {
    TokenEntity * patrol= 0;
    // check if stack or vessel can enter location
    //(it may be guarded or other reasons)
      LocationEntity * destination = moving_->getDestination();
      if(!destination->tokenAllowedToEnter(this,moving_->getMode(),patrol))
      {
	if  (moving_->isMarching())
	{
	  destination->getCombatManager()->attackAttempt( this,
				patrol, 0, TokenEntity::marchAttackPostprocessing );
	}
	else
	{
	  moving_->retreat();
	  moving_->advance();// Unit immediately moves back
	  addReport(new UnaryMessage( movementBlockedReporter, destination),0, 0);
	  TokenEntity * enemyPatrol = destination->getBlockingPatrol( this,
						moving_->getMode() , hostileStance);
	  if(patrol)
	  {
	    destination->getCombatManager()->attackAttempt(enemyPatrol,this,0,0);
	  }
	}
	
      }
      else
	moveGroupToLocation();
  }
  
  
  void TokenEntity::marchAttackPostprocessing(TokenEntity * attacker, TokenEntity * defender,const BATTLE_RESULT  result)
  {
    if( result == ATTACKER_VICTORY)
    {
      attacker->moveGroupToLocation();
      if(!attacker->isAlive())
      {
	// attacker is dead
	// should reform group and find new group leader
	return;
      }
      TravelElement * travelStatus = attacker->getTravelStatus();
      if(travelStatus == 0)
      {
	return;
      }
      if(travelStatus->isArrived())
      {
	attacker->movingGroupArrived();
	delete travelStatus;
	attacker->setTravelStatus(0);
	return ;
      }
      
      if(defender->isAlive())
	defender->setPatrolling(false);
    }
    else 
    {
      attacker->retreat();
    }
    return ;
  }
  
  
  
  void TokenEntity::moveToLocation()
  {
    LocationEntity * newLocation = moving_->getDestination();
    moveToLocation(newLocation);
  }
  
  
  void TokenEntity::moveToLocation(LocationEntity * newLocation)
  {
    getLocation()->addReport(new UnaryMessage(leavePublicReporter, this), 0 ,
			     ObservationCondition::createObservationCondition(getStealth() ));
    addReport(new UnaryMessage(leavePrivateReporter, getLocation()));
    getFaction()->addVisitedLocation(newLocation);
	     
    if (isTraced())
    {
      cout <<"== TRACING " <<print()<< " ==> Enters "<< newLocation->print()<<"\n";
    }
    newLocation->addReport(new UnaryMessage(enterPublicReporter, this), 0 ,
			      ObservationCondition::createObservationCondition(getStealth() ));
    addReport(new UnaryMessage(enterPrivateReporter, newLocation));
  }
  
  
  void TokenEntity::moveGroupToLocation()
  {
  }
  
  
  
  void TokenEntity::movingGroupArrived()
  {
  }
  
  
  
  bool TokenEntity::retreat()
  {
    if(moving_)
    {
      moving_->retreat();
      return true;
    }
    else
      return false;
  }
  
  
  // Sometimes Entity wants to retain ability of movement with some mode
  MovementVariety * TokenEntity::getReservedMode()
  {
    // if moving - current mode
    // if flying mode = flying
    // if at water mode = swimming/sailing
    // if the only way out 'mode'
    // else mode = walking
    if(getCapacity(flyingMode) >= getWeight()) // may fly
      return flyingMode;
    
    if(location_->getTerrain()->isAquatic()) // should swim othewise drawn
      return swimingMode;
    
    if(moving_) // current mode
    {
      if(moving_->getMode())
      {
	return moving_->getMode();
      }
    }
    
    return walkingMode; //default
  }
  
  
  
  // Other ========================================================
  /** Unit is not exist anymore (dead or disbanded) */
//   bool TokenEntity::isDisbanded()
//   {
//     return (location_ == 0);
//   }
  
  
  
  void TokenEntity::recalculateStats()
  {
  }
  
  
  
  bool TokenEntity::mayMove()
  {
    return false;
  }
  
  
  
  bool TokenEntity::leaveStaying()
  {
    return false;
  }
  
  
  
  int  TokenEntity::calculateTotalWeight (int & weight)
  {
    return  weight;
  }
  
  
  
  void TokenEntity::calculateTotalCapacity(int & capacity, int modeIndex)
  {
  }
  
  
  
  void TokenEntity::calculateTotalCapacityMode(int & capacity, MovementVariety * mode)
  {
    calculateTotalCapacity(capacity, gameFacade->movementModes.getIndex(mode->getTag()));
  }
  
  
  
  /** Climate and other bonuses */
  int TokenEntity::calculateTravelTime(int time, MovementVariety * mode)
  {
//      if(isTraced())
//      {
//          cout << print()<< " "<<
//      }
    time = time * (calculateMovementBonus(mode) + 100) / 100;
    return time;
  }
  
  int TokenEntity::calculateMovementBonus(MovementVariety * mode)
  {
    int bonus = 0;
    // Item
    bonus += inventory_.getMovementBonus(mode);
    
    // weather
    LocationEntity * currentLocation = getLocation();
    if(currentLocation)
    {
      WeatherRule * weather = currentLocation->getWeather();
      if(weather)
      {
	bonus += weather->getMovementBonus(mode);
      }
      bonus += currentLocation->getMovementBonus(mode);
    }
    // Enchantment
    bonus += enchantments_.getMovementBonus(mode);
    
    // Skills
    bonus += skills_.getMovementBonus(mode);
    return bonus;
  }
  
  
  
  
  
  
  /** Overloading effects */
  int TokenEntity::calculateOverloading(int time, int weight, int capacity)
  {
    int overloadFactor;
    if(capacity == 0)
      return 0;
    overloadFactor = weight  * 100 / capacity;
    
    if(overloadFactor >= 200) // overloaded twice
      return 0;
    
    if(time == 0)
      return 0;
    
    // Overload
    if(overloadFactor > 100)
      time = time + time *  3 * (overloadFactor - 100)  / 100;   //  overloading
      return time;
  }
  
  
 

  // For tokens at the same location
  bool TokenEntity::mayObserveTokenEntity(TokenEntity * tokenEntity)
  {

   if(tokenEntity->getFaction() == this->getFaction() ) // the same faction
      return true;
    if(tokenEntity->isExposed())
      return true;
    if (this->getObservation() >= tokenEntity->getStealth() )
      return true;
    if (this->getLocation()->getFactionalObservation(getFaction()) >= tokenEntity->getStealth() )
      return true;
    else
      return false;
    
  }
  
  
  
  bool TokenEntity::mayInterractTokenEntity(TokenEntity * tokenEntity)
  {
    if( tokenEntity == 0)
      return false;
    
    if(tokenEntity->getGlobalLocation() != this->getGlobalLocation() )  // take into account buildings
      return false;
    else
      return mayObserveTokenEntity(tokenEntity);
  }
  
  
  
  bool TokenEntity::mayInterractFaction(FactionEntity * faction)
  {
    if( faction == 0)
      return false;
    
    for(UnitIterator iter = getLocation()->unitsPresent().begin();
    iter != getLocation()->unitsPresent().end(); ++iter)
    {
      if( (*iter)->getFaction() == faction)
      {
	if (mayInterractTokenEntity(*iter))
	  //            if (Entity::mayInterract(*iter))// <- check this
	  return true;
      }
    }
    
    return false;
    
  }
  
 bool TokenEntity::mayBeIdentified(FactionEntity * faction)
  {
    if( faction == 0)
      return false;
    LocationEntity * location = getLocation();
    if(location == 0)
      return false;

    for(UnitIterator iter = location->unitsPresent().begin();
    iter != location->unitsPresent().end(); ++iter)
    {
      if( (*iter)->getFaction() == faction)
      {
         if ((*iter)->getObservation() > getStealth() )
         {
            return true;
         }
      }
    }
  // Constructions may observe too?
    for(ConstructionIterator iter = location->constructionsPresent().begin();
    iter != location->constructionsPresent().end(); ++iter)
    {
      if( (*iter)->getFaction() == faction)
      {
         if ((*iter)->getObservation() > getStealth() )
         {
            return true;
         }
      }
    }
    return false;

  }

  
  
  ORDER_STATUS TokenEntity::oath(FactionEntity * faction)
  {
    if(faction == 0)
    {
      return INVALID;
    }
    
    if(faction == getFaction())
    {
      addReport(new SimpleMessage(cannotOathYourselfReporter));
      return INVALID;
    }
    
    if(!faction->isNPCFaction()) // you may allways oath to NPC
    {
      if (!mayInterractFaction(faction)) // Not In the same place or can't see
	return FAILURE;
      
      if(faction->stanceAtLeast(getFaction(), friendlyStance))
      {
	// not accepting. Reports to both sides
	UnaryMessage * oathRejectedMessage = new UnaryMessage(oathRejectedReporter, faction);
	addReport(oathRejectedMessage,0,0);
	faction->addReport(oathRejectedMessage,0,0);
	return INVALID;
      }
    }
    BinaryMessage * oathReportMessage = new BinaryMessage(oathReporter, this,faction);
    getFaction()->addReport(oathReportMessage,0,0 );
    markToOath(faction);
    faction->addReport(oathReportMessage,0,0);
    return SUCCESS;
    
  }
  
  
  
  BasicOrderSynchronizationRequest * TokenEntity::hasOrderSyncRequest(BasicOrderSynchronizationRequest * request)
  {
    for(SyncRequestIterator iter = orderSyncRequests_.begin();
    iter != orderSyncRequests_.end(); ++iter)
    {
      if (request->isEqual(*iter))
      {
	return (*iter);
      }
    }
    return 0;
  }
  
  
  
  void TokenEntity::removeOrderSyncRequest(BasicOrderSynchronizationRequest * request)
  {
    for(SyncRequestIterator iter = orderSyncRequests_.begin();
    iter != orderSyncRequests_.end(); ++iter)
    {
      if (request->isEqual(*iter))
      {
	orderSyncRequests_.erase(iter);
	delete (*iter);
	return;
      }
    }
  }
  
  
  
  void TokenEntity::addOrderSyncRequest(BasicOrderSynchronizationRequest * request)
  {
    orderSyncRequests_.push_back(request);
  }
  
  
  
  bool TokenEntity::doneOrderSyncRequest(BasicOrderSynchronizationRequest * request)
  {
    for(SyncRequestIterator iter = orderSyncRequests_.begin();
    iter != orderSyncRequests_.end(); ++iter)
    {
      if (request->isEqual(*iter))
      {
	return (*iter)->syncDone();
      }
    }
    return false;
  }
  
  
  
  void TokenEntity::markDoneOrderSyncRequest(BasicOrderSynchronizationRequest * request)
  {
    for(SyncRequestIterator iter = orderSyncRequests_.begin();
    iter != orderSyncRequests_.end(); ++iter)
    {
      if (request->isEqual(*iter))
      {
	(*iter)->setSyncDone();
      }
    }
  }
  
  
  void TokenEntity::setTarget(AbstractData * target)
  {
    //  if(target_)
	//    target_->clean();
	target_ = target;
  }
  
  
  
  bool TokenEntity::combatStanceAtLeast(CombatStanceVariety * combatStance) const
  {
    //  cout<< "Combat stance of "<<tag_<<" : "<<combatTactics_.getCombatStance()->print()<<" and "<<combatStance->print()<<endl;
    return (*(combatTactics_.getCombatStance()) >=  *combatStance);
  }
  
  
  
  int  TokenEntity::getAttackRating() const
  {
    return 0;
  }
  
  
  
  int  TokenEntity::getDefenceRating() const
  {
    return 0;
  }



int TokenEntity::getObservationRating() const
{
   return 0;
}



int TokenEntity::getStealthRating() 
{
  return 0;
}

  // Convert combat setting strings to combat orders.
  void TokenEntity::setDefaultCombatMovement()
  {
    if(combatTactics_.getCombatMove())
      defaultCombatMovement_ =
      new CombatOrderLine( combatTactics_.getCombatMove()->getTag(), this);
    else
      defaultCombatMovement_ =
      new CombatOrderLine( defaultCombatMove->getTag(), this);
  }
  
  
  
  void TokenEntity::preprocessData()
  {
    guarding_ = false;
    if(getFaction()->isNPCFaction())
    {	
      isPayingUpkeep_ = false;
    }
    setCombatSettings();
  }



void TokenEntity::setCombatSettings()
  {
     for (vector <string>::iterator iter = combatSettings_.begin(); iter != combatSettings_.end(); ++iter)
    {
      checkCombatSetting(*iter);
    }
    setDefaultCombatMovement();

  }



  
  void TokenEntity::clearCombatSettings()
  {
    combatSettings_.clear();
  }
  
  
  
  void TokenEntity::addCombatSetting(string combatOrderText)
  {
    combatSettings_.push_back(combatOrderText);
    // 	if(CombatOrder::checkCombatAction(combatOrderText,this))
      // 	{
	// 		CombatOrderLine * currentOrder = new CombatOrderLine(combatOrderText,this);
	// //		currentOrder->parse(combatOrderText,this);
	//   	defaultCombatOrders_.push_back(currentOrder);
	//	}
  }
  
  void TokenEntity::checkCombatSetting(string combatOrderText)
  {
    if(isTraced())
    {
      //cout << "...";
    }
    
    if(CombatOrder::checkCombatAction(combatOrderText,this,false))
    {
      CombatOrderLine * currentOrder = new CombatOrderLine(combatOrderText,this);
      //		currentOrder->parse(combatOrderText,this);
      defaultCombatOrders_.push_back(currentOrder);
    }
  }
  
  
  BattleInstance * TokenEntity::createBattleInstantiation(BattleField * battleField)
  {
    if(battleInstance_)
      delete battleInstance_;
    // copy all combat orders to BattleInstance
    // transform combat and tactic settings into orders
    // for each order line set executed on turn flag on 0 (reset)
      battleInstance_ = new BattleInstance(this, battleField->getCombatReport());
      battleInstance_->initialize(battleField);
      return battleInstance_;
  }
  
  
  
  void TokenEntity::addOrder(string newOrder)
  {
  //    cout<< "TokenEntity::addOrder "<< newOrder<<endl;
    Parser parser = Parser(newOrder);
    if(parser.matchKeyword("COMBAT_ORDER"))
    {
      combatOrders_.push_back(new CombatOrderLine(parser.getText(),this));
    }
    else
    {
      orders_.push_back(new OrderLine(newOrder,this));
    }
    
  }
  
  
  
  void TokenEntity::setCombatMove(CombatMoveVariety * value)
  {
    combatTactics_.setCombatMove(value);
    if (defaultCombatMovement_)
      delete defaultCombatMovement_;
    defaultCombatMovement_ = new CombatOrderLine(
    value->getKeyword(), this);
  }
  
  
  
  /*
  * delete all entity's orders
  */
  void TokenEntity::clearOrders()
  {
    CombatOrderIterator iter;
    for ( iter = combatOrders_.begin(); iter != combatOrders_.end(); iter++)
    {
      delete (*iter);
    }
    combatOrders_.clear();
    Entity::clearOrders();
  }
  
  
  
  
  
  
  
  
  
  
  int TokenEntity::getProductionBonus(SkillRule * skill)
  {
    int bonus = 0;
    // Item
    bonus += inventory_.getProductionBonus(skill);
    
    
    LocationEntity * currentLocation = getLocation();
    // weather
    bonus += currentLocation->getWeather()->getProductionBonusValue(skill);
    
    // Season
    currentLocation->getSeason()->getProductionBonusValue(skill);
    // terrain
    bonus += currentLocation->getTerrain()->getProductionBonusValue(skill);
    
    
    // Enchantment
    bonus += enchantments_.getProductionBonus(skill);
    
    // Skills
    bonus += skills_.getProductionBonus(skill);
    
    
    
    return bonus;
  }
  
  
  
  // overloaded unit looks for items adding capacity
  bool TokenEntity::takeTransport(vector <ItemElement> & items, MovementVariety *mode)
  {
    if(items.empty())
      return false;
    int missingCapacity = getWeight() - getCapacity(mode);
    if(missingCapacity <= 0)
      return false;
    
    //		cout << "== "<<this->print();
    int transportCapacity;
    int num;
    int numberTaken;
    
    for (ItemElementIterator iter = items.begin(); iter != items.end();)
    {
      ItemRule * item = (*iter).getItemType();
      num =(*iter).getItemNumber();
      //		cout << " looking for transport in the loot: ==> " <<item->print()<<endl;
      
      // First try equip capacity if able to equip
      transportCapacity = item->getCapacity(mode) - item->getWeight();
      if (transportCapacity <= 0)
      {
	++iter;
	continue;
      }
      if(transportCapacity * num < missingCapacity) // take all
      {
	items.erase(iter);
	addToInventory(item,num);
	addReport(new UnaryMessage(lootReporter, new ItemElement(item,num)));
	continue;
      }
      else // take enougth and exit
      {
	numberTaken = missingCapacity/transportCapacity;
	if(numberTaken * transportCapacity != missingCapacity)
	{
	  numberTaken++;
	}
	addToInventory(item,numberTaken);
	addReport(new UnaryMessage(lootReporter,
				   new ItemElement(item, numberTaken)));
				   (*iter).setItemNumber(num - numberTaken);
				   if(num == numberTaken)
				     items.erase(iter);
				   return true;
				   
      }
    }
    return false;
  }
  
  
  
  // tries to take as much loot of one sort as possible  from the beginning of the loot list
  bool TokenEntity::takeLoot(vector <ItemElement> & items)
  {
    if(items.empty())
      return false;
    int num;
    int max;
    MovementVariety * mode = getReservedMode();//-- determine -- mode
      int freeCapacity = getCapacity(mode) - getWeight();
      for (ItemElementIterator iter = items.begin(); iter != items.end();)
      {
	
	ItemRule * item = (*iter).getItemType();
	num =(*iter).getItemNumber();
	
	if(item->getWeight() == 0) // take all
	{
	  items.erase(iter);
	  addReport(new UnaryMessage(lootReporter, new ItemElement(item,num)));
	  addToInventory(item,num);
	  return true;
	}
	
	max = freeCapacity/item->getWeight();
	if (max ==0) // can't take anything let's try next item
	{
	  ++iter;
	  continue;
	}
	if (num > max) // take but not all
	{
	  (*iter).setItemNumber(num - max);
	  num = max;
	  addReport(new UnaryMessage(lootReporter, new ItemElement(item,num)));
	  addToInventory(item,num);
	  return true;
	}
	else // take all
	{
	  items.erase(iter);
	  addReport(new UnaryMessage(lootReporter, new ItemElement(item,num)));
	  addToInventory(item,num);
	  return true;
	}
      }
      return false;
  }
  
  
  TeachingOffer * TokenEntity::findTeachingOffer(SkillRule  * skill, int level)
  {
    TeachingOffer *offer = location_->findTeachingOffer(skill, level);
    if(offer)
      return offer;
    return Entity::findTeachingOffer(skill, level);
  }
  
  
  
  bool             TokenEntity::isNpcGuard() const 
  {
    return guarding_ && faction_->isNPCFaction();
  }
  
void TokenEntity::extractAndAddKnowledge(Entity * recipient, int parameter)
{
//    if(recipient==factions["f08"])
//    {
//        cout<<"<======== Extracting Token "<<getTag()<<endl;
//    }
    //SkillsAttribute
    skills_.extractAndAddKnowledge(recipient, parameter);
    //InventoryAttribute
    inventory_.extractAndAddKnowledge(recipient, parameter);;
}
