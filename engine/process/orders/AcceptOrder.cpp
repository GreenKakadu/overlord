/***************************************************************************
                             AcceptOrder.cpp 
                             -------------------
    begin                : Thu Mar 6 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "AcceptOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "UnaryPattern.h"
#include "BinaryPattern.h"
#include "TertiaryPattern.h"
#include "EntitiesCollection.h"
extern EntitiesCollection <UnitEntity>      units;
extern Reporter *	invalidOrderReporter;
extern Reporter *	invalidParameterReporter;
extern Reporter *	missingParameterReporter;
extern Reporter * acceptOwnReporter;
extern Reporter *	acceptReporter;

AcceptOrder::AcceptOrder(){
  keyword_ = "accept";
  description = string("ACCEPT [unit-id] \n") +
  "Immediate, special.  This order executes when the designated unit attempts to\n" +
  "STACK itself under your leadership.  The unit is allowed to do so, regardless\n" +
  "of the stance toward its faction.\n";

  orderType_   = IMMEDIATE_ORDER;
}

STATUS AcceptOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  if(unit==0)  // Wrong Entity type
				{
					cout  << "=<>= ACCEPT: order is available only for units "
								<< entity->printName() << endl;
         entity->addReport(new BinaryPattern(invalidOrderReporter, new StringData(keyword_), new StringData("units")));
         return IO_ERROR;
				}
  string followerTag = parser->getWord();
  if (followerTag.size() == 0)  // Missing parameter
        {
					cout  << "=<>= ACCEPT: missing Parameter (unit id expected) for "
								<< entity->printName() << endl;
         entity->addReport(new BinaryPattern(missingParameterReporter, new StringData(keyword_), new StringData("unit id")));
         return IO_ERROR;
        }
  if (!units.checkDataType(followerTag)) // this can't be a tag
				{
					cout  << "=<>= STACK: Wrong  Parameter <"
								<< followerTag<< "> (unit id expected) for "
								<< entity->printName() << endl;
         entity->addReport(new TertiaryPattern(invalidParameterReporter, new StringData(keyword_), new StringData(followerTag), new StringData("unit id")));
         return IO_ERROR;
				}

  UnitEntity * follower = units[followerTag];
  if( follower == 0) // unit doesn't exist but we don't want to let player to know that
				{
          StringData * dummyFollower = new StringData(followerTag);
   		     parameters.push_back(dummyFollower);
          return OK;
				}


  parameters.push_back(follower);
  return OK;

}

ORDER_STATUS AcceptOrder::process (Entity * entity, vector <AbstractData *>  &parameters,
			                      Order * orderId)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
//  UnitEntity * follower = dynamic_cast<UnitEntity *>(parameters[0]);
	UnitEntity * follower   =  DOWNCAST_ENTITY<UnitEntity>(parameters[0]);

  if ( follower == 0) // unit doesn't exist but we don't want to let player to know that
     {
 		  return FAILURE;
      }
  if(unit->getFaction() == follower->getFaction())
  {
    unit->addReport(new UnaryPattern(acceptOwnReporter, follower) );
		  return SUCCESS;
    } 

  if(unit->isLeading(follower))
    {
      unit->clearAccept(follower);
		  return SUCCESS;
    }
   if(unit->isAccepting(follower))
    {
		  return FAILURE;
    }
   else
    {
      unit->accept(follower);   
     unit->addReport(new UnaryPattern(acceptReporter, follower) );
      }
	return FAILURE;
}
