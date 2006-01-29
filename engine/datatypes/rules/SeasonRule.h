/***************************************************************************
                          SeasonRule.h

                             -------------------
    begin                : Jan 27 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef SEASON_H
#define SEASON_H
#include <vector>
#include "Rule.h"
#include "MovementMode.h"
#include "RulesCollection.h"
#include "DataStorageHandler.h"
#include "SkillBonusComboAttribute.h"



class SkillRule;
class WeatherRule;
class WeatherElement;

class  SeasonRule : public Rule
{
    public:
      SeasonRule ( const string & keyword, GameData * parent) : Rule(keyword, parent){}
      SeasonRule ( const SeasonRule * prototype );
      ~SeasonRule (){}
      virtual STATUS     initialize      ( Parser *parser);
      GameData * createInstanceOfSelf();
      void printDescription(ReportPrinter & out);
      WeatherRule * predictWeather();
         int getProductionBonusValue(SkillRule * skill);
         int getStudyBonus(SkillRule * skill);
         int getLearningBonus(SkillRule * skill);
    protected:

    private:
		vector <WeatherElement *> weatherProbabilities_;
		int totalProbabilityScore_;
		SkillBonusComboAttribute skillBonuses_;
};

extern SeasonRule    sampleSeason;
extern RulesCollection <SeasonRule>   seasons;
#endif
