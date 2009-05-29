/***************************************************************************
                          MergeOrder.cpp
                             -------------------
    begin                : Tue Jul 8 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "MergeOrder.h"
#include "Entity.h"
#include "RaceElement.h"
#include "RaceRule.h"
#include "UnitEntity.h"
#include "EntitiesCollection.h"
#include "SimpleMessage.h"
#include "UnaryMessage.h"
#include "TertiaryMessage.h"
extern EntitiesCollection <UnitEntity>      units;
extern ReportPattern * mergeRaceMismatchReporter;
extern ReportPattern * mergeRaceErrorReporter;
extern ReportPattern * mergeFactionMismatchReporter;
extern ReportPattern * mergeReporter;

//MergeOrder instantiateMergeOrder;
MergeOrder * instantiateMergeOrder = new MergeOrder();

MergeOrder::MergeOrder(){
  keyword_ = "merge";
  registerOrder_();
  description = string("MERGE unit-id [number]\n") +
  "Immediate, follower/creature-only.  This orders executes when unit-id\n"+
  "is of the same race as unit, belongs to your faction and is present at\n"+
  "the same location. If number specified unit must have at least number\n" +
  "figures. The skills of the two units are mixed and averaged.  \n" +
  "If this would cause the target unit to know two or more basic skills,\n" +
  "the figures transferred will forget their skills before merging.\n" +
  "No items transferred to the target unit unless the number\n" +
  "of figures is not specified or equal to the number of figures in the unit.\n" +
  "In that case, all figures  are transferred to the target-id along with\n" +
  "their possessions.  Empty unit will be disbanded.\n";

  orderType_   = IMMEDIATE_ORDER;
}



STATUS MergeOrder::loadParameters(Parser * parser, vector <AbstractData *>
          &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;
    if(!parseGameDataParameter(entity, parser, units, "unit id", parameters))
            return IO_ERROR;

     parseIntegerParameter(parser, parameters);

            return OK;
}



ORDER_STATUS MergeOrder::process (Entity * entity, vector <AbstractData *>
          &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);

  UnitEntity * recipient   =  DOWNCAST_ENTITY<UnitEntity>(parameters[0]);
  int number =  getIntegerParameter(parameters,1);

  if(!unit->mayInterract(recipient))
  {
    return FAILURE;
  }
  if(unit->getFaction() != recipient->getFaction())
  {
     unit->addReport(new SimpleMessage(mergeFactionMismatchReporter));
    return INVALID;
  }

  if(!unit->getRace()->mayTransferFigures())
  {
     unit->addReport(new UnaryMessage(mergeRaceErrorReporter, unit));
    return INVALID;
    }

  if(unit->getRace() != recipient->getRace())
  {
     unit->addReport(new SimpleMessage(mergeRaceMismatchReporter));
    return INVALID;
  }
   int totalFigures = unit->getFiguresNumber();
  if( number > totalFigures)
       number = totalFigures;
  
  if( number == 0)
    number = totalFigures;

   unit->setFigures(totalFigures - number);
   recipient->mergeUnits(number,unit);

    if(number == totalFigures)    // if merging all give all items
     {
       unit->giveAllInventory(recipient);
     }
       // efects
    recipient->updateEquipement();
//QQQ
     unit->addReport(new TertiaryMessage(mergeReporter, unit,
          new RaceElement(unit->getRace(),number), recipient));
  return SUCCESS;
}
