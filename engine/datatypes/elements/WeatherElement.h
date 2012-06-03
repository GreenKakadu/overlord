/***************************************************************************
                          WeatherElement.h
                             -------------------
    begin                : Fri Feb 14 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef WEATHER_ELEMENT_H
#define WEATHER_ELEMENT_H
#include "Element.h"
#include "WeatherRule.h"

using namespace std;

/**
  *@author Alex Dribin
  */
// WeatherElement used for defining probability of some weather
typedef  Element2<WeatherRule,int> BasicWeatherElement;

class WeatherElement : public BasicWeatherElement{
public:
	WeatherElement(WeatherRule * rule, int probability) : BasicWeatherElement (rule,probability){}
 ~WeatherElement(){}
   inline WeatherRule *   getWeather()      const     {return rule_;}
   inline int          getProbability()    const     {return parameter1_;}
   inline void         setProbability(int value)     { parameter1_ = value;}
   inline bool isValidElement() const {return (( rule_ != 0 ) && (parameter1_ != 0));}
//   inline void         setWeather(WeatherRule * rule) { rule_ = rule;}
//   void save(ostream & out)
//    {out << rule_->getTag() << " " <<  parameter1_   << endl;}

    string print()
    {
      if( parameter1_ >= 100)
        return string (1,'+') + longtostr(parameter1_ - 100)  + "% to " + rule_->getName();
      else
        return string (1,'-') + longtostr(100 - parameter1_)  + "% to " + rule_->getName();
    }

  static WeatherElement  * readElement (Parser * parser)
      {
        WeatherRule * weather = gameFacade->weathers[parser->getWord()];
        int probability = parser->getInteger();
        if(( weather == 0 ) || (probability == 0) )
          return 0;
        else
        return new WeatherElement(weather, probability);
      }
	protected:

};

#endif
