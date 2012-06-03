/***************************************************************************
                          Entity.cpp  -  description
                             -------------------
    begin                : Sun Nov 17 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include <algorithm>
#include "Entity.h"
#include "UnitEntity.h"
#include "ReportElement.h"
#include "InventoryElement.h"
#include "TeachingOffer.h"
#include "OrderProcessor.h"
#include "DataManipulator.h"
#include "Event.h"

extern bool testMode;
//extern DataManipulator * dataManipulatorPtr;
Entity         sampleEntity  ("ENTITY",  &sampleGameData);

//static vector <BasicAttribute *> Entity::attributes;
Entity::Entity ( const Entity * prototype ): GameData(prototype)
{
  fullDayOrder_ = 0;
  silent_ = false;
  traced_ = false;
  disobeying_ = false;
  isPayingUpkeep_ = true;
  imageLevel_ = UNKNOWN_IMAGE;
  isImage_ = false;
  isUnknownEntity_ = false;
}



Entity::~Entity()
{
  cleanCollectedReports();
//  cout << this << " destroyed.\n";
}


//  Processes all possible ( at this phase) orders for Entity.
// Each order processed only once
bool  Entity::process(ProcessingMode * processingMode)

{
	return OrderProcessor::process(this,processingMode);
}



bool Entity::updateOrderResults(ORDER_STATUS result)
{
 OrderIterator  currentIterator = find(orders_.begin(), orders_.end(), currentOrder_);
 if(currentIterator != orders_.end())
  return OrderProcessor::processOrderResults(this,result,currentIterator);
 else
 {
   cout << "Warning: current order was not found for "<<(*this)<<endl;
   return true;
   }
}



//bool Entity::processOrderResults(ORDER_STATUS result, OrderIterator & currentIterator)
//{
//
//  return false;
//}


/*
 * When order completed  state of all conditional orders depending on it
 * should be reexamined:
 * For successfuly completed orders "-" modifiers should be removed and
 * orders with "+" removed.
 * For orders completed with invalid status  "+" modifiers should be
 * removed and orders with "-" removed.

 */
//void Entity::postProcessOrder(ORDER_STATUS result, OrderIterator  iter)
//
//{
//}



GameData * Entity::createInstanceOfSelf()
{
   return CREATE_INSTANCE<Entity> (this);
}



STATUS
Entity::initialize        ( Parser *parser )
{
STATUS currentResult = OK;
//STATUS result = OK;

      if (parser->matchKeyword ("NAME") )
        {
	  setName(parser->getText());
	  return OK;
	}
      if (parser->matchKeyword("DESCRIPTION"))
	{
	  setDescription(parser->getText());
	  return OK;
	}
       if (parser->matchKeyword("ORDER"))
 	{
	  orders_.push_back(new OrderLine(parser->getText(),this));
 	}

       if (parser->matchKeyword ("UNKNOWN") )
         {
    	   isUnknownEntity_ = true;
         }
       if (parser->matchKeyword("EVENT"))
 	{
    	   string tag =parser->getText();
    	   Event * event = gameFacade->events.findEvent(tag);
    	   if(event)
    	   {
    	   collectedEvents_.push_back(event);
    	   }
 	}
       if (parser->matchKeyword("RELATED_EVENT"))
 	{
    	   string tag =parser->getText();
    	   Event * event = gameFacade->events.findEvent(tag);
    	   if(event)
    	   {
    	   relatedEvents_.push_back(event);
    	   }
 	}

        if (parser->matchKeyword("TRACED"))
        {
          traced_ = true;
          return OK;
        }
        if (parser->matchKeyword("TIMESTAMP"))
        {
          timeStamp_.init(parser);
          return OK;
        }


  // currentResult = enchantments_.initialize(parser);
   if(currentResult != OK)
    return currentResult;

	  return OK;

}

