/***************************************************************************
                             CombatOrder.cpp 
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "CombatOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "UnaryPattern.h"
#include "BinaryPattern.h"
#include "TertiaryPattern.h"
#include "EntitiesCollection.h"
extern EntitiesCollection <UnitEntity>      units;
extern Reporter *	invalidParameterReporter;
extern Reporter *	missingParameterReporter;
extern Reporter *	AtReporter;

CombatOrder * instantiateCombatOrder = new CombatOrder();

CombatOrder::CombatOrder(){
  keyword_ = "Combat";
  registerOrder_();
  description = string("COMBAT actions \n") +
  "Immediate, one-shot.  Executes immediately, and sets the list of combat actions\n" +
  "during the battle to the list given.  You may specify as combat actions:\n" +
  "- MELEE, to attack in hand-to-hand combat;\n" +
  "- PARRY, to defend against melee;\n" +
  "- skill-tag, to use the requisite skill or magic spell in combat;\n" +
  "- item-tag, to use the requisite item in combat.\n" +
  "\n" +
  "Your combat list ends up with an implicit \"MELEE PARRY\".\n";

  orderType_   = IMMEDIATE_ORDER;
  mayInterrupt_ = true;
}

STATUS CombatOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

    if(!parseGameDataParameter(entity,  parser, units, "unit id", parameters))
            return IO_ERROR;
  return OK;

}



ORDER_STATUS CombatOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
	return FAILURE;
}

