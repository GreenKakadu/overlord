/* 
 * File:   AffectWeatherAction.cpp
 * Author: alex
 * 
 * Created on February 15, 2010, 9:16 PM
 */

#include "AffectWeatherAction.h"
#include "Entity.h"
#include "LocationEntity.h"
#include "WeatherRule.h"
#include "BinaryMessage.h"

extern ReportPattern * weatherChangedReport;
extern ReportPattern * weatherChangedReport2;

AffectWeatherAction     sampleAffectWeatherAction =     AffectWeatherAction("AFFECT_WEATHER_ACTION", &sampleAction);

AffectWeatherAction::AffectWeatherAction(const AffectWeatherAction * prototype )
                    : ActionRule(prototype)
{
    name_ = "Affect Weather Action";
}


GameData * AffectWeatherAction::createInstanceOfSelf()
{
  return CREATE_INSTANCE<AffectWeatherAction> (this);
}




ACTION_RESULT AffectWeatherAction::carryOut(Entity * entity, AbstractData * parameter,  int value)
{
  TokenEntity * mage = dynamic_cast<TokenEntity *>(entity);
  assert(mage);
  LocationEntity * target= dynamic_cast<LocationEntity *>(mage->getTarget());
  if(target == 0)
  {
      target = mage->getLocation();
  }

  WeatherRule * weather = dynamic_cast<WeatherRule *>(parameter);
  if(weather==0)
  {
      cout<<"Error: Weather parameter "<< parameter->print()<<" is wrong"<<endl;
  }
    target->setWeather(weather);
    
    target->addReport(new BinaryMessage(weatherChangedReport,target,weather));
    mage->addReport(new BinaryMessage(weatherChangedReport2,target,weather));

    return     ACTION_SUCCESS;
}

