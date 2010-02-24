/***************************************************************************
                          WeatherRule.cpp
                             -------------------
    begin                :
    copyright            : (C) 2005 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "StringData.h"
#include "WeatherRule.h"
#include "SkillRule.h"
#include "BonusElement.h"

WeatherRule   sampleWeather   ("WEATHER",  &sampleGameData);
RulesCollection  <WeatherRule> weathers(new DataStorageHandler("weathers.rules"));




WeatherRule * findWeatherByTag(const string &tag)
{
 return GET_FROM_COLLECTION<WeatherRule>(&weathers,tag);
}



WeatherRule::WeatherRule ( const WeatherRule * prototype ) : Rule(prototype)
{
}


GameData * WeatherRule::createInstanceOfSelf()
{
  return CREATE_INSTANCE<WeatherRule> (this);
}


STATUS
WeatherRule::initialize        ( Parser *parser )
{
  GameData::initialize(parser);
		movementBonuses_.initialize(parser);
		skillBonuses_.initialize(parser);
   Rule::initialize(parser);
     return OK;

 }



void WeatherRule::printDescription(ReportPrinter & out)
{
    out << print()<< ": "<< getDescription()<<".";
}


/*
 * Get bonus to skill use
 */
int WeatherRule::getProductionBonusValue(SkillRule * skill)
{
  return skillBonuses_.getProductionBonus(skill);
}



int WeatherRule::getLearningBonus(SkillRule * skill)
{
  return skillBonuses_.getLearningBonus(skill);
}



/*
 * Get bonus to study speed
 */
int WeatherRule::getStudyBonus(SkillRule * skill)
{
  return skillBonuses_.getStudyBonus(skill);
}
  
vector <AbstractData *> WeatherRule::aPrint()
{
  vector <AbstractData *> v;
  v.push_back(this);
  v.push_back(new StringData(": " + getDescription() + "."));
  return v;
}