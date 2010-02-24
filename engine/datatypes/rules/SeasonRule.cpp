/***************************************************************************
                          SeasonRule.cpp
                             -------------------
    begin                : Jan 27 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include <algorithm>
#include "StringData.h"
#include "SeasonRule.h"
#include "SkillRule.h"
#include "BonusElement.h"
#include "WeatherRule.h"
#include "WeatherElement.h"

SeasonRule   sampleSeason   ("SEASON",  &sampleGameData);
RulesCollection  <SeasonRule> seasons(new DataStorageHandler("seasons.rules"));




SeasonRule * findSeasonByTag(const string &tag)
{
 return GET_FROM_COLLECTION<SeasonRule>(&seasons,tag);
}



SeasonRule::SeasonRule ( const SeasonRule * prototype ) : Rule(prototype)
{
	totalProbabilityScore_ =0;
}


GameData * SeasonRule::createInstanceOfSelf()
{
  return CREATE_INSTANCE<SeasonRule> (this);
}


STATUS
SeasonRule::initialize        ( Parser *parser )
{
  GameData::initialize(parser);

  if (parser->matchKeyword("WEATHER"))
	{
        WeatherElement * newWeather = WeatherElement::readElement(parser);
        if(newWeather)
				{
		// "concatenate" probabilities - add to each one sum of all previous
					int current = newWeather->getProbability();
					newWeather->setProbability(current + totalProbabilityScore_);
					totalProbabilityScore_ += current;
          weatherProbabilities_.push_back(newWeather);
				}
	  return OK;
	}
		skillBonuses_.initialize(parser);
   Rule::initialize(parser);
     return OK;

 }





void SeasonRule::printDescription(ReportPrinter & out)
{
    out << print()<< ": "<< getDescription()<<".";
}

vector <AbstractData *> SeasonRule::aPrint()
{
  vector <AbstractData *> v;
  return v;
}



extern int Roll_1Dx(int n);

WeatherRule * SeasonRule::predictWeather()
{
	int roll;
	if(weatherProbabilities_.empty())
	{
		if(weathers.size() == 0)
			return 0;
		roll = Roll_1Dx(weathers.size());
		return weathers[roll];
	}
	roll = Roll_1Dx(totalProbabilityScore_);
	for (vector <WeatherElement *>::iterator iter = weatherProbabilities_.begin();
			iter != weatherProbabilities_.end(); ++iter)
			{
				if (roll < (*iter)->getProbability())
					return (*iter)->getWeather();
			}
	return 0;
}


/*
 * Get bonus to skill use
 */

int SeasonRule::getProductionBonusValue(SkillRule * skill)
{
  return skillBonuses_.getProductionBonus(skill);
}



int SeasonRule::getLearningBonus(SkillRule * skill)
{
  return skillBonuses_.getLearningBonus(skill);
}



/*
 * Get bonus to study speed
 */
int SeasonRule::getStudyBonus(SkillRule * skill)
{
  return skillBonuses_.getStudyBonus(skill);
}