//STATUS
//Entity::initializeEvents        ( Parser *parser )
//{
//       if (parser->matchKeyword("EVENT"))
// 	{
//    	   string tag =parser->getText();
//    	   Event * event = gameFacade->events.findEvent(tag);
//    	   if(event)
//    	   {
//    	    collectedEvents_.push_back(event);
//    	    return OK;
//    	   }
// 	}
//       if (parser->matchKeyword("RELATED_EVENT"))
// 	{
//    	   string tag =parser->getText();
//    	   Event * event = gameFacade->events.findEvent(tag);
//    	   if(event)
//    	   {
//    	    relatedEvents_.push_back(event);
//    	    return OK;
//    	   }
// 	}
//	  return IO_ERROR;
//}


void Entity::save(ostream &out) {
	out << endl << getCollectionKeyword() << " " << tag_ << " " << getKeyword()
			<< endl;
	getTimeStamp().save(out);
	if (!name_.empty())
		out << "NAME " << name_ << endl;
	if (!description_.empty())
		out << "DESCRIPTION " << description_ << endl;
	enchantments_.save(out);
	//  out << endl;

	saveEvents(out);
	for (OrderIterator iter = orders_.begin(); iter != orders_.end(); iter++)
	{
		(*iter)->save(out);
	}

	if (traced_)
		out << "TRACED" << endl;
}

void Entity::save(ostream &out, string prefix)
{
    GameData::save(out,prefix);
  enchantments_.save(out,prefix);
  getTimeStamp().save(out);
  for (OrderIterator iter = orders_.begin(); iter != orders_.end(); iter++)
    {
           (*iter)->save(out);
    }
}

/*
 * This method updates current Entity's  with the data from imagetoAdd
 * In some cases overwrite, in some add in some combine.
 *
 */


void Entity::updateImage(Entity *imagetoAdd)
{
  if(this->getTimeStamp() < imagetoAdd->getTimeStamp())      // Image is newer
    {
        setName(imagetoAdd->getName());
        setDescription(imagetoAdd->getDescription());
        setTimeStamp(imagetoAdd->getTimeStamp());
        collectedEvents_ = imagetoAdd->getAllCollectedEvents();
        relatedEvents_= imagetoAdd->getAllRelatedEvents();

        if(imagetoAdd->getImageLevel() == PRIVATE_IMAGE)
          {
            orders_ = imagetoAdd->orders_;
          }
        imageLevel_ = imagetoAdd->getImageLevel();

    }
  else if(this->getTimeStamp() == imagetoAdd->getTimeStamp())// Image is the same age
    {
      if(this->getImageLevel() < imagetoAdd->getImageLevel())// new image has more reliable data
        {
          setName(imagetoAdd->getName());
          setDescription(imagetoAdd->getDescription());
          if(imagetoAdd->getImageLevel() == PRIVATE_IMAGE)
            {
              orders_ = imagetoAdd->orders_;
            }
          imageLevel_ = imagetoAdd->getImageLevel();
        }
     // try to combine events
      for(auto iter = relatedEvents_.begin(); iter != relatedEvents_.end(); ++iter)
        {
          for(auto iter2 = imagetoAdd->relatedEvents_.begin(); iter2 != imagetoAdd->relatedEvents_.end(); ++iter2)
            {
              (*iter)->updateEvent(*iter2);
            }
        }
      for(auto iter = collectedEvents_.begin(); iter != collectedEvents_.end(); ++iter)
         {
           for(auto iter2 = imagetoAdd->collectedEvents_.begin(); iter2 != imagetoAdd->collectedEvents_.end(); ++iter2)
             {
               (*iter)->updateEvent(*iter2);
             }
         }
    }
  else                                                       // Image is older
    {

    }

}


ostream &operator << ( ostream &out, Entity * entity)
{
  out << entity->getTag() << " ";
	return out;
}



void      Entity::saveEvents (ostream &out)
{
	for (EventIterator iter = collectedEvents_.begin(); iter
			!= collectedEvents_.end(); ++iter)
	{
		out << "EVENT" << " " << (*iter)->getTag() << endl;
	}
	for (EventIterator iter = relatedEvents_.begin(); iter
			!= relatedEvents_.end(); ++iter)
	{
		out << "RELATED_EVENT" << " " << (*iter)->getTag() << endl;
	}

}
string Entity::getCollectionKeyword()
{
  return gameFacade->getDataManipulator()->getEntityCollectionKeyword(getTag());
}


