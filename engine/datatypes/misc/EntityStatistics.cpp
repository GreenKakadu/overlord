/***************************************************************************
                          EntityStatistics.cpp 
                             -------------------
    begin                : Wed Feb 5 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "EntityStatistics.h"

EntityStatistics::EntityStatistics()
{
  clearStats();
}



/*
 * Does Initialization of stats data
 */
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
  if (parser->matchKeyword("RANGED_DAMAGE"))
    {
     rangedDamage_ = parser->getInteger();
      return OK;
    }
  if (parser->matchKeyword("DAMAGE_TYPE"))
    {
			if(parser->matchInteger())
				{
					switch(parser->getInteger())
					{
						case AIR:
						{
				 			damageType_= AIR;
      	 	    return OK;
						}
						case WATER:
						{
				 			damageType_= WATER;
      	 	    return OK;
						}
						case EARTH:
						{
				 			damageType_= EARTH;
      	 	    return OK;
						}
						case FIRE:
						{
				 			damageType_= FIRE;
      	 	    return OK;
						}
						case VOID_DAMAGE:
						{
                                                  damageType_= VOID_DAMAGE;
      	 	    return OK;
						}
						default:
						{
				 			damageType_= PHYSICAL;
      	 	    return OK;
						}

				 }
				}
			else
			{
				if(parser->matchKeyword("AIR"))
					{
						damageType_ = AIR;
      			return OK;
					}
				if(parser->matchKeyword("EARTH"))
					{
						damageType_ = EARTH;
      			return OK;
					}
				if(parser->matchKeyword("FIRE"))
					{
						damageType_ = FIRE;
      			return OK;
					}
				if(parser->matchKeyword("VOID"))
					{
                                          damageType_ = VOID_DAMAGE;
      			return OK;
					}
				if(parser->matchKeyword("WATER"))
					{
						damageType_ = WATER;
      			return OK;
					}
			}
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

void EntityStatistics::scaleControlPoints(int num, int factor)
{
    int cp = controlPoints_ * num;
    if(cp % factor ==0)
    {
        controlPoints_ = cp / factor;
    }
    else
    {
        controlPoints_ = (cp / factor) + 1;
    }
}


/*
 * Print stats data to output stream
 */
void EntityStatistics::reportStatistics(ostream &out)
{
   bool isFirst = true;
  printThisStat_(initiative_, "init", isFirst, out);
  printThisStat_(melee_, "att", isFirst, out);
  printThisStat_(missile_, "miss", isFirst, out);
  printThisStat_(defence_, "def", isFirst, out);
  printThisStat_(damage_, "dmg", isFirst, out);
  printThisStat_(rangedDamage_, "rng", isFirst, out);
  printThisStat_(damageType_, "type", isFirst, out);
  printThisStat_(numOfHits_, "hits", isFirst, out);
  printThisStat_(life_, "life", isFirst, out);
//  printThisStat_(mana_, "mana", isFirst, out); printed separately for all unit
//  printThisStat_(upkeep_, "upkeep", isFirst, out); printed separately for all unit
  printThisStat_(controlPoints_, "CP", isFirst, out);
  printThisStat_(stealth_, "ste", isFirst, out);
  printThisStat_(observation_, "obs", isFirst, out);
  out << ". ";
}

/*
 * Prints to output one statistic data if it is non-zero.
 */
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

/*
 * Adds stat modifiers
 */
void EntityStatistics::addStats(EntityStatistics * stats)
{
 	initiative_ += stats->getInitiative();
	melee_ += stats->getMelee();
	defence_ += stats->getDefence();
	damage_ += stats->getDamage();
	rangedDamage_ += stats->getRangedDamage();
	modifyDamageType(stats->getDamageType());// not additive
	numOfHits_ += stats->getHits();
	missile_ += stats->getMissile();
	life_ += stats->getLife();
	mana_ += stats->getMana();
	upkeep_ += stats->getUpkeep();
	controlPoints_ += stats->getControlPoints();
	stealth_ += stats->getStealth();
	observation_ += stats->getObservation();


}



/*
 * Set stat modifiers
 */
void EntityStatistics::setStats(EntityStatistics * stats)
{
 	initiative_ = stats->getInitiative();
	melee_ = stats->getMelee();
	defence_ = stats->getDefence();
	damage_ = stats->getDamage();
	rangedDamage_ = stats->getRangedDamage();
	modifyDamageType(stats->getDamageType());
	numOfHits_ = stats->getHits();
	missile_ = stats->getMissile();
	life_ = stats->getLife();
	mana_ = stats->getMana();
	upkeep_ = stats->getUpkeep();
	controlPoints_ = stats->getControlPoints();
	stealth_ = stats->getStealth();
	observation_ = stats->getObservation();


}



/*
 * Replace stat modifiers with those refined in stats
 * (only non-zero values will be used for replacement)
 */
void EntityStatistics::replaceStats(EntityStatistics * stats)
{
  if( stats->getInitiative() != VERY_BIG_NUMBER  ) initiative_ = stats->getInitiative();
  if( stats->getMelee() != VERY_BIG_NUMBER         ) melee_ = stats->getMelee();
  if( stats->getDefence()  != VERY_BIG_NUMBER     ) defence_ = stats->getDefence();
  if( stats->getDamage() != VERY_BIG_NUMBER       ) damage_ = stats->getDamage();
  if( stats->getRangedDamage() != VERY_BIG_NUMBER       ) rangedDamage_ = stats->getRangedDamage();
	 modifyDamageType(stats->getDamageType());
         if( stats->getHits()  != VERY_BIG_NUMBER        ) numOfHits_ = stats->getHits();
         if( stats->getMissile() != VERY_BIG_NUMBER      ) missile_ = stats->getMissile();
         if( stats->getLife()   != VERY_BIG_NUMBER       ) life_ = stats->getLife();
         if( stats->getMana()  != VERY_BIG_NUMBER        ) mana_ = stats->getMana();
         if( stats->getUpkeep() != VERY_BIG_NUMBER       ) upkeep_ = stats->getUpkeep();
         if( stats->getControlPoints() != VERY_BIG_NUMBER ) controlPoints_ = stats->getControlPoints();
         if( stats->getStealth()   != VERY_BIG_NUMBER    ) stealth_ = stats->getStealth();
         if( stats->getObservation() != VERY_BIG_NUMBER  ) observation_ = stats->getObservation();


}



/*
 * Adds proportional part of stat modifier
 */
void EntityStatistics::addPartialStats(EntityStatistics * stats, int numenator, int denominator)
{

 	initiative_ += (stats->getInitiative() * numenator) / denominator;
        if(initiative_ >= 100)
        {
            cout << "initiative = "<<initiative_<<endl;
        }
	melee_ += (stats->getMelee()* numenator) / denominator;
	defence_ += (stats->getDefence()* numenator) / denominator;
	damage_ += (stats->getDamage()* numenator) / denominator;
	rangedDamage_ += (stats->getRangedDamage()* numenator) / denominator;
	modifyDamageType(stats->getDamageType());// not additive
	numOfHits_ += (stats->getHits()* numenator) / denominator;
	missile_ += (stats->getMissile()* numenator) / denominator;
	life_ += (stats->getLife()* numenator) / denominator;
	mana_ += (stats->getMana()* numenator) / denominator;
	upkeep_ += (stats->getUpkeep()* numenator / denominator) ;
	controlPoints_ += (stats->getControlPoints()* numenator); // CP are not scaled!
	stealth_ += (stats->getStealth()* numenator) / denominator;
	observation_ += (stats->getObservation()* numenator) / denominator;
  
}



/*
 * Sets all stats to some value
 */
void EntityStatistics::clearStats(int value)
{
  initiative_ = value;
  melee_= value;
  defence_= value;
  damage_= value;
  rangedDamage_= value;
  damageType_= PHYSICAL;
  numOfHits_= value;
  missile_= value;
  life_= value;
  mana_= value;
  upkeep_= value;
  controlPoints_= value;
  stealth_= value;
  observation_= value;
}

/*
 * Checks that stats have some non-trivial info
 */
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
	if(rangedDamage_!= 0)
    return false;
	if(damageType_!= 0)
    return false;
	if(numOfHits_!= 0)
    return false;
	if(missile_!= 0)
    return false;
	if(life_!= 0)
    return false;
	if(mana_!= 0)
    return false;
//	if(upkeep_!= 0)
//    return false;
	if(controlPoints_!= 0)
    return false;
	if(stealth_!= 0)
    return false;
	if(observation_!= 0)
    return false;

    return true;
}


// Physical + Magic type of damage = Magic type of damage
// Physical + Physical  type of damage = Physical type of damage
// Magic + Magic type of damage = Physical type of damage

void EntityStatistics::modifyDamageType(DAMAGE_TYPE damageType)
{
	if (damageType_ == PHYSICAL)
	{
		damageType_ = damageType;
	}
	else
		{
			if (damageType != PHYSICAL) // two magic damage neutralize each other
				damageType_ = PHYSICAL;
		}
}
