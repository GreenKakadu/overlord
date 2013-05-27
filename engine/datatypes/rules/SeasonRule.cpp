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
//RulesCollection  <SeasonRule> seasons(new DataStorageHandler("seasons.rules"),&sampleSeason);




SeasonRule * findSeasonByTag(const string &tag)
{
 return GET_FROM_COLLECTION<SeasonRule>(&(gameFacade->seasons),tag);
}



SeasonRule::SeasonRule ( const SeasonRule * prototype ) : Rule(prototype)
{
	totalProbabilityScore_ =0;
}
GameData * SeasonRule::createInstanceOfSelf()
{
    return CREATE_INSTANCE<SeasonRule > (this);
}

STATUS
SeasonRule::initialize(Parser *parser)
{
    GameData::initialize(parser);

    if (parser->matchKeyword("WEATHERS"))
    {
        WeatherElement * newWeather = WeatherElement::readElement(parser);
        if (newWeather)
        {
            // First check, that this weather was not added
            for (vector <WeatherElement>::iterator iter = weather_.begin(); iter != weather_.end(); ++iter)
            {
                if ((*iter).getWeather() == newWeather->getWeather())// It was already added we should overwrite it and recalculate
                {
                    int oldProbability = (*iter).getProbability();
                    (*iter).setProbability(newWeather->getProbability());
                    int delta = newWeather->getProbability() - oldProbability; // may be negative
                    if (delta)
                    {
                        cout << "Warning: weather record overwriting in Seasons.rules"<<endl;
                        totalProbabilityScore_ += delta;
                        bool flag = false;
                        for (vector <WeatherElement *>::iterator iter = weatherProbabilities_.begin();
                                iter != weatherProbabilities_.end(); ++iter)
                        {
                            if ((*iter)->getWeather() == newWeather->getWeather())//
                            {
                                flag = true;// from here start to apply change (delta) to all consequent probbilities
                            }
                            if(flag)
                            {
                            (*iter)->setProbability((*iter)->getProbability() + delta);
                            }
                        }
                    }


                    return OK;
                }
            }
            // "concatenate" probabilities - add to each one sum of all previous
            int current = newWeather->getProbability();
            weather_.push_back(WeatherElement(newWeather->getWeather(),
                                              newWeather->getProbability()));
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


void SeasonRule::save(ostream &out)
{
  Rule::save(out);
  for (vector <WeatherElement>::iterator iter = weather_.begin();
          iter != weather_.end(); ++iter)
    {
         out<<"WEATHERS ";
         (*iter).save(out);
    }

  skillBonuses_.save(out);
}



void SeasonRule::cleanAttributes()
{
      Rule::cleanAttributes();
        skillBonuses_.clean();
}


void SeasonRule::printDescription(ReportPrinter & out)
{
    out << print()<< ": "<< getDescription()<<".";
}




vector <AbstractData *> SeasonRule::aPrint()
{
    vector <AbstractData *> out;
    out.push_back(new StringData(getDescription()));

    out.push_back(new StringData(" "));

  vector<AbstractArray> v = skillBonuses_.aPrintReport();
 // bool isFirst = true;

  for(vector<AbstractArray>::iterator iter= v.begin(); iter != v.end(); iter++)
    {
//      if(!isFirst)
//      {
//      out.push_back(new StringData(", "));
//        }
//      else
//      {
//          isFirst = false;
//      }
      for(AbstractArray::iterator iter2= (*iter).begin(); iter2 != (*iter).end(); iter2++)
        {
            out.push_back(*iter2);
        }

    }


  return out;
}



extern int Roll_1Dx(int n);

WeatherRule * SeasonRule::predictWeather()
{
	int roll;
	if(weatherProbabilities_.empty())
	{
		if(gameFacade->weathers.size() == 0)
			return 0;
		roll = Roll_1Dx(gameFacade->weathers.size());
		return gameFacade->weathers[roll];
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
