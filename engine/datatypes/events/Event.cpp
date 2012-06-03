/* 
 * File:   Event.cpp
 * Author: alex
 * 
 * Created on February 25, 2010, 5:48 PM
 */

#include "Event.h"
#include "EventRule.h"
#include "Entity.h"
#include "FactionEntity.h"
#include "LocationEntity.h"
#include "GameConfig.h"
#include "LineParser.h"
#include "datatypes/AbstractData.h"
#include "datatypes/IntegerData.h"
#include "conditions/BasicCondition.h"
#include "TokenEntity.h"
#include "OrderLine.h"

extern string longtostr(long u);
Event sampleEvent("EVENT", &sampleGameData);
int Event::eventCounter=0;
//map<string, Event *> Event::eventPool;
string Event::eventPrefix = "V";

Event::Event(const string & keyword, GameData * parent ): GameData(keyword,parent)
{
    turn_ = 0;
    day_ =0;
}


Event::Event( const Event * prototype ): GameData(prototype)
{
    turn_ = 0;
    day_ =0;
}

Event::~Event()
{
}

GameData * Event::createInstanceOfSelf()
{
  return CREATE_INSTANCE<Event> (this);
}


STATUS     Event::initialize ( Parser *parser )
{
  if (parser->matchKeyword ("TYPE") )
    {
      eventType_ = gameFacade->eventRules[parser->getWord()];
      return OK;
    }
  if (parser->matchKeyword ("ORIGIN") )
    {
      origin_ = gameFacade->getDataManipulator()->findEntity(parser->getWord());
      return OK;
    }
  if (parser->matchKeyword ("TURN") )
    {
      turn_ = parser->getInteger();
      return OK;
    }
  if (parser->matchKeyword ("DAY") )
    {
      day_ = parser->getInteger();
      return OK;
    }

  if (parser->matchKeyword ("PARAMETER") )
    {
        int pramNum = parameters_.size();
        AbstractData * temp = eventType_->getParamType(pramNum)->loadInstance(parser);
        if(temp)
        {
          parameters_.push_back(temp);
          //cout<<"<===par=== "<<tag_<<" "<<temp->print()<<endl;
        }
      return OK;
    }

      return OK;
}

void       Event::save (ostream &out)
{
   out << keyword_ << " " <<tag_ << endl;
   out << "TYPE "<<eventType_->getTag()<< endl;
   out << "ORIGIN "<<origin_->getTag()<< endl;
   out << "TURN "<<turn_<< endl;
   if(gameFacade->getGameConfig()->daysInMonth > 1)
   {
    out << "DAY "<<day_<< endl;
   }

    for(vector <AbstractData * >::iterator iter = parameters_.begin(); iter != parameters_.end(); ++iter)
    {
            out << "PARAMETER ";
            if(*iter) // May be empty
            {
            (*iter)->saveAsParameter(out);
            }
            out<< endl;
    }
  out << endl;
}


Event * Event::createNewEvent(TokenEntity * origin, string event, OrderLine *  orderId,
                            AbstractData * param1, AbstractData * param2,
                            AbstractData * param3, AbstractData * param4,
                            AbstractData * param5, AbstractData * param6)
{
    EventRule * eventRule = gameFacade->eventRules[event];
    if (eventRule)
    {
        if (!eventRule->isGlobal())
        {
            LocationEntity * location = origin->getLocation();
            if (location)
            {
                return createEvent(location, eventRule, orderId, param1, param2, param3, param4, param5, param6);
            }
            else
            {
                cerr << "ERROR: Origin " << origin->getTag()
                        << " has no location defined when creating event " << eventRule->getTag() << endl;
                return 0;
            }

        }
        else // Event is global
        {

                cerr << "ERROR: Origin " << origin->getTag()
                        << " has no location defined when creating event " << eventRule->getTag() << endl;
                return 0;

        }
    }
    return 0;
}


 
 Event * Event::createNewEvent(LocationEntity * origin, string event, OrderLine *  orderId,
        AbstractData * param1, AbstractData * param2,
        AbstractData * param3, AbstractData * param4,
        AbstractData * param5, AbstractData * param6)
 {
     EventRule * eventRule = gameFacade->eventRules[event];
    if(eventRule)
    {
        if(!eventRule->isGlobal())
        {
           return createEvent(origin, eventRule, orderId, param1, param2, param3, param4, param5, param6);
        }
        else
        {
           cerr << "Location "<<origin->getTag()<<" was given as origin " 
                   << " to global event("<<eventRule->getTag()
                   <<"). \nChange event to non-global or provide Faction as origin in event creation."<<endl;
          return 0;            
        }    
    }
   return 0;  
 }
 
 

Event * Event::createNewEvent(string event, OrderLine *  orderId,
        AbstractData * param1, AbstractData * param2,
        AbstractData * param3, AbstractData * param4,
        AbstractData * param5, AbstractData * param6)
 {
      EventRule * eventRule = gameFacade->eventRules[event];
    if(eventRule)
    {
        if(eventRule->isGlobal())
        {
           return createEvent(gameFacade->getGameEntity(), eventRule, orderId, param1, param2, param3, param4, param5, param6);
        }
        else
        {
           cerr << "Not provided origin to non-global event("<<eventRule->getTag()
                   <<"). \nChange event to global or provide Token as origin in event creation."<<endl;
          return 0;            
        }
    }
     return 0; 
 }
 
 


