/***************************************************************************
                          WeatherRule.h

                             -------------------
    begin                : Jan 27 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#ifndef WEATHER_H
#define WEATHER_H
#include <vector>
#include "Rule.h"
#include "MovementVariety.h"
#include "RulesCollection.h"
#include "DataStorageHandler.h"
#include "Element.h"
#include "SkillBonusComboAttribute.h"

class SkillRule;


class  WeatherRule : public Rule
{
    public:
      WeatherRule ( const string & keyword, GameData * parent) : Rule(keyword, parent){}
      WeatherRule ( const WeatherRule * prototype );
      ~WeatherRule (){}
      virtual STATUS     initialize      ( Parser *parser);
      virtual void save(ostream &out);
      void cleanAttributes();
      GameData * createInstanceOfSelf();
      void printDescription(ReportPrinter & out);
      vector <AbstractData *> aPrint();
         int getProductionBonusValue(SkillRule * skill);
         int getStudyBonus(SkillRule * skill);
         int getLearningBonus(SkillRule * skill);
    protected:

    private:
		SkillBonusComboAttribute skillBonuses_;

};

extern WeatherRule * findWeatherByTag(const string &tag);
extern WeatherRule    sampleWeather;
//extern RulesCollection  <WeatherRule> weathers;


#endif
