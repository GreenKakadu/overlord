/***************************************************************************
                             TacticOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "TacticOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "EntitiesCollection.h"
extern EntitiesCollection <UnitEntity>      units;
extern ReportPattern *	invalidParameterReporter;
extern ReportPattern *	missingParameterReporter;
extern ReportPattern *	AtReporter;

TacticOrder * instantiateTacticOrder = new TacticOrder();

TacticOrder::TacticOrder(){
  keyword_ = "Tactic";
  registerOrder_();
  description = string("TACTIC battle rank file move \n") +
  "Immediate, one-shot.  This order executes immediately and sets your tactical\n" +
  "battle settings.\n" +
  "\n" +
  "Battle is one of either: AVOID (do not enter battle unless attacked), DEFEND\n" +
  "(do not enter battle unless you are on the defending side) or FIGHT.\n" +
  "\n" +
  "Rank is one of either: FRONT, MIDDLE or REAR.\n" +
  "\n" +
  "File is one of either: LEFT, CENTER or RIGHT.\n" +
  "\n" +
  "Move is one of either: FLEE, RETREAT, STAND, ADVANCE, SWEEP, FLANK, CHARGE,\n" +
  "FIRE, or SUPPORT (see the description of the battle phase)\n";

  orderType_   = IMMEDIATE_ORDER;
  mayInterrupt_ = true;
}

STATUS TacticOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;
   bool parsingResult = false;
   parsingResult |= parseOptionalStringParameter(entity, parser, parameters, "AVOID");
   parsingResult |= parseOptionalStringParameter(entity, parser, parameters, "DEFEND");
   parsingResult |= parseOptionalStringParameter(entity, parser, parameters, "FIGHT");
   parsingResult |= parseOptionalStringParameter(entity, parser, parameters, "FRONT");
   parsingResult |= parseOptionalStringParameter(entity, parser, parameters, "MIDDLE");
   parsingResult |= parseOptionalStringParameter(entity, parser, parameters, "REAR");
   parsingResult |= parseOptionalStringParameter(entity, parser, parameters, "LEFT");
   parsingResult |= parseOptionalStringParameter(entity, parser, parameters, "CENTER");
   parsingResult |= parseOptionalStringParameter(entity, parser, parameters, "RIGHT");
   parsingResult |= parseOptionalStringParameter(entity, parser, parameters, "FLEE");
   parsingResult |= parseOptionalStringParameter(entity, parser, parameters, "RETREAT");
   parsingResult |= parseOptionalStringParameter(entity, parser, parameters, "STAND");
   parsingResult |= parseOptionalStringParameter(entity, parser, parameters, "ADVANCE");
   parsingResult |= parseOptionalStringParameter(entity, parser, parameters, "SWEEP");
   parsingResult |= parseOptionalStringParameter(entity, parser, parameters, "FLANK");
   parsingResult |= parseOptionalStringParameter(entity, parser, parameters, "CHARGE");
   parsingResult |= parseOptionalStringParameter(entity, parser, parameters, "FIRE");
   parsingResult |= parseOptionalStringParameter(entity, parser, parameters, "SUPPORT");
   if(parsingResult)
     return OK;
    else
        {
          entity->addReport(new TertiaryMessage(invalidParameterReporter, new StringData(keyword_), new StringData(parser->getWord()), new StringData("one of tactic keywords")));
          return IO_ERROR;
        }

}



ORDER_STATUS TacticOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
	return FAILURE;
}

