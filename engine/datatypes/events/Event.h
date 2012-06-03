/* 
 * File:   Event.h
 * Author: alex
 * Serializable object keeping information about event that happened or can happen.
 * Created on February 25, 2010, 5:48 PM
 */

#ifndef _EVENT_H
#define	_EVENT_H
#include <string>
#include <vector>
#include <map>
#include "GameData.h"

using namespace std;
class Entity;
class EventRule;
class TokenEntity;
class FactionEntity;
class LocationEntity;
class OrderLine;
class Event : public GameData{
public:
       Event(const string & keyword, GameData * parent ); // Obligatory constructor used for prototyping
       Event( const Event * prototype );// Obligatory constructor used for empty event creation


    virtual ~Event();
    GameData * createInstanceOfSelf();
    static int eventCounter;
//    static map<string, Event *> eventPool;
    static string eventPrefix;
// Interfaces
static Event * createNewEvent(TokenEntity * origin, string event, OrderLine *  orderId = 0,
        AbstractData * param1 = 0, AbstractData * param2 = 0,
        AbstractData * param3 = 0, AbstractData * param4 = 0,
        AbstractData * param5 = 0, AbstractData * param6 = 0);
static Event * createNewEvent(LocationEntity * origin, string event, OrderLine *  orderId = 0,
        AbstractData * param1 = 0, AbstractData * param2 = 0,
        AbstractData * param3 = 0, AbstractData * param4 = 0,
        AbstractData * param5 = 0, AbstractData * param6 = 0);
static Event * createNewEvent(string event, OrderLine *  orderId = 0,
        AbstractData * param1 = 0, AbstractData * param2 = 0,
        AbstractData * param3 = 0, AbstractData * param4 = 0,
        AbstractData * param5 = 0, AbstractData * param6 = 0);

virtual Event * createEventImage(FactionEntity * faction);
void updateEvent(Event * imageToAdd);
   virtual STATUS     initialize ( Parser *parser );
   virtual void       save (ostream &out);
   bool isObservableBy(TokenEntity * agent);
  inline EventRule * getEventRule(){return eventType_;}
  vector <AbstractData *> aPrint();
private:
  static Event * createEvent(Entity * origin, EventRule * eventRule, OrderLine *  orderId = 0, // Actual constructor
          AbstractData * param1 = 0, AbstractData * param2 = 0,
          AbstractData * param3 = 0, AbstractData * param4 = 0,
          AbstractData * param5 = 0, AbstractData * param6 = 0);
//    int setOrigin(Entity * origin);
    EventRule * eventType_;
    int turn_;
    int day_;
    Entity * origin_;
    vector <AbstractData *> parameters_;
};
extern Event sampleEvent;
typedef vector <Event *>::iterator  EventIterator;
#endif	/* _EVENT_H */