void  Entity::loadOrders()

{
//cout << "Loading orders for "<< print()<<endl;

//
//  vector<Order *>::iterator iter;
//  for ( iter = orders_.begin(); iter !=  orders_.end(); iter++)
//    {
//           (*iter)->load();
//    }


}




void Entity::addOrder(string newOrder)
{
	orders_.push_back(new OrderLine(newOrder,this));
}



/*
 * delete all entity's orders
 */
void Entity::clearOrders()
{
  OrderIterator iter;
  for ( iter = orders_.begin(); iter != orders_.end(); iter++)
    {
           delete (*iter);
    }
  orders_.clear();
}



STATUS Entity::prepareData()
{
//	STATUS status = OK;
#ifdef DEBUG
          cname = tag_.c_str();
#endif
    if(isUnknownEntity())
    {
    	return OK; // Do not check dummy UnknownEntity
    }
	preprocessData();    // Re-establish internal references, there they  were not saved.
   if(IO_ERROR == dataConsistencyCheck())    // Check consistency of data
      return IO_ERROR;
//   prepareOrders();
		return OK;
}



void Entity::preprocessData()
{
}



void Entity::postProcessData()
{
}



void Entity::postPostProcessData()
{
}



void Entity::dailyPreProcess()
{
}


/** Adds public report */



void Entity::addReport(ReportRecord  report)
{
if (!isSilent())
	publicReports_.push_back(report);
// cout << "Added report "; report->print(cout);
}

void     Entity::addEvent(Event * event, OrderLine *  orderId)
{
	cout<< "=====> Event "<< event->getTag()<<" was added fo "<< getTag()<<endl;
   eventRecords_.push_back(EventElement(event,orderId));
}



void     Entity::addRelatedEvent(Event * event)
{
	cout<< "=====> Related Event "<< event->getTag()<<" was added fo "<< getTag()<<endl;
 	relatedEvents_.push_back(event);
}



void Entity::addReport(ReportMessage * report,OrderLine *  orderId, BasicCondition * observationCriteria)
{
  Entity::addReport(ReportRecord(report, orderId, observationCriteria));
}
/** No descriptions */



void Entity::dailyUpdate()
{
 setDisobeying(false);
}


/*
 * Unit tries to obtain public reports from given entity
 */
void Entity::extractReport(UnitEntity * unit, vector < ReportElement * > & extractedReports)
{
//    if (isTraced())
//    {
//        cout << "Extracting Reports for [" << tag_ << "] " << unit->printTag() << endl;
//    }
    vector<ReportRecord >::iterator iter;
    for (iter = publicReports_.begin(); iter != publicReports_.end(); iter++)
    {
        if ((*iter).observableBy(unit))
        {
            //             cout << "Report extracting by "<< unit->print()<<" =} "/*<< <<endl*/;(*iter).reportMessage->print(cout);
            extractedReports.push_back
                    (new ReportElement((*iter).reportMessage, this));
        }
    }
}
/*
 * Unit tries to get Event image from event holder (event origin)
 */

void     Entity::extractEventsImages(UnitEntity * unit, vector < Event *  > & extractedEvents)
{
	Event * event;
   for(vector < EventElement>::iterator iter = eventRecords_.begin(); iter != eventRecords_.end(); ++iter)
   {
	   event = (*iter).getEvent();
//		cout<< "=====> "<< unit->getTag()<<" tries to extract "<< event->getTag()<<" from "<<getTag()<<endl;
      if(event->isObservableBy(unit))
       {
           extractedEvents.push_back(event);
           cout<<"   === Extracted from  "<<getTag() <<" by "<<unit->getTag()  <<" ===> "<<event->getTag()<<endl;
       }
   }
}


/*
 * Returns reference to Entity, to whom entity's events should be reported
 */
Entity * Entity::getReportDestination(){
return 0;
}



/** prints  report for Entity (stats, posessions, private events) */
void Entity::produceFactionReport(FactionEntity * faction, ReportPrinter &out){
 out  << print()<<endl;
// Stats
// Posessions
 reportEvents(out);
}



