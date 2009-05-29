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
//#include "BasicAttribute.h"
extern bool testMode;
Entity         sampleEntity  ("ENTITY",  &sampleGameData);

//static vector <BasicAttribute *> Entity::attributes;
Entity::Entity ( const Entity * prototype ): GameData(prototype)
{
  fullDayOrder_ = 0;
  silent_ = false;
  traced_ = false;
  disobeying_ = false;
  isPayingUpkeep_ = true;
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
   
        if (parser->matchKeyword("TRACED"))
        {
          traced_ = true;
          return OK;
        }

   currentResult = enchantments_.initialize(parser);
   if(currentResult != OK)
    return currentResult;

	  return OK;

}



void
Entity::save(ostream &out)
{
  out << keyword_ << " " <<tag_ << endl;
  if(!name_.empty()) out << "NAME " <<name_ << endl;
  if(!description_.empty()) out << "DESCRIPTION " <<description_  << endl;
  enchantments_.save(out);
//  out << endl;
  for (OrderIterator iter = orders_.begin(); iter != orders_.end(); iter++)
    {
           (*iter)->save(out);
    }
    if (traced_) out << "TRACED" << endl;
}



ostream &operator << ( ostream &out, Entity * entity)
{
  out << entity->getTag() << " ";
	return out;
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



void Entity::addReport(ReportMessage * report,OrderLine *  orderId, BasicCondition * observationCriteria)
{
if (!isSilent())
  publicReports_.push_back(ReportRecord(report, orderId, observationCriteria));
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
//	cout << "Extracting Reports for [" <<tag_ <<"] " << unit->printTag()<< endl;
   vector<ReportRecord >::iterator iter;
  for ( iter = publicReports_.begin(); iter != publicReports_.end(); iter++)
    {
       if( (*iter).observableBy(unit))
			{
//             cout << "Report extracting by "<< unit->print()<<" =} "/*<< <<endl*/;(*iter).reportMessage->print(cout);
            	extractedReports.push_back
						(new ReportElement((*iter).reportMessage,this));
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
//cout << "Finalizing report for " <<printTag()<<endl;
// It is possible that exist two or more duplicates of the same report
// (results of multiple attempts of the execution of the same order)
// the earlier one should be deleted

   vector<ReportRecord>::iterator iter1;
   vector<ReportRecord>::iterator iter2;
		bool duplicate;

  for ( iter1 = publicReports_.begin(); iter1 != publicReports_.end(); )
		{
       duplicate = false;
       if((*iter1).orderId == 0)
       {
					  iter1++;
            continue; // This is non-order generated report
       }
  		for ( iter2 = iter1 + 1; iter2 != publicReports_.end(); iter2++)
					{
					if( (*iter2).orderId == (*iter1).orderId)
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

  for ( iter1 = publicReports_.begin(); iter1 != publicReports_.end(); iter1++)
		{
					collectedReports_.push_back
							(new ReportElement((*iter1).reportMessage,this));
		}
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
  teachingOffers_.clear();
//  cout << *this << " clears all teachin offers"<<endl;
}



void Entity::addTeachingOffer(TeachingOffer * offer)
{
 // cout <<" -------------------- Teching Offer added for "<< print()<<endl;
 teachingOffers_.push_back(offer);
// cout <<*offer << " added to "<< *this<<endl;
}



int Entity::getSkillLevel(SkillRule  * const skill)
{
  return 0;
}



TeachingOffer * Entity::findTeachingOffer(SkillRule  * skill, int level)
{
//  cout <<" -------------------- Looking for Teching Offer in "<< print()<<" here "<<teachingOffers_.size() <<" offers"<<endl;
  vector <TeachingOffer  *>::iterator iter;
  for(iter = teachingOffers_.begin(); iter != teachingOffers_.end(); ++iter)
  {

    if((*iter)->getSkill() == skill && (*iter)->getLevel() >= level)
        return (*iter);

    }
 return 0;
}



bool Entity::checkTeachingConfirmation()
{
  vector <TeachingOffer  *>::iterator iter;
  for(iter = teachingOffers_.begin(); iter != teachingOffers_.end(); ++iter)
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
