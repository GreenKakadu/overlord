/***************************************************************************
                          EntityStatistics.cpp 
                             -------------------
    begin                : Wed Feb 5 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "EntityStatistics.h"

EntityStatistics::EntityStatistics()
{
  clearStats();
}



EntityStatistics::~EntityStatistics(){
}



/** Does Initialization of stats data */
STATUS EntityStatistics::initialize(Parser * parser)
{
  if (parser->matchKeyword("MELEE"))
    {
      melee_  = parser->getInteger();
      return OK;
    }
  if (parser->matchKeyword("INITIATIVE"))
    {
      initiative_= parser->getInteger();
      return OK;
    }
  if (parser->matchKeyword("DEFENSE"))
    {
      defence_ = parser->getInteger();
      return OK;
    }
  if (parser->matchKeyword("DAMAGE"))
    {
     damage_ = parser->getInteger();
      return OK;
    }
  if (parser->matchKeyword("MISSILE"))
    {
      missile_ = parser->getInteger();
      return OK;
    }
  if (parser->matchKeyword("HITS"))
    {
      numOfHits_ = parser->getInteger();
      return OK;
    }
  if (parser->matchKeyword("LIFE"))
    {
      life_ = parser->getInteger();
      return OK;
    }
  if (parser->matchKeyword("MANA"))
    {
      mana_ = parser->getInteger();
      return OK;
    }
  if (parser->matchKeyword("UPKEEP"))
    {
      upkeep_ = parser->getInteger();
      return OK;
    }
  if (parser->matchKeyword("CONTROL"))
    {
      controlPoints_ = parser->getInteger();
      return OK;
      }
  if (parser->matchKeyword("STEALTH"))
    {
      stealth_ = parser->getInteger();
      return OK;
      }
  if (parser->matchKeyword("OBSERVATION"))
    {
      observation_ = parser->getInteger();
      return OK;
    }
      return OK;
}

void EntityStatistics::save(ostream &out)
{
  out << "Stats: ";
  if(initiative_) out<< " init: " << initiative_<<",";
  if(melee_) out<< " att: " << melee_ << ",";
  if(missile_) out<< " miss: " << missile_<< ",";
  if(defence_) out<< " def: " << defence_<< ",";
  if(damage_) out<< " dmg: " << damage_<< ",";
  if(numOfHits_) out<< " hits: " << numOfHits_<< ",";
  if(life_) out<< " life: " << life_<< ",";
  if(mana_) out<< " mana: " << mana_<< ",";
//  if(upkeep_) out<< ": " << upkeep_; // upkeep calculation 
  if(controlPoints_) out<< " CP: " << controlPoints_<< ",";
  if(stealth_) out<< " ste: " << stealth_<< ",";
  if(observation_) out<< " obs: " << observation_<< ".";
}



void EntityStatistics::print(ostream &out)
{
   bool isFirst = true;
  printThisStat_(initiative_, "init", isFirst, out);
  printThisStat_(melee_, "att", isFirst, out);
  printThisStat_(missile_, "miss", isFirst, out);
  printThisStat_(defence_, "def", isFirst, out);
  printThisStat_(damage_, "dmg", isFirst, out);
  printThisStat_(numOfHits_, "hits", isFirst, out);
  printThisStat_(life_, "life", isFirst, out);
  printThisStat_(mana_, "mana", isFirst, out);
  printThisStat_(controlPoints_, "CP", isFirst, out);
  printThisStat_(stealth_, "ste", isFirst, out);
  printThisStat_(observation_, "obs", isFirst, out);
//  if(initiative_) out<< " init: " << initiative_<<",";
//  if(melee_) out<< " att: " << melee_<< ",";
//  if(missile_) out<< " miss: " << missile_<< ",";
//  if(defence_) out<< " def: " << defence_<< ",";
//  if(damage_) out<< " dmg: " << damage_<< ",";
//  if(numOfHits_) out<< " hits: " << numOfHits_<< ",";
//  if(life_) out<< " life: " << life_<< ",";
//  if(mana_) out<< " mana: " << mana_<< ",";
////  if(upkeep_) out<< ": " << upkeep_; // upkeep calculation
//  if(controlPoints_) out<< " CP: " << controlPoints_<< ",";
//  if(stealth_) out<< " ste: " << stealth_<< ",";
//  if(observation_) out<< " obs: " << observation_; 
  out << ". ";
}

void  EntityStatistics::printThisStat_(int thisStat, const char * name, bool & isFirst, ostream &out)
{
    if(thisStat)
      {
          if(!isFirst)
              out <<", ";
          else
            isFirst = false;
          
          out<< name<<": " << thisStat;
       }
}

void EntityStatistics::addStats(EntityStatistics * stats)
{
 	initiative_ += stats->getInitiative();
	melee_ += stats->getMelee();
	defence_ += stats->getDefence();
	damage_ += stats->getDamage();
	numOfHits_ += stats->getHits();
	missile_ += stats->getMissile();
	life_ += stats->getLife();
	mana_ += stats->getMana();
	upkeep_ += stats->getUpkeep();
	controlPoints_ += stats->getControlPoints();
	stealth_ += stats->getStealth();
	observation_ += stats->getObservation();

    
}



void EntityStatistics::addPartialStats(EntityStatistics * stats, int numenator, int denominator)
{
  
 	initiative_ += (stats->getInitiative() * numenator) / denominator;
	melee_ += (stats->getMelee()* numenator) / denominator;
	defence_ += (stats->getDefence()* numenator) / denominator;
	damage_ += (stats->getDamage()* numenator) / denominator;
	numOfHits_ += (stats->getHits()* numenator) / denominator;
	missile_ += (stats->getMissile()* numenator) / denominator;
	life_ += (stats->getLife()* numenator) / denominator;
	mana_ += (stats->getMana()* numenator) / denominator;
	upkeep_ += (stats->getUpkeep()* numenator) / denominator;
	controlPoints_ += (stats->getControlPoints()* numenator); // CP are not scaled!
	stealth_ += (stats->getStealth()* numenator) / denominator;
	observation_ += (stats->getObservation()* numenator) / denominator;
  
}



void EntityStatistics::clearStats()
{
	initiative_ = 0;
	melee_= 0;
	defence_= 0;
	damage_= 0;
	numOfHits_= 0;
	missile_= 0;
	life_= 0;
	mana_= 0;
	upkeep_= 0;
	controlPoints_= 0;
	stealth_= 0;
	observation_= 0;
}

bool EntityStatistics::empty()
{
	if(initiative_ != 0)
    return false;
	if(melee_!= 0)
    return false;
	if(defence_!= 0)
    return false;
	if(damage_!= 0)
    return false;
	if(numOfHits_!= 0)
    return false;
	if(missile_!= 0)
    return false;
	if(life_!= 0)
    return false;
	if(mana_!= 0)
    return false;
	if(upkeep_!= 0)
    return false;
	if(controlPoints_!= 0)
    return false;
	if(stealth_!= 0)
    return false;
	if(observation_!= 0)
    return false;

    return true;
}