/** prints list of events related to this Entity */
void Entity::reportEvents(ReportPrinter &out)
{
    out.incr_indent();
   vector<ReportElement *>::iterator iter;
  for ( iter = collectedReports_.begin(); iter != collectedReports_.end(); iter++)
    {
		 (*iter)->printReport(out);
	}
    out.decr_indent();
}

/** Transforms public reports into collected reports and cleans all unused public reports. */
void Entity::finalizeReports()
{
    //	for_each(publicReports_.begin(),publicReports_.end(),  )
    if (isTraced())
    {
        //cout << "Finalizing report for " <<printTag()<<endl;
    }
    // It is possible that exist two or more duplicates of the same report
    // (results of multiple attempts of the execution of the same order)
    // the earlier one should be deleted

    vector<ReportRecord>::iterator iter1;
    vector<ReportRecord>::iterator iter2;
    bool duplicate;

    for (iter1 = publicReports_.begin(); iter1 != publicReports_.end();)
    {
        duplicate = false;
        if ((*iter1).orderId == 0)
        {
            iter1++;
            continue; // This is non-order generated report
        }
        for (iter2 = iter1 + 1; iter2 != publicReports_.end(); iter2++)
        {
            if ((*iter2).orderId == (*iter1).orderId)
            {
                duplicate = true;
                break;
            }
        }
        if (duplicate)
        {
            publicReports_.erase(iter1);
        }
        else
            iter1++;
    }

    for (iter1 = publicReports_.begin(); iter1 != publicReports_.end(); iter1++)
    {
        collectedReports_.push_back
                (new ReportElement((*iter1).reportMessage, this));
    }
}

///** Transforms crude EventElements into collected events and cleans all unused eventElements */
//void Entity::finalizeEvents()
//{
//// It is possible that exist two or more duplicates of the same event
//// (results of multiple attempts of the execution of the same order)
//// the earlier one should be deleted
//    vector<EventElement>::iterator iter1;
//    vector<EventElement>::iterator iter2;
//    bool duplicate;
//
//    for (iter1 = eventRecords_.begin(); iter1 != eventRecords_.end();)
//    {
//        duplicate = false;
//        if ((*iter1).getOrder() == 0)
//        {
//            iter1++;
//            continue; // This is non-order generated report
//        }
//        for (iter2 = iter1 + 1; iter2 != eventRecords_.end(); iter2++)
//        {
//            if ((*iter2).getOrder() == (*iter1).getOrder())
//            {
//                duplicate = true;
//                break;
//            }
//        }
//        if (duplicate)
//        {
//            eventRecords_.erase(iter1);
//        }
//        else
//            iter1++;
//    }
//
//    for (iter1 = eventRecords_.begin(); iter1 != eventRecords_.end(); iter1++)
//    {
//        collectedEvents_.push_back((*iter1).getEvent());
//    }
//}

/*
 * Transforms crude EventElements into Collected events 
 */
void Entity::finalizeEvents()
{
    for (vector<EventElement>::iterator iter = eventRecords_.begin(); 
            iter != eventRecords_.end(); iter++)
    {
        collectedEvents_.push_back((*iter).getEvent());
    }
    eventRecords_.clear();
}



/*
 * Post-turn processing of Collected events
 */
void     Entity::finalizeTurnEvents()
{

}



bool Entity::isEventDuplicateExist(EventRule * eventRule, OrderLine * orderId)
{
    for (vector<EventElement>::iterator iter = eventRecords_.begin(); iter != eventRecords_.end(); ++iter)
    {
        if ((*iter).getOrder() == 0)
        {
            continue; // This is non-order generated report
        }
        if ((orderId == (*iter).getOrder()) && (eventRule == (*iter).getEvent()->getEventRule()))
        {
            return true;
        }
    }
    return false;
}
/*
 * cleans all unused public reports.
 */
void Entity::cleanPublicReports()
{
					publicReports_.clear();
}


/*
 * cleans  collected reports this destroys all report information
 */
void Entity::cleanCollectedReports()
{
//   cout<< "Cleaning report info for " << *this<<endl;
   vector<ReportElement *>::iterator iter;
  for ( iter = collectedReports_.begin(); iter != collectedReports_.end(); iter++)
		{
          if(*iter)
            delete (*iter);
		}
					collectedReports_.clear();
}