/*
 * This is actual constructor that is used to create event
 */

 Event * Event::createEvent(Entity * origin, EventRule * eventRule, OrderLine *  orderId,
        AbstractData * param1, AbstractData * param2,
        AbstractData * param3, AbstractData * param4,
        AbstractData * param5, AbstractData * param6)
 {
     if(eventRule==0)
     {
         return 0;
     }
     if(origin->isEventDuplicateExist(eventRule,orderId))
      {
         return 0;
      }
        
     Event * newEvent = new Event(&sampleEvent);
     string tag = Event::eventPrefix + longtostr(eventCounter);
 //    eventPool[tag] = newEvent;
     eventCounter++;
     newEvent->tag_ = tag;
     gameFacade->events.addEvent(newEvent);
     newEvent->eventType_ = eventRule;
     newEvent->origin_ = origin;
     newEvent->turn_ = gameFacade->getGameTurn();
     newEvent->day_ = gameFacade->getCurrentDay();

     // day, turn
     if(param1)
     {
        newEvent->parameters_.push_back(param1);
     }
     if(param2)
     {
        newEvent->parameters_.push_back(param2);
     }

     if(param3)
     {
        newEvent->parameters_.push_back(param3);
     }
     if(param4)
     {
        newEvent->parameters_.push_back(param4);
     }
     if(param5)
     {
        newEvent->parameters_.push_back(param5);
     }
     if(param6)
     {
        newEvent->parameters_.push_back(param6);
     }
     origin->addEvent(newEvent,orderId);
     cout<< "Event "<<newEvent->getTag()<<" ("<<eventRule->getTag()<<") created for "<<origin->getTag()<<endl; 
     return newEvent;
 }


bool Event::isObservableBy(TokenEntity * agent)
{
    BasicCondition * condition = eventType_->getCondition();
    if(condition)
    {
   return condition->isSatisfied(agent,origin_);
    }
    else
    {
        return true;
    }
}

Event  * Event::createEventImage(FactionEntity * faction)
{
   Event * newEvent = new Event(&sampleEvent);
     newEvent->tag_ = tag_;
     newEvent->origin_ = origin_;
     newEvent->eventType_ = eventType_;
     newEvent->turn_ =turn_ ;
     newEvent->day_ = day_;
     for(int i=0; i<eventType_->getParamNumber(); ++i)
     {
        newEvent->parameters_.push_back(faction->findImage(parameters_[i]));
      //cout<< "==="<<tag_ <<"===> "<<(parameters_[i])->print()<<" saved as "<<(newEvent->parameters_[i])->print()<<endl;;

     }
   return newEvent;
}

void Event::updateEvent(Event * imageToAdd)
{
  if (getTag() == imageToAdd->getTag())
    {
      for (int i = 0; i < eventType_->getParamNumber(); ++i)
        {
          if(GameFacade::isUnknownParameter(parameters_[i]))
            {
              parameters_[i] = imageToAdd->parameters_[i];
            }
        }
    }

}




//void Event::setOrigin(Entity * origin)
//{
//     // origin may be Location or Faction
//     if (eventType_->isLocalizable())
//     {
//       LocationEntity * location;
//       TokenEntity * token = dynamic_cast<TokenEntity *>(origin);
//       if(token)
//       {
//        location = token->getLocation();
//        if(location)
//        {
//        origin_ = location; 
//        }
//        else
//        {
//            cerr << "ERROR: Origin "<<origin->getTag() 
//                   << " has no location defined when creating event "<<eventType_->getTag()<<endl;
//          return 0; 
//        }
//       }
//       else // Origin is not a token
//       {
//            // May be this is already location?
//            location = dynamic_cast<LocationEntity *>(origin); 
//            if(location)
//            {
//              origin_ = location;  
//            }
//            else
//            {
//           cerr << "Non-token origin "<<origin->getTag() 
//                   << " was given to localizable event("<<eventType_->getTag()
//                   <<"). \nChange event to global or provide Token as origin in event creation."<<endl;
//          return 0; 
//            }
//       }
//     }
//     else // Non-localizable event
//     {
//         
//     }
//     
//   return 1; 
//}
vector <AbstractData *> Event::aPrint()
    {
  vector <AbstractData *> v;
  int i=0;
  v.push_back(this);
  v.push_back(new StringData("turn_ "));
  if(gameFacade->getGameConfig()->daysInMonth > 1)
    {
  v.push_back(new StringData(" : "));
  v.push_back(new IntegerData(day_));
    }
  v.push_back(new StringData(" "));

  for(vector <AbstractData * >::iterator iter = parameters_.begin(); iter != parameters_.end(); ++iter)
  {
      v.push_back(new StringData(eventType_->getParam(i)));
      v.push_back(*iter);
      i++;
  }
  v.push_back(new StringData(eventType_->getParam(i)));
 return v;
    }
