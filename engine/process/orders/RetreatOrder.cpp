/***************************************************************************
                             RetreatOrder.cpp 
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "RetreatOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "UnaryPattern.h"
#include "BinaryPattern.h"
#include "TertiaryPattern.h"
#include "EntitiesCollection.h"
#include "ObservationCondition.h"
	
extern Reporter * retreatGroupReporter;
extern Reporter * retreatPublicReporter;
extern Reporter * retreatPrivateReporter;

RetreatOrder * instantiateRetreatOrder = new RetreatOrder();

RetreatOrder::RetreatOrder(){
  keyword_ = "Retreat";
  registerOrder_();
  description = string("RETREAT \n") +
  "Immediate, special.  Aborts the current movement, and go back to the previous\n" +
  "location.  RETREAT can be initiated only by the leader of the stack, or the\n" +
  "moving unit.  The stack will\n" +
  "spend as much time it had spent moving going back to its starting point.\n";

  orderType_   = STACK_ORDER;
  mayInterrupt_ = true;
}

STATUS RetreatOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
   if(!entityIsPhysicalEntity(entity))
            return IO_ERROR;

  return OK;

}

ORDER_STATUS RetreatOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{
  PhysicalEntity * unit = dynamic_cast<PhysicalEntity *>(entity);
  assert(unit);
  if( unit->retreat())
    {
        unit->addReport(
            new UnaryPattern(retreatPrivateReporter, unit->getLocation()));
         unit->getLocation()->addReport(
            new BinaryPattern(retreatPublicReporter, unit, unit->getLocation()),
             0 ,ObservationCondition::createObservationCondition(unit->getStealth())
                                       );
        ReportRecord * report = new  ReportRecord(new BinaryPattern(retreatGroupReporter,  
                unit->getLocation(), unit), 0, 0);
        unit->movingGroupReport(report);
	      return SUCCESS;
	  }
	else
    return INVALID;  
}