bool      Entity::defaultAction()
{
  return false;
}



bool Entity::mayInterract(UnitEntity * unit)
{
        return false;
}



bool Entity::mayInterractTokenEntity(TokenEntity * tokenEntity)
{
        return false;
}



void Entity::clearTeachingOffers()
{
//  cout <<" -------------------- All Teching Offers cleared for "<< print()<<endl;
  teachingAcceptorOffers_.clear();
//  cout << *this << " clears all teachin offers"<<endl;
}

void Entity::cleanTeachingOffers()
{
//  cout <<" -------------------- All Teching Offers cleared for "<< print()<<endl;
  teachingAcceptorOffers_.clear();
  vector <TeachingOffer  *>::iterator iter;
   for(iter = teachingDonorOffers_.begin(); iter != teachingDonorOffers_.end(); ++iter)
    {
	        // cout<< *this<<" deletes " << *(*iter) <<endl;
        if(*iter)
        {
	delete (*iter);
        }
    }
	teachingDonorOffers_.clear();

//  cout << *this << " clears all teachin offers"<<endl;
}



void Entity::addTeachingAcceptorOffer(TeachingOffer * offer)
{
// cout <<" -------------------- Teaching Offer: "<<*offer  <<" ( "<<(int)offer <<") added for "<< print()<<endl;
 teachingAcceptorOffers_.push_back(offer);
//cout <<*offer << " added to "<< *this<<endl;
}


void Entity::addTeachingDonorOffer(TeachingOffer * offer)
{
// cout <<" -------------------- Teaching Offer: "<<*offer  <<" ( "<<(int)offer <<") added for "<< print()<<endl;
 teachingDonorOffers_.push_back(offer);
//cout <<*offer << " added to "<< *this<<endl;
}



int Entity::getSkillLevel(SkillRule  * const skill)
{
  return 0;
}



TeachingOffer * Entity::findTeachingOffer(SkillRule  * skill, int level)
{
  //cout <<" -------------------- Looking for Teching Offer in "<< print()<<" here "<<teachingAcceptorOffers_.size() <<" offers"<<endl;
  vector <TeachingOffer  *>::iterator iter;
  for(iter = teachingAcceptorOffers_.begin(); iter != teachingAcceptorOffers_.end(); ++iter)
  {
      //cout<< "Teching Offer: "<< (int)(*iter)<<endl;
    if((*iter)->getSkill() == skill && (*iter)->getLevel() >= level)
    {
        return (*iter);
    }

    }
 return 0;
}



vector <AbstractArray> Entity::aPrintTeaching()
{
    vector <AbstractArray> out;
    vector <TeachingOffer  *>::iterator iter;
    for(iter = teachingDonorOffers_.begin(); iter != teachingDonorOffers_.end(); ++iter)
    {
         vector <AbstractData *> v;
//      if((*iter)->getTeacher() == this)
//          return (*iter)->isConfirmed();
      out.push_back(v);
      }
    return out;

}


bool Entity::checkTeachingConfirmation()
{
  vector <TeachingOffer  *>::iterator iter;
  for(iter = teachingDonorOffers_.begin(); iter != teachingDonorOffers_.end(); ++iter)
  {
    if((*iter)->getTeacher() == this)
        return (*iter)->isConfirmed();
    }
  cerr << "ERROR."<< print() <<" Can't find his own teachingOffers\n";
  return false;
}

/** How many seats occupies this entity in the class. Number of entities that can be tought by one teacher determined by this value. */

int Entity::getLearningCapacity()
{
  return 10000; // very big. can't study
}



bool Entity::teacherRequired(SkillRule * skill)
{
  return false;
}



bool Entity::addKnowledge(Rule * info)
{
  return false;
}



bool Entity::addSkillKnowledge(SkillRule * knowledge, int level)
{
  return false;
}


void Entity::extractKnowledge(Entity * recipient, int parameter)
{

}

//void Entity::extractSkillKnowledge(Entity * recipient, int parameter)
//{
//
//}
