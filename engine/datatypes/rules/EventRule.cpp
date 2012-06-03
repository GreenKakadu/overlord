/* 
 * File:   EventRule.cpp
 * Author: alex
 * 
 * Created on March 29, 2011, 10:34 AM
 */

#include "EventRule.h"
#include "StringData.h"
#include "PrototypeManager.h"
#include "BasicCondition.h"

EventRule     sampleEventRule      ("EVENT_TYPE",     &sampleGameData);
BasicCondition  privateCondition ("PRIVATE",      &sampleGameData); // to define private events
EventRule:: EventRule( const string & keyword, GameData * parent): Rule(keyword, parent)
{
}



EventRule:: EventRule( const EventRule * prototype ) : Rule(prototype)
{
    nParam_=0;
    condition_ =0;
    isGlobal_ = false;
}



//EventRule::EventRule(const EventRule& orig)
//{
//}



EventRule::~EventRule()
{
}





GameData * EventRule::createInstanceOfSelf()
{
  return CREATE_INSTANCE<EventRule> (this);
}


STATUS
EventRule::initialize        ( Parser *parser )
{
  if (parser->matchKeyword("PARAMETERS_NUM"))
  {
    nParam_  = parser->getInteger();
    return OK;
  }

  if (parser->matchKeyword("EVENT_TEXT"))
  {
      string parameter = parser->getText();
      //if(!parameter.empty())// Parameter may be empty
      //{
        params_.push_back(parser->getText());
      //}
     return OK;
  }
  if (parser->matchKeyword("PARAMETER_TYPE"))
  {
      string type = parser->getWord();
      if(!type.empty())
      {
        AbstractData * dataType =   prototypeManager->abstractFindInRegistry(type);
        if(dataType)
        {
         paramTypes_ .push_back(dataType);
        }
        else
        {
            string eventName;
            if(!tag_.empty())
            {
              eventName = tag_;
            }
            else
            {
              eventName = "unknown";
            }
            cerr << "Unknown type of parameter "<<type<<" in event  rule "<<eventName <<" initialization"  <<endl;
        }
      }
     return OK;
  }
   if (parser->matchKeyword("CONDITION"))
    {

      condition_  = BasicCondition::readCondition(parser);
      if(condition_)
      {
        condition_->initialize(parser) ;
       }
      return OK;
    }
   if (parser->matchKeyword("GLOBAL"))
    {
    isGlobal_ = true;
   }
  
  GameData::initialize(parser);
  return OK;
}

// Actually not implemented. No need to serialize types of event
void EventRule::save(ostream &out)
{
  Rule::save(out);
}


// Actually not implemented. No need to print description  for type of event
void EventRule::printDescription(ReportPrinter & out)
{
  out << print()<< ": ";
}


// Actually not implemented. No need to print  type of event
vector <AbstractData *> EventRule::aPrint()
{
  vector <AbstractData *> v;
  v.push_back(this);
  v.push_back(new StringData(": "));
  return v;
}



string EventRule::getParam(int num)
{
  if(paramTypes_.size() > num)
  {
  return params_[num];
  }
  else
      return string("");
}

AbstractData * EventRule::getParamType(int num)
{
    if(paramTypes_.size() > num)
    {
    return paramTypes_[num];
    }
    else
        return 0;
}
